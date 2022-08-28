/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-02 22:24:27
 * @LastEditTime: 2021-12-23 14:20:46
 * @LastEditors: 0x9DEFA478
 * @Description: 
 * 消息队列实现 
 * 每个消息由两个指针组成 
 * 同一个信息队列只能(同一时间只能使用一项): 
 *   1.接收一个中断的消息(中断使用H_TS_MessageQueueSend_ISR())
 *   2.接收多个(无数量限制)线程的信息(线程使用H_TS_MessageQueueSend())
 * 对于接收,只有线程能接收消息队列的消息
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
 * @brief 向消息队列发送消息 由中断调用
 * @param MessageQueue 要操作的消息队列
 * @param v0 消息指针0
 * @param v1 消息指针1
 * @return 0:成功 -1:队列已满
 */
int H_TS_MessageQueueSend_ISR(H_TS_MessageQueue* MessageQueue,void* v0,void* v1){
  H_TS_Thread* Owner;
  Hsize InIsThread1;
  Hsize OutIsThread1;

  int retVal;
  
  if(0!=H_TS_MessageFIFO_Write(MessageQueue->MessageFIFO,v0,v1)){
    return -1;
  }

  MessageQueue->AddFlag=0;

  Owner=MessageQueue->Owner;
  if(Owner!=NULL){
    //有线程正在等待消息队列

    switch(Owner->Status){
      case vH_TS_Thread_Status_Block:

        Owner->Status=vH_TS_Thread_Status_Ready;//线程就绪
        MessageQueue->Owner=NULL;//清除占用标记

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
        MessageQueue->Owner=NULL;//清除占用标记

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
 * @brief 向消息队列发送消息
 * @param MessageQueue 要操作的消息队列
 * @param v0 消息指针0
 * @param v1 消息指针1
 * @return 0:成功 -1:队列已满
 */
int H_TS_MessageQueueSend(H_TS_MessageQueue* MessageQueue,void* v0,void* v1){
  H_TS_Thread* Owner;
  H_TS_Thread* p_last;
  H_TS_Thread* p_next;

  Hsize Priority;
  int retVal;

  oH_TS_SchedulerSuspend();

  if(0!=H_TS_MessageFIFO_Write(MessageQueue->MessageFIFO,v0,v1)){
    oH_TS_ResumeSchedulerAuto();
    return -1;
  }

  MessageQueue->AddFlag=0;

  Owner=MessageQueue->Owner;
  if(Owner!=NULL){
    //有线程正在等待消息队列

    switch(Owner->Status){
      case vH_TS_Thread_Status_Block:

        Owner->Status=vH_TS_Thread_Status_Ready;//线程就绪
        MessageQueue->Owner=NULL;//清除占用标记

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
        MessageQueue->Owner=NULL;//清除占用标记

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
 * @brief 从队列中读取消息
 * @param MessageQueue 要操作的队列
 * @param v0 存放消息空间的指针0
 * @param v1 存放消息空间的指针1
 * @return 如果无消息 将一直阻塞 直到获取成功
 */
void H_TS_MessageQueueReceive(H_TS_MessageQueue* MessageQueue,void** v0,void** v1){
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

  MessageQueue->AddFlag=-1;

  if(0==H_TS_MessageFIFO_Read(MessageQueue->MessageFIFO,v0,v1)){
    return;
  }

  //无信号量 开始阻塞

  oH_TS_SchedulerSuspend();

  _this->Status=vH_TS_Thread_Status_Block;
  _this->BlockObject.Block.BlockObject=(void*)&MessageQueue->Owner;

  //从就绪链表里移除
  H_TS_ThreadRemoteThisFromReadyList(_this,p_next,p_last);

  //添加到阻塞链表
#if vH_TS_BlockThreadListNum > 1
  H_TS_ThreadAddToBlockList(_this,p_next,ListId);
#else
  H_TS_ThreadAddToBlockList(_this,p_next);
#endif

  //标记消息队列拥有线程
  MessageQueue->Owner=_this;

  if(MessageQueue->AddFlag==0){
    if(MessageQueue->Owner!=NULL){
      //在修改链表期间接收到消息 中断不会触发PendSV
      
      _this->Status=vH_TS_Thread_Status_Ready;//线程就绪
      MessageQueue->Owner=NULL;//清除占用标记

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

  if(0!=H_TS_MessageFIFO_Read(MessageQueue->MessageFIFO,v0,v1)){
    //H_TS出现BUG了 这是不应该出现的情况
    for(;;){}
  }
}

/**
 * @brief 尝试从队列中读取消息
 * @param MessageQueue 要操作的队列
 * @param v0 存放消息空间的指针0
 * @param v1 存放消息空间的指针1
 * @return 0:成功 其他:获取失败
 */
int H_TS_MessageQueueTryReceive(H_TS_MessageQueue* MessageQueue,void** v0,void** v1){

  return H_TS_MessageFIFO_Read(MessageQueue->MessageFIFO,v0,v1);
}

/**
 * @brief 从队列中读取消息 时间限制
 * @param MessageQueue 要操作的队列
 * @param v0 存放消息空间的指针0
 * @param v1 存放消息空间的指针1
 * @param Timeout 最大等待时间
 * @return 0:成功 其他:获取失败
 */
int H_TS_MessageQueueReceive_Time(H_TS_MessageQueue* MessageQueue,void** v0,void** v1,H_TS_Tick Timeout){
  H_TS_Thread* _this;
  H_TS_Thread* p_next;
  H_TS_Thread* p_last;
  Hsize InIsThread1;
  Hsize OutIsThread1;

  int retVal;

  _this=cH_TS_RunThread(H_TS_Core)[1];

  MessageQueue->AddFlag=-1;

  if(0==H_TS_MessageFIFO_Read(MessageQueue->MessageFIFO,v0,v1)){
    return 0;
  }

  if(Timeout<1){
    return -1;
  }

  //无信号量 开始阻塞

  oH_TS_SchedulerSuspend();

  _this->Status=vH_TS_Thread_Status_TimeBlock;
  _this->BlockObject.TimeBlock.BlockObject=(void*)&MessageQueue->Owner;

  //从就绪链表里移除
  H_TS_ThreadRemoteThisFromReadyList(_this,p_next,p_last);

  //添加到时间限制阻塞链表
  H_TS_ThreadAddToTimeBlockList(_this,Timeout);

  //标记消息队列拥有线程
  MessageQueue->Owner=_this;

  if(MessageQueue->AddFlag==0){
    if(MessageQueue->Owner!=NULL){
      //在修改链表期间有信号量释放 中断不会触发PendSV
      
      _this->Status=vH_TS_Thread_Status_Ready;//线程就绪
      MessageQueue->Owner=NULL;//清除占用标记

      //让PendSV处理链表


      //尝试添加到快速通道
      H_TS_ThreadAddToUnblockThread(_this,H_TS_Core->TimeUnblockThread,InIsThread1,OutIsThread1,retVal);
        
      if(retVal!=0){
        H_TS_Core->TimeBlockThreadsEvent=-1;
      }
      
    }
  }

  oH_TS_ResumeScheduler();//允许线程调度

  if(0!=H_TS_MessageFIFO_Read(MessageQueue->MessageFIFO,v0,v1)){
    //超时
    return -1;
  }

  return 0;
}



