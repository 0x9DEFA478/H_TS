/*
 * @Author: 0x9DEFA478
 * @Date: 2021-10-18 21:06:09
 * @LastEditTime: 2022-01-13 20:47:29
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



//???????????????????????????
//#define vH_Printf_USBVCOM_BufferSize            1024
#define vH_Printf_USART_BufferSize             1024



//_H_Printf _H_Printf_USBVCOM;
_H_Printf _H_Printf_USART;





// //??????????????????, ???????????? ???H_Printf_USBVCOM ??????
// void* H_Printf_USBVCOM_CallBeforeFormat(){
//   H_TS_LockLock(_H_Printf_USBVCOM.Lock);
//   return _H_Printf_USBVCOM.Buffer;
// }

// //??????????????????, ???????????? ???H_Printf_USBVCOM ??????
// void H_Printf_USBVCOM_CallAfterFormat(int Length){
  
//   if(Length>_H_Printf_USBVCOM.MaxLength){
//     _H_Printf_USBVCOM.MaxLength=Length;
//   }

//   I_VCOM_In(_H_Printf_USBVCOM.Buffer,Length);

//   H_TS_LockUnlock(_H_Printf_USBVCOM.Lock);
// }


//??????????????????, ???????????? ???H_Printf_USART ??????
void* H_Printf_USART_CallBeforeFormat(void){
  H_TS_LockLock(_H_Printf_USART.Lock);
  return _H_Printf_USART.Buffer;
}

//??????????????????, ???????????? ???H_Printf_USART ??????
void H_Printf_USART_CallAfterFormat(int Length){
  if(Length>_H_Printf_USART.MaxLength){
    _H_Printf_USART.MaxLength=Length;
  }

  I_DebugPort_Write(_H_Printf_USART.Buffer,Length);

  H_TS_LockUnlock(_H_Printf_USART.Lock);
}



//?????????????????????
void H_Printf_Init(){

  // _H_Printf_USBVCOM.MaxLength=0;
  // _H_Printf_USBVCOM.Lock=new_H_TS_Lock();
  // _H_Printf_USBVCOM.Buffer=MemoryMalloc_DTCM.Malloc(vH_Printf_USBVCOM_BufferSize);
  // if(cH_IsNull(_H_Printf_USBVCOM.Lock)||cH_IsNull(_H_Printf_USBVCOM.Buffer)){
  //   for(;;){
  //   }
  // }

  _H_Printf_USART.MaxLength=0;
  _H_Printf_USART.Lock=new_H_TS_Lock();
  _H_Printf_USART.Buffer=MemoryMalloc_SRAM.Malloc(vH_Printf_USART_BufferSize);
  if(cH_IsNull(_H_Printf_USART.Lock)||cH_IsNull(_H_Printf_USART.Buffer)){
    for(;;){
    }
  }
  
}



