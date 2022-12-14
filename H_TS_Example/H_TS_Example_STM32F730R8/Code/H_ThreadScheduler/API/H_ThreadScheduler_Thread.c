/*
 * @Author: 0x9DEFA478
 * @Date: 2021-08-13 17:22:44
 * @LastEditTime: 2021-10-24 21:36:37
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



/**
 * @brief ????????????
 * @param NumOfTick ?????????tick???
 * @return ???
 */
void H_TS_ThreadSleep(H_TS_Tick NumOfTick){
  H_TS_Thread* _this;
  H_TS_Thread* p_last;
  H_TS_Thread* p_next;

  _this=H_TS_Core->RunThreadPtr[0][1];

  if(NumOfTick<=0){
    //??????????????????0????????? ?????????????????????
    return;
  }

  oH_TS_SchedulerSuspend();//??????????????????

  _this->Status=vH_TS_Thread_Status_Sleep;//????????????

  //???_this????????????????????????
  p_last=_this->List.Ready.Last;
  p_next=_this->List.Ready.Next;
  if(p_last==NULL){
    H_TS_Core->ReadyThreads=p_next;
    while(p_next==NULL){
      //?????????????????????????????? H_TS_Core->ReadyThreads?????????NULL
    }
  }else{
    p_last->List.Ready.Next=p_next;
  }
  if(p_next!=NULL){
    p_next->List.Ready.Last=p_last;
  }

  //???_this?????????????????????(????????????????????????,????????????????????????)
  H_TS_ThreadAddToSleepList(_this,NumOfTick);

  oH_TS_ResumeScheduler();//??????????????????

}



