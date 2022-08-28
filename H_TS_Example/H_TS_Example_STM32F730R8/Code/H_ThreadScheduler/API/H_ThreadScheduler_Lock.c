/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-02 22:24:58
 * @LastEditTime: 2021-10-24 21:32:15
 * @LastEditors: 0x9DEFA478
 * @Description: 
 * 锁实现
 * 锁只能给线程使用,锁定成功一个锁后,需要释放锁才能让别的线程锁定锁
 * 当有多个线程因尝试锁定同一个锁而阻塞时,锁对象中有个链表记录这些锁,并且按优先级排序.当锁被释放时,链表中最高优先级的线程将锁定这个锁
 * 线程锁定一个锁后,只有该线程才能解开这个锁
 * 锁可重复锁定 锁定之后也要使用同样数量的解锁方法来才能解开锁
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
#include "H_ThreadScheduler_Core.h"
#include "..\LL\H_ThreadScheduler_LL.h"

extern H_TS* volatile H_TS_Core;




//将线程添加到等待锁的链表中
static void LockAddWaitLockThreads(H_TS_Lock* Lock,H_TS_Thread* Thread){
  H_TS_Thread* p_next;
  H_TS_Thread* p_next_next;
  H_TS_Thread* p_last;

  p_next=Lock->WaitLockThreads;

  if(p_next==NULL){
    
    Thread->List.Block.Last=NULL;
    Thread->List.Block.Next=NULL;
    Lock->WaitLockThreads=Thread;
    return;
  }

  if(p_next->Priority > Thread->Priority){

    Thread->List.Block.Last=NULL;
    Thread->List.Block.Next=p_next;
    p_next->List.Block.Last=Thread;
    Lock->WaitLockThreads=Thread;

    return;

  }else{

    p_next_next=p_next->List.Block.Next;
    while(p_next_next!=NULL){
      p_next=p_next_next;

      if(p_next->Priority > Thread->Priority){
        //找到插入点

        p_last=p_next->List.Block.Last;

        Thread->List.Block.Next=p_next;
        Thread->List.Block.Last=p_last;
        p_last->List.Block.Next=Thread;
        p_next->List.Block.Last=Thread;

        return;
      }

      p_next_next=p_next->List.Block.Next;
    }
    //无插入点 添加到尾部

    Thread->List.Block.Next=NULL;
    Thread->List.Block.Last=p_next;
    p_next->List.Block.Next=Thread;
  }

  return;
}










/**
 * @brief 尝试锁定一个锁
 * @param Lock 要锁定的锁对象
 * @return 0:成功 其他:锁定失败 锁正在被其他线程占用
 */
int H_TS_TryLock(H_TS_Lock* Lock){
  H_TS_Thread* _this;
  H_TS_Thread* Owner;

  _this=H_TS_Core->RunThreadPtr[0][1];

  oH_TS_SchedulerSuspend();

  Owner=Lock->Owner;

  if(Owner==_this){
    Lock->ReLockCnt++;
    oH_TS_ResumeSchedulerAuto();
    return 0;
  }

  if(Owner!=NULL){
    oH_TS_ResumeSchedulerAuto();
    return -1;
  }

  Lock->Owner=_this;
  Lock->ReLockCnt=0;

  oH_TS_ResumeSchedulerAuto();

  return 0;
}

/**
 * @brief 锁定一个锁 如果锁正被别的线程占用,则等待直到锁被释放
 * @param Lock 要锁定的锁对象
 * @return 0:锁正常锁定 其他:锁已经被该线程锁定
 */
int H_TS_Locking(H_TS_Lock* Lock){
  H_TS_Thread* _this;
  H_TS_Thread* p_last;
  H_TS_Thread* p_next;
  H_TS_Thread* Owner;

  _this=H_TS_Core->RunThreadPtr[0][1];

  oH_TS_SchedulerSuspend();

  Owner=Lock->Owner;

  if(Owner==NULL){
    
    Lock->Owner=_this;
    Lock->ReLockCnt=0;

    oH_TS_ResumeSchedulerAuto();
    return 0;
  }

  if(Owner==_this){
    Lock->ReLockCnt++;
    oH_TS_ResumeSchedulerAuto();
    return -1;
  }

  //锁已被其他线程锁定

  _this->Status=vH_TS_Thread_Status_Block;//修改状态

  //将_this从就绪链表里删除
  p_last=_this->List.Ready.Last;
  p_next=_this->List.Ready.Next;
  if(p_last==NULL){
    H_TS_Core->ReadyThreads=p_next;
  }else{
    p_last->List.Ready.Next=p_next;
  }
  if(p_next!=NULL){
    p_next->List.Ready.Last=p_last;
  }

  //将_this添加到锁的阻塞链表中
  LockAddWaitLockThreads(Lock,_this);

  oH_TS_ResumeScheduler();//允许线程调度

  return 0;
}

/**
 * @brief 释放一个锁 即使锁被同一线程重复锁定,该方法会将锁解锁
 * @param Lock 要释放的锁
 * @return 无
 */
void H_TS_UnLock(H_TS_Lock* Lock){
  H_TS_Thread* _this;
  H_TS_Thread* p;
  H_TS_Thread* p_next;
  unsigned int ReLockCnt;

  _this=H_TS_Core->RunThreadPtr[0][1];

  if(Lock->Owner!=_this){
    //居然想解别人的锁
    return;
  }

  ReLockCnt=Lock->ReLockCnt;
  if(ReLockCnt>0){
    Lock->ReLockCnt=ReLockCnt-1;
    return;
  }

  oH_TS_SchedulerSuspend();

  p=Lock->WaitLockThreads;
  if(p==NULL){
    Lock->Owner=NULL;
    oH_TS_ResumeSchedulerAuto();
    return;
  }
  
  p->Status=vH_TS_Thread_Status_Ready;//更新阻塞线程的状态
  Lock->Owner=p;//让新的线程锁定锁

  //从等待锁定链表中删除线程
  p_next=p->List.Block.Next;
  if(p_next!=NULL){
    p_next->List.Block.Last=NULL;
  }
  Lock->WaitLockThreads=p_next;

  //添加新锁定线程到就绪链表
  if(H_TS_ThreadAddToReadyList(p)==0){
    oH_TS_ResumeSchedulerAuto();
  }else{
    oH_TS_ResumeSchedulerFixed();
  }
}

