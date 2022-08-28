/*
 * @Author: 0x9DEFA478
 * @Date: 2021-10-18 21:06:09
 * @LastEditTime: 2021-12-05 22:08:43
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
#include "H_Printf.h"
#include "H_ThreadScheduler.h"
#include "MemoryMalloc.h"
#include "I_DebugPort.h"



//最大生成字符串大小
//#define vH_Printf_USBVCOM_BufferSize            1024
#define vH_Printf_USART1_BufferSize             1024



//_H_Printf _H_Printf_USBVCOM;
_H_Printf _H_Printf_USART1;





// //不要单独调用, 这个是为 宏H_Printf_USBVCOM 用的
// void* H_Printf_USBVCOM_CallBeforeFormat(){
//   H_TS_LockLock(_H_Printf_USBVCOM.Lock);
//   return _H_Printf_USBVCOM.Buffer;
// }

// //不要单独调用, 这个是为 宏H_Printf_USBVCOM 用的
// void H_Printf_USBVCOM_CallAfterFormat(int Length){
  
//   if(Length>_H_Printf_USBVCOM.MaxLength){
//     _H_Printf_USBVCOM.MaxLength=Length;
//   }

//   I_VCOM_In(_H_Printf_USBVCOM.Buffer,Length);

//   H_TS_LockUnlock(_H_Printf_USBVCOM.Lock);
// }


//不要单独调用, 这个是为 宏H_Printf_USART1 用的
void* H_Printf_USART1_CallBeforeFormat(void){
  H_TS_LockLock(_H_Printf_USART1.Lock);
  return _H_Printf_USART1.Buffer;
}

//不要单独调用, 这个是为 宏H_Printf_USART1 用的
void H_Printf_USART1_CallAfterFormat(int Length){
  if(Length>_H_Printf_USART1.MaxLength){
    _H_Printf_USART1.MaxLength=Length;
  }

  I_DebugPort_Write(_H_Printf_USART1.Buffer,Length);

  H_TS_LockUnlock(_H_Printf_USART1.Lock);
}



//打印端口初始化
void H_Printf_Init(){

  // _H_Printf_USBVCOM.MaxLength=0;
  // _H_Printf_USBVCOM.Lock=new_H_TS_Lock();
  // _H_Printf_USBVCOM.Buffer=MemoryMalloc_SRAM.Malloc(vH_Printf_USBVCOM_BufferSize);
  // if(cH_IsNull(_H_Printf_USBVCOM.Lock)||cH_IsNull(_H_Printf_USBVCOM.Buffer)){
  //   for(;;){
  //   }
  // }

  _H_Printf_USART1.MaxLength=0;
  _H_Printf_USART1.Lock=new_H_TS_Lock();
  _H_Printf_USART1.Buffer=MemoryMalloc_SRAM.Malloc(vH_Printf_USART1_BufferSize);
  if(cH_IsNull(_H_Printf_USART1.Lock)||cH_IsNull(_H_Printf_USART1.Buffer)){
    for(;;){
    }
  }
  
}



