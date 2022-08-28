/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-01 21:37:17
 * @LastEditTime: 2022-01-12 15:34:14
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
#ifndef __LL_H_ThreadScheduler_UserConfig_H_
#define __LL_H_ThreadScheduler_UserConfig_H_
#include "H_Type.h"//这个必须包含 H_TS内部会使用


//用户头文件
#define iH_TS_UserInclude                                          "Peripheral.h"


#define H_TS_TickType                                              Hint32
#define H_TS_TimeType                                              Hint32
#define vH_TS_ThreadMaxPriorityValue                               0x7FFFFFFF
#define vH_TS_IsEnableStackUsageStatistics                         1
#define vH_TS_BlockThreadListNum                                   1
#define vH_TS_IsStoreThreadRetVal                                  1
#define vH_TS_IsEnableCPULoadCalculate                             1
#define vH_TS_CPULoad_T                                            400000




#define vH_TS_EnableDebug                                          0
#define vH_TS_DebugOutput(Format,...)                              __disable_irq();while(1)




#endif //__LL_H_ThreadScheduler_UserConfig_H_