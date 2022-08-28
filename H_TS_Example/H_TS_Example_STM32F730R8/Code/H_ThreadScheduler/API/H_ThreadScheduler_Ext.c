/*
 * @Author: 0x9DEFA478
 * @Date: 2021-08-21 20:05:18
 * @LastEditTime: 2021-10-24 23:28:02
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
#include "H_ThreadScheduler_Core.h"
#include "..\LL\H_ThreadScheduler_LL.h"

extern H_TS* volatile H_TS_Core;



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
int H_TS_GetCPULoad(){
  return H_TS_Core->CPU_Load;
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

  oH_TS_CallPendSV();//触发PendSV

  for(;;){}
}

