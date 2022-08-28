/*
 * @Author: 0x9DEFA478
 * @Date: 2021-08-13 17:22:44
 * @LastEditTime: 2021-12-02 22:16:55
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



/**
 * @brief 线程休眠
 * @param NumOfTick 休眠的tick数
 * @return 无
 */
void H_TS_ThreadSleep(H_TS_Tick NumOfTick){
  H_TS_Thread* _this;
  H_TS_Thread* p_next;
  H_TS_Thread* p_last;

  _this=cH_TS_RunThread(H_TS_Core)[1];

  if(NumOfTick<=0){
    //进来个不大于0的家伙 是不是不想活了
    return;
  }

  oH_TS_SchedulerSuspend();//挂起线程调度

  _this->Status=vH_TS_Thread_Status_Sleep;//修改状态

  //从就绪链表里移除
  H_TS_ThreadRemoteThisFromReadyList(_this,p_next,p_last);

  //将_this添加到睡眠链表(先解除睡眠的在前,后解除睡眠的在后)
  H_TS_ThreadAddToSleepList(_this,NumOfTick);

  oH_TS_ResumeScheduler();//允许线程调度

}



