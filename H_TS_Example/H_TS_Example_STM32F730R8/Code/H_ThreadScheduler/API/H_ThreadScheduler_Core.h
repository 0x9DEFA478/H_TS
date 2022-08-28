/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-03 14:30:05
 * @LastEditTime: 2021-10-24 21:36:41
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
#ifndef __H_ThreadScheduler_Core_H_
#define __H_ThreadScheduler_Core_H_
#include "..\H_ThreadScheduler.h"
#include "..\LL\H_ThreadScheduler_Config.h"

typedef struct
{
  void* volatile * volatile * RunThreadPtr;
  
  volatile int SchedulerSuspend;//当此段不为0时挂起线程调度
  volatile int SysTickSuspend;//当此段不为0时停止响应定时器回调事件
  volatile int SchedulerForIrq;//当此段不为0时有中断请求调用PendSV
  volatile int ResumeSchedulerDone;//当恢复调度成功后, 该段被置零

  volatile int CPU_Load;

  H_TS_Thread* IdleThread;//空闲线程句柄
#if vH_TS_isEnableStackUsageStatistics != 0
  volatile int IdleThreadResume;
#endif

  //就绪线程链表
  H_TS_Thread* volatile ReadyThreads;
  

  //睡眠线程链表
  H_TS_Thread* volatile SleepThreads;
  //当此段不为0时SleepThreads有更新
  volatile int SleepThreadsEvent;


  //阻塞线程链表(多个链表)
  H_TS_Thread* volatile BlockThreads[vH_TS_BlockThreadListNum];
  //当此段不为0时BlockThreads有更新
  volatile int BlockThreadsEvent;
  //当此段不为0时对应的BlockThreads有更新
  volatile int BlockThreadsEvents[vH_TS_BlockThreadListNum];
  //多个链表BlockThreads的节点数目信息
  volatile int BlockThreadsItemCnt[vH_TS_BlockThreadListNum];


  //时间限制阻塞线程链表
  H_TS_Thread* volatile TimeBlockThreads;
  //当此段不为0时TimeBlockThreads有更新
  volatile int TimeBlockThreadsEvent;


  //等待结束的线程链表
  H_TS_Thread* volatile ExitThreads;


  //线程链表
  H_TS_Thread* volatile Threads;


  void* volatile IdleProcess;

#if vH_TS_isEnableCPULoadCalculate != 0
  struct{
    volatile H_TS_Tick RunTickCnt;//总运行时间
    volatile H_TS_Tick IdleTickCnt;//空闲线程运行时间
  }CPULoadCalculator;
#endif

  


  //内存管理函数集合
  struct{
    void* (*Malloc)(Hsize);
    void (*Free)(void*);
  }Memory;
  
}H_TS;

struct _H_TS_Semaphore{
  volatile int I;
  volatile int O;
  int Max;
};

struct _H_TS_MessageFIFO{
  void* volatile * Messages;
  volatile int I;
  volatile int O;
  int NumOfMessages;
};




void* H_TS_Semaphore_Create(int MaxCount);

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

/**
 * @brief Semaphore是否大于0
 * @param Semaphore 信号量
 * @return 0:空 其他:不为空
 */
int H_TS_Semaphore_IsNoEmpty(struct _H_TS_Semaphore* Semaphore);




void* H_TS_MessageFIFO_Create(int NumOfMessages);

void H_TS_MessageFIFO_Delete(void* MessageFIFO);

/**
 * @brief 消息写入
 * @param MessageFIFO 消息FIFO
 * @return 0:成功 其他:失败
 */
int H_TS_MessageFIFO_Write(struct _H_TS_MessageFIFO* MessageFIFO,void* v0,void* v1);

/**
 * @brief 读取消息
 * @param MessageFIFO 消息FIFO
 * @return 0:成功 其他:失败
 */
int H_TS_MessageFIFO_Read(struct _H_TS_MessageFIFO* MessageFIFO,void** v0,void** v1);

/**
 * @brief MessageFIFO是否有消息
 * @param MessageFIFO 消息FIFO
 * @return 0:空 其他:不为空
 */
int H_TS_MessageFIFO_IsNoEmpty(struct _H_TS_MessageFIFO* MessageFIFO);




/**
 * @brief 添加线程到就绪链表 
 * @param Thread 要添加到的线程
 * @return 0:添加到非头部 其他:添加到头部
 */
int H_TS_ThreadAddToReadyList(H_TS_Thread* Thread);

/**
 * @brief 添加线程到阻塞(Block)链表
 * @param Thread 要添加的线程
 * @return 无
 */
void H_TS_ThreadAddToBlockList(H_TS_Thread* Thread);

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


#endif //__H_ThreadScheduler_Core_H_
