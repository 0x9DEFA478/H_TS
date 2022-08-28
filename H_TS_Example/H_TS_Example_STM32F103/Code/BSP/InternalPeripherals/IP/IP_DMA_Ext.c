/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-30 13:21:34
 * @LastEditTime: 2021-12-06 21:00:23
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


extern void DMA1_Channel0_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel1_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel2_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel3_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel4_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel5_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel6_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel7_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Channel0_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Channel1_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Channel2_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Channel3_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Channel4_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Channel5_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Channel6_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA2_Channel7_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);




static void IP_DMA_SetIF(IP_DMA* IP_DMA){
  void* Channel;

  Channel=IP_DMA->Instance.Channel;

#ifdef DMA1_Channel0
  if(Channel==DMA1_Channel0){
    IP_DMA->TC=DMA_FLAG_TC0;
    IP_DMA->HT=DMA_FLAG_HT0;
    IP_DMA->TE=DMA_FLAG_TE0;
    IP_DMA->ALL=DMA_FLAG_GL0;

    IP_DMA->Instance.ISR=&DMA1->ISR;
    IP_DMA->Instance.IFCR=&DMA1->IFCR;

    IP_DMA->SetIrqCallback=DMA1_Channel0_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel1
  if(Channel==DMA1_Channel1){
    IP_DMA->TC=DMA_FLAG_TC1;
    IP_DMA->HT=DMA_FLAG_HT1;
    IP_DMA->TE=DMA_FLAG_TE1;
    IP_DMA->ALL=DMA_FLAG_GL1;

    IP_DMA->Instance.ISR=&DMA1->ISR;
    IP_DMA->Instance.IFCR=&DMA1->IFCR;

    IP_DMA->SetIrqCallback=DMA1_Channel1_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel2
  if(Channel==DMA1_Channel2){
    IP_DMA->TC=DMA_FLAG_TC2;
    IP_DMA->HT=DMA_FLAG_HT2;
    IP_DMA->TE=DMA_FLAG_TE2;
    IP_DMA->ALL=DMA_FLAG_GL2;

    IP_DMA->Instance.ISR=&DMA1->ISR;
    IP_DMA->Instance.IFCR=&DMA1->IFCR;

    IP_DMA->SetIrqCallback=DMA1_Channel2_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel3
  if(Channel==DMA1_Channel3){
    IP_DMA->TC=DMA_FLAG_TC3;
    IP_DMA->HT=DMA_FLAG_HT3;
    IP_DMA->TE=DMA_FLAG_TE3;
    IP_DMA->ALL=DMA_FLAG_GL3;

    IP_DMA->Instance.ISR=&DMA1->ISR;
    IP_DMA->Instance.IFCR=&DMA1->IFCR;

    IP_DMA->SetIrqCallback=DMA1_Channel3_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel4
  if(Channel==DMA1_Channel4){
    IP_DMA->TC=DMA_FLAG_TC4;
    IP_DMA->HT=DMA_FLAG_HT4;
    IP_DMA->TE=DMA_FLAG_TE4;
    IP_DMA->ALL=DMA_FLAG_GL4;

    IP_DMA->Instance.ISR=&DMA1->ISR;
    IP_DMA->Instance.IFCR=&DMA1->IFCR;

    IP_DMA->SetIrqCallback=DMA1_Channel4_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel5
  if(Channel==DMA1_Channel5){
    IP_DMA->TC=DMA_FLAG_TC5;
    IP_DMA->HT=DMA_FLAG_HT5;
    IP_DMA->TE=DMA_FLAG_TE5;
    IP_DMA->ALL=DMA_FLAG_GL5;

    IP_DMA->Instance.ISR=&DMA1->ISR;
    IP_DMA->Instance.IFCR=&DMA1->IFCR;

    IP_DMA->SetIrqCallback=DMA1_Channel5_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel6
  if(Channel==DMA1_Channel6){
    IP_DMA->TC=DMA_FLAG_TC6;
    IP_DMA->HT=DMA_FLAG_HT6;
    IP_DMA->TE=DMA_FLAG_TE6;
    IP_DMA->ALL=DMA_FLAG_GL6;

    IP_DMA->Instance.ISR=&DMA1->ISR;
    IP_DMA->Instance.IFCR=&DMA1->IFCR;

    IP_DMA->SetIrqCallback=DMA1_Channel6_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel7
  if(Channel==DMA1_Channel7){
    IP_DMA->TC=DMA_FLAG_TC7;
    IP_DMA->HT=DMA_FLAG_HT7;
    IP_DMA->TE=DMA_FLAG_TE7;
    IP_DMA->ALL=DMA_FLAG_GL7;

    IP_DMA->Instance.ISR=&DMA1->ISR;
    IP_DMA->Instance.IFCR=&DMA1->IFCR;

    IP_DMA->SetIrqCallback=DMA1_Channel7_SetIrqCallback;
  }else
#endif
#ifdef DMA2_Channel0
  if(Channel==DMA2_Channel0){
    IP_DMA->TC=DMA_FLAG_TC0;
    IP_DMA->HT=DMA_FLAG_HT0;
    IP_DMA->TE=DMA_FLAG_TE0;
    IP_DMA->ALL=DMA_FLAG_GL0;

    IP_DMA->Instance.ISR=&DMA2->ISR;
    IP_DMA->Instance.IFCR=&DMA2->IFCR;

    IP_DMA->SetIrqCallback=DMA2_Channel0_SetIrqCallback;
  }else
#endif
#ifdef DMA2_Channel1
  if(Channel==DMA2_Channel1){
    IP_DMA->TC=DMA_FLAG_TC1;
    IP_DMA->HT=DMA_FLAG_HT1;
    IP_DMA->TE=DMA_FLAG_TE1;
    IP_DMA->ALL=DMA_FLAG_GL1;

    IP_DMA->Instance.ISR=&DMA2->ISR;
    IP_DMA->Instance.IFCR=&DMA2->IFCR;

    IP_DMA->SetIrqCallback=DMA2_Channel1_SetIrqCallback;
  }else
#endif
#ifdef DMA2_Channel2
  if(Channel==DMA2_Channel2){
    IP_DMA->TC=DMA_FLAG_TC2;
    IP_DMA->HT=DMA_FLAG_HT2;
    IP_DMA->TE=DMA_FLAG_TE2;
    IP_DMA->ALL=DMA_FLAG_GL2;

    IP_DMA->Instance.ISR=&DMA2->ISR;
    IP_DMA->Instance.IFCR=&DMA2->IFCR;

    IP_DMA->SetIrqCallback=DMA2_Channel2_SetIrqCallback;
  }else
#endif
#ifdef DMA2_Channel3
  if(Channel==DMA2_Channel3){
    IP_DMA->TC=DMA_FLAG_TC3;
    IP_DMA->HT=DMA_FLAG_HT3;
    IP_DMA->TE=DMA_FLAG_TE3;
    IP_DMA->ALL=DMA_FLAG_GL3;

    IP_DMA->Instance.ISR=&DMA2->ISR;
    IP_DMA->Instance.IFCR=&DMA2->IFCR;

    IP_DMA->SetIrqCallback=DMA2_Channel3_SetIrqCallback;
  }else
#endif
#ifdef DMA2_Channel4
  if(Channel==DMA2_Channel4){
    IP_DMA->TC=DMA_FLAG_TC4;
    IP_DMA->HT=DMA_FLAG_HT4;
    IP_DMA->TE=DMA_FLAG_TE4;
    IP_DMA->ALL=DMA_FLAG_GL4;

    IP_DMA->Instance.ISR=&DMA2->ISR;
    IP_DMA->Instance.IFCR=&DMA2->IFCR;

    IP_DMA->SetIrqCallback=DMA2_Channel4_SetIrqCallback;
  }else
#endif
#ifdef DMA2_Channel5
  if(Channel==DMA2_Channel5){
    IP_DMA->TC=DMA_FLAG_TC5;
    IP_DMA->HT=DMA_FLAG_HT5;
    IP_DMA->TE=DMA_FLAG_TE5;
    IP_DMA->ALL=DMA_FLAG_GL5;

    IP_DMA->Instance.ISR=&DMA2->ISR;
    IP_DMA->Instance.IFCR=&DMA2->IFCR;

    IP_DMA->SetIrqCallback=DMA2_Channel5_SetIrqCallback;
  }else
#endif
#ifdef DMA2_Channel6
  if(Channel==DMA2_Channel6){
    IP_DMA->TC=DMA_FLAG_TC6;
    IP_DMA->HT=DMA_FLAG_HT6;
    IP_DMA->TE=DMA_FLAG_TE6;
    IP_DMA->ALL=DMA_FLAG_GL6;

    IP_DMA->Instance.ISR=&DMA2->ISR;
    IP_DMA->Instance.IFCR=&DMA2->IFCR;

    IP_DMA->SetIrqCallback=DMA2_Channel6_SetIrqCallback;
  }else
#endif
#ifdef DMA2_Channel7
  if(Channel==DMA2_Channel7){
    IP_DMA->TC=DMA_FLAG_TC7;
    IP_DMA->HT=DMA_FLAG_HT7;
    IP_DMA->TE=DMA_FLAG_TE7;
    IP_DMA->ALL=DMA_FLAG_GL7;

    IP_DMA->Instance.ISR=&DMA2->ISR;
    IP_DMA->Instance.IFCR=&DMA2->IFCR;

    IP_DMA->SetIrqCallback=DMA2_Channel7_SetIrqCallback;
  }else
#endif
  {
    for(;;){
    }
  }

}




/**
 * @brief 设置DMA请求
 * @param ip_dma IP_DMA对象
 * @param Req 请求代号(通道号)
 * @return 无
 */
void IP_DMA_SetReq(IP_DMA* ip_dma,unsigned int Req){
  
}

/**
 * @brief 关闭DMA的所有中断
 * @param ip_dma IP_DMA对象
 * @return 0:成功 其他:不支持的操作
 */
int IP_DMA_DisableAll_IT(IP_DMA* ip_dma){

  ip_dma->Instance.Channel->CCR&=(~(DMA_CCR_TCIE|DMA_CCR_HTIE|DMA_CCR_TEIE));//关闭中断

  return 0;
}

/**
 * @brief 初始化 IP_DMA
 * @param ip_dma 空的IP_DMA对象(已分配内存的)
 * @param dma dma句柄
 * @return 0:成功 其他:不支持的操作
 */
int IP_DMA_Init(IP_DMA* ip_dma,DMA_Channel_TypeDef* dma){

  ip_dma->Instance.Channel=dma;

  if(ip_dma->Instance.Channel->CCR&DMA_CCR_EN){
    ip_dma->Instance.Channel->CCR&=(~DMA_CCR_EN);
    while(ip_dma->Instance.Channel->CCR&DMA_CCR_EN){
    }
  }
  

  IP_DMA_SetIF(ip_dma);
  IP_DMA_DisableAll_IT(ip_dma);

  return 0;
}
