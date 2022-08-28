/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-01 19:48:05
 * @LastEditTime: 2021-10-24 23:28:59
 * @LastEditors: 0x9DEFA478
 * @Description: 线程调度器总头文件 使用时,包含该文件即可
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
#ifndef __H_ThreadScheduler_H_
#define __H_ThreadScheduler_H_
#include ".\LL\H_ThreadScheduler_Config.h"


//线程的Status字段小于0代表线程将要停止, 不是停止就是在停止的路上

//线程已结束运行
#define vH_TS_Thread_Status_Stop         -2

//线程请求停止状态
#define vH_TS_Thread_Status_RequestStop  -1

//线程就绪
#define vH_TS_Thread_Status_Ready         0

//线程因事件阻塞
#define vH_TS_Thread_Status_Block         1

//线程因睡眠阻塞
#define vH_TS_Thread_Status_Sleep         2

//线程因事件阻塞(时间限制)
#define vH_TS_Thread_Status_TimeBlock     3



//线程结束后不自动回收占用的资源
#define vH_TS_Thread_Feature_NoAutoFree     0x00000001U


//线程定义
typedef struct _H_TS_Thread{

  volatile int Status;//线程状态

#if vH_TS_isUseThreadName != 0
  char* ThreadName;//线程名字 由用户直接访问 H_TS内部只读
#endif

  //链表相关
  union{

    //就绪链表
    struct{
      struct _H_TS_Thread* volatile Last;
      struct _H_TS_Thread* volatile Next;
    }Ready;

    //因睡眠而阻塞
    struct{
      struct _H_TS_Thread* volatile Last;
      struct _H_TS_Thread* volatile Next;
    }Sleep;

    //因等待阻塞(无限等待)
    struct{
      struct _H_TS_Thread* volatile Last;
      struct _H_TS_Thread* volatile Next;
      volatile int ListId;//链表ID 
    }Block;

    //因等待阻塞(超时限制)
    struct{
      struct _H_TS_Thread* volatile Last;
      struct _H_TS_Thread* volatile Next;
    }TimeBlock;

    //待退出的线程链表
    struct{
      struct _H_TS_Thread* volatile Next;
    }Exit;

  }List;

  //这个链表存放着所有未结束的线程
  struct
  {
    struct _H_TS_Thread* volatile Last;
    struct _H_TS_Thread* volatile Next;
  }AllList;

  

  void* StackPointer;//栈指针所在的位置(指针)
  void* Stack;//指向堆栈使用的内存
  Hsize StackSize;//堆栈大小 单位:字节

#if vH_TS_isEnableStackUsageStatistics != 0
  volatile Hsize StackMaxUsage;//历史最大占用
#endif
#if vH_TS_isStoreThreadRetVal !=0
  int RetVal;//线程返回值
#endif

  Huint32 Feature;
  int Priority;//优先级 仅影响线程在列表中的排列 值越小 在列表的位置就靠前 优先级高 若有两个线程优先级相同 实际优先级也是一高一低 取决于创建线程时线程加入列表的方式

  //阻塞对象
  union{
    volatile H_TS_Tick SleepTick;//用于标记睡眠时间的位端,不一定表示还有多久解除阻塞 (线程在阻塞链表头部时,表示阻塞时间,如果不在头部,还需要加上之前链表的阻塞时间)

    //阻塞
    struct{
      void* volatile BlockObject;//阻塞对象
    }Block;

    //时间限制阻塞
    struct{
      void* volatile BlockObject;//阻塞对象 解除阻塞后被清零
      volatile H_TS_Tick SleepTick;//用于标记睡眠时间的位端,不一定表示还有多久解除阻塞 (线程在阻塞链表头部时,表示阻塞时间,如果不在头部,还需要加上之前链表的阻塞时间)
    }TimeBlock;
  }BlockObject;
  
}H_TS_Thread;

//锁定义
typedef struct{
  H_TS_Thread* volatile Owner;//锁定锁的线程
  volatile unsigned int ReLockCnt;//重复锁定计数
  H_TS_Thread* volatile WaitLockThreads;//等待锁定该锁的线程
}H_TS_Lock;

//信号量定义
typedef struct{
  H_TS_Thread* volatile Owner;//因尝试获取信号量而阻塞线程

  void* Semaphore;
  
}H_TS_Semaphore;

//消息队列定义
typedef struct{
  H_TS_Thread* volatile Owner;//因读取队列而阻塞的线程

  void* MessageFIFO;
  
}H_TS_MessageQueue;

//复合队列定义
typedef struct{
  H_TS_Thread* volatile Owner;//因读取队列而阻塞的线程

  struct{
    volatile int ReEnter;
    int NumOfSemaphores;
    void** Semaphores;
  }Semaphore_List;

  struct{
    volatile int ReEnter;
    int NumOfMessageFIFOs;
    void** MessageFIFOs;
  }MessageQueue_List;

}H_TS_MultipleQueue;

//复合队列读取参数
typedef struct{
  int IsMessage;//0表示读取到的是一个信号量 其他代表读取到的是一个消息
  void* v0;//存放消息空间的指针0
  void* v1;//存放消息空间的指针1
}H_TS_MultipleQueueTakeReceive_Param;

//线程信息
typedef struct _H_TS_ThreadInfo{
  int Priority;//优先级
  char* ThreadName;//线程名字 由用户直接访问 H_TS内部并不直接使用
  Hsize StackMaxUsage;//历史最大占用
  Hsize StackSize;//堆栈大小 单位:字节
  struct _H_TS_ThreadInfo* Next;
}H_TS_ThreadInfo;


//==============================================================================================================================
//
//       普通方法
//
//==============================================================================================================================

/**
 * @brief 挂起线程调度 调度器挂起期间 不能使用任何H_TS的方法(防止这些方法恢复了任务调度)
 * @return 无
 */
void H_TS_SchedulerSuspend(void);

/**
 * @brief 恢复任务调度  不能使用任何H_TS的方法(防止这些方法恢复了任务调度)
 * @param isCallPendSV 0:由调度器决定是否触发PendSV 其他:返回前触发PendSV
 * @return 无
 */
void H_TS_ResumeScheduler(int isCallPendSV);

/**
 * @brief 获取CPU使用率
 * @return CPU使用率 单位0.1%
 */
int H_TS_GetCPULoad(void);

/**
 * @brief 设置空闲函数空闲时调用的方法
 * @param IdleProcess 空闲回调
 * @return 无
 */
void H_TS_SetIdleProcess(void (*IdleProcess)(void*));

/**
 * @brief 获取堆栈内存占用
 * @return 堆栈内存占用率 单位0.1%
 */
int H_TS_GetStackUtilization(void);

/**
 * @brief 开始线程调度 使用此方法之前 必须调用H_TS_Init()
 * @return 正常情况 该方法不会返回
 */
void H_TS_StartScheduler(void);




//==============================================================================================================================
//
//       线程相关方法
//
//==============================================================================================================================

/**
 * @brief 创建一个线程句柄 分配栈空间 (此方法并不会开始运行线程,需使用H_TS_ThreadStart()线程才会运行)
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param Feature 一些特性信息
 * @return 线程句柄 如果为NULL 代表失败
 */
H_TS_Thread* new_H_TS_Thread(int StackSize,Huint32 Feature);

/**
 * @brief 释放一个线程占用的资源
 * @param Thread 要释放的线程
 * @return 0:成功 -1:失败 线程未完全停止
 */
int delete_H_TS_Thread(H_TS_Thread* Thread);

/**
 * @brief 开始运行一个线程 如果未开始线程调度 线程将在开始线程调度后运行
 * @param Thread 线程句柄 可以是已停止的线程句柄
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param Priority 优先级
 * @return 0:成功 -1:Thread不为停止状态 -2:优先级无效
 */
int H_TS_ThreadStart(H_TS_Thread* Thread,int (*Code)(void*),void* v,int Priority);

/**
 * @brief 开始运行一个线程 这个方法于H_TS_ThreadStart()不同的地方在于:
 *          它不需要传入优先级, 它会自动将线程优先级设置为当前所有线程的最高级还要高一级 即Priority为现有线程的最小值减1
 *          在存在空闲的高优先级的情况下(一般int有32位, 几乎不用考虑优先级用完的情况) 它不需要去避免优先级冲突的问题 这个方法很适合需要临时新建一个线程的情况
 * @param Thread 线程句柄 可以是已停止的线程句柄
 * @param Code 线程代码
 * @param v 线程传入参数
 * @return 0:成功 -1:Thread不为停止状态
 */
int H_TS_ThreadStart_MaxPriority(H_TS_Thread* Thread,int (*Code)(void*),void* v);

/**
 * @brief 开始运行一个线程 这个方法于H_TS_ThreadStart()不同的地方在于:
 *          它不需要传入优先级, 它会自动将线程优先级设置为当前所有线程的最低级还要低一级 即Priority为现有线程的最小值加1
 *          (这个"当前所有线程"不包括空闲线程 以保证空闲线程优先级是最低的)
 * @param Thread 线程句柄 可以是已停止的线程句柄
 * @param Code 线程代码
 * @param v 线程传入参数
 * @return 0:成功 -1:Thread不为停止状态
 */
int H_TS_ThreadStart_MinPriority(H_TS_Thread* Thread,int (*Code)(void*),void* v);

/**
 * @brief 开始运行一个线程 这个方法于H_TS_ThreadStart()不同的地方在于:
 *          它不需要传入优先级, 它会自动将线程优先级设置为等于高于接近refPriority且空闲的一个优先级
 * @param Thread 线程句柄 可以是已停止的线程句柄
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param refPriority 参考优先级
 * @return 0:成功 -1:Thread不为停止状态
 */
int H_TS_ThreadStart_HighPriority(H_TS_Thread* Thread,int (*Code)(void*),void* v,int refPriority);

/**
 * @brief 开始运行一个线程 这个方法于H_TS_ThreadStart()不同的地方在于:
 *          它不需要传入优先级, 它会自动将线程优先级设置为等于低于接近refPriority且空闲的一个优先级 但不会比空闲线程低
 * @param Thread 线程句柄 可以是已停止的线程句柄
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param refPriority 参考优先级
 * @return 0:成功 -1:Thread不为停止状态
 */
int H_TS_ThreadStart_LowPriority(H_TS_Thread* Thread,int (*Code)(void*),void* v,int refPriority);

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param Priority 优先级
 * @param Feature 一些特性信息
 * @return 线程句柄 如果为NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread(int (*Code)(void*),void* v,int StackSize,int Priority,Huint32 Feature);

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 *        它不需要传入优先级, 它会自动将线程优先级设置为当前所有线程的最高级还要高一级 即Priority为现有线程的最小值减1
 *          在存在空闲的高优先级的情况下(一般int有32位, 几乎不用考虑优先级用完的情况) 它不需要去避免优先级冲突的问题 这个方法很适合需要临时新建一个线程的情况
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param Feature 一些特性信息
 * @return 线程句柄 如果为NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_MaxPriority(int (*Code)(void*),void* v,int StackSize,Huint32 Feature);

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 *          它不需要传入优先级, 它会自动将线程优先级设置为当前所有线程的最低级还要低一级 即Priority为现有线程的最小值加1
 *          (这个"当前所有线程"不包括空闲线程 以保证空闲线程优先级是最低的)
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param Feature 一些特性信息
 * @return 线程句柄 如果为NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_MinPriority(int (*Code)(void*),void* v,int StackSize,Huint32 Feature);

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 *          它不需要传入优先级, 它会自动将线程优先级设置为等于高于接近refPriority且空闲的一个优先级
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param refPriority 参考优先级
 * @param Feature 一些特性信息
 * @return 线程句柄 如果为NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_HighPriority(int (*Code)(void*),void* v,int StackSize,int refPriority,Huint32 Feature);

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 *          它不需要传入优先级, 它会自动将线程优先级设置为等于低于接近refPriority且空闲的一个优先级 但不会比空闲线程低
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param refPriority 参考优先级
 * @param Feature 一些特性信息
 * @return 线程句柄 如果为NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_LowPriority(int (*Code)(void*),void* v,int StackSize,int refPriority,Huint32 Feature);

/**
 * @brief 结束线程自身
 * @return 无
 */
void H_TS_ThreadExit(void);

/**
 * @brief 由线程调用, 获取线程本身的句柄
 * @return 线程本身的句柄
 */
H_TS_Thread* H_TS_GetThisThread(void);

/**
 * @brief 获取所有线程的信息
 * @return 线程信息链表
 */
H_TS_ThreadInfo* H_TS_ThreadInfoGet(void);

/**
 * @brief 释放线程信息链表
 * @param Info 线程信息
 * @return 无
 */
void H_TS_ThreadInfoFree(H_TS_ThreadInfo* Info);

/**
 * @brief 线程休眠
 * @param NumOfTick 休眠的tick数
 * @return 无
 */
void H_TS_ThreadSleep(H_TS_Tick NumOfTick);

/**
 * @brief 初始化线程调度器
 * @param v 空闲线程传入参数
 * @param IdleThreadStackSize 空闲线程堆栈大小
 * @return 0:成功 -1,-2,-3,-4:内存申请失败
 */
int H_TS_Init(void* v,Hsize IdleThreadStackSize);




//==============================================================================================================================
//
//       锁相关方法
//
//==============================================================================================================================

/**
 * @brief 新建一个锁
 * @return 锁对象 如果为NULL 则失败
 */
H_TS_Lock* new_H_TS_Lock(void);

/**
 * @brief 删除一个锁
 * @param Lock 要删除的锁对象
 * @return 0:成功 -1:锁正在被其他线程占用
 */
int delete_H_TS_Lock(H_TS_Lock* Lock);

/**
 * @brief 尝试锁定一个锁
 * @param Lock 要锁定的锁对象
 * @return 0:成功 其他:锁定失败 锁正在被其他线程占用
 */
int H_TS_TryLock(H_TS_Lock* Lock);

/**
 * @brief 锁定一个锁 如果锁正被别的线程占用,则等待直到锁被释放
 * @param Lock 要锁定的锁对象
 * @return 0:锁正常锁定 其他:锁已经被该线程锁定
 */
int H_TS_Locking(H_TS_Lock* Lock);

/**
 * @brief 释放一个锁 即使锁被同一线程重复锁定,该方法会将锁解锁
 * @param Lock 要释放的锁
 * @return 无
 */
void H_TS_UnLock(H_TS_Lock* Lock);




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
H_TS_Semaphore* new_H_TS_Semaphore(int MaxCount);

/**
 * @brief 删除一个信号量
 * @param Semaphore 要删除的信号量对象
 * @return 0:成功 -1:有线程因此信号量阻塞
 */
int delete_H_TS_Semaphore(H_TS_Semaphore* Semaphore);

/**
 * @brief 释放一个信号量 由中断调用
 * @param Semaphore 要操作的信号量
 * @return 0:成功 -1:队列已满
 */
int H_TS_SemaphoreGive_ISR(H_TS_Semaphore* Semaphore);

/**
 * @brief 释放一个信号量
 * @param Semaphore 要操作的信号量
 * @return 0:成功 -1:队列已满
 */
int H_TS_SemaphoreGive(H_TS_Semaphore* Semaphore);

/**
 * @brief 获取信号量
 * @param Semaphore 要操作的信号量
 * @return 如果无信号量 将一直阻塞 直到获取成功
 */
void H_TS_SemaphoreTake(H_TS_Semaphore* Semaphore);

/**
 * @brief 尝试获取信号量
 * @param Semaphore 要操作的信号量
 * @return 0:成功 其他:获取失败
 */
int H_TS_SemaphoreTryTake(H_TS_Semaphore* Semaphore);

/**
 * @brief 获取信号量 时间限制
 * @param Semaphore 要操作的信号量
 * @param timeOut 最大等待时间
 * @return 0:成功 其他:获取失败
 */
int H_TS_SemaphoreTake_Time(H_TS_Semaphore* Semaphore,H_TS_Tick timeOut);




//==============================================================================================================================
//
//       消息队列相关方法
//
//==============================================================================================================================

/**
 * @brief 新建一个消息队列
 * @param NumOfMessage 队列容量大小 队列最大可容纳(NumOfMessage-1)个消息 (全部占用与队列空的偏移指示是一致的,所以队列设计成不能占满)
 * @return 信号量对象 如果为NULL 则失败
 */
H_TS_MessageQueue* new_H_TS_MessageQueue(int NumOfMessage);

/**
 * @brief 删除一个消息队列
 * @param MessageQueue 要删除的消息队列对象
 * @return 0:成功 -1:有线程因此消息队列阻塞
 */
int delete_H_TS_MessageQueue(H_TS_MessageQueue* MessageQueue);

/**
 * @brief 向消息队列发送消息 由中断调用
 * @param MessageQueue 要操作的消息队列
 * @param v0 消息指针0
 * @param v1 消息指针1
 * @return 0:成功 -1:队列已满
 */
int H_TS_MessageQueueSend_ISR(H_TS_MessageQueue* MessageQueue,void* v0,void* v1);

/**
 * @brief 向消息队列发送消息
 * @param MessageQueue 要操作的消息队列
 * @param v0 消息指针0
 * @param v1 消息指针1
 * @return 0:成功 -1:队列已满
 */
int H_TS_MessageQueueSend(H_TS_MessageQueue* MessageQueue,void* v0,void* v1);

/**
 * @brief 从队列中读取消息
 * @param MessageQueue 要操作的队列
 * @param v0 存放消息空间的指针0
 * @param v1 存放消息空间的指针1
 * @return 如果无消息 将一直阻塞 直到获取成功
 */
void H_TS_MessageQueueReceive(H_TS_MessageQueue* MessageQueue,void** v0,void** v1);

/**
 * @brief 尝试从队列中读取消息
 * @param MessageQueue 要操作的队列
 * @param v0 存放消息空间的指针0
 * @param v1 存放消息空间的指针1
 * @return 0:成功 其他:获取失败
 */
int H_TS_MessageQueueTryReceive(H_TS_MessageQueue* MessageQueue,void** v0,void** v1);

/**
 * @brief 从队列中读取消息 时间限制
 * @param MessageQueue 要操作的队列
 * @param v0 存放消息空间的指针0
 * @param v1 存放消息空间的指针1
 * @param timeOut 最大等待时间
 * @return 0:成功 其他:获取失败
 */
int H_TS_MessageQueueReceive_Time(H_TS_MessageQueue* MessageQueue,void** v0,void** v1,H_TS_Tick timeOut);




//==============================================================================================================================
//
//       复合队列相关方法
//
//==============================================================================================================================

/**
 * @brief 新建一个复合队列
 * @param MaxCount_Array 一个数组, 表示每一个信号量的最大计数值
 * @param NumOfSemaphores 信号量数量
 * @param NumOfMessages_Array 一个数组, 表示每一个消息队列容量大小(最多只能添加到(容量-1)个消息)
 * @param NumOfMessageFIFOs 消息队列数量
 * @return 复合队列对象 如果为NULL 则失败
 */
H_TS_MultipleQueue* new_H_TS_MultipleQueue(int* MaxCount_Array,int NumOfSemaphores,int* NumOfMessages_Array,int NumOfMessageFIFOs);

/**
 * @brief 删除一个复合队列
 * @param MultipleQueue 要删除的复合队列对象
 * @return 0:成功 -1:有线程因此复合队列阻塞
 */
int delete_H_TS_MultipleQueue(H_TS_MultipleQueue* MultipleQueue);

/**
 * @brief 向复合队列释放信号量 中断与线程均可调用(不同的线程等同于不同的中断 每个线程访问的ID不能相同)
 * @param MultipleQueue 要操作的复合队列
 * @param SemaphoreIndex 信号量ID
 * @return 0:成功 -1:信号量已满
 */
int H_TS_MultipleQueueGive(H_TS_MultipleQueue* MultipleQueue,int SemaphoreIndex);

/**
 * @brief 向复合队列释放信号量 中断与线程均可调用(不同的线程等同于不同的中断 每个线程访问的ID不能相同)
 *        此方法无需输入ID 内部有重入统计自动选择ID
 *        对同一个MultipleQueue, 此方法不能与H_TS_MultipleQueueGive混用
 * @param MultipleQueue 要操作的复合队列
 * @return 0:成功 -1:信号量已满
 */
int H_TS_MultipleQueueGive_ReEnter(H_TS_MultipleQueue* MultipleQueue);

/**
 * @brief 向复合队列发送消息 由中断线程均可调用(不同的线程等同于不同的中断 每个线程访问的ID不能相同)
 * @param MultipleQueue 要操作的复合队列
 * @param MessageQueueIndex 消息队列ID
 * @param v0 消息指针0
 * @param v1 消息指针1
 * @return 0:成功 -1:队列已满
 */
int H_TS_MultipleQueueSend(H_TS_MultipleQueue* MultipleQueue,int MessageQueueIndex,void* v0,void* v1);

/**
 * @brief 向复合队列发送消息 由中断线程均可调用(不同的线程等同于不同的中断 每个线程访问的ID不能相同)
 *        此方法无需输入ID 内部有重入统计自动选择ID
 *        对同一个MultipleQueue, 此方法不能与H_TS_MultipleQueueSend混用
 * @param MultipleQueue 要操作的复合队列
 * @param v0 消息指针0
 * @param v1 消息指针1
 * @return 0:成功 -1:队列已满
 */
int H_TS_MultipleQueueSend_ReEnter(H_TS_MultipleQueue* MultipleQueue,void* v0,void* v1);

/**
 * @brief 从复合队列中接收信号量/读取消息
 * @param MultipleQueue 要操作的复合队列
 * @param Param 读取参数指针, 用于返回读取到的数据
 * @return 如果无消息 将一直阻塞 直到获取成功
 */
void H_TS_MultipleQueueTakeReceive(H_TS_MultipleQueue* MultipleQueue,H_TS_MultipleQueueTakeReceive_Param* Param);

/**
 * @brief 尝试从队列中读取消息/获取信号量
 * @param MultipleQueue 要操作的复合队列
 * @param Param 读取参数指针, 用于返回读取到的数据
 * @return 0:成功 其他:获取失败
 */
int H_TS_MultipleQueueTryTakeReceive(H_TS_MultipleQueue* MultipleQueue,H_TS_MultipleQueueTakeReceive_Param* Param);

/**
 * @brief 从队列中读取消息 时间限制
 * @param MultipleQueue 要操作的复合队列
 * @param Param 读取参数指针, 用于返回读取到的数据
 * @param timeOut 最大等待时间
 * @return 0:成功 其他:获取失败
 */
int H_TS_MultipleQueueTakeReceive_Time(H_TS_MultipleQueue* MultipleQueue,H_TS_MultipleQueueTakeReceive_Param* Param,H_TS_Tick timeOut);


#endif //__H_ThreadScheduler_H_
