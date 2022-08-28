/*
 * @Author: 0x9DEFA478
 * @Date: 2021-10-18 21:07:46
 * @LastEditTime: 2021-11-27 23:17:02
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
#ifndef __H_Printf_H_
#define __H_Printf_H_
#include "H_String.h"

typedef struct{
  void* Lock;
  int MaxLength;//保存Buffer保存字符串出现过的长度最大值
  unsigned char* Buffer;
}_H_Printf;

//extern _H_Printf _H_Printf_USBVCOM;
extern _H_Printf _H_Printf_USART1;


//打印端口初始化
void H_Printf_Init(void);


// //不要单独调用, 这个是为 宏H_Printf_USBVCOM 用的
// void* H_Printf_USBVCOM_CallBeforeFormat(void);

// //不要单独调用, 这个是为 宏H_Printf_USBVCOM 用的
// void H_Printf_USBVCOM_CallAfterFormat(int Length);

// //格式化打印到USB虚拟串口 仅能在线程中使用
// #define H_Printf_USBVCOM(Format,...)            H_Printf_USBVCOM_CallAfterFormat(\
//                                                   H_StringFormat(\
//                                                     H_Printf_USBVCOM_CallBeforeFormat()\
//                                                     ,Format,##__VA_ARGS__))


//不要单独调用, 这个是为 宏H_Printf_USART1 用的
void* H_Printf_USART1_CallBeforeFormat(void);

//不要单独调用, 这个是为 宏H_Printf_USART1 用的
void H_Printf_USART1_CallAfterFormat(int Length);

//格式化打印到串口 仅能在线程中使用
#define H_Printf_USART1(Format,...)             H_Printf_USART1_CallAfterFormat(\
                                                  H_StringFormat(\
                                                    H_Printf_USART1_CallBeforeFormat()\
                                                    ,Format,##__VA_ARGS__))

//C标准格式化输出 到串口 仅能在线程中使用
#define Printf_USART1(Format,...)               H_Printf_USART1_CallAfterFormat(\
                                                  H_sprintf(\
                                                    H_Printf_USART1_CallBeforeFormat()\
                                                    ,Format,##__VA_ARGS__))


//默认打印调用
#define H_Printf                                H_Printf_USART1

#define Printf                                  Printf_USART1

#endif //__H_Printf_H_
