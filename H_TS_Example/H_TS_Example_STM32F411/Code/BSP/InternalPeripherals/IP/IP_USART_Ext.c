/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-16 22:46:16
 * @LastEditTime: 2021-12-05 21:45:07
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
#include "IP_USART.h"
#include "System/System.h"

extern void IP_USART_TxDMA_IRQ(IP_USART* ip_usart);

/**
 * @brief 重初始化USART
 * @param ip_usart 空的IP_USART句柄(已分配内存的)
 * @param DMA_Tx 空的dma对象(已分配内存的)
 * @param DMA_Rx 空的dma对象(已分配内存的)
 * @param _usart usart句柄
 * @return 无
 */
void IP_USART_Init(IP_USART* ip_usart,IP_DMA* DMA_Tx,IP_DMA* DMA_Rx,USART_TypeDef* _usart){

  cIP_USART_UASRT(ip_usart)=_usart;
  
  DMA_Tx->Instance.Stream->PAR=(unsigned int)&cIP_USART_UASRT(ip_usart)->DR;

  DMA_Rx->Instance.Stream->PAR=(unsigned int)&cIP_USART_UASRT(ip_usart)->DR;

  
  cIP_USART_UASRT(ip_usart)->CR1&=(~USART_CR1_UE);//关闭USART

  //关闭中断
  cIP_USART_UASRT(ip_usart)->CR1&=(~(USART_CR1_PEIE
                          |USART_CR1_TXEIE|USART_CR1_TCIE|USART_CR1_RXNEIE|USART_CR1_IDLEIE));
  cIP_USART_UASRT(ip_usart)->CR2&=(~(USART_CR2_LBDIE|USART_CR2_LBDL));
  cIP_USART_UASRT(ip_usart)->CR3&=(~(USART_CR3_EIE));
  

  ip_usart->Instance.DMA_Tx=DMA_Tx;
  ip_usart->Instance.DMA_Rx=DMA_Rx;


  ip_usart->IsStart=0;

}

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
  ,void (*TransmitDoneCallback)(void*),void (*ReceiveDoneCallback)(void*,void*,int),void* v){

  if(ip_usart->IsStart!=0){
    return;
  }

  ip_usart->IsStart=-1;
  ip_usart->IsOdd=0;

  ip_usart->Callback_v=v;
  ip_usart->TransmitDoneCallback=TransmitDoneCallback;
  ip_usart->ReceiveDoneCallback=ReceiveDoneCallback;

  ip_usart->TransmitBuffer=TransmitBuffer;
  ip_usart->TransmitBufferSize=TransmitBufferSize;
  ip_usart->ReceiveBuffer=ReceiveBuffer;
  ip_usart->ReceiveBufferHaftSize=ReceiveBufferSize/2;


  cIP_USART_rxDMA(ip_usart)->M0AR=(unsigned int)ip_usart->ReceiveBuffer;
	cIP_USART_rxDMA(ip_usart)->NDTR=ip_usart->ReceiveBufferHaftSize;

  ip_usart->Instance.DMA_Tx->SetIrqCallback((void (*)(void*))IP_USART_TxDMA_IRQ,ip_usart);

  cIP_USART_txDMA(ip_usart)->CR|=DMA_SxCR_TCIE;//使能发送DMA发送完成中断

  cIP_USART_rxDMA(ip_usart)->CR|=DMA_SxCR_EN;//使能接收DMA
  
	cIP_USART_UASRT(ip_usart)->CR3|=USART_CR3_DMAT|USART_CR3_DMAR;//使能dma请求
  cIP_USART_UASRT(ip_usart)->CR1|=USART_CR1_IDLEIE;//使能IDLE中断
  System_CompileBarrier();
  cIP_USART_UASRT(ip_usart)->CR1|=USART_CR1_UE;//启动USART
}

/**
 * @brief 停止USART
 * @param ip_usart IP_USART句柄
 * @return 无
 */
void IP_USART_Stop(IP_USART* ip_usart){

  ip_usart->IsStart=0;

  System_CompileBarrier();
  
  cIP_USART_UASRT(ip_usart)->CR1&=(~USART_CR1_UE);//关闭USART
  cIP_USART_UASRT(ip_usart)->CR1&=(~USART_CR1_IDLEIE);//失能IDLE中断

}





