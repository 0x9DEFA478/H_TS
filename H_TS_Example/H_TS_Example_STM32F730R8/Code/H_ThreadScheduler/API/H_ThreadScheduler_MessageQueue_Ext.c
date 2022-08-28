/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-02 22:24:27
 * @LastEditTime: 2021-10-24 18:37:49
 * @LastEditors: 0x9DEFA478
 * @Description: 实现说明详见H_ThreadScheduler_MessageQueue.c
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



/**
 * @brief 新建一个消息队列
 * @param NumOfMessage 队列容量大小 队列最大可容纳(NumOfMessage-1)个消息 (全部占用与队列空的偏移指示是一致的,所以队列设计成不能占满)
 * @return 信号量对象 如果为NULL 则失败
 */
H_TS_MessageQueue* new_H_TS_MessageQueue(int NumOfMessage){
  H_TS_MessageQueue* r;

  oH_TS_SchedulerSuspend();

  r=H_TS_Core->Memory.Malloc(sizeof(H_TS_MessageQueue));
  if(r==NULL){
    oH_TS_ResumeSchedulerAuto();
    return NULL;
  }

  r->MessageFIFO=H_TS_MessageFIFO_Create(NumOfMessage);
  if(r->MessageFIFO==NULL){
    H_TS_Core->Memory.Free(r);
    oH_TS_ResumeSchedulerAuto();
    return NULL;
  }

  r->Owner=NULL;

  oH_TS_ResumeSchedulerAuto();

  return r;
}

/**
 * @brief 删除一个消息队列
 * @param MessageQueue 要删除的消息队列对象
 * @return 0:成功 -1:有线程因此消息队列阻塞
 */
int delete_H_TS_MessageQueue(H_TS_MessageQueue* MessageQueue){

  oH_TS_SchedulerSuspend();

  if(MessageQueue->Owner!=NULL){
    oH_TS_ResumeSchedulerAuto();
    return -1;
  }

  H_TS_MessageFIFO_Delete(MessageQueue->MessageFIFO);
  H_TS_Core->Memory.Free(MessageQueue);

  oH_TS_ResumeSchedulerAuto();

  return 0;
}

