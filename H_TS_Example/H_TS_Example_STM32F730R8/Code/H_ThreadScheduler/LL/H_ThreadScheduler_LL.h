/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-01 20:51:38
 * @LastEditTime: 2021-10-24 21:48:28
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
#ifndef __H_ThreadScheduler_LL_H_
#define __H_ThreadScheduler_LL_H_
#include "..\H_ThreadScheduler.h"

//用户包含, 用于包含用户的头文件. H_ThreadScheduler_LL.c H_ThreadScheduler.c会包含H_ThreadScheduler_LL.h
#include iH_TS_UserInclude



//栈标记值
#define vH_TS_StackMarkValue        0x9DEFA478U







//触发PendSV
#define oH_TS_CallPendSV()                                        do{\
                                                                    __schedule_barrier();\
                                                                    SCB->ICSR|=SCB_ICSR_PENDSVSET_Msk;\
                                                                    __schedule_barrier();\
                                                                  }while(0)

//挂起任务调度
#define oH_TS_SchedulerSuspend()                                  do{\
                                                                    __schedule_barrier();\
                                                                    H_TS_Core->SchedulerSuspend=-1;\
                                                                    __schedule_barrier();\
                                                                  }while(0)

//恢复任务调度(线程用) 根据中断情况自动选择触发调度
#define oH_TS_ResumeSchedulerAuto()                               do{\
                                                                    __schedule_barrier();\
                                                                    H_TS_Core->SchedulerSuspend=0;\
                                                                    __schedule_barrier();\
                                                                    if(H_TS_Core->SchedulerForIrq!=0){\
                                                                      H_TS_Core->SchedulerForIrq=0;\
                                                                      oH_TS_CallPendSV();\
                                                                    }\
                                                                     __schedule_barrier();\
                                                                  }while(0)

//恢复任务调度(线程用) 并且固定触发一次PendSV
#define oH_TS_ResumeSchedulerFixed()                               do{\
                                                                    __schedule_barrier();\
                                                                    H_TS_Core->SchedulerSuspend=0;\
                                                                    __schedule_barrier();\
                                                                    if(H_TS_Core->SchedulerForIrq!=0){\
                                                                      H_TS_Core->SchedulerForIrq=0;\
                                                                    }\
                                                                    oH_TS_CallPendSV();\
                                                                    __schedule_barrier();\
                                                                  }while(0)

//中断中触发PendSV
#define oH_TS_ISR_CallPendSV()                                    do{\
                                                                    __schedule_barrier();\
                                                                    if(H_TS_Core->SchedulerSuspend!=0){\
                                                                      H_TS_Core->SchedulerForIrq=-1;\
                                                                    }else{\
                                                                      oH_TS_CallPendSV();\
                                                                    }\
                                                                    __schedule_barrier();\
                                                                  }while(0)

//恢复调度并触发调度(线程使用) 并等待线程调度完成
#define oH_TS_ResumeScheduler()                                   do{\
                                                                    __schedule_barrier();\
                                                                    H_TS_Core->ResumeSchedulerDone=-1;\
                                                                    oH_TS_ResumeSchedulerFixed();\
                                                                    while(H_TS_Core->ResumeSchedulerDone!=0){}\
                                                                    __schedule_barrier();\
                                                                  }while(0)


//挂起Tick
#define oH_TS_TickSuspend()                                       do{\
                                                                    __schedule_barrier();\
                                                                    H_TS_Core->SysTickSuspend=-1;\
                                                                    __schedule_barrier();\
                                                                  }while(0)

//恢复Tick
#define oH_TS_ReusmeTick()                                        do{\
                                                                    __schedule_barrier();\
                                                                    H_TS_Core->SysTickSuspend=0;\
                                                                    __schedule_barrier();\
                                                                  }while(0)

//线程调度时被调用(对于线程来说 相当于是调度后被调用)
#define oH_TS_ResumeSchedulerDone()                               do{\
                                                                    __schedule_barrier();\
                                                                    H_TS_Core->ResumeSchedulerDone=0;\
                                                                    __schedule_barrier();\
                                                                  }while(0)




#endif //__H_ThreadScheduler_LL_H_
