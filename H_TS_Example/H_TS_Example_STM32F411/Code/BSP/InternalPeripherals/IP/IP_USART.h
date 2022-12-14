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



//????????????dma??????
#define cIP_USART_txDMA(ip_usart)  ((ip_usart)->Instance.DMA_Tx->Instance.Stream)

//????????????dma??????
#define cIP_USART_rxDMA(ip_usart)  ((ip_usart)->Instance.DMA_Rx->Instance.Stream)

//??????usart??????
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
 * @brief ????????????USART
 * @param ip_usart ??????IP_USART??????(??????????????????)
 * @param DMA_Tx ??????dma??????(??????????????????)
 * @param DMA_Rx ??????dma??????(??????????????????)
 * @param _usart usart??????
 * @return ???
 */
void IP_USART_Init(IP_USART* ip_usart,IP_DMA* DMA_Tx,IP_DMA* DMA_Rx,USART_TypeDef* _usart);

/**
 * @brief ??????USART
 * @param ip_usart IP_USART??????
 * @param TransmitBuffer ???????????????(??????????????????????????????????????????)
 * @param TransmitBufferSize ????????????????????? ??????:??????
 * @param ReceiveBuffer ???????????????(???????????????????????????????????????????????????)
 * @param ReceiveBufferSize ????????????????????? ??????:??????
 * @param TransDoneCallback ??????????????????
 * @param ReceiveDoneCallback ??????????????????
 * @param v ???????????????????????????
 * @return ???
 */
void IP_USART_Start(IP_USART* ip_usart
  ,unsigned char* TransmitBuffer,int TransmitBufferSize
  ,unsigned char* ReceiveBuffer,int ReceiveBufferSize
  ,void (*TransmitDoneCallback)(void*),void (*ReceiveDoneCallback)(void*,void*,int),void* v);

/**
 * @brief ??????USART
 * @param ip_usart IP_USART??????
 * @return ???
 */
void IP_USART_Stop(IP_USART* ip_usart);




/**
 * @brief ????????????
 * @param ip_usart IP_USART??????
 * @param Buffer ????????????
 * @param Length ????????????
 * @return ???
 */
void IP_USART_Transmit(IP_USART* ip_usart,void* Buffer,int Length);

/**
 * @brief USART????????????
 * @param ip_usart IP_USART??????
 * @return ???
 */
void IP_USART_IRQ(IP_USART* ip_usart);

#endif //__IP_UART_H_
