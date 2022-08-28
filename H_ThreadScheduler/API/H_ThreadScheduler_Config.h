/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-01 21:37:17
 * @LastEditTime: 2022-01-23 15:27:22
 * @LastEditors: 0x9DEFA478
 * @Description: H_ThreadScheduler配置文件
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
#ifndef __API_H_ThreadScheduler_Config_H_
#define __API_H_ThreadScheduler_Config_H_
#include "../LL/H_ThreadScheduler_UserConfig.h"


//用户头文件 用于包含用户的头文件. H_ThreadScheduler_LL.c H_ThreadScheduler.c H_ThreadScheduler_LL.h 会包含它
#ifndef iH_TS_UserInclude
#error "iH_TS_UserInclude必须在H_ThreadScheduler_UserConfig.h定义"
#endif


//Tick计数使用的类型
#ifndef H_TS_TickType
#define H_TS_TickType Hint32
#endif
typedef H_TS_TickType H_TS_Tick;


//记时使用的类型 这个用于线程时间计算 用于生成CPU占用率
#ifndef H_TS_TimeType
#define H_TS_TimeType Hint32
#endif
typedef H_TS_TimeType H_TS_Time;


//最低优先级 数字越大 优先级越小
#ifndef vH_TS_ThreadMaxPriorityValue
#define vH_TS_ThreadMaxPriorityValue                               0x7FFFFFFF
#endif

//是否使能堆栈占用统计 0:关闭 其他:开启
#ifndef vH_TS_IsEnableStackUsageStatistics
#define vH_TS_IsEnableStackUsageStatistics                         1
#endif

//阻塞链表数 设置为 sqrt(阻塞任务数) 为最佳 (实际上阻塞任务数量随着程序运行是不定的 根据实际情况确定)
#ifndef vH_TS_BlockThreadListNum
#define vH_TS_BlockThreadListNum                                   1
#endif

//是否使用CPU使用率统计 0:关闭 其他:开启
#ifndef vH_TS_IsEnableCPULoadCalculate
#define vH_TS_IsEnableCPULoadCalculate                             1
#endif

//如果使能了CPU占用率统计(通过vH_TS_IsEnableCPULoadCalculate来设置) 该段表示每运行多少时间后更新CPU占用值(时间由H_TS_GetDT()来提供)
#ifndef vH_TS_CPULoad_T
#define vH_TS_CPULoad_T                                            400000
#endif




#ifndef vH_TS_EnableDebug
//是否使能Debug 如果使能 将加入一些判定代码
#define vH_TS_EnableDebug                                          1
#endif

#ifndef vH_TS_DebugOutput
//调试输出
#define vH_TS_DebugOutput(Format,...)                              while(1)
#endif

#if vH_TS_EnableDebug != 0
#define oH_TS_DebugAssert(IsAssert,Format,...)                     if((IsAssert)){vH_TS_DebugOutput(Format,##__VA_ARGS__);}
#else
#define oH_TS_DebugAssert(IsAssert,Format,...)
#endif




#if vH_TS_BlockThreadListNum < 1
#error "请使用合理的链表数"
#endif




#endif //__API_H_ThreadScheduler_Config_H_
