/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-02 22:24:58
 * @LastEditTime: 2022-01-04 21:18:48
 * @LastEditors: 0x9DEFA478
 * @Description: 
 * 锁实现
 * 锁只能给线程使用,锁定成功一个锁后,需要释放锁才能让别的线程锁定锁
 * 当有多个线程因尝试锁定同一个锁而阻塞时,锁对象中有个链表记录这些锁,并且按优先级排序.当锁被释放时,链表中最高优先级的线程将锁定这个锁
 * 线程锁定一个锁后,只有该线程才能解开这个锁
 * 锁可重复锁定 锁定之后也要使用同样数量的解锁方法来才能解开锁
 * 支持优先级继承
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
 * @brief 锁定一个锁 如果锁正被别的线程占用, 则等待直到锁被释放后锁定锁
 * @param Lock 要锁定的锁对象
 * @return 无
 */
void H_TS_LockLock(H_TS_Lock* Lock){
  H_TS_Thread* _this;
  H_TS_Thread* Owner;
  H_TS_Thread* p_next;
  H_TS_Thread* p_next_next;
  H_TS_Thread* p_last;

  Hsize Priority;
  //int retVal;

  _this=cH_TS_RunThread(H_TS_Core)[1];

  oH_TS_SchedulerSuspend();

  Owner=Lock->Owner;

  if(Owner==NULL){
    
    Lock->Owner=_this;
    Lock->Priority=_this->Priority;

    oH_TS_ResumeSchedulerAuto();
    return;
  }

  if(Owner==_this){

    Lock->ReLockCnt++;

    oH_TS_ResumeSchedulerAuto();
    return;
  }

  //锁已被其他线程锁定

  _this->Status=vH_TS_Thread_Status_Block;//修改状态

  //从就绪链表里移除
  H_TS_ThreadRemoteThisFromReadyList(_this,p_next,p_last);

  Priority=_this->Priority;

  //将_this添加到锁的阻塞链表中
  
  p_next=Lock->WaitLockThreads;
  if(p_next==NULL){
    //为空 添加到头部
    _this->List.Block.Last=NULL;
    _this->List.Block.Next=NULL;
    Lock->WaitLockThreads=_this;
  }else{
    //不为空 按优先级添加

    if(p_next->Priority<Priority){
      //按优先级添加

      p_next_next=p_next->List.Block.Next;
      for(;;){
        if(p_next_next!=NULL){
          p_next=p_next_next;

          if(p_next->Priority>=Priority){
            //找到插入点

            p_last=p_next->List.Block.Last;

            _this->List.Block.Next=p_next;
            _this->List.Block.Last=p_last;
            p_last->List.Block.Next=_this;
            p_next->List.Block.Last=_this;

            break;
          }

          p_next_next=p_next->List.Block.Next;
        }else{
          //无插入点 添加到尾部

          _this->List.Block.Next=NULL;
          _this->List.Block.Last=p_next;
          p_next->List.Block.Next=_this;

          break;
        }
      }
    }else{
      //优先级最高 放置到开头
      _this->List.Block.Last=NULL;
      _this->List.Block.Next=p_next;
      p_next->List.Block.Last=_this;
      Lock->WaitLockThreads=_this;
    }
  }

  
  if(Owner->Priority>Priority){
    //优先级处理 需要提升线程Owner的优先级

    Owner->Priority=Priority;
    
    if(Owner->IsNotReady==0){
      //线程在就绪链表中 移除再添加来重排序

      //从就绪/副就绪链表里移除
      H_TS_ThreadRemoteFromReadyList(Owner,p_next,p_last);

      //添加到就绪链表
      //H_TS_ThreadAddToReadyList(Owner,p_next,Priority,retVal);
      Owner->IsNotReady=0;
      p_next=H_TS_Core->ReadyThreads;
      oH_TS_DebugAssert(p_next==NULL,"不可能出现无就绪线程的情况(至少存在一个空闲线程)");
      oH_TS_DebugAssert(p_next->Priority<Priority,"正在运行的线程优先级是最高的, 继承的优先级也是就绪线程中最高的, 但现在有点不对劲");
      Owner->List.Ready.IsSubReady=0;
      Owner->List.Ready.Last=NULL;
      Owner->List.Ready.Next=p_next;
      p_next->List.Ready.Last=Owner;
      H_TS_Core->ReadyThreads=Owner;
      //retVal=-1;

      //(void)retVal;
    }

  }

  oH_TS_ResumeScheduler();//允许线程调度

}

/**
 * @brief 尝试锁定一个锁 如果锁正被别的线程占用, 返回一个失败值
 * @param Lock 要锁定的锁对象
 * @return 0:成功 其他:失败
 */
int H_TS_LockTryLock(H_TS_Lock* Lock){
  H_TS_Thread* _this;
  H_TS_Thread* Owner;
  int r;

  _this=cH_TS_RunThread(H_TS_Core)[1];

  oH_TS_SchedulerSuspend();

  Owner=Lock->Owner;

  r=-1;

  if(Owner==NULL){
    
    Lock->Owner=_this;
    Lock->Priority=_this->Priority;

    r=0;
    goto Return;
  }

  if(Owner==_this){

    Lock->ReLockCnt++;

    r=0;
    goto Return;
  }

  //锁已被其他线程锁定

Return:
  oH_TS_ResumeSchedulerAuto();
  return r;
}

/**
 * @brief 释放一个锁
 * @param Lock 要释放的锁
 * @return 无
 */
void H_TS_LockUnlock(H_TS_Lock* Lock){
  H_TS_Thread* _this;
  H_TS_Thread* p;
  H_TS_Thread* p_next;
  H_TS_Thread* p_last;
  Husize ReLockCnt;
  
  Hsize Priority;
  int retVal;

  _this=cH_TS_RunThread(H_TS_Core)[1];

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
    
    oH_TS_DebugAssert(Lock->Priority!=_this->Priority,"不应该出现的情况 等待线程里没有线程 并没有发生优先级继承");

    oH_TS_ResumeSchedulerAuto();
    return;
  }

  Priority=Lock->Priority;

  oH_TS_DebugAssert(Priority<_this->Priority,"不应该出现的情况 优先级继承机制只会把优先级提高 而现在优先级被降低了");
  
  if(_this->Priority<Priority){
    _this->Priority=Priority;//恢复优先级

    //优先级降低 移除再添加来重排序

    //从就绪/副就绪链表里移除
    H_TS_ThreadRemoteFromReadyList(_this,p_next,p_last);

    //添加到就绪链表
    H_TS_ThreadAddToReadyList(_this,p_next,Priority,retVal);
    //并不要理会retVal 因为优先级继承机制 此时肯定有一个高优先级的线程因本锁阻塞 并且即将要解锁

  }else{
    _this->Priority=Priority;//恢复优先级
  }
  
  p->Status=vH_TS_Thread_Status_Ready;//更新阻塞线程的状态
  Lock->Owner=p;//让新的线程锁定锁
  Lock->Priority=p->Priority;

  //从等待锁定链表中删除线程
  p_next=p->List.Block.Next;
  if(p_next!=NULL){
    p_next->List.Block.Last=NULL;
  }
  Lock->WaitLockThreads=p_next;

  //添加新锁定线程到就绪链表
  H_TS_ThreadAddToReadyList(p,p_next,Priority,retVal);
  if(retVal==0){
    oH_TS_ResumeSchedulerAuto();
  }else{
    oH_TS_ResumeSchedulerFixed();
  }
}



