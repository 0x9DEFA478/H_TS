/*
 * @Author: 0x9DEFA478
 * @Date: 2021-08-21 19:58:33
 * @LastEditTime: 2022-01-12 15:17:32
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
#include "./H_ThreadScheduler_LL.h"
#include "../API/H_ThreadScheduler_Core.h"

extern H_TS* H_TS_Core;


extern void* H_TS_StartFirstThread;//在 LL_ASM中

extern void* volatile * volatile H_TS_RunThread;
extern void* H_TS_ScheduleISR_Call;



/**
 * @brief 设置Tick回调 并且使能
 * 根据平台修改
 * @param callback 使能Tick回调
 * @param v 回调传入参数
 * @return 无
 */
void H_TS_EnableTickCallback(void (*callback)(void*),void* v){
  //该方法在开始调度时被调用
  //在这里将callback设置为Tick回调 使得每次Tick中断callback都会被调用一次

  TimeTick_Set_IRQ_Callback(callback,v);//Tick回调设置
}

/**
 * @brief 底层初始化
 * 根据平台修改
 * @return 无
 */
void H_TS_LL_Init(){
  //该方法在开始调度时被调用

  //FPU->FPCCR|=FPU_FPCCR_ASPEN_Msk|FPU_FPCCR_LSPEN_Msk;//使能Control自动设置与FPU惰性压栈
  HAL_NVIC_SetPriority(PendSV_IRQn, 15U, 0U);//设置PendSV优先级为最低

  //线程相关指针配置
  H_TS_ScheduleISR_Call=(void*)&H_TS_StartFirstThread;//PendSV调用设置为H_TS_StartFirstThread
  H_TS_RunThread=H_TS_Core->ReadyThreads->StackPointer;
  H_TS_Core->RunThreadPtr=(void*)&H_TS_RunThread;
  
}
