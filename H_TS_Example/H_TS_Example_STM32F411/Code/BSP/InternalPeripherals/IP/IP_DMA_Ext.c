/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-30 13:21:34
 * @LastEditTime: 2021-11-29 21:43:14
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
#include "IP_DMA.h"


extern void DMA1_Stream0_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Stream1_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Stream2_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Stream3_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Stream4_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Stream5_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Stream6_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Stream7_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Stream0_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Stream1_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Stream2_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Stream3_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Stream4_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Stream5_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Stream6_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Stream7_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);




static void IP_DMA_SetIF(IP_DMA* IP_DMA){
  void* Stream;

  Stream=IP_DMA->Instance.Stream;

  if(Stream==DMA1_Stream0){
    IP_DMA->TC=DMA_FLAG_TCIF0_4;
    IP_DMA->HT=DMA_FLAG_HTIF0_4;
    IP_DMA->TE=DMA_FLAG_TEIF0_4;
    IP_DMA->DME=DMA_FLAG_DMEIF0_4;
    IP_DMA->FE=DMA_FLAG_FEIF0_4;

    IP_DMA->Instance.ISR=&DMA1->LISR;
    IP_DMA->Instance.IFCR=&DMA1->LIFCR;

    IP_DMA->SetIrqCallback=DMA1_Stream0_SetIrqCallback;
  }else if(Stream==DMA1_Stream1){
    IP_DMA->TC=DMA_FLAG_TCIF1_5;
    IP_DMA->HT=DMA_FLAG_HTIF1_5;
    IP_DMA->TE=DMA_FLAG_TEIF1_5;
    IP_DMA->DME=DMA_FLAG_DMEIF1_5;
    IP_DMA->FE=DMA_FLAG_FEIF1_5;

    IP_DMA->Instance.ISR=&DMA1->LISR;
    IP_DMA->Instance.IFCR=&DMA1->LIFCR;

    IP_DMA->SetIrqCallback=DMA1_Stream1_SetIrqCallback;
  }else if(Stream==DMA1_Stream2){
    IP_DMA->TC=DMA_FLAG_TCIF2_6;
    IP_DMA->HT=DMA_FLAG_HTIF2_6;
    IP_DMA->TE=DMA_FLAG_TEIF2_6;
    IP_DMA->DME=DMA_FLAG_DMEIF2_6;
    IP_DMA->FE=DMA_FLAG_FEIF2_6;

    IP_DMA->Instance.ISR=&DMA1->LISR;
    IP_DMA->Instance.IFCR=&DMA1->LIFCR;

    IP_DMA->SetIrqCallback=DMA1_Stream2_SetIrqCallback;
  }else if(Stream==DMA1_Stream3){
    IP_DMA->TC=DMA_FLAG_TCIF3_7;
    IP_DMA->HT=DMA_FLAG_HTIF3_7;
    IP_DMA->TE=DMA_FLAG_TEIF3_7;
    IP_DMA->DME=DMA_FLAG_DMEIF3_7;
    IP_DMA->FE=DMA_FLAG_FEIF3_7;

    IP_DMA->Instance.ISR=&DMA1->LISR;
    IP_DMA->Instance.IFCR=&DMA1->LIFCR;

    IP_DMA->SetIrqCallback=DMA1_Stream3_SetIrqCallback;
  }else if(Stream==DMA1_Stream4){
    IP_DMA->TC=DMA_FLAG_TCIF0_4;
    IP_DMA->HT=DMA_FLAG_HTIF0_4;
    IP_DMA->TE=DMA_FLAG_TEIF0_4;
    IP_DMA->DME=DMA_FLAG_DMEIF0_4;
    IP_DMA->FE=DMA_FLAG_FEIF0_4;

    IP_DMA->Instance.ISR=&DMA1->HISR;
    IP_DMA->Instance.IFCR=&DMA1->HIFCR;

    IP_DMA->SetIrqCallback=DMA1_Stream4_SetIrqCallback;
  }else if(Stream==DMA1_Stream5){
    IP_DMA->TC=DMA_FLAG_TCIF1_5;
    IP_DMA->HT=DMA_FLAG_HTIF1_5;
    IP_DMA->TE=DMA_FLAG_TEIF1_5;
    IP_DMA->DME=DMA_FLAG_DMEIF1_5;
    IP_DMA->FE=DMA_FLAG_FEIF1_5;

    IP_DMA->Instance.ISR=&DMA1->HISR;
    IP_DMA->Instance.IFCR=&DMA1->HIFCR;

    IP_DMA->SetIrqCallback=DMA1_Stream5_SetIrqCallback;
  }else if(Stream==DMA1_Stream6){
    IP_DMA->TC=DMA_FLAG_TCIF2_6;
    IP_DMA->HT=DMA_FLAG_HTIF2_6;
    IP_DMA->TE=DMA_FLAG_TEIF2_6;
    IP_DMA->DME=DMA_FLAG_DMEIF2_6;
    IP_DMA->FE=DMA_FLAG_FEIF2_6;

    IP_DMA->Instance.ISR=&DMA1->HISR;
    IP_DMA->Instance.IFCR=&DMA1->HIFCR;

    IP_DMA->SetIrqCallback=DMA1_Stream6_SetIrqCallback;
  }else if(Stream==DMA1_Stream7){
    IP_DMA->TC=DMA_FLAG_TCIF3_7;
    IP_DMA->HT=DMA_FLAG_HTIF3_7;
    IP_DMA->TE=DMA_FLAG_TEIF3_7;
    IP_DMA->DME=DMA_FLAG_DMEIF3_7;
    IP_DMA->FE=DMA_FLAG_FEIF3_7;

    IP_DMA->Instance.ISR=&DMA1->HISR;
    IP_DMA->Instance.IFCR=&DMA1->HIFCR;

    IP_DMA->SetIrqCallback=DMA1_Stream7_SetIrqCallback;
  }else if(Stream==DMA2_Stream0){
    IP_DMA->TC=DMA_FLAG_TCIF0_4;
    IP_DMA->HT=DMA_FLAG_HTIF0_4;
    IP_DMA->TE=DMA_FLAG_TEIF0_4;
    IP_DMA->DME=DMA_FLAG_DMEIF0_4;
    IP_DMA->FE=DMA_FLAG_FEIF0_4;

    IP_DMA->Instance.ISR=&DMA2->LISR;
    IP_DMA->Instance.IFCR=&DMA2->LIFCR;

    IP_DMA->SetIrqCallback=DMA2_Stream0_SetIrqCallback;
  }else if(Stream==DMA2_Stream1){
    IP_DMA->TC=DMA_FLAG_TCIF1_5;
    IP_DMA->HT=DMA_FLAG_HTIF1_5;
    IP_DMA->TE=DMA_FLAG_TEIF1_5;
    IP_DMA->DME=DMA_FLAG_DMEIF1_5;
    IP_DMA->FE=DMA_FLAG_FEIF1_5;

    IP_DMA->Instance.ISR=&DMA2->LISR;
    IP_DMA->Instance.IFCR=&DMA2->LIFCR;

    IP_DMA->SetIrqCallback=DMA2_Stream1_SetIrqCallback;
  }else if(Stream==DMA2_Stream2){
    IP_DMA->TC=DMA_FLAG_TCIF2_6;
    IP_DMA->HT=DMA_FLAG_HTIF2_6;
    IP_DMA->TE=DMA_FLAG_TEIF2_6;
    IP_DMA->DME=DMA_FLAG_DMEIF2_6;
    IP_DMA->FE=DMA_FLAG_FEIF2_6;

    IP_DMA->Instance.ISR=&DMA2->LISR;
    IP_DMA->Instance.IFCR=&DMA2->LIFCR;

    IP_DMA->SetIrqCallback=DMA2_Stream2_SetIrqCallback;
  }else if(Stream==DMA2_Stream3){
    IP_DMA->TC=DMA_FLAG_TCIF3_7;
    IP_DMA->HT=DMA_FLAG_HTIF3_7;
    IP_DMA->TE=DMA_FLAG_TEIF3_7;
    IP_DMA->DME=DMA_FLAG_DMEIF3_7;
    IP_DMA->FE=DMA_FLAG_FEIF3_7;

    IP_DMA->Instance.ISR=&DMA2->LISR;
    IP_DMA->Instance.IFCR=&DMA2->LIFCR;

    IP_DMA->SetIrqCallback=DMA2_Stream3_SetIrqCallback;
  }else if(Stream==DMA2_Stream4){
    IP_DMA->TC=DMA_FLAG_TCIF0_4;
    IP_DMA->HT=DMA_FLAG_HTIF0_4;
    IP_DMA->TE=DMA_FLAG_TEIF0_4;
    IP_DMA->DME=DMA_FLAG_DMEIF0_4;
    IP_DMA->FE=DMA_FLAG_FEIF0_4;
    
    IP_DMA->Instance.ISR=&DMA2->HISR;
    IP_DMA->Instance.IFCR=&DMA2->HIFCR;

    IP_DMA->SetIrqCallback=DMA2_Stream4_SetIrqCallback;
  }else if(Stream==DMA2_Stream5){
    IP_DMA->TC=DMA_FLAG_TCIF1_5;
    IP_DMA->HT=DMA_FLAG_HTIF1_5;
    IP_DMA->TE=DMA_FLAG_TEIF1_5;
    IP_DMA->DME=DMA_FLAG_DMEIF1_5;
    IP_DMA->FE=DMA_FLAG_FEIF1_5;

    IP_DMA->Instance.ISR=&DMA2->HISR;
    IP_DMA->Instance.IFCR=&DMA2->HIFCR;

    IP_DMA->SetIrqCallback=DMA2_Stream5_SetIrqCallback;
  }else if(Stream==DMA2_Stream6){
    IP_DMA->TC=DMA_FLAG_TCIF2_6;
    IP_DMA->HT=DMA_FLAG_HTIF2_6;
    IP_DMA->TE=DMA_FLAG_TEIF2_6;
    IP_DMA->DME=DMA_FLAG_DMEIF2_6;
    IP_DMA->FE=DMA_FLAG_FEIF2_6;

    IP_DMA->Instance.ISR=&DMA2->HISR;
    IP_DMA->Instance.IFCR=&DMA2->HIFCR;

    IP_DMA->SetIrqCallback=DMA2_Stream6_SetIrqCallback;
  }else if(Stream==DMA2_Stream7){
    IP_DMA->TC=DMA_FLAG_TCIF3_7;
    IP_DMA->HT=DMA_FLAG_HTIF3_7;
    IP_DMA->TE=DMA_FLAG_TEIF3_7;
    IP_DMA->DME=DMA_FLAG_DMEIF3_7;
    IP_DMA->FE=DMA_FLAG_FEIF3_7;

    IP_DMA->Instance.ISR=&DMA2->HISR;
    IP_DMA->Instance.IFCR=&DMA2->HIFCR;

    IP_DMA->SetIrqCallback=DMA2_Stream7_SetIrqCallback;
  }else{
    for(;;){
    }
  }

  IP_DMA->ALL=IP_DMA->TC|IP_DMA->HT|IP_DMA->TE|IP_DMA->DME|IP_DMA->FE;

}




/**
 * @brief 设置DMA请求
 * @param ip_dma IP_DMA对象
 * @param Req 请求代号(通道号)
 * @return 无
 */
void IP_DMA_SetReq(IP_DMA* ip_dma,unsigned int Req){
  ip_dma->Instance.Stream->CR&=(ip_dma->Instance.Stream->CR&(~DMA_SxCR_CHSEL_Msk))|((Req<<DMA_SxCR_CHSEL_Pos)&(DMA_SxCR_CHSEL_Msk));
}

/**
 * @brief 关闭DMA的所有中断
 * @param ip_dma IP_DMA对象
 * @return 0:成功 其他:不支持的操作
 */
int IP_DMA_DisableAll_IT(IP_DMA* ip_dma){

  ip_dma->Instance.Stream->CR&=(~(DMA_SxCR_TCIE|DMA_SxCR_HTIE|DMA_SxCR_TEIE|DMA_SxCR_DMEIE));//关闭中断
  ip_dma->Instance.Stream->FCR&=(~DMA_SxFCR_FEIE);//关闭中断

  return 0;
}

/**
 * @brief 初始化 IP_DMA
 * @param ip_dma 空的IP_DMA对象(已分配内存的)
 * @param dma dma句柄
 * @return 0:成功 其他:不支持的操作
 */
int IP_DMA_Init(IP_DMA* ip_dma,DMA_Stream_TypeDef* dma){

  ip_dma->Instance.Stream=dma;

  if(ip_dma->Instance.Stream->CR&DMA_SxCR_EN){
    ip_dma->Instance.Stream->CR&=(~DMA_SxCR_EN);
    while(ip_dma->Instance.Stream->CR&DMA_SxCR_EN){
    }
  }
  

  IP_DMA_SetIF(ip_dma);
  IP_DMA_DisableAll_IT(ip_dma);

  return 0;
}
