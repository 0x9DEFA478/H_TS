/*
 * @Author: 0x9DEFA478
 * @Date: 2022-01-13 17:45:01
 * @LastEditTime: 2022-01-13 22:06:33
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
#ifndef __dma_H_
#define __dma_H_
#include "gd32vf103_dma.h"

typedef struct{
  volatile unsigned int CHxCTL;
  volatile unsigned int CHxCNT;
  volatile unsigned int CHxPADDR;
  volatile unsigned int CHxMADDR;
}DMA_Channel_TypeDef;

#define DMA0_Channel0                          ((DMA_Channel_TypeDef*)(DMA0+0x8U+0x14U*0))
#define DMA0_Channel1                          ((DMA_Channel_TypeDef*)(DMA0+0x8U+0x14U*1))
#define DMA0_Channel2                          ((DMA_Channel_TypeDef*)(DMA0+0x8U+0x14U*2))
#define DMA0_Channel3                          ((DMA_Channel_TypeDef*)(DMA0+0x8U+0x14U*3))
#define DMA0_Channel4                          ((DMA_Channel_TypeDef*)(DMA0+0x8U+0x14U*4))
#define DMA0_Channel5                          ((DMA_Channel_TypeDef*)(DMA0+0x8U+0x14U*5))
#define DMA0_Channel6                          ((DMA_Channel_TypeDef*)(DMA0+0x8U+0x14U*6))
#define DMA0_Channel7                          ((DMA_Channel_TypeDef*)(DMA0+0x8U+0x14U*7))

#define DMA1_Channel0                          ((DMA_Channel_TypeDef*)(DMA1+0x8U+0x14U*0))
#define DMA1_Channel1                          ((DMA_Channel_TypeDef*)(DMA1+0x8U+0x14U*1))
#define DMA1_Channel2                          ((DMA_Channel_TypeDef*)(DMA1+0x8U+0x14U*2))
#define DMA1_Channel3                          ((DMA_Channel_TypeDef*)(DMA1+0x8U+0x14U*3))
#define DMA1_Channel4                          ((DMA_Channel_TypeDef*)(DMA1+0x8U+0x14U*4))
#define DMA1_Channel5                          ((DMA_Channel_TypeDef*)(DMA1+0x8U+0x14U*5))
#define DMA1_Channel6                          ((DMA_Channel_TypeDef*)(DMA1+0x8U+0x14U*6))
#define DMA1_Channel7                          ((DMA_Channel_TypeDef*)(DMA1+0x8U+0x14U*7))




void DMA_Init(void);

void DMA_DeInit(void);


#endif //__dma_H_
