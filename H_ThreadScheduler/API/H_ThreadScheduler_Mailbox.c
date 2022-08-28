/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-06 22:03:00
 * @LastEditTime: 2021-12-23 14:20:44
 * @LastEditors: 0x9DEFA478
 * @Description: 
 *    邮箱实现
 *       邮箱无发送对象限制(是否中断 数量等)
 *    对于接收,只有线程能接收邮箱
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




static void Mailbox_RequestReady_ISR(H_TS_Mailbox* Mailbox){
  H_TS_Thread* Owner;
  Hsize InIsThread1;
  Hsize OutIsThread1;

  int retVal;

  Owner=Mailbox->Owner;
  if(Owner!=NULL){
    //有线程正在等待邮箱

    switch(Owner->Status){
      case vH_TS_Thread_Status_Block:

        Owner->Status=vH_TS_Thread_Status_Ready;//线程就绪
        Mailbox->Owner=NULL;//清除占用标记

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

        break;
      case vH_TS_Thread_Status_TimeBlock:

        Owner->Status=vH_TS_Thread_Status_Ready;//线程就绪
        Mailbox->Owner=NULL;//清除占用标记

        //让PendSV处理链表
        

        //尝试添加到快速通道
        H_TS_ThreadAddToUnblockThread(Owner,H_TS_Core->TimeUnblockThread,InIsThread1,OutIsThread1,retVal);
        
        if(retVal!=0){
          H_TS_Core->TimeBlockThreadsEvent=-1;
        }

        oH_TS_ISR_CallScheduleISR();

        break;
      case vH_TS_Thread_Status_Ready:
        //这是重入导致的 虽然会导致一些重复操作 但不影响结果
        //直接返回即可
        break;

      default:
        for(;;){
          //H_TS出BUG了!!
        }
    }
  }
}

static void Mailbox_RequestReady(H_TS_Mailbox* Mailbox){
  H_TS_Thread* Owner;
  Hsize InIsThread1;
  Hsize OutIsThread1;

  int retVal;

  Owner=Mailbox->Owner;
  if(Owner!=NULL){
    //有线程正在等待邮箱

    switch(Owner->Status){
      case vH_TS_Thread_Status_Block:

        Owner->Status=vH_TS_Thread_Status_Ready;//线程就绪
        Mailbox->Owner=NULL;//清除占用标记

        //让PendSV处理链表


        //尝试添加到快速通道
        H_TS_ThreadAddToUnblockThread(Owner,H_TS_Core->UnblockThread,InIsThread1,OutIsThread1,retVal);

        if(retVal!=0){
#if vH_TS_BlockThreadListNum > 1
          H_TS_Core->BlockThreadsEvents[Owner->List.Block.ListId]=-1;
#endif
          H_TS_Core->BlockThreadsEvent=-1;
        }

        oH_TS_ResumeSchedulerFixed();

        break;
      case vH_TS_Thread_Status_TimeBlock:

        Owner->Status=vH_TS_Thread_Status_Ready;//线程就绪
        Mailbox->Owner=NULL;//清除占用标记

        //让PendSV处理链表
        

        //尝试添加到快速通道
        H_TS_ThreadAddToUnblockThread(Owner,H_TS_Core->TimeUnblockThread,InIsThread1,OutIsThread1,retVal);
        
        if(retVal!=0){
          H_TS_Core->TimeBlockThreadsEvent=-1;
        }

        oH_TS_ResumeSchedulerFixed();

        break;
      case vH_TS_Thread_Status_Ready:
        //这是被中断抢占导致的 虽然会导致一些重复操作 但不影响结果
        
        oH_TS_ResumeSchedulerAuto();
        break;

      default:
        for(;;){
          //H_TS出BUG了!!
        }
    }
  }
}

/**
 * @brief 向邮箱发送数据 中断调用
 * @param Mailbox 要操作的邮箱
 * @param Data 指向要发送的数据
 * @return 0:成功 -1:邮箱已满
 */
int H_TS_MailboxPost_ISR(H_TS_Mailbox* Mailbox,void* Data){
  int Result;
  Hsize Index;
  Hsize NextIndex;
  Hsize AddCnt;
  Hsize NumOfMailbox;

  Result=-1;
  AddCnt=0;
  NumOfMailbox=Mailbox->NumOfMailboxs;

  Index=Mailbox->ReEnter;//ReEnter每次调用后都会复原, 无需线程(中断)保护. 因为即使被抢占了, 抢占的线程(中断)在退出之前会将ReEnter复原
  if(Index<NumOfMailbox){
    do{

      NextIndex=Index+1;
      Mailbox->ReEnter=NextIndex;//标记占用
      AddCnt++;

      //尝试发送消息
      if(0==H_TS_Mailbox_Post(Mailbox->Mailboxs[Index],Data)){
        Result=0;
      }

      Index=NextIndex;

    }while((Result!=0)&&(Index<NumOfMailbox));//如果消息发送成功或者所有消息满退出循环

    NextIndex-=AddCnt;
    Mailbox->ReEnter=NextIndex;

    if(Result==0){
      //写入成功
      Mailbox->AddFlag=0;
      Mailbox_RequestReady_ISR(Mailbox);
    }
  }
  
  return Result;
}

/**
 * @brief 向邮箱发送数据
 * @param Mailbox 要操作的邮箱
 * @param Data 指向要发送的数据
 * @return 0:成功 -1:邮箱已满
 */
int H_TS_MailboxPost(H_TS_Mailbox* Mailbox,void* Data){
  int Result;
  Hsize Index;

  Result=-1;

  oH_TS_SchedulerSuspend();

  Index=Mailbox->ReEnter;//ReEnter每次调用后都会复原, 无需线程(中断)保护. 因为即使被抢占了, 抢占的线程(中断)在退出之前会将ReEnter复原

  oH_TS_DebugAssert(Index!=0,"线程访问有限是互斥的(由禁止调度保证), 不可能出现这种情况");

  (void)Index;

  Mailbox->ReEnter=1;//标记占用

  //尝试发送消息
  if(0==H_TS_Mailbox_Post(Mailbox->Mailboxs[0],Data)){
    Result=0;
  }
  
  Mailbox->ReEnter=0;
  
  if(Result==0){
    //写入成功
    Mailbox->AddFlag=0;
    Mailbox_RequestReady(Mailbox);
  }else{
    oH_TS_ResumeSchedulerAuto();
  }
  
  return Result;
}

/**
 * @brief 从邮箱中接收数据
 * @param Mailbox 要操作的邮箱
 * @param Data 指向容纳数据的内存
 * @return 无
 */
void H_TS_MailboxReceive(H_TS_Mailbox* Mailbox,void* Data){
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

  Mailbox->AddFlag=-1;

  if(0==H_TS_MailboxTryReceive(Mailbox,Data)){
    return;
  }

  //无信号量 开始阻塞

  oH_TS_SchedulerSuspend();

  _this->Status=vH_TS_Thread_Status_Block;
  _this->BlockObject.Block.BlockObject=(void*)&Mailbox->Owner;

  //从就绪链表里移除
  H_TS_ThreadRemoteThisFromReadyList(_this,p_next,p_last);

  //添加到阻塞链表
#if vH_TS_BlockThreadListNum > 1
  H_TS_ThreadAddToBlockList(_this,p_next,ListId);
#else
  H_TS_ThreadAddToBlockList(_this,p_next);
#endif

  //标记信号量拥有线程
  Mailbox->Owner=_this;

  if(Mailbox->AddFlag==0){
    if(Mailbox->Owner!=NULL){
      //在修改链表期间有信号量释放 中断不会触发PendSV
      
      _this->Status=vH_TS_Thread_Status_Ready;//线程就绪
      Mailbox->Owner=NULL;//清除占用标记

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

  if(0!=H_TS_MailboxTryReceive(Mailbox,Data)){
    //H_TS出现BUG了 这是不应该出现的情况
    for(;;){}
  }
  
}

/**
 * @brief 尝试从邮箱中接收数据
 * @param Mailbox 要操作的邮箱
 * @param Data 指向容纳数据的内存
 * @return 0:成功 其他:获取失败
 */
int H_TS_MailboxTryReceive(H_TS_Mailbox* Mailbox,void* Data){
  Hsize i;

  i=Mailbox->NumOfMailboxs;
  while(i>0){
    i--;
    if(0==H_TS_Mailbox_Receive(Mailbox->Mailboxs[i],Data)){
      return 0;
    }
  }

  return -1;
}

/**
 * @brief 从邮箱中接收数据 时间限制
 * @param Mailbox 要操作的邮箱
 * @param Data 指向容纳数据的内存
 * @param Timeout 最大等待时间
 * @return 0:成功 其他:获取失败
 */
int H_TS_MailboxReceive_Time(H_TS_Mailbox* Mailbox,void* Data,H_TS_Tick Timeout){
  H_TS_Thread* _this;
  H_TS_Thread* p_next;
  H_TS_Thread* p_last;
  Hsize InIsThread1;
  Hsize OutIsThread1;

  int retVal;

  _this=cH_TS_RunThread(H_TS_Core)[1];

  Mailbox->AddFlag=-1;

  if(0==H_TS_MailboxTryReceive(Mailbox,Data)){
    return 0;
  }

  if(Timeout<1){
    return -1;
  }

  //无信号量 开始阻塞

  oH_TS_SchedulerSuspend();

  _this->Status=vH_TS_Thread_Status_TimeBlock;
  _this->BlockObject.TimeBlock.BlockObject=(void*)&Mailbox->Owner;

  //从就绪链表里移除
  H_TS_ThreadRemoteThisFromReadyList(_this,p_next,p_last);

  //添加到时间限制阻塞链表
  H_TS_ThreadAddToTimeBlockList(_this,Timeout);

  //标记消息队列拥有线程
  Mailbox->Owner=_this;

  if(Mailbox->AddFlag==0){
    if(Mailbox->Owner!=NULL){
      //在修改链表期间有信号量释放 中断不会触发PendSV
      
      _this->Status=vH_TS_Thread_Status_Ready;//线程就绪
      Mailbox->Owner=NULL;//清除占用标记

      //让PendSV处理链表
      
      
      //尝试添加到快速通道
      H_TS_ThreadAddToUnblockThread(_this,H_TS_Core->TimeUnblockThread,InIsThread1,OutIsThread1,retVal);
        
      if(retVal!=0){
        H_TS_Core->TimeBlockThreadsEvent=-1;
      }
      
    }
  }

  oH_TS_ResumeScheduler();//允许线程调度

  if(0!=H_TS_MailboxTryReceive(Mailbox,Data)){
    //超时
    return -1;
  }

  return 0;
}



