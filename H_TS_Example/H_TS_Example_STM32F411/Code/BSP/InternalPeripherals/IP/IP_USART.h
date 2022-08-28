/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-16 22:45:51
 * @LastEditTime: 2021-11-24 21:43:48
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
#ifndef __IP_USART_H_
#define __IP_USART_H_
#include "usart.h"
#include "IP_DMA.h"



//获取发送dma实例
#define cIP_USART_txDMA(ip_usart)  ((ip_usart)->Instance.DMA_Tx->Instance.Stream)

//获取接收dma实例
#define cIP_USART_rxDMA(ip_usart)  ((ip_usart)->Instance.DMA_Rx->Instance.Stream)

//获取usart实例
#define cIP_USART_UASRT(ip_usart)  ((ip_usart)->Instance.USART)


typedef struct{
  struct{
    USART_TypeDef* USART;
    IP_DMA* DMA_Tx;
    IP_DMA* DMA_Rx;
  }Instance;

  unsigned char* TransmitBuffer;
  int TransmitBufferSize;
  unsigned char* ReceiveBuffer;
  int ReceiveBufferHaftSize;

  int IsStart;
  volatile int IsOdd;
  void* volatile TransmitLastBuffer;
  volatile int TransmitLastLength;

  void (* volatile TransmitDoneCallback)(void*);
  void (* volatile ReceiveDoneCallback)(void*,void*,int);
  void* volatile Callback_v;

  
}IP_USART;






/**
 * @brief 重初始化USART
 * @param ip_usart 空的IP_USART句柄(已分配内存的)
 * @param DMA_Tx 空的dma对象(已分配内存的)
 * @param DMA_Rx 空的dma对象(已分配内存的)
 * @param _usart usart句柄
 * @return 无
 */
void IP_USART_Init(IP_USART* ip_usart,IP_DMA* DMA_Tx,IP_DMA* DMA_Rx,USART_TypeDef* _usart);

/**
 * @brief 启动USART
 * @param ip_usart IP_USART句柄
 * @param TransmitBuffer 发送缓冲区(大于此大小的数据将分多次发送)
 * @param TransmitBufferSize 发送缓冲区大小 单位:字节
 * @param ReceiveBuffer 接收缓冲区(单次最多只能接收缓存一般大小的数据)
 * @param ReceiveBufferSize 接收缓冲区大小 单位:字节
 * @param TransDoneCallback 发送完成回调
 * @param ReceiveDoneCallback 接收完成回调
 * @param v 发送完成回调的参数
 * @return 无
 */
void IP_USART_Start(IP_USART* ip_usart
  ,unsigned char* TransmitBuffer,int TransmitBufferSize
  ,unsigned char* ReceiveBuffer,int ReceiveBufferSize
  ,void (*TransmitDoneCallback)(void*),void (*ReceiveDoneCallback)(void*,void*,int),void* v);

/**
 * @brief 停止USART
 * @param ip_usart IP_USART句柄
 * @return 无
 */
void IP_USART_Stop(IP_USART* ip_usart);




/**
 * @brief 发送数据
 * @param ip_usart IP_USART句柄
 * @param Buffer 数据缓存
 * @param Length 数据长度
 * @return 无
 */
void IP_USART_Transmit(IP_USART* ip_usart,void* Buffer,int Length);

/**
 * @brief USART中断调用
 * @param ip_usart IP_USART句柄
 * @return 无
 */
void IP_USART_IRQ(IP_USART* ip_usart);

#endif //__IP_UART_H_
