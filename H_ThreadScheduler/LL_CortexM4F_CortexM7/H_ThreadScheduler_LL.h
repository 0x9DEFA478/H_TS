/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-01 20:51:38
 * @LastEditTime: 2022-01-12 18:43:20
 * @LastEditors: 0x9DEFA478
 * @Description: 底层定义头文件
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
#ifndef __LL_H_ThreadScheduler_LL_H_
#define __LL_H_ThreadScheduler_LL_H_
#include "../H_ThreadScheduler.h"




//用户包含, 用于包含用户的头文件. H_ThreadScheduler_LL.c H_ThreadScheduler.c会包含H_ThreadScheduler_LL.h
#include iH_TS_UserInclude




//栈标记值
#define vH_TS_StackMarkValue                                       0x9DEFA478U

//堆栈指针增长方向 0:地址减少方向 其他:地址增加方向
#define vH_TS_StackPtrDirection                                    0

//指针最大值 (不能为0)
#define vH_TS_VoidPtrMaxValue                                      ((void*)0xFFFFFFFFU)

//编译顺序屏障 使得编译出的代码 在oH_TS_CompileBarrier()之前的代码执行完毕后才进行其之后的代码(防优化)
#define oH_TS_CompileBarrier()                                     __schedule_barrier()

//数据内存屏障指令 该宏应该自带编译顺序屏蔽
//#define oH_TS_DataMemoryBarrier()                                  __DMB()

//数据同步屏障指令 该宏应该自带编译顺序屏蔽
//#define oH_TS_DataSynchronizationBarrier()                         __DSB()

//指令同步屏障指令 该宏应该自带编译顺序屏蔽
//#define oH_TS_InstructionSynchronizationBarrier()                  __ISB()




//触发调度中断 屏蔽类型根据平台使用合适的屏蔽 
//这个操作需是可重入的(或者有应付重入的措施)
#define oH_TS_CallScheduleISR()                                   do{\
                                                                    oH_TS_CompileBarrier();\
                                                                    SCB->ICSR=SCB_ICSR_PENDSVSET_Msk;\
                                                                    oH_TS_CompileBarrier();\
                                                                  }while(0)

//挂起任务调度
#define oH_TS_SchedulerSuspend()                                  do{\
                                                                    oH_TS_CompileBarrier();\
                                                                    H_TS_Core->SchedulerSuspend=-1;\
                                                                    oH_TS_CompileBarrier();\
                                                                  }while(0)

//恢复任务调度(线程用) 根据中断情况自动选择触发调度
#define oH_TS_ResumeSchedulerAuto()                               do{\
                                                                    oH_TS_CompileBarrier();\
                                                                    H_TS_Core->SchedulerSuspend=0;\
                                                                    if(H_TS_Core->SchedulerForISR!=0){\
                                                                      H_TS_Core->SchedulerForISR=0;\
                                                                      oH_TS_CallScheduleISR();\
                                                                    }\
                                                                    oH_TS_CompileBarrier();\
                                                                  }while(0)

//恢复任务调度(线程用) 并且固定触发一次PendSV
#define oH_TS_ResumeSchedulerFixed()                               do{\
                                                                    oH_TS_CompileBarrier();\
                                                                    H_TS_Core->SchedulerSuspend=0;\
                                                                    H_TS_Core->SchedulerForISR=0;\
                                                                    oH_TS_CallScheduleISR();\
                                                                  }while(0)

//恢复调度并触发调度(线程使用) 并等待线程调度完成
#define oH_TS_ResumeScheduler()                                   do{\
                                                                    oH_TS_CompileBarrier();\
                                                                    H_TS_Core->ResumeSchedulerDone=-1;\
                                                                    oH_TS_ResumeSchedulerFixed();\
                                                                    while(H_TS_Core->ResumeSchedulerDone!=0){}\
                                                                    oH_TS_CompileBarrier();\
                                                                  }while(0)

//中断中触发线程中断
#define oH_TS_ISR_CallScheduleISR()                               do{\
                                                                    oH_TS_CompileBarrier();\
                                                                    if(H_TS_Core->SchedulerSuspend!=0){\
                                                                      H_TS_Core->SchedulerForISR=-1;\
                                                                    }else{\
                                                                      oH_TS_CallScheduleISR();\
                                                                    }\
                                                                    oH_TS_CompileBarrier();\
                                                                  }while(0)


//挂起Tick
#define oH_TS_TickSuspend()                                       do{\
                                                                    oH_TS_CompileBarrier();\
                                                                    H_TS_Core->SysTickSuspend=-1;\
                                                                    oH_TS_CompileBarrier();\
                                                                  }while(0)

//恢复Tick
#define oH_TS_ReusmeTick()                                        do{\
                                                                    oH_TS_CompileBarrier();\
                                                                    H_TS_Core->SysTickSuspend=0;\
                                                                    oH_TS_CompileBarrier();\
                                                                  }while(0)

//线程调度时被调用(对于线程来说 相当于是调度后被调用)
#define oH_TS_ResumeSchedulerDone()                               do{\
                                                                    oH_TS_CompileBarrier();\
                                                                    H_TS_Core->ResumeSchedulerDone=0;\
                                                                    oH_TS_CompileBarrier();\
                                                                  }while(0)




#if vH_TS_IsEnableCPULoadCalculate != 0

extern H_TS_Time H_TS_GetDT_LastT;

/**
 * @brief 返回上次调用该函数是多少时间之前 该方法在使能CPU占用率统计后被使用
 * 根据平台修改
 * @param _retVal 上次调用该函数是多少时间之前
 */
#define H_TS_GetDT(_retVal) \
do{\
  H_TS_Time _H_TS_GetDT_nowT;\
  _H_TS_GetDT_nowT=(H_TS_Time)TIM5->CNT;\
  _retVal=_H_TS_GetDT_nowT-H_TS_GetDT_LastT;\
  H_TS_GetDT_LastT=_H_TS_GetDT_nowT;\
}while(0)

#endif




#endif //__LL_H_ThreadScheduler_LL_H_
