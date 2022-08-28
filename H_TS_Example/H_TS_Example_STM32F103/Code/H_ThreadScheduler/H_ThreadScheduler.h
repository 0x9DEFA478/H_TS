/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-01 19:48:05
 * @LastEditTime: 2022-01-14 16:32:16
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
#include "./API/H_ThreadScheduler_Config.h"


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

  volatile Hsize Status;//线程状态

  volatile Hsize CPU_Load;
#if vH_TS_IsEnableCPULoadCalculate != 0
  volatile H_TS_Time CPU_Load_RunningTime;//线程运行时间
#endif  

  char* ThreadName;//线程名字 由用户直接访问 H_TS内部只读

  //线程是否不在就绪链表/副就绪链表中
  volatile Hsize IsNotReady;

  //链表相关
  union{

    //就绪链表
    struct{
      struct _H_TS_Thread* volatile Last;
      struct _H_TS_Thread* volatile Next;
      volatile Hsize IsSubReady;//是否在副就绪链表
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
#if vH_TS_BlockThreadListNum > 1
      volatile Hsize ListId;//链表ID
#endif
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
  struct{
    struct _H_TS_Thread* volatile Last;
    struct _H_TS_Thread* volatile Next;
  }AllList;

  

  void* StackPointer;//栈指针所在的位置(指针)
  void* Stack;//指向堆栈使用的内存
  Hsize StackSize;//堆栈大小 单位:字节

#if vH_TS_IsEnableStackUsageStatistics != 0
  volatile Hsize StackMaxUsage;//历史最大占用
#endif
#if vH_TS_IsStoreThreadRetVal !=0
  int RetVal;//线程返回值
#endif

  Huint32 Feature;
  volatile Hsize Priority;//优先级 仅影响线程在列表中的排列 值越小 在列表的位置就靠前 优先级高 若有两个线程优先级相同 实际优先级也是一高一低 取决于创建线程时线程加入列表的方式

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
  volatile Hsize Priority;//用于保存优先级 (优先级继承机制会使线程优先级被修改 此段用于备份原来的优先级)
  volatile Husize ReLockCnt;//重复锁定计数
  H_TS_Thread* volatile WaitLockThreads;//等待锁定该锁的线程
}H_TS_Lock;

//二值信号量定义
typedef struct{
  H_TS_Thread* volatile Owner;//锁定锁的线程
  volatile Hsize Value;//信号量的值 0/非0 此段同时也附带AddFlag的功能
}H_TS_BinarySemaphore;

//信号量定义
typedef struct{
  H_TS_Thread* volatile Owner;//因尝试获取信号量而阻塞线程
  volatile Hsize AddFlag;//当被Give成功, 该段清零 (H_TS利用此段某些情况下可以判断内部是否有信号量)
  void* Semaphore;//内部的信号量定义 在H_ThreadScheduler_Core.h中的struct _H_TS_Semaphore
}H_TS_Semaphore;

//消息队列定义
typedef struct{
  H_TS_Thread* volatile Owner;//因读取队列而阻塞的线程
  volatile Hsize AddFlag;//当被Send成功, 该段清零 (H_TS利用此段某些情况下可以判断内部是否有消息)
  void* MessageFIFO;//内部的消息队列FIFO定义 在H_ThreadScheduler_Core.h中的struct _H_TS_MessageFIFO
}H_TS_MessageQueue;

//邮箱定义
typedef struct{
  H_TS_Thread* volatile Owner;//因读取队列而阻塞的线程
  volatile Hsize AddFlag;//当被Post成功, 该段清零 (H_TS利用此段某些情况下可以判断内部是否有数据)

  volatile Hsize ReEnter;//重入数 发生重入时 此段指示重叠层数 用于决定当前上下文只能操作哪个邮箱
  Hsize NumOfMailboxs;//邮箱数量
  void** Mailboxs;//内部的邮箱FIFO定义 在H_ThreadScheduler_Core.h中的struct _H_TS_Mailbox

}H_TS_Mailbox;

//线程信息
typedef struct _H_TS_ThreadInfo{
  Hsize Priority;//优先级
  char* ThreadName;//线程名字 由用户直接访问 H_TS内部并不直接使用
  Hsize CPU_Load;//CPU占用率
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
int H_TS_GetCPU_Load(void);

/**
 * @brief 获取线程的CPU占用率 保证目标线程存在时调用
 * @param Thread 要获取的线程 如果为NULL 代表获取的是调用这个方法的线程
 * @return CPU使用率 单位0.1%
 */
int H_TS_GetThreadCPU_Load(H_TS_Thread* Thread);

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
H_TS_Thread* new_H_TS_Thread(Hsize StackSize,Huint32 Feature);

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
int H_TS_ThreadStart(H_TS_Thread* Thread,int (*Code)(void*),void* v,Hsize Priority);

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
int H_TS_ThreadStart_HighPriority(H_TS_Thread* Thread,int (*Code)(void*),void* v,Hsize refPriority);

/**
 * @brief 开始运行一个线程 这个方法于H_TS_ThreadStart()不同的地方在于:
 *          它不需要传入优先级, 它会自动将线程优先级设置为等于低于接近refPriority且空闲的一个优先级 但不会比空闲线程低
 * @param Thread 线程句柄 可以是已停止的线程句柄
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param refPriority 参考优先级
 * @return 0:成功 -1:Thread不为停止状态
 */
int H_TS_ThreadStart_LowPriority(H_TS_Thread* Thread,int (*Code)(void*),void* v,Hsize refPriority);

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param Priority 优先级
 * @param Feature 一些特性信息
 * @return 线程句柄 NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread(int (*Code)(void*),void* v,Hsize StackSize,Hsize Priority,Huint32 Feature);

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 *        它不需要传入优先级, 它会自动将线程优先级设置为当前所有线程的最高级还要高一级 即Priority为现有线程的最小值减1
 *          在存在空闲的高优先级的情况下(一般int有32位, 几乎不用考虑优先级用完的情况) 它不需要去避免优先级冲突的问题 这个方法很适合需要临时新建一个线程的情况
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param Feature 一些特性信息
 * @return 线程句柄 NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_MaxPriority(int (*Code)(void*),void* v,Hsize StackSize,Huint32 Feature);

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 *          它不需要传入优先级, 它会自动将线程优先级设置为当前所有线程的最低级还要低一级 即Priority为现有线程的最小值加1
 *          (这个"当前所有线程"不包括空闲线程 以保证空闲线程优先级是最低的)
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param Feature 一些特性信息
 * @return 线程句柄 NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_MinPriority(int (*Code)(void*),void* v,Hsize StackSize,Huint32 Feature);

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 *          它不需要传入优先级, 它会自动将线程优先级设置为等于高于接近refPriority且空闲的一个优先级
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param refPriority 参考优先级
 * @param Feature 一些特性信息
 * @return 线程句柄 NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_HighPriority(int (*Code)(void*),void* v,Hsize StackSize,Hsize refPriority,Huint32 Feature);

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 *          它不需要传入优先级, 它会自动将线程优先级设置为等于低于接近refPriority且空闲的一个优先级 但不会比空闲线程低
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param refPriority 参考优先级
 * @param Feature 一些特性信息
 * @return 线程句柄 NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_LowPriority(int (*Code)(void*),void* v,Hsize StackSize,Hsize refPriority,Huint32 Feature);

/**
 * @brief 结束线程自身
 * @param RetVal 线程返回值
 * @return 无
 */
void H_TS_ThreadExit(int RetVal);

/**
 * @brief 由线程调用, 获取线程本身的句柄
 * @return 线程本身的句柄
 */
H_TS_Thread* H_TS_GetThisThread(void);

/**
 * @brief 获取所有线程的信息
 * @return 线程信息链表, 如果为NULL则代表失败
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
 * @return 0:成功 其他:内存申请失败
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
 * @brief 锁定一个锁 如果锁正被别的线程占用, 则等待直到锁被释放
 * @param Lock 要锁定的锁对象
 * @return 无
 */
void H_TS_LockLock(H_TS_Lock* Lock);

/**
 * @brief 尝试锁定一个锁 如果锁正被别的线程占用, 返回一个失败值
 * @param Lock 要锁定的锁对象
 * @return 0:成功 其他:失败
 */
int H_TS_LockTryLock(H_TS_Lock* Lock);

/**
 * @brief 释放一个锁
 * @param Lock 要释放的锁
 * @return 无
 */
void H_TS_LockUnlock(H_TS_Lock* Lock);




//==============================================================================================================================
//
//       二值信号量相关方法
//
//==============================================================================================================================

/**
 * @brief 新建一个二值信号量
 * @return 二值信号量对象 如果为NULL 则失败
 */
H_TS_BinarySemaphore* new_H_TS_BinarySemaphore(void);

/**
 * @brief 删除二值信号量
 * @param BinarySemaphore 要删除的二值信号量对象
 * @return 0:成功 -1:有线程因此邮箱阻塞
 */
int delete_H_TS_BinarySemaphore(H_TS_BinarySemaphore* BinarySemaphore);

/**
 * @brief 释放一个二值信号量 由中断调用
 * @param BinarySemaphore 要操作的二值信号量
 * @return 0:成功 -1:队列已满
 */
int H_TS_BinarySemaphoreGive_ISR(H_TS_BinarySemaphore* BinarySemaphore);

/**
 * @brief 释放一个二值信号量
 * @param BinarySemaphore 要操作的二值信号量
 * @return 0:成功 -1:队列已满
 */
int H_TS_BinarySemaphoreGive(H_TS_BinarySemaphore* BinarySemaphore);

/**
 * @brief 获取二值信号量
 * @param BinarySemaphore 要操作的二值信号量
 * @return 如果无信号量 将一直阻塞 直到获取成功
 */
void H_TS_BinarySemaphoreTake(H_TS_BinarySemaphore* BinarySemaphore);

/**
 * @brief 尝试获取二值信号量
 * @param BinarySemaphore 要操作的二值信号量
 * @return 0:成功 其他:获取失败
 */
int H_TS_BinarySemaphoreTryTake(H_TS_BinarySemaphore* BinarySemaphore);

/**
 * @brief 获取二值信号量 时间限制
 * @param BinarySemaphore 要操作的二值信号量
 * @param Timeout 最大等待时间
 * @return 0:成功 其他:获取失败
 */
int H_TS_BinarySemaphoreTake_Time(H_TS_BinarySemaphore* BinarySemaphore,H_TS_Tick Timeout);




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
H_TS_Semaphore* new_H_TS_Semaphore(Hsize MaxCount);

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
 * @param Timeout 最大等待时间
 * @return 0:成功 其他:获取失败
 */
int H_TS_SemaphoreTake_Time(H_TS_Semaphore* Semaphore,H_TS_Tick Timeout);




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
H_TS_MessageQueue* new_H_TS_MessageQueue(Hsize NumOfMessage);

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
 * @param Timeout 最大等待时间
 * @return 0:成功 其他:获取失败
 */
int H_TS_MessageQueueReceive_Time(H_TS_MessageQueue* MessageQueue,void** v0,void** v1,H_TS_Tick Timeout);




//==============================================================================================================================
//
//       邮箱相关方法
//
//==============================================================================================================================

/**
 * @brief 新建一个邮箱
 * @param MaxNumOfMail 每一个内部邮箱的最大容纳个数+1 (最大容纳数=MaxNumOfMail-1)
 * @param MailSize 每个信息大小 单位:字节
 * @param NumOfMailboxs 内部邮箱个数
 * @return 邮箱对象 如果为NULL 则失败
 */
H_TS_Mailbox* new_H_TS_Mailbox(Hsize MaxNumOfMail,Hsize MailSize,Hsize NumOfMailboxs);

/**
 * @brief 删除一个邮箱
 * @param Mailbox 要删除的邮箱对象
 * @return 0:成功 -1:有线程因此邮箱阻塞
 */
int delete_H_TS_Mailbox(H_TS_Mailbox* Mailbox);

/**
 * @brief 向邮箱发送数据 中断调用
 * @param Mailbox 要操作的邮箱
 * @param Data 指向要发送的数据
 * @return 0:成功 -1:邮箱已满
 */
int H_TS_MailboxPost_ISR(H_TS_Mailbox* Mailbox,void* Data);

/**
 * @brief 向邮箱发送数据
 * @param Mailbox 要操作的邮箱
 * @param Data 指向要发送的数据
 * @return 0:成功 -1:邮箱已满
 */
int H_TS_MailboxPost(H_TS_Mailbox* Mailbox,void* Data);

/**
 * @brief 从邮箱中接收数据
 * @param Mailbox 要操作的邮箱
 * @param Data 指向容纳数据的内存
 * @return 无
 */
void H_TS_MailboxReceive(H_TS_Mailbox* Mailbox,void* Data);

/**
 * @brief 尝试从邮箱中接收数据
 * @param Mailbox 要操作的邮箱
 * @param Data 指向容纳数据的内存
 * @return 0:成功 其他:获取失败
 */
int H_TS_MailboxTryReceive(H_TS_Mailbox* Mailbox,void* Data);

/**
 * @brief 从邮箱中接收数据 时间限制
 * @param Mailbox 要操作的邮箱
 * @param Data 指向容纳数据的内存
 * @param Timeout 最大等待时间
 * @return 0:成功 其他:获取失败
 */
int H_TS_MailboxReceive_Time(H_TS_Mailbox* Mailbox,void* Data,H_TS_Tick Timeout);




#endif //__H_ThreadScheduler_H_
