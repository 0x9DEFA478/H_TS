/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-16 22:46:16
 * @LastEditTime: 2022-01-13 20:34:38
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
  
  DMA_Tx->Instance.Channel->CHxPADDR=(unsigned int)&cIP_USART_UASRT(ip_usart)->DATA;

  DMA_Rx->Instance.Channel->CHxPADDR=(unsigned int)&cIP_USART_UASRT(ip_usart)->DATA;

  
  cIP_USART_UASRT(ip_usart)->CTL0&=(~USART_CTL0_UEN);//??????USART

  //????????????
  cIP_USART_UASRT(ip_usart)->CTL0&=(~(USART_CTL0_PERRIE
                          |USART_CTL0_TBEIE|USART_CTL0_TCIE|USART_CTL0_RBNEIE|USART_CTL0_IDLEIE));
  cIP_USART_UASRT(ip_usart)->CTL1&=(~(USART_CTL1_LBDIE));
  cIP_USART_UASRT(ip_usart)->CTL1&=(~(USART_CTL2_ERRIE));
  

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


  cIP_USART_rxDMA(ip_usart)->CHxMADDR=(unsigned int)ip_usart->ReceiveBuffer;
	cIP_USART_rxDMA(ip_usart)->CHxCNT=ip_usart->ReceiveBufferHaftSize;

  ip_usart->Instance.DMA_Tx->SetIrqCallback((void (*)(void*))IP_USART_TxDMA_IRQ,ip_usart);

  cIP_USART_txDMA(ip_usart)->CHxCTL|=DMA_CHXCTL_FTFIE;//????????????DMA??????????????????

  cIP_USART_rxDMA(ip_usart)->CHxCTL|=DMA_CHXCTL_CHEN;//????????????DMA
  
	cIP_USART_UASRT(ip_usart)->CTL2|=USART_CTL2_DENT|USART_CTL2_DENR;//??????dma??????
  cIP_USART_UASRT(ip_usart)->CTL0|=USART_CTL0_IDLEIE;//??????IDLE??????
  System_CompileBarrier();
  cIP_USART_UASRT(ip_usart)->CTL0|=USART_CTL0_UEN;//??????USART
}

/**
 * @brief ??????USART
 * @param ip_usart IP_USART??????
 * @return ???
 */
void IP_USART_Stop(IP_USART* ip_usart){

  ip_usart->IsStart=0;

  System_CompileBarrier();
  
  cIP_USART_UASRT(ip_usart)->CTL0&=(~USART_CTL0_UEN);//??????USART
  cIP_USART_UASRT(ip_usart)->CTL0&=(~USART_CTL0_IDLEIE);//??????IDLE??????

}





