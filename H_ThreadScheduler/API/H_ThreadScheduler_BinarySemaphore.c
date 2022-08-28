/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-04 20:16:16
 * @LastEditTime: 2021-12-26 22:14:22
 * @LastEditors: 0x9DEFA478
 * @Description:
 * 二值信号量实现
 *   同一个信号量只能(同一时间只能使用一项): 
 *   1.接收一个中断释放的信号量(H_TS_SemaphoreGive_ISR())
 *   2.接收多个(无数量限制)线程的信号量(线程使用H_TS_SemaphoreGive())
 * 对于接收,只有线程能接收信号量
 * 
 * 实际上, 二值信号量是能实现为中断与线程都能重入的, 但这样的实现会降低线程Give的效率(给快速通道造成额外的负担)
 * 因为二值信号量被Give多次后状态是一致的,
 * 用户可用一个重入标记来保证Give_ISR不被重入来使得多个中断都可以"同时操作"同一个二值信号量
 * 发送中断嵌套时, 高层嵌套将获得信号量满的返回值(此时信号量不一定已添加, 但此时一定有一个最底层的中断嵌套能Give成功)
 * 
 * QQ:2652450237
 * ============================================================================================================================================
 * 
 * 
 * 
 *                                                                                               ************      ****************************
 *                                                                                             ************      ****************************  
 *                                                                                           ************      ****************************    
 *                                                                                         ************      ****************************      
 *                                                                                       ************      ************                        
 *                                                                                     ************      ************                          
 *                                                                                   ************      ************                            
 *                                                                                 ************      ************                              
 *                                                                               ************      ************                                
 *                                                                             ************      ************                                  
 *                                                                           ************      ************                                    
 *                                                                         ************      ************                                      
 *                                                                       ************      ************                                        
 *                                                                     ************      ************                                          
 *                                                                   ************      ************                                            
 *                                                                 ************      ************                                              
 *                                                               ************      ************                                                
 *                                                             ************      ************                                                  
 *                                                           ************      ************                                                    
 *                                                         ************      ************                                                      
 *                                                       ************      ************                                                        
 *                                                     ************      ************                                                          
 *                                                   ************      ************                                                            
 *                                                 ************      ************                                                              
 *                                               ************      ************                                                                
 *                                             ************      ************                                                                  
 *                                           ************      ************                                                                    
 *                                         ************      ************                                                                      
 *                                       ************      ************                                                                        
 *                                     ************      ************                                                                          
 *                                   ************      ************                                                                            
 *                                 ************      ************                                                                              
 *                               ************      ************                                                                                
 *                             ************      ************                                                                                  
 *                           ************      ************                                                                                    
 *                         ************      ************                                                                                      
 *       ****************************      ************                                                                                        
 *     ****************************      ************                                                                                          
 *   ****************************      ************                                                                                            
 * ****************************      ************                                                                                              
 * 
 * 
 * 
 * ============================================================================================================================================
 * 
 */
#include "./H_ThreadScheduler_Core.h"
#include "../LL/H_ThreadScheduler_LL.h"




extern H_TS* H_TS_Core;




/**
 * @brief 释放一个二值信号量 由中断调用
 * @param BinarySemaphore 要操作的二值信号量
 * @return 0:成功 -1:队列已满
 */
int H_TS_BinarySemaphoreGive_ISR(H_TS_BinarySemaphore* BinarySemaphore){
  H_TS_Thread* Owner;
  Hsize InIsThread1;
  Hsize OutIsThread1;

  int retVal;

  if(BinarySemaphore->Value!=0){
    return -1;
  }
  BinarySemaphore->Value=-1;//设置信号量

  Owner=BinarySemaphore->Owner;
  if(Owner!=NULL){
    //有线程正在等待信号量

    switch(Owner->Status){
      case vH_TS_Thread_Status_Block:

        Owner->Status=vH_TS_Thread_Status_Ready;//线程就绪
        BinarySemaphore->Owner=NULL;//清除占用标记

        //让PendSV处理链表


        //尝试添加到快速通道
        H_TS_ThreadAddToUnblockThread(Owner,H_TS_Core->UnblockThread,InIsThread1,OutIsThread1,retVal);

        if(retVal!=0){
#if vH_TS_BlockThreadListNum > 1
          H_TS_Core->BlockThreadsEvents[Owner->List.Block.ListId]=-1;
#endif
          H_TS_Core->BlockThreadsEvent=-1;
        }

        oH_TS_ISR_CallScheduleISR();

        return 0;
      case vH_TS_Thread_Status_TimeBlock:

        Owner->Status=vH_TS_Thread_Status_Ready;//线程就绪
        BinarySemaphore->Owner=NULL;//清除占用标记

        //让PendSV处理链表


        //尝试添加到快速通道
        H_TS_ThreadAddToUnblockThread(Owner,H_TS_Core->TimeUnblockThread,InIsThread1,OutIsThread1,retVal);
        
        if(retVal!=0){
          H_TS_Core->TimeBlockThreadsEvent=-1;
        }

        oH_TS_ISR_CallScheduleISR();

        return 0;
      case vH_TS_Thread_Status_Ready:
        //这是重入导致的 虽然会导致一些重复操作 但不影响结果
        //直接返回即可
        return 0;
        
      default:
        for(;;){
          //H_TS出BUG了!!
        }
    }
  }

  return 0;
}

/**
 * @brief 释放一个二值信号量
 * @param BinarySemaphore 要操作的二值信号量
 * @return 0:成功 -1:队列已满
 */
int H_TS_BinarySemaphoreGive(H_TS_BinarySemaphore* BinarySemaphore){
  H_TS_Thread* Owner;
  H_TS_Thread* p_last;
  H_TS_Thread* p_next;

  Hsize Priority;
  int retVal;

  oH_TS_SchedulerSuspend();

  if(BinarySemaphore->Value!=0){
    oH_TS_ResumeSchedulerAuto();
    return -1;
  }
  BinarySemaphore->Value=-1;//设置信号量

  Owner=BinarySemaphore->Owner;
  if(Owner!=NULL){
    //有线程正在等待信号量
    switch(Owner->Status){
      case vH_TS_Thread_Status_Block:

        Owner->Status=vH_TS_Thread_Status_Ready;//线程就绪
        BinarySemaphore->Owner=NULL;//清除占用标记

        //将线程从阻塞链表中移除
        p_last=Owner->List.Block.Last;
        p_next=Owner->List.Block.Next;
        if(p_last==NULL){
#if vH_TS_BlockThreadListNum > 1
          H_TS_Core->BlockThreads[Owner->List.Block.ListId]=p_next;
#else
          H_TS_Core->BlockThread=p_next;
#endif
        }else{
          p_last->List.Block.Next=p_next;
        }
        if(p_next!=NULL){
          p_next->List.Block.Last=p_last;
        }

        H_TS_ThreadAddToReadyList(Owner,p_next,Priority,retVal);
        if(retVal==0){
          oH_TS_ResumeSchedulerAuto();
        }else{
          oH_TS_ResumeSchedulerFixed();
        }

        return 0;
      case vH_TS_Thread_Status_TimeBlock:

        Owner->Status=vH_TS_Thread_Status_Ready;//线程就绪
        BinarySemaphore->Owner=NULL;//清除占用标记

        //将线程从时间限制阻塞链表中移除
        p_last=Owner->List.TimeBlock.Last;
        p_next=Owner->List.TimeBlock.Next;
        if(p_last==NULL){
          H_TS_Core->TimeBlockThreads=p_next;
        }else{
          p_last->List.TimeBlock.Next=p_next;
        }
        if(p_next!=NULL){
          p_next->List.TimeBlock.Last=p_last;
          p_next->BlockObject.TimeBlock.SleepTick+=Owner->BlockObject.TimeBlock.SleepTick;
        }

        H_TS_ThreadAddToReadyList(Owner,p_next,Priority,retVal);
        if(retVal==0){
          oH_TS_ResumeSchedulerAuto();
        }else{
          oH_TS_ResumeSchedulerFixed();
        }

        return 0;
      default:
        for(;;){
          //H_TS出BUG了!!
        }
    }
  }

  oH_TS_ResumeSchedulerAuto();
  return 0;
}

/**
 * @brief 获取二值信号量
 * @param BinarySemaphore 要操作的二值信号量
 * @return 如果无信号量 将一直阻塞 直到获取成功
 */
void H_TS_BinarySemaphoreTake(H_TS_BinarySemaphore* BinarySemaphore){
  H_TS_Thread* _this;
  H_TS_Thread* p_next;
  H_TS_Thread* p_last;
#if vH_TS_BlockThreadListNum > 1
  Hsize ListId;
#endif
  Hsize InIsThread1;
  Hsize OutIsThread1;

  int retVal;
  
  _this=cH_TS_RunThread(H_TS_Core)[1];

  if(BinarySemaphore->Value!=0){
    BinarySemaphore->Value=0;
    return;
  }

  //无信号量 开始阻塞

  oH_TS_SchedulerSuspend();

  _this->Status=vH_TS_Thread_Status_Block;
  _this->BlockObject.Block.BlockObject=(void*)&BinarySemaphore->Owner;

  //从就绪链表里移除
  H_TS_ThreadRemoteThisFromReadyList(_this,p_next,p_last);

  //添加到阻塞链表
#if vH_TS_BlockThreadListNum > 1
  H_TS_ThreadAddToBlockList(_this,p_next,ListId);
#else
  H_TS_ThreadAddToBlockList(_this,p_next);
#endif

  //标记信号量拥有线程
  BinarySemaphore->Owner=_this;

  if(BinarySemaphore->Value!=0){
    if(BinarySemaphore->Owner!=NULL){
      //在修改链表期间有信号量释放 释放者不会触发PendSV
      
      _this->Status=vH_TS_Thread_Status_Ready;//线程就绪
      BinarySemaphore->Owner=NULL;//清除占用标记

      //让PendSV处理链表


      //尝试添加到快速通道
      H_TS_ThreadAddToUnblockThread(_this,H_TS_Core->UnblockThread,InIsThread1,OutIsThread1,retVal);

      if(retVal!=0){
#if vH_TS_BlockThreadListNum > 1
        H_TS_Core->BlockThreadsEvents[_this->List.Block.ListId]=-1;
#endif
        H_TS_Core->BlockThreadsEvent=-1;
      }

    }
  }

  oH_TS_ResumeScheduler();//允许线程调度

  if(BinarySemaphore->Value==0){
    //H_TS出现BUG了 这是不应该出现的情况
    for(;;){}
  }
  BinarySemaphore->Value=0;
}

/**
 * @brief 尝试获取二值信号量
 * @param BinarySemaphore 要操作的二值信号量
 * @return 0:成功 其他:获取失败
 */
int H_TS_BinarySemaphoreTryTake(H_TS_BinarySemaphore* BinarySemaphore){
  if(BinarySemaphore->Value==0){
    return -1;
  }
  BinarySemaphore->Value=0;
  return 0;
}

/**
 * @brief 获取二值信号量 时间限制
 * @param BinarySemaphore 要操作的二值信号量
 * @param Timeout 最大等待时间
 * @return 0:成功 其他:获取失败
 */
int H_TS_BinarySemaphoreTake_Time(H_TS_BinarySemaphore* BinarySemaphore,H_TS_Tick Timeout){
  H_TS_Thread* _this;
  H_TS_Thread* p_next;
  H_TS_Thread* p_last;
  Hsize InIsThread1;
  Hsize OutIsThread1;

  int retVal;

  _this=cH_TS_RunThread(H_TS_Core)[1];

  if(BinarySemaphore->Value!=0){
    BinarySemaphore->Value=0;
    return 0;
  }

  if(Timeout<1){
    return -1;
  }

  //无信号量 开始阻塞

  oH_TS_SchedulerSuspend();

  _this->Status=vH_TS_Thread_Status_TimeBlock;
  _this->BlockObject.TimeBlock.BlockObject=(void*)&BinarySemaphore->Owner;

  //从就绪链表里移除
  H_TS_ThreadRemoteThisFromReadyList(_this,p_next,p_last);

  //添加到时间限制阻塞链表
  H_TS_ThreadAddToTimeBlockList(_this,Timeout);

  //标记信号量拥有线程
  BinarySemaphore->Owner=_this;

  if(BinarySemaphore->Value!=0){
    if(BinarySemaphore->Owner!=NULL){
      //在修改链表期间有信号量释放 释放者不会触发PendSV
      
      _this->Status=vH_TS_Thread_Status_Ready;//线程就绪
      BinarySemaphore->Owner=NULL;//清除占用标记

      //让PendSV处理链表


      //尝试添加到快速通道
      H_TS_ThreadAddToUnblockThread(_this,H_TS_Core->TimeUnblockThread,InIsThread1,OutIsThread1,retVal);
        
      if(retVal!=0){
        H_TS_Core->TimeBlockThreadsEvent=-1;
      }

    }
  }

  oH_TS_ResumeScheduler();//允许线程调度

  if(BinarySemaphore->Value==0){
    //超时
    return -1;
  }
  BinarySemaphore->Value=0;

  return 0;
}



