/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-30 10:17:50
 * @LastEditTime: 2021-09-28 18:24:20
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
#ifndef __IP_DMA_H_
#define __IP_DMA_H_
#include "dma.h"


//DMA状态寄存器
#define oIP_DMA_ISR(ip_dma)               (*((ip_dma)->Instance.ISR)) 
//DMA状态清除寄存器
#define oIP_DMA_IFCR(ip_dma)              (*((ip_dma)->Instance.IFCR))


//获取某个标志位
#define oIP_DMA_GetBit_ISR(ip_dma,Msk)        (oIP_DMA_ISR(IP_DMA)&(Msk))

//获取标志位
#define oIP_DMA_GetTC_ISR(ip_dma)             (oIP_DMA_ISR(ip_dma)&(ip_dma)->TC)

//获取标志位
#define oIP_DMA_GetTE_ISR(ip_dma)             (oIP_DMA_ISR(ip_dma)&(ip_dma)->TE)

//获取标志位
#define oIP_DMA_GetHT_ISR(ip_dma)             (oIP_DMA_ISR(ip_dma)&(ip_dma)->HT)

//获取标志位
#define oIP_DMA_GetDME_ISR(ip_dma)            (oIP_DMA_ISR(ip_dma)&(ip_dma)->DME)

//获取标志位
#define oIP_DMA_GetFE_ISR(ip_dma)             (oIP_DMA_ISR(ip_dma)&(ip_dma)->FE)

//获取标志位
#define oIP_DMA_GetALL_ISR(ip_dma)            (oIP_DMA_ISR(ip_dma)&(ip_dma)->ALL)


//清除某个标志位
#define oIP_DMA_ClearBit_IFCR(ip_dma,Msk)     (oIP_DMA_IFCR(ip_dma)=(Msk))

//清除标志位
#define oIP_DMA_ClearTC_IFCR(ip_dma)          (oIP_DMA_IFCR(ip_dma)=(ip_dma)->TC)

//清除标志位
#define oIP_DMA_ClearTE_IFCR(ip_dma)          (oIP_DMA_IFCR(ip_dma)=(ip_dma)->TE)

//清除标志位
#define oIP_DMA_ClearHT_IFCR(ip_dma)          (oIP_DMA_IFCR(ip_dma)=(ip_dma)->HT)

//清除标志位
#define oIP_DMA_ClearDME_IFCR(ip_dma)         (oIP_DMA_IFCR(ip_dma)=(ip_dma)->DME)

//清除标志位
#define oIP_DMA_ClearFE_IFCR(ip_dma)          (oIP_DMA_IFCR(ip_dma)=(ip_dma)->FE)

//清除所有标志位
#define oIP_DMA_ClearAllBit_IFCR(ip_dma)      oIP_DMA_ClearBit_IFCR(ip_dma,(ip_dma)->ALL)


typedef struct{

  //外设实例
	struct{
    DMA_Channel_TypeDef* Channel;
    volatile unsigned int* ISR;//状态寄存器
    volatile unsigned int* IFCR;//状态清除寄存器
  }Instance;

	unsigned int TC;//TC标志位掩码
	unsigned int TE;//TE标志位掩码
	unsigned int HT;//HT标志位掩码
	unsigned int ALL;//TC TE HT标志位掩码

  void (*SetIrqCallback)(void (*)(void*),void*);//用于设置中断回调

}IP_DMA;



/**
 * @brief 设置DMA请求
 * @param ip_dma IP_DMA对象
 * @param Req 请求代号(通道号)
 * @return 无
 */
void IP_DMA_SetReq(IP_DMA* ip_dma,unsigned int Req);

/**
 * @brief 关闭DMA的所有中断
 * @param ip_dma IP_DMA对象
 * @return 0:成功 其他:不支持的操作
 */
int IP_DMA_DisableAll_IT(IP_DMA* ip_dma);

/**
 * @brief 初始化 IP_DMA
 * @param ip_dma 空的IP_DMA对象(已分配内存的)
 * @param dma dma句柄
 * @return 0:成功 其他:不支持的操作
 */
int IP_DMA_Init(IP_DMA* ip_dma,DMA_Channel_TypeDef* dma);



#endif //__IP_DMA_H_
