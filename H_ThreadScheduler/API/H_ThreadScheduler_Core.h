/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-03 14:30:05
 * @LastEditTime: 2022-06-24 16:13:19
 * @LastEditors: 0x9DEFA478
 * @Description: H_TS内部头文件
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
#ifndef __API_H_ThreadScheduler_Core_H_
#define __API_H_ThreadScheduler_Core_H_
#include "../H_ThreadScheduler.h"
#include "./H_ThreadScheduler_Config.h"




typedef struct{
    H_TS_Thread* volatile Thread0;
    H_TS_Thread* volatile Thread1;
    volatile Hsize InIsThread1;
    volatile Hsize OutIsThread1;
    volatile Hsize ReEnter;
}H_TS_UnblockThread;

typedef struct{

  void* RunThreadPtr;                            //指向底层线程控制句柄

  volatile Hsize SchedulerSuspend;               //当此段不为0时挂起线程调度
  volatile Hsize SysTickSuspend;                 //当此段不为0时停止响应定时器回调事件
  volatile Hsize SchedulerForISR;                //当此段不为0时有中断请求调用PendSV
  volatile Hsize ResumeSchedulerDone;            //当恢复调度成功后, 该段被置零

  volatile Hsize CPU_Load;                       //CPU占用率
#if vH_TS_IsEnableCPULoadCalculate != 0
  volatile H_TS_Time CPU_Load_RunningTime;       //总运行时间
#endif

  void (* volatile IdleProcess)(void*);

  H_TS_Thread* IdleThread;                       //空闲线程句柄
  volatile Hsize IdleThreadResume;

  //内存管理函数集合
  struct{
    void* (*Malloc)(Hsize);
    void (*Free)(void*);
  }Memory;


  H_TS_Thread* volatile Threads;                 //线程链表


  H_TS_Thread* volatile ExitThreads;             //等待结束的线程链表
  H_TS_Thread* volatile WaitAddThreads;          //等待添加到Threads中的线程


  H_TS_Thread* volatile ReadyThreads;            //就绪线程链表
  H_TS_Thread* volatile SubReadyThreads;         //副就绪线程链表 能使高优先级的线程免于因为低优先级解阻塞导致过多查询链表的麻烦 用于快速放置低优先级的就绪线程(直接添加到头部 不进行排序, 在SubReadyThreads有高于ReadyThreads优先级的线程时才开始排序添加到ReadyThreads)
  volatile Hsize SubReadyThreadsHighestPriority; //副就绪线程链表中最高优先级的线程的优先级 当SubReadyThreads不为NULL时有效


  volatile Hsize SleepThreadsEvent;              //当此段不为0时SleepThreads有更新
  H_TS_Thread* volatile SleepThreads;            //睡眠线程链表


  volatile Hsize TimeBlockThreadsEvent;          //当此段不为0时TimeBlockThreads有更新
  H_TS_Thread* volatile TimeBlockThreads;        //时间限制阻塞线程链表


  volatile Hsize BlockThreadsEvent;              //当此段不为0时BlockThreads有更新
#if vH_TS_BlockThreadListNum > 1
  volatile Hsize BlockThreadsNextAddIndex;       //下一个阻塞线程要加到的链表ID
  volatile Hsize BlockThreadsEvents[vH_TS_BlockThreadListNum];//当此段不为0时对应的BlockThreads有更新
  H_TS_Thread* volatile BlockThreads[vH_TS_BlockThreadListNum];//阻塞线程链表(多个链表)
#else
  H_TS_Thread* volatile BlockThread;             //阻塞线程链表
#endif

  H_TS_UnblockThread UnblockThread;              //解阻塞线程快速通道
  H_TS_UnblockThread TimeUnblockThread;          //时间限制解阻塞快速通道

}H_TS;

#define cH_TS_RunThread(h_ts)                                     ((void* volatile * volatile *)h_ts->RunThreadPtr)[0]

struct _H_TS_Semaphore{
  volatile Hsize I;
  volatile Hsize O;
  Hsize Max;
};

struct _H_TS_MessageFIFO{
  void* volatile * Messages;
  volatile Hsize I;
  volatile Hsize O;
  Hsize NumOfMessages;
};

struct _H_TS_Mailbox{
  volatile Hbyte* Mails;
  Hsize MailSize;
  volatile Hsize I;
  volatile Hsize O;
  Hsize Max;
};




void* H_TS_Semaphore_Create(Hsize MaxCount);

void H_TS_Semaphore_Delete(void* Semaphore);

/**
 * @brief 信号量增加
 * @param Semaphore 要增加的信号量
 * @return 0:成功 其他:失败
 */
int H_TS_Semaphore_Add(struct _H_TS_Semaphore* Semaphore);

/**
 * @brief 信号量减少
 * @param Semaphore 要减少的信号量
 * @return 0:成功 其他:失败
 */
int H_TS_Semaphore_Sub(struct _H_TS_Semaphore* Semaphore);




void* H_TS_MessageFIFO_Create(Hsize NumOfMessages);

void H_TS_MessageFIFO_Delete(void* MessageFIFO);

/**
 * @brief 消息写入
 * @param MessageFIFO 消息FIFO
 * @param v0 指针0
 * @param v1 指针1
 * @return 0:成功 其他:失败
 */
int H_TS_MessageFIFO_Write(struct _H_TS_MessageFIFO* MessageFIFO,void* v0,void* v1);

/**
 * @brief 读取消息
 * @param MessageFIFO 消息FIFO
 * @param v0 用于接收指针0
 * @param v1 用于接收指针1
 * @return 0:成功 其他:失败
 */
int H_TS_MessageFIFO_Read(struct _H_TS_MessageFIFO* MessageFIFO,void** v0,void** v1);




void* H_TS_Mailbox_Create(Hsize MaxNumOfMail,Hsize MailSize);

void H_TS_Mailbox_Delete(void* Mailbox);

/**
 * @brief 数据写入
 * @param Mailbox 邮箱
 * @param Data 数据
 * @return 0:成功 其他:失败
 */
int H_TS_Mailbox_Post(struct _H_TS_Mailbox* Mailbox,void* Data);

/**
 * @brief 读取数据
 * @param Mailbox 邮箱
 * @param Data 指向容纳数据的内存
 * @return 0:成功 其他:失败
 */
int H_TS_Mailbox_Receive(struct _H_TS_Mailbox* Mailbox,void* Data);




/**
 * @brief 添加线程到就绪链表
 * @param _Thread 要添加的线程 不会修改此变量
 * @param _p_temp 暂用线程指针 会修改此变量
 * @param _Priority 占用优先级变量 会修改此变量
 * @param _retVal 返回值
 * @return 0:添加到非头部 其他:添加到头部
 */
#define H_TS_ThreadAddToReadyList(_Thread,_p_temp,_Priority,_retVal) \
do{\
  _Thread->IsNotReady=0;\
  _p_temp=H_TS_Core->ReadyThreads;\
  oH_TS_DebugAssert(_p_temp==NULL,"不可能出现无就绪线程的情况(至少存在一个空闲线程)");\
  _Priority=_Thread->Priority;\
  if(_p_temp->Priority<_Priority){\
    _p_temp=H_TS_Core->SubReadyThreads;\
    _Thread->List.Ready.IsSubReady=-1;\
    if(_p_temp==NULL){\
      H_TS_Core->SubReadyThreadsHighestPriority=_Priority;\
      _Thread->List.Ready.Last=NULL;\
      _Thread->List.Ready.Next=NULL;\
      H_TS_Core->SubReadyThreads=_Thread;\
    }else{\
      if(H_TS_Core->SubReadyThreadsHighestPriority>_Priority){\
        H_TS_Core->SubReadyThreadsHighestPriority=_Priority;\
      }\
      _Thread->List.Ready.Last=NULL;\
      _Thread->List.Ready.Next=_p_temp;\
      _p_temp->List.Ready.Last=_Thread;\
      H_TS_Core->SubReadyThreads=_Thread;\
    }\
    _retVal=0;\
  }else{\
    _Thread->List.Ready.IsSubReady=0;\
    _Thread->List.Ready.Last=NULL;\
    _Thread->List.Ready.Next=_p_temp;\
    _p_temp->List.Ready.Last=_Thread;\
    H_TS_Core->ReadyThreads=_Thread;\
    _retVal=-1;\
  }\
}while(0)

/**
 * @brief 添加尝试线程到快速通道
 * @param _Thread 要添加的线程
 * @param _UnblockThread 要添加到的通道
 * @param _InIsThread1 暂用变量 会修改此变量
 * @param _OutIsThread1 暂用变量 会修改此变量
 * @param _retVal 0:成功 其他:失败
 */
#define H_TS_ThreadAddToUnblockThread(_Thread,_UnblockThread,_InIsThread1,_OutIsThread1,_retVal) \
do{\
  if(_UnblockThread.ReEnter==0){\
    _UnblockThread.ReEnter=1;\
    _InIsThread1=_UnblockThread.InIsThread1;\
    _OutIsThread1=_UnblockThread.OutIsThread1;\
    if(_InIsThread1==_OutIsThread1){\
      if(_InIsThread1==0){\
        _UnblockThread.Thread0=_Thread;\
        _UnblockThread._InIsThread1=-1;\
      }else{\
        _UnblockThread.Thread1=_Thread;\
        _UnblockThread._InIsThread1=0;\
      }\
      _retVal=0;\
    }else{\
      _retVal=-1;\
    }\
    _UnblockThread.ReEnter=0;\
  }else{\
    _retVal=-1;\
  }\
}while(0)

/**
 * @brief 固定添加线程链表到就绪链表
 * @param Thread 要添加的线程链表
 * @return 无
 */
void H_TS_ThreadListForceAddToReadyList(H_TS_Thread* Thread);

/**
 * @brief 将当前线程从就绪链表中移除
 * @param __this 当前线程 该变量不会被修改
 * @param _p_next 暂用线程指针 会修改此变量
 * @param _p_last 暂用线程指针 会修改此变量
 * @return 无
 */
#define H_TS_ThreadRemoteThisFromReadyList(__this,_p_next,_p_last) \
do{\
  __this->IsNotReady=-1;\
  _p_last=__this->List.Ready.Last;\
  _p_next=__this->List.Ready.Next;\
  oH_TS_DebugAssert(_p_next==NULL,"至少存在一个空闲线程 要移除的线程不可能是空闲线程 即p_next不可能为NULL");\
  oH_TS_DebugAssert(_p_last!=NULL,"在单核CPU中 p_last的值肯定为NULL");\
  H_TS_Core->ReadyThreads=_p_next;\
  _p_next->List.Ready.Last=NULL;\
  _p_last=H_TS_Core->SubReadyThreads;\
  if(_p_last!=NULL){\
    if(_p_next->Priority>H_TS_Core->SubReadyThreadsHighestPriority){\
      H_TS_Core->SubReadyThreads=NULL;\
      H_TS_ThreadListForceAddToReadyList(_p_last);\
    }\
  }\
}while(0)

/**
 * @brief 将线程从就绪链表中移除
 * @param _Thread 当前线程 该变量不会被修改
 * @param _p_next 暂用线程指针 会修改此变量
 * @param _p_last 暂用线程指针 会修改此变量
 * @return 无
 */
#define H_TS_ThreadRemoteFromReadyList(_Thread,_p_next,_p_last) \
do{\
  _Thread->IsNotReady=-1;\
  if(_Thread->List.Ready.IsSubReady==0){\
    _p_last=_Thread->List.Ready.Last;\
    _p_next=_Thread->List.Ready.Next;\
    oH_TS_DebugAssert(_p_next==NULL,"至少存在一个空闲线程 要移除的线程不可能是空闲线程 即p_next不可能为NULL");\
    if(_p_last==NULL){\
      H_TS_Core->ReadyThreads=_p_next;\
    }else{\
      _p_last->List.Ready.Next=_p_next;\
    }\
    _p_next->List.Ready.Last=_p_last;\
    _p_last=H_TS_Core->SubReadyThreads;\
    if(_p_last!=NULL){\
      if(_p_next->Priority>H_TS_Core->SubReadyThreadsHighestPriority){\
        H_TS_Core->SubReadyThreads=NULL;\
        H_TS_ThreadListForceAddToReadyList(_p_last);\
      }\
    }\
  }else{\
    _p_last=H_TS_Core->SubReadyThreads;\
    oH_TS_DebugAssert(_p_last==NULL,"线程在副就绪链表中 此段不可能为NULL");\
    H_TS_Core->SubReadyThreads=NULL;\
    H_TS_ThreadListForceAddToReadyList(_p_last);\
    _p_last=_Thread->List.Ready.Last;\
    _p_next=_Thread->List.Ready.Next;\
    oH_TS_DebugAssert(_p_next==NULL,"至少存在一个空闲线程 要移除的线程不可能是空闲线程 即_p_next不可能为NULL");\
    if(_p_last==NULL){\
      H_TS_Core->ReadyThreads=_p_next;\
    }else{\
      _p_last->List.Ready.Next=_p_next;\
    }\
    _p_next->List.Ready.Last=_p_last;\
  }\
}while(0)

#if vH_TS_BlockThreadListNum > 1
/**
 * @brief 添加线程到阻塞链表
 * @param _Thread 要添加的线程 不会修改此变量
 * @param _p_next 暂用线程指针 会修改此变量
 * @param _ListId 链表ID号    会修改此变量
 * @return 无
 */
#define H_TS_ThreadAddToBlockList(_Thread,_p_next,_ListId) \
do{\
  _ListId=H_TS_Core->BlockThreadsNextAddIndex;\
  _p_next=H_TS_Core->BlockThreads[_ListId];\
  _Thread->List.Block.ListId=_ListId;\
  _Thread->List.Block.Next=_p_next;\
  _Thread->List.Block.Last=NULL;\
  if(_p_next!=NULL){\
    _p_next->List.Block.Last=_Thread;\
  }\
  H_TS_Core->BlockThreads[_ListId]=_Thread;\
  if(_ListId==(vH_TS_BlockThreadListNum-1)){\
    H_TS_Core->BlockThreadsNextAddIndex=0;\
  }else{\
    H_TS_Core->BlockThreadsNextAddIndex=_ListId+1;\
  }\
}while(0)
#else
/**
 * @brief 添加线程到阻塞链表
 * @param _Thread 要添加的线程 不会修改此变量
 * @param _p_next 暂用线程指针 会修改此变量
 * @return 无
 */
#define H_TS_ThreadAddToBlockList(_Thread,_p_next) \
do{\
  _p_next=H_TS_Core->BlockThread;\
  _Thread->List.Block.Next=_p_next;\
  _Thread->List.Block.Last=NULL;\
  if(_p_next!=NULL){\
    _p_next->List.Block.Last=_Thread;\
  }\
  H_TS_Core->BlockThread=_Thread;\
}while(0)
#endif

/**
 * @brief 将线程添加到睡眠(Sleep)链表
 * @param Thread 要添加的线程
 * @param NumOfTick 睡眠tick数
 * @return 无
 */
void H_TS_ThreadAddToSleepList(H_TS_Thread* Thread,H_TS_Tick NumOfTick);

/**
 * @brief 将线程添加到时间限制阻塞(TimeBlock)链表
 * @param Thread 要添加的线程
 * @param timeOut 最大等待时间
 * @return 无
 */
void H_TS_ThreadAddToTimeBlockList(H_TS_Thread* Thread,H_TS_Tick timeOut);




#endif //__API_H_ThreadScheduler_Core_H_
