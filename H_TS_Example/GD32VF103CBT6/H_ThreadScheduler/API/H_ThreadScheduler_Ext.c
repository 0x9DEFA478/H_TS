/*
 * @Author: 0x9DEFA478
 * @Date: 2021-08-21 20:05:18
 * @LastEditTime: 2021-12-26 22:07:08
 * @LastEditors: 0x9DEFA478
 * @Description: 
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



extern void H_TS_MemoryGetInfo(Hsize* FreeSize,Hsize* AllSize);
extern void H_TS_EnableTickCallback(void (*callback)(void*),void* v);
extern void H_TS_LL_Init(void);



extern void H_TS_TickCallback(void* v);



/**
 * @brief 挂起线程调度 调度器挂起期间 不能使用任何H_TS的方法(防止这些方法恢复了任务调度)
 * @return 无
 */
void H_TS_SchedulerSuspend(){
  oH_TS_SchedulerSuspend();//挂起线程调度
}

/**
 * @brief 恢复任务调度  不能使用任何H_TS的方法(防止这些方法恢复了任务调度)
 * @param isCallPendSV 0:由调度器决定是否触发PendSV 其他:返回前触发PendSV
 * @return 无
 */
void H_TS_ResumeScheduler(int isCallPendSV){

  if(isCallPendSV==0){
    oH_TS_ResumeSchedulerAuto();
  }else{
    oH_TS_ResumeSchedulerFixed();
  }
}

/**
 * @brief 获取CPU使用率
 * @return CPU使用率 单位0.1%
 */
int H_TS_GetCPU_Load(){
  return H_TS_Core->CPU_Load;
}

/**
 * @brief 获取线程的CPU占用率 保证目标线程存在时调用
 * @param Thread 要获取的线程 如果为NULL 代表获取的是调用这个方法的线程
 * @return CPU使用率 单位0.1%
 */
int H_TS_GetThreadCPU_Load(H_TS_Thread* Thread){
  if(Thread==NULL){
    Thread=cH_TS_RunThread(H_TS_Core)[1];
  }
  return Thread->CPU_Load;
}

/**
 * @brief 设置空闲函数空闲时调用的方法
 * @param IdleProcess 空闲回调
 * @return 无
 */
void H_TS_SetIdleProcess(void (*IdleProcess)(void*)){
  H_TS_Core->IdleProcess=IdleProcess;
}

/**
 * @brief 获取堆栈内存占用
 * @return 堆栈内存占用率 单位0.1%
 */
int H_TS_GetStackUtilization(){
  Hsize FreeSize;
  Hsize AllSize;

  oH_TS_SchedulerSuspend();

  H_TS_MemoryGetInfo(&FreeSize,&AllSize);

  oH_TS_ResumeSchedulerAuto();

  return 1000-1000*FreeSize/AllSize;
}

/**
 * @brief 开始线程调度 使用此方法之前 必须调用H_TS_Init()
 * @return 正常情况 该方法不会返回
 */
void H_TS_StartScheduler(){

  H_TS_LL_Init();
  H_TS_EnableTickCallback(H_TS_TickCallback,NULL);

  oH_TS_CallScheduleISR();//触发调度中断

  for(;;){}
}




//==============================================================================================================================
//
//       锁相关方法
//
//==============================================================================================================================

/**
 * @brief 新建一个锁
 * @return 锁对象 如果为NULL 则失败
 */
H_TS_Lock* new_H_TS_Lock(){
  H_TS_Lock* r;

  oH_TS_SchedulerSuspend();

  r=H_TS_Core->Memory.Malloc(sizeof(H_TS_Lock));
  if(r==NULL){
    oH_TS_ResumeSchedulerAuto();
    return NULL;
  }

  r->Owner=NULL;
  r->ReLockCnt=0;
  r->WaitLockThreads=NULL;

  oH_TS_ResumeSchedulerAuto();

  return r;
}

/**
 * @brief 删除一个锁
 * @param Lock 要删除的锁对象
 * @return 0:成功 -1:锁正在被其他线程占用
 */
int delete_H_TS_Lock(H_TS_Lock* Lock){

  oH_TS_SchedulerSuspend();

  if(Lock->Owner!=NULL){
    oH_TS_ResumeSchedulerAuto();
    return -1;
  }

  H_TS_Core->Memory.Free(Lock);

  oH_TS_ResumeSchedulerAuto();

  return 0;
}




//==============================================================================================================================
//
//       二值信号量相关方法
//
//==============================================================================================================================

/**
 * @brief 新建一个二值信号量
 * @return 二值信号量对象 如果为NULL 则失败
 */
H_TS_BinarySemaphore* new_H_TS_BinarySemaphore(){
  H_TS_BinarySemaphore* r;

  oH_TS_SchedulerSuspend();

  r=H_TS_Core->Memory.Malloc(sizeof(H_TS_BinarySemaphore));
  if(r==NULL){
    oH_TS_ResumeSchedulerAuto();
    return NULL;
  }

  r->Owner=NULL;
  r->Value=0;

  oH_TS_ResumeSchedulerAuto();

  return r;
}

/**
 * @brief 删除二值信号量
 * @param BinarySemaphore 要删除的二值信号量对象
 * @return 0:成功 -1:有线程因此邮箱阻塞
 */
int delete_H_TS_BinarySemaphore(H_TS_BinarySemaphore* BinarySemaphore){

  oH_TS_SchedulerSuspend();

  if(BinarySemaphore->Owner!=NULL){
    oH_TS_ResumeSchedulerAuto();
    return -1;
  }

  H_TS_Core->Memory.Free(BinarySemaphore);

  oH_TS_ResumeSchedulerAuto();

  return 0;
}




//==============================================================================================================================
//
//       信号量相关方法
//
//==============================================================================================================================

/**
 * @brief 新建一个信号量
 * @param MaxCount 最大信号量数量(如果为1 则相当于为二值信号量)
 * @return 信号量对象 如果为NULL 则失败
 */
H_TS_Semaphore* new_H_TS_Semaphore(Hsize MaxCount){
  H_TS_Semaphore* r;

  oH_TS_SchedulerSuspend();

  r=H_TS_Core->Memory.Malloc(sizeof(H_TS_Semaphore));
  if(r==NULL){
    oH_TS_ResumeSchedulerAuto();
    return NULL;
  }

  r->Semaphore=H_TS_Semaphore_Create(MaxCount);
  if(r->Semaphore==NULL){
    H_TS_Core->Memory.Free(r);
    oH_TS_ResumeSchedulerAuto();
    return NULL;
  }

  r->Owner=NULL;

  oH_TS_ResumeSchedulerAuto();

  return r;
}

/**
 * @brief 删除一个信号量
 * @param Semaphore 要删除的信号量对象
 * @return 0:成功 -1:有线程因此信号量阻塞
 */
int delete_H_TS_Semaphore(H_TS_Semaphore* Semaphore){

  oH_TS_SchedulerSuspend();

  if(Semaphore->Owner!=NULL){
    oH_TS_ResumeSchedulerAuto();
    return -1;
  }

  H_TS_Semaphore_Delete(Semaphore->Semaphore);
  H_TS_Core->Memory.Free(Semaphore);

  oH_TS_ResumeSchedulerAuto();

  return 0;
}




//==============================================================================================================================
//
//       消息队列相关方法
//
//==============================================================================================================================

/**
 * @brief 新建一个消息队列
 * @param NumOfMessage 队列容量大小 队列最大可容纳(NumOfMessage-1)个消息 (全部占用与队列空的偏移指示是一致的,所以队列设计成不能占满)
 * @return 消息队列对象 如果为NULL 则失败
 */
H_TS_MessageQueue* new_H_TS_MessageQueue(Hsize NumOfMessage){
  H_TS_MessageQueue* r;

  oH_TS_SchedulerSuspend();

  r=H_TS_Core->Memory.Malloc(sizeof(H_TS_MessageQueue));
  if(r==NULL){
    oH_TS_ResumeSchedulerAuto();
    return NULL;
  }

  r->MessageFIFO=H_TS_MessageFIFO_Create(NumOfMessage);
  if(r->MessageFIFO==NULL){
    H_TS_Core->Memory.Free(r);
    oH_TS_ResumeSchedulerAuto();
    return NULL;
  }

  r->Owner=NULL;

  oH_TS_ResumeSchedulerAuto();

  return r;
}

/**
 * @brief 删除一个消息队列
 * @param MessageQueue 要删除的消息队列对象
 * @return 0:成功 -1:有线程因此消息队列阻塞
 */
int delete_H_TS_MessageQueue(H_TS_MessageQueue* MessageQueue){

  oH_TS_SchedulerSuspend();

  if(MessageQueue->Owner!=NULL){
    oH_TS_ResumeSchedulerAuto();
    return -1;
  }

  H_TS_MessageFIFO_Delete(MessageQueue->MessageFIFO);
  H_TS_Core->Memory.Free(MessageQueue);

  oH_TS_ResumeSchedulerAuto();

  return 0;
}



//==============================================================================================================================
//
//       邮箱相关方法
//
//==============================================================================================================================

static void** H_TS_CreateObjects_Mailbox(void* (*Create)(Hsize,Hsize),void (*Delete)(void*),Hsize Param0,Hsize Param1,Hsize Num){
  void** r;
  Hsize i;

  r=H_TS_Core->Memory.Malloc(Num*sizeof(void*));
  if(r==NULL){
    return NULL;
  }

  for(i=0;i<Num;i++){
    r[i]=Create(Param0,Param1);
    if(r[i]==NULL){
      while(i>0){
        i--;
        Delete(r[i]);
      }
      H_TS_Core->Memory.Free(r);
      return NULL;
    }
  }

  return r;
}

static void H_TS_DeleteObjects_Mailbox(void (*Delete)(void*),void** ObjectArray,Hsize Num){
  while(Num>0){
    Num--;
    Delete(ObjectArray[Num]);
  }
  H_TS_Core->Memory.Free(ObjectArray);
}




/**
 * @brief 新建一个邮箱
 * @param MaxNumOfMail 每一个内部邮箱的最大容纳个数+1 (最大容纳数=MaxNumOfMail-1)
 * @param MailSize 每个信息大小 单位:字节
 * @param NumOfMailboxs 内部邮箱个数
 * @return 邮箱对象 如果为NULL 则失败
 */
H_TS_Mailbox* new_H_TS_Mailbox(Hsize MaxNumOfMail,Hsize MailSize,Hsize NumOfMailboxs){
  H_TS_Mailbox* r;

  oH_TS_SchedulerSuspend();

  r=H_TS_Core->Memory.Malloc(sizeof(H_TS_Mailbox));
  if(r==NULL){
    oH_TS_ResumeSchedulerAuto();
    return NULL;
  }

  r->Mailboxs=H_TS_CreateObjects_Mailbox(H_TS_Mailbox_Create,H_TS_Mailbox_Delete,MaxNumOfMail,MailSize,NumOfMailboxs);
  if(r->Mailboxs==NULL){
    H_TS_Core->Memory.Free(r);
    oH_TS_ResumeSchedulerAuto();
    return NULL;
  }

  r->NumOfMailboxs=NumOfMailboxs;
  
  r->Owner=NULL;
  r->ReEnter=0;

  oH_TS_ResumeSchedulerAuto();

  return r;
}

/**
 * @brief 删除一个邮箱
 * @param Mailbox 要删除的邮箱对象
 * @return 0:成功 -1:有线程因此邮箱阻塞
 */
int delete_H_TS_Mailbox(H_TS_Mailbox* Mailbox){

  oH_TS_SchedulerSuspend();

  if(Mailbox->Owner!=NULL){
    oH_TS_ResumeSchedulerAuto();
    return -1;
  }

  H_TS_DeleteObjects_Mailbox(H_TS_Semaphore_Delete,Mailbox->Mailboxs,Mailbox->NumOfMailboxs);
  H_TS_Core->Memory.Free(Mailbox);

  oH_TS_ResumeSchedulerAuto();

  return 0;
}



