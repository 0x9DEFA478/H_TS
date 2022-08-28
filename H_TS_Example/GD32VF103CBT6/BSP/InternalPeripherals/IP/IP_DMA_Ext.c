/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-30 13:21:34
 * @LastEditTime: 2022-01-13 19:45:41
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


extern void DMA0_Channel0_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA0_Channel1_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA0_Channel2_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA0_Channel3_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA0_Channel4_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA0_Channel5_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA0_Channel6_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA0_Channel7_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel0_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel1_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel2_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel3_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel4_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel5_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel6_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
extern void DMA1_Channel7_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);




static void IP_DMA_SetIF(IP_DMA* IP_DMA){
  void* Channel;

  Channel=IP_DMA->Instance.Channel;

#ifdef DMA0_Channel0
  if(Channel==DMA0_Channel0){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*0);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*0);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*0);
    IP_DMA->ALL=DMA_FLAG_G<<(4*0);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA0);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA0);

    IP_DMA->SetIrqCallback=DMA0_Channel0_SetIrqCallback;
  }else
#endif
#ifdef DMA0_Channel1
  if(Channel==DMA0_Channel1){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*1);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*1);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*1);
    IP_DMA->ALL=DMA_FLAG_G<<(4*1);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA0);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA0);

    IP_DMA->SetIrqCallback=DMA0_Channel1_SetIrqCallback;
  }else
#endif
#ifdef DMA0_Channel2
  if(Channel==DMA0_Channel2){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*2);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*2);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*2);
    IP_DMA->ALL=DMA_FLAG_G<<(4*2);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA0);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA0);

    IP_DMA->SetIrqCallback=DMA0_Channel2_SetIrqCallback;
  }else
#endif
#ifdef DMA0_Channel3
  if(Channel==DMA0_Channel3){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*3);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*3);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*3);
    IP_DMA->ALL=DMA_FLAG_G<<(4*3);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA0);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA0);

    IP_DMA->SetIrqCallback=DMA0_Channel3_SetIrqCallback;
  }else
#endif
#ifdef DMA0_Channel4
  if(Channel==DMA0_Channel4){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*4);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*4);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*4);
    IP_DMA->ALL=DMA_FLAG_G<<(4*4);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA0);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA0);

    IP_DMA->SetIrqCallback=DMA0_Channel4_SetIrqCallback;
  }else
#endif
#ifdef DMA0_Channel5
  if(Channel==DMA0_Channel5){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*5);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*5);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*5);
    IP_DMA->ALL=DMA_FLAG_G<<(4*5);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA0);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA0);

    IP_DMA->SetIrqCallback=DMA0_Channel5_SetIrqCallback;
  }else
#endif
#ifdef DMA0_Channel6
  if(Channel==DMA0_Channel6){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*6);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*6);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*6);
    IP_DMA->ALL=DMA_FLAG_G<<(4*6);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA0);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA0);

    IP_DMA->SetIrqCallback=DMA0_Channel6_SetIrqCallback;
  }else
#endif
#ifdef DMA0_Channel7
  if(Channel==DMA0_Channel7){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*7);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*7);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*7);
    IP_DMA->ALL=DMA_FLAG_G<<(4*7);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA0);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA0);

    IP_DMA->SetIrqCallback=DMA0_Channel7_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel0
  if(Channel==DMA1_Channel0){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*0);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*0);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*0);
    IP_DMA->ALL=DMA_FLAG_G<<(4*0);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA1);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA1);

    IP_DMA->SetIrqCallback=DMA1_Channel0_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel1
  if(Channel==DMA1_Channel1){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*1);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*1);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*1);
    IP_DMA->ALL=DMA_FLAG_G<<(4*1);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA1);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA1);

    IP_DMA->SetIrqCallback=DMA1_Channel1_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel2
  if(Channel==DMA1_Channel2){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*2);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*2);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*2);
    IP_DMA->ALL=DMA_FLAG_G<<(4*2);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA1);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA1);

    IP_DMA->SetIrqCallback=DMA1_Channel2_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel3
  if(Channel==DMA1_Channel3){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*3);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*3);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*3);
    IP_DMA->ALL=DMA_FLAG_G<<(4*3);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA1);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA1);

    IP_DMA->SetIrqCallback=DMA1_Channel3_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel4
  if(Channel==DMA1_Channel4){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*4);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*4);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*4);
    IP_DMA->ALL=DMA_FLAG_G<<(4*4);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA1);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA1);

    IP_DMA->SetIrqCallback=DMA1_Channel4_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel5
  if(Channel==DMA1_Channel5){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*5);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*5);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*5);
    IP_DMA->ALL=DMA_FLAG_G<<(4*5);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA1);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA1);

    IP_DMA->SetIrqCallback=DMA1_Channel5_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel6
  if(Channel==DMA1_Channel6){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*6);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*6);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*6);
    IP_DMA->ALL=DMA_FLAG_G<<(4*6);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA1);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA1);

    IP_DMA->SetIrqCallback=DMA1_Channel6_SetIrqCallback;
  }else
#endif
#ifdef DMA1_Channel7
  if(Channel==DMA1_Channel7){
    IP_DMA->TC=DMA_FLAG_FTF<<(4*7);
    IP_DMA->HT=DMA_FLAG_HTF<<(4*7);
    IP_DMA->TE=DMA_FLAG_ERR<<(4*7);
    IP_DMA->ALL=DMA_FLAG_G<<(4*7);

    IP_DMA->Instance.ISR=&DMA_INTF(DMA1);
    IP_DMA->Instance.IFCR=&DMA_INTC(DMA1);

    IP_DMA->SetIrqCallback=DMA1_Channel7_SetIrqCallback;
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

  ip_dma->Instance.Channel->CHxCTL&=(~(DMA_CHXCTL_ERRIE|DMA_CHXCTL_HTFIE|DMA_CHXCTL_FTFIE));//关闭中断

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

  if(ip_dma->Instance.Channel->CHxCTL&DMA_CHXCTL_CHEN){
    ip_dma->Instance.Channel->CHxCTL&=(~DMA_CHXCTL_CHEN);
    while(ip_dma->Instance.Channel->CHxCTL&DMA_CHXCTL_CHEN){
    }
  }
  

  IP_DMA_SetIF(ip_dma);
  IP_DMA_DisableAll_IT(ip_dma);

  return 0;
}
