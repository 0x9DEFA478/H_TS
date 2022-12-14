/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-16 22:46:16
 * @LastEditTime: 2021-12-06 21:02:19
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
 * @brief ????????????USART
 * @param ip_usart ??????IP_USART??????(??????????????????)
 * @param DMA_Tx ??????dma??????(??????????????????)
 * @param DMA_Rx ??????dma??????(??????????????????)
 * @param _usart usart??????
 * @return ???
 */
void IP_USART_Init(IP_USART* ip_usart,IP_DMA* DMA_Tx,IP_DMA* DMA_Rx,USART_TypeDef* _usart){

  cIP_USART_UASRT(ip_usart)=_usart;
  
  DMA_Tx->Instance.Channel->CPAR=(unsigned int)&cIP_USART_UASRT(ip_usart)->DR;

  DMA_Rx->Instance.Channel->CPAR=(unsigned int)&cIP_USART_UASRT(ip_usart)->DR;

  
  cIP_USART_UASRT(ip_usart)->CR1&=(~USART_CR1_UE);//??????USART

  //????????????
  cIP_USART_UASRT(ip_usart)->CR1&=(~(USART_CR1_PEIE
                          |USART_CR1_TXEIE|USART_CR1_TCIE|USART_CR1_RXNEIE|USART_CR1_IDLEIE));
  cIP_USART_UASRT(ip_usart)->CR2&=(~(USART_CR2_LBDIE|USART_CR2_LBDL));
  cIP_USART_UASRT(ip_usart)->CR3&=(~(USART_CR3_EIE));
  

  ip_usart->Instance.DMA_Tx=DMA_Tx;
  ip_usart->Instance.DMA_Rx=DMA_Rx;


  ip_usart->IsStart=0;

}

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


  cIP_USART_rxDMA(ip_usart)->CMAR=(unsigned int)ip_usart->ReceiveBuffer;
	cIP_USART_rxDMA(ip_usart)->CNDTR=ip_usart->ReceiveBufferHaftSize;

  ip_usart->Instance.DMA_Tx->SetIrqCallback((void (*)(void*))IP_USART_TxDMA_IRQ,ip_usart);

  cIP_USART_txDMA(ip_usart)->CCR|=DMA_CCR_TCIE;//????????????DMA??????????????????

  cIP_USART_rxDMA(ip_usart)->CCR|=DMA_CCR_EN;//????????????DMA
  
	cIP_USART_UASRT(ip_usart)->CR3|=USART_CR3_DMAT|USART_CR3_DMAR;//??????dma??????
  cIP_USART_UASRT(ip_usart)->CR1|=USART_CR1_IDLEIE;//??????IDLE??????
  System_CompileBarrier();
  cIP_USART_UASRT(ip_usart)->CR1|=USART_CR1_UE;//??????USART
}

/**
 * @brief ??????USART
 * @param ip_usart IP_USART??????
 * @return ???
 */
void IP_USART_Stop(IP_USART* ip_usart){

  ip_usart->IsStart=0;

  System_CompileBarrier();
  
  cIP_USART_UASRT(ip_usart)->CR1&=(~USART_CR1_UE);//??????USART
  cIP_USART_UASRT(ip_usart)->CR1&=(~USART_CR1_IDLEIE);//??????IDLE??????

}





