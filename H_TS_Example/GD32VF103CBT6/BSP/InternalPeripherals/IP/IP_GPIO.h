/*
 * @Author: 0x9DEFA478
 * @Date: 2022-01-12 19:22:42
 * @LastEditTime: 2022-01-13 22:43:34
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
#ifndef __IP_GPIO_H_
#define __IP_GPIO_H_
#include "gd32vf103_gpio.h"

#define vIP_GPIO_Mode_AN                 0x00000000U
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
#define oIP_GPIO_Config_0_7(GPIOx,PinNum,Mode)  GPIO_CTL0(GPIOx)=(GPIO_CTL0(GPIOx)&(~(0xFU<<((PinNum)*4))))|((Mode)<<((PinNum)*4))

/*IO号8~15使用*/
#define oIP_GPIO_Config_8_15(GPIOx,PinNum,Mode) GPIO_CTL1(GPIOx)=(GPIO_CTL1(GPIOx)&(~(0xFU<<(((PinNum)-8)*4))))|((Mode)<<(((PinNum)-8)*4))

//配置IO
#define oIP_GPIO_Config(GPIOx,PinNum,Mode)      do{if((PinNum)<8){oIP_GPIO_Config_0_7(GPIOx,PinNum,Mode);}else{oIP_GPIO_Config_8_15(GPIOx,PinNum,Mode);}}while(0)


//io号转低16位
#define cIP_GPIO_PinToLMsk(PinNum)                  (0x1U<<(PinNum))

//io号转高16位
#define cIP_GPIO_PinToHMsk(PinNum)                  (0x10000U<<(PinNum))


//获取引脚输入状态
#define oIP_GPIO_GetInputBit(GPIOx,PinNum)          (GPIO_ISTAT(GPIOx)&cIP_GPIO_PinToLMsk(PinNum))

//获取引脚输出状态
#define oIP_GPIO_GetOutputBit(GPIOx,PinNum)         (GPIO_OCTL(GPIOx)&cIP_GPIO_PinToLMsk(PinNum))


//设置输高
#define oIP_GPIO_SetOutput_High(GPIOx,PinNum)       GPIO_BOP(GPIOx)=cIP_GPIO_PinToLMsk(PinNum)

//设置输低
#define oIP_GPIO_SetOutput_Low(GPIOx,PinNum)        GPIO_BOP(GPIOx)=cIP_GPIO_PinToHMsk(PinNum)


//输出电平翻转
#define oIP_GPIO_OutputBitReverse(GPIOx,PinNum)     GPIO_BOP(GPIOx)=oIP_GPIO_GetOutputBit(GPIOx,PinNum)?cIP_GPIO_PinToHMsk(PinNum):cIP_GPIO_PinToLMsk(PinNum)









#endif //__IP_GPIO_H_
