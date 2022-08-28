/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-16 22:46:01
 * @LastEditTime: 2021-12-06 21:09:05
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
#include "H_Memory.h"




/**
 * @brief 发送数据
 * @param ip_usart IP_USART句柄
 * @param Buffer 数据缓存
 * @param Length 数据长度
 * @return 无
 */
void IP_USART_Transmit(IP_USART* ip_usart,void* Buffer,int Length){
  int TransmitLastLength;
  int TransmitBufferSize;
  void* TransmitLastBuffer;


  if(Length==0){
    ip_usart->TransmitDoneCallback(ip_usart->Callback_v);//传输完成 调用回调
    return;
  }

  oIP_DMA_ClearTC_IFCR(ip_usart->Instance.DMA_Rx);//清除标志位

  while((cIP_USART_txDMA(ip_usart)->CR&DMA_SxCR_EN)!=0){
  }

  TransmitLastBuffer=Buffer;
  TransmitLastLength=Length;
  TransmitBufferSize=ip_usart->TransmitBufferSize;
  if(TransmitLastLength>0){

    cIP_USART_txDMA(ip_usart)->M0AR=(unsigned int)ip_usart->TransmitBuffer;

    if(TransmitLastLength>TransmitBufferSize){
      cIP_USART_txDMA(ip_usart)->NDTR=TransmitBufferSize;

      H_MemoryCopy_byte(ip_usart->TransmitBuffer,TransmitLastBuffer,TransmitBufferSize);
      System_DataMemoryBarrier();

      ip_usart->TransmitLastBuffer=&((unsigned char*)TransmitLastBuffer)[TransmitBufferSize];
      TransmitLastLength-=TransmitBufferSize;
      ip_usart->TransmitLastLength=TransmitLastLength;
    }else{
      cIP_USART_txDMA(ip_usart)->NDTR=TransmitLastLength;

      H_MemoryCopy_byte(ip_usart->TransmitBuffer,TransmitLastBuffer,TransmitLastLength);
      System_DataMemoryBarrier();

      ip_usart->TransmitLastLength=0;
    }

    cIP_USART_txDMA(ip_usart)->CR|=DMA_SxCR_EN;//重新启动DMA
  }

}





void IP_USART_TxDMA_IRQ(IP_USART* ip_usart){
  int TransmitLastLength;
  int TransmitBufferSize;
  void* TransmitLastBuffer;

  if(oIP_DMA_GetTC_ISR(ip_usart->Instance.DMA_Tx)!=0){
    //传输完成

    oIP_DMA_ClearTC_IFCR(ip_usart->Instance.DMA_Tx);//清除标志位

    while((cIP_USART_txDMA(ip_usart)->CR&DMA_SxCR_EN)!=0){
    }

    TransmitLastBuffer=ip_usart->TransmitLastBuffer;
    TransmitLastLength=ip_usart->TransmitLastLength;
    TransmitBufferSize=ip_usart->TransmitBufferSize;
    if(TransmitLastLength>0){

      cIP_USART_txDMA(ip_usart)->M0AR=(unsigned int)ip_usart->TransmitBuffer;

      if(TransmitLastLength>TransmitBufferSize){
        cIP_USART_txDMA(ip_usart)->NDTR=TransmitBufferSize;

        H_MemoryCopy_byte(ip_usart->TransmitBuffer,TransmitLastBuffer,TransmitBufferSize);
        System_DataMemoryBarrier();

        ip_usart->TransmitLastBuffer=&((unsigned char*)TransmitLastBuffer)[TransmitBufferSize];
        TransmitLastLength-=TransmitBufferSize;
        ip_usart->TransmitLastLength=TransmitLastLength;
      }else{
        cIP_USART_txDMA(ip_usart)->NDTR=TransmitLastLength;

        H_MemoryCopy_byte(ip_usart->TransmitBuffer,TransmitLastBuffer,TransmitLastLength);
        System_DataMemoryBarrier();

        ip_usart->TransmitLastLength=0;
      }

      cIP_USART_txDMA(ip_usart)->CR|=DMA_SxCR_EN;//重新启动DMA
    }else{
      ip_usart->TransmitDoneCallback(ip_usart->Callback_v);//传输完成 调用回调
    }
  }
}

/**
 * @brief USART中断调用
 * @param ip_usart IP_USART句柄
 * @return 无
 */
void IP_USART_IRQ(IP_USART* ip_usart){
  int Length;
  void* Buffer;
  unsigned int SR;

  SR=cIP_USART_UASRT(ip_usart)->SR;
  if((SR&USART_SR_IDLE)!=0){
    Length=((volatile unsigned char*)&cIP_USART_UASRT(ip_usart)->DR)[0];

    cIP_USART_rxDMA(ip_usart)->CR&=(~DMA_SxCR_EN);
    oIP_DMA_ClearAllBit_IFCR(ip_usart->Instance.DMA_Rx);

    Length=ip_usart->ReceiveBufferHaftSize-cIP_USART_rxDMA(ip_usart)->NDTR;

    if(ip_usart->IsOdd==0){
      Buffer=ip_usart->ReceiveBuffer;
      cIP_USART_rxDMA(ip_usart)->M0AR=(unsigned int)&ip_usart->ReceiveBuffer[ip_usart->ReceiveBufferHaftSize];
      ip_usart->IsOdd=-1;
    }else{
      Buffer=&ip_usart->ReceiveBuffer[ip_usart->ReceiveBufferHaftSize];
      cIP_USART_rxDMA(ip_usart)->M0AR=(unsigned int)ip_usart->ReceiveBuffer;
      ip_usart->IsOdd=0;
    }

    cIP_USART_rxDMA(ip_usart)->NDTR=ip_usart->ReceiveBufferHaftSize;
    cIP_USART_rxDMA(ip_usart)->CR|=DMA_SxCR_EN;

    System_CompileBarrier();

    ip_usart->ReceiveDoneCallback(ip_usart->Callback_v,Buffer,Length);
    
  }
}



