/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-04 20:16:16
 * @LastEditTime: 2021-12-23 14:20:47
 * @LastEditors: 0x9DEFA478
 * @Description:
 * 信号量实现
 * 同一个信号量只能(同一时间只能使用一项): 
 *   1.接收一个中断释放的信号量(H_TS_SemaphoreGive_ISR())
 *   2.接收多个(无数量限制)线程的信号量(线程使用H_TS_SemaphoreGive())
 * 对于接收,只有线程能接收信号量
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
 * @brief 释放一个信号量 由中断调用
 * @param Semaphore 要操作的信号量
 * @return 0:成功 -1:队列已满
 */
int H_TS_SemaphoreGive_ISR(H_TS_Semaphore* Semaphore){
  H_TS_Thread* Owner;
  Hsize InIsThread1;
  Hsize OutIsThread1;

  int retVal;

  if(0!=H_TS_Semaphore_Add(Semaphore->Semaphore)){
    return -1;
  }

  Semaphore->AddFlag=0;

  Owner=Semaphore->Owner;
  if(Owner!=NULL){
    //有线程正在等待信号量

    switch(Owner->Status){
      case vH_TS_Thread_Status_Block:

        Owner->Status=vH_TS_Thread_Status_Ready;//线程就绪
        Semaphore->Owner=NULL;//清除占用标记

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
        Semaphore->Owner=NULL;//清除占用标记

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
 * @brief 释放一个信号量
 * @param Semaphore 要操作的信号量
 * @return 0:成功 -1:队列已满
 */
int H_TS_SemaphoreGive(H_TS_Semaphore* Semaphore){
  H_TS_Thread* Owner;
  H_TS_Thread* p_last;
  H_TS_Thread* p_next;

  Hsize Priority;
  int retVal;

  oH_TS_SchedulerSuspend();

  if(0!=H_TS_Semaphore_Add(Semaphore->Semaphore)){
    oH_TS_ResumeSchedulerAuto();
    return -1;
  }

  Semaphore->AddFlag=0;

  Owner=Semaphore->Owner;
  if(Owner!=NULL){
    //有线程正在等待信号量
    switch(Owner->Status){
      case vH_TS_Thread_Status_Block:

        Owner->Status=vH_TS_Thread_Status_Ready;//线程就绪
        Semaphore->Owner=NULL;//清除占用标记

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
        Semaphore->Owner=NULL;//清除占用标记

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
 * @brief 获取信号量
 * @param Semaphore 要操作的信号量
 * @return 如果无信号量 将一直阻塞 直到获取成功
 */
void H_TS_SemaphoreTake(H_TS_Semaphore* Semaphore){
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

  Semaphore->AddFlag=-1;

  if(0==H_TS_Semaphore_Sub(Semaphore->Semaphore)){
    return;
  }

  //无信号量 开始阻塞

  oH_TS_SchedulerSuspend();

  _this->Status=vH_TS_Thread_Status_Block;
  _this->BlockObject.Block.BlockObject=(void*)&Semaphore->Owner;

  //从就绪链表里移除
  H_TS_ThreadRemoteThisFromReadyList(_this,p_next,p_last);

  //添加到阻塞链表
#if vH_TS_BlockThreadListNum > 1
  H_TS_ThreadAddToBlockList(_this,p_next,ListId);
#else
  H_TS_ThreadAddToBlockList(_this,p_next);
#endif

  //标记信号量拥有线程
  Semaphore->Owner=_this;

  if(Semaphore->AddFlag==0){
    if(Semaphore->Owner!=NULL){
      //在修改链表期间有信号量释放 中断不会触发PendSV
      
      _this->Status=vH_TS_Thread_Status_Ready;//线程就绪
      Semaphore->Owner=NULL;//清除占用标记

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

  if(0!=H_TS_Semaphore_Sub(Semaphore->Semaphore)){
    //H_TS出现BUG了 这是不应该出现的情况
    for(;;){}
  }
}

/**
 * @brief 尝试获取信号量
 * @param Semaphore 要操作的信号量
 * @return 0:成功 其他:获取失败
 */
int H_TS_SemaphoreTryTake(H_TS_Semaphore* Semaphore){

  return H_TS_Semaphore_Sub(Semaphore->Semaphore);
}

/**
 * @brief 获取信号量 时间限制
 * @param Semaphore 要操作的信号量
 * @param Timeout 最大等待时间
 * @return 0:成功 其他:获取失败
 */
int H_TS_SemaphoreTake_Time(H_TS_Semaphore* Semaphore,H_TS_Tick Timeout){
  H_TS_Thread* _this;
  H_TS_Thread* p_next;
  H_TS_Thread* p_last;
  Hsize InIsThread1;
  Hsize OutIsThread1;

  int retVal;

  _this=cH_TS_RunThread(H_TS_Core)[1];

  Semaphore->AddFlag=-1;

  if(0==H_TS_Semaphore_Sub(Semaphore->Semaphore)){
    return 0;
  }

  if(Timeout<1){
    return -1;
  }

  //无信号量 开始阻塞

  oH_TS_SchedulerSuspend();

  _this->Status=vH_TS_Thread_Status_TimeBlock;
  _this->BlockObject.TimeBlock.BlockObject=(void*)&Semaphore->Owner;

  //从就绪链表里移除
  H_TS_ThreadRemoteThisFromReadyList(_this,p_next,p_last);

  //添加到时间限制阻塞链表
  H_TS_ThreadAddToTimeBlockList(_this,Timeout);

  //标记信号量拥有线程
  Semaphore->Owner=_this;

  if(Semaphore->AddFlag==0){
    if(Semaphore->Owner!=NULL){
      //在修改链表期间有信号量释放 中断不会触发PendSV
      
      _this->Status=vH_TS_Thread_Status_Ready;//线程就绪
      Semaphore->Owner=NULL;//清除占用标记

      //让PendSV处理链表


      //尝试添加到快速通道
      H_TS_ThreadAddToUnblockThread(_this,H_TS_Core->TimeUnblockThread,InIsThread1,OutIsThread1,retVal);
        
      if(retVal!=0){
        H_TS_Core->TimeBlockThreadsEvent=-1;
      }
      
    }
  }

  oH_TS_ResumeScheduler();//允许线程调度

  if(0!=H_TS_Semaphore_Sub(Semaphore->Semaphore)){
    //超时
    return -1;
  }

  return 0;
}



