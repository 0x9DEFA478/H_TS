/*
 * @Author: 0x9DEFA478
 * @Date: 2021-04-21 20:23:29
 * @LastEditTime: 2021-09-28 18:46:12
 * @LastEditors: 0x9DEFA478
 * @Description: GPIO驱动头文件
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
#ifndef __IP_GPIO_H_
#define __IP_GPIO_H_

#include "stm32f1xx.h"

#define vIP_GPIO_Mode_AIN                0x00000000U
#define vIP_GPIO_Mode_IN_FLOAT           0x00000004U
#define vIP_GPIO_Mode_IN_PUPD            0x00000008U

/*复用或输出模式使用*/
#define vIP_GPIO_Mode_OUTAF_10MHz        0x00000001U
/*复用或输出模式使用*/
#define vIP_GPIO_Mode_OUTAF_2MHz         0x00000002U
/*复用或输出模式使用*/
#define vIP_GPIO_Mode_OUTAF_50MHz        0x00000003U


#define vIP_GPIO_Mode_OUT_PP_xMHz        0x00000000U
#define vIP_GPIO_Mode_OUT_OD_xMHz        0x00000004U
#define vIP_GPIO_Mode_AF_PP_xMHz         0x00000008U
#define vIP_GPIO_Mode_AF_OD_xMHz         0x0000000CU




/*IO号0~7使用*/
#define oIP_GPIO_Config_0_7(GPIOx,PinNum,Mode)  GPIOx->CRL=(GPIOx->CRL&(~(0xFU<<(PinNum*4))))|(Mode<<(PinNum*4))

/*IO号8~15使用*/
#define oIP_GPIO_Config_8_15(GPIOx,PinNum,Mode) GPIOx->CRH=(GPIOx->CRH&(~(0xFU<<((PinNum-8)*4))))|(Mode<<((PinNum-8)*4))

//配置IO
#define oIP_GPIO_Config(GPIOx,PinNum,Mode)      do{if(PinNum<8){oIP_GPIO_Config_0_7(GPIOx,PinNum,Mode);}else{oIP_GPIO_Config_8_15(GPIOx,PinNum,Mode);}}while(0)


//io号转低16位
#define cIP_GPIO_PinToLMsk(PinNum)                  (0x1U<<(PinNum))

//io号转高16位
#define cIP_GPIO_PinToHMsk(PinNum)                  (0x10000U<<(PinNum))


//获取引脚输入状态
#define oIP_GPIO_GetInputBit(GPIOx,PinNum)          (GPIOx->IDR&cIP_GPIO_PinToLMsk(PinNum))

//获取引脚输出状态
#define oIP_GPIO_GetOutputBit(GPIOx,PinNum)         (GPIOx->ODR&cIP_GPIO_PinToLMsk(PinNum))


//设置输高
#define oIP_GPIO_SetOutput_High(GPIOx,PinNum)       GPIOx->BSRR=cIP_GPIO_PinToLMsk(PinNum)

//设置输低
#define oIP_GPIO_SetOutput_Low(GPIOx,PinNum)        GPIOx->BSRR=cIP_GPIO_PinToHMsk(PinNum)


//输出电平翻转
#define oIP_GPIO_OutputBitReverse(GPIOx,PinNum)     GPIOx->BSRR=oIP_GPIO_GetOutputBit(GPIOx,PinNum)?cIP_GPIO_PinToHMsk(PinNum):cIP_GPIO_PinToLMsk(PinNum)









#endif //__IP_GPIO_H_



