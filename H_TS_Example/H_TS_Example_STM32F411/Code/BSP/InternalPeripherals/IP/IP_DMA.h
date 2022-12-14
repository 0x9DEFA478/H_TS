/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-30 10:17:50
 * @LastEditTime: 2021-08-15 22:55:43
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


//DMA???????????????
#define oIP_DMA_ISR(ip_dma)               (*((ip_dma)->Instance.ISR)) 
//DMA?????????????????????
#define oIP_DMA_IFCR(ip_dma)              (*((ip_dma)->Instance.IFCR))


//?????????????????????
#define oIP_DMA_GetBit_ISR(ip_dma,Msk)        (oIP_DMA_ISR(IP_DMA)&(Msk))

//???????????????
#define oIP_DMA_GetTC_ISR(ip_dma)             (oIP_DMA_ISR(ip_dma)&(ip_dma)->TC)

//???????????????
#define oIP_DMA_GetTE_ISR(ip_dma)             (oIP_DMA_ISR(ip_dma)&(ip_dma)->TE)

//???????????????
#define oIP_DMA_GetHT_ISR(ip_dma)             (oIP_DMA_ISR(ip_dma)&(ip_dma)->HT)

//???????????????
#define oIP_DMA_GetDME_ISR(ip_dma)            (oIP_DMA_ISR(ip_dma)&(ip_dma)->DME)

//???????????????
#define oIP_DMA_GetFE_ISR(ip_dma)             (oIP_DMA_ISR(ip_dma)&(ip_dma)->FE)

//???????????????
#define oIP_DMA_GetALL_ISR(ip_dma)            (oIP_DMA_ISR(ip_dma)&(ip_dma)->ALL)


//?????????????????????
#define oIP_DMA_ClearBit_IFCR(ip_dma,Msk)     (oIP_DMA_IFCR(ip_dma)=(Msk))

//???????????????
#define oIP_DMA_ClearTC_IFCR(ip_dma)          (oIP_DMA_IFCR(ip_dma)=(ip_dma)->TC)

//???????????????
#define oIP_DMA_ClearTE_IFCR(ip_dma)          (oIP_DMA_IFCR(ip_dma)=(ip_dma)->TE)

//???????????????
#define oIP_DMA_ClearHT_IFCR(ip_dma)          (oIP_DMA_IFCR(ip_dma)=(ip_dma)->HT)

//???????????????
#define oIP_DMA_ClearDME_IFCR(ip_dma)         (oIP_DMA_IFCR(ip_dma)=(ip_dma)->DME)

//???????????????
#define oIP_DMA_ClearFE_IFCR(ip_dma)          (oIP_DMA_IFCR(ip_dma)=(ip_dma)->FE)

//?????????????????????
#define oIP_DMA_ClearAllBit_IFCR(ip_dma)      oIP_DMA_ClearBit_IFCR(ip_dma,(ip_dma)->ALL)


typedef struct{

  //????????????
	struct{
    DMA_Stream_TypeDef* Stream;
    volatile unsigned int* ISR;//???????????????
    volatile unsigned int* IFCR;//?????????????????????
  }Instance;

	unsigned int TC;//TC???????????????
	unsigned int TE;//TE???????????????
	unsigned int HT;//HT???????????????
	unsigned int DME;//DME???????????????
	unsigned int FE;//FE???????????????
	unsigned int ALL;//TC TE HT DME FE???????????????

  void (*SetIrqCallback)(void (*)(void*),void*);//????????????????????????

}IP_DMA;



/**
 * @brief ??????DMA??????
 * @param ip_dma IP_DMA??????
 * @param Req ????????????(?????????)
 * @return ???
 */
void IP_DMA_SetReq(IP_DMA* ip_dma,unsigned int Req);

/**
 * @brief ??????DMA???????????????
 * @param ip_dma IP_DMA??????
 * @return 0:?????? ??????:??????????????????
 */
int IP_DMA_DisableAll_IT(IP_DMA* ip_dma);

/**
 * @brief ????????? IP_DMA
 * @param ip_dma ??????IP_DMA??????(??????????????????)
 * @param dma dma??????
 * @return 0:?????? ??????:??????????????????
 */
int IP_DMA_Init(IP_DMA* ip_dma,DMA_Stream_TypeDef* dma);



#endif //__IP_DMA_H_
