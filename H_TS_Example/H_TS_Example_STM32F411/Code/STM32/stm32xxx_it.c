/*
 * @Author: 0x9DEFA478
 * @Date: 2021-09-18 23:20:59
 * @LastEditTime: 2021-12-05 22:03:00
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
#include "Peripheral.h"


static volatile IP_IRQ_Call_Def OTG_HS_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def OTG_FS_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};

static volatile IP_IRQ_Call_Def USART2_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};








void OTG_HS_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  OTG_HS_IRQ_Call.v=v;
  OTG_HS_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void OTG_FS_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  OTG_FS_IRQ_Call.v=v;
  OTG_FS_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}


void USART2_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  USART2_IRQ_Call.v=v;
  USART2_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}








void OTG_HS_IRQHandler(void){
  OTG_HS_IRQ_Call.IP_IRQ_Callback(OTG_HS_IRQ_Call.v);
}

void OTG_FS_IRQHandler(void){
  OTG_FS_IRQ_Call.IP_IRQ_Callback(OTG_FS_IRQ_Call.v);
}


void USART2_IRQHandler(){
  USART2_IRQ_Call.IP_IRQ_Callback(USART2_IRQ_Call.v);
}
