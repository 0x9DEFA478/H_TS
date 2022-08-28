/*
 * @Author: 0x9DEFA478
 * @Date: 2021-04-21 20:23:29
 * @LastEditTime: 2021-07-27 17:54:42
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

#include "stm32f7xx.h"



#define vIP_GPIO_Mode_IN                 0x00000000U
#define vIP_GPIO_Mode_OUT                0x00000001U
#define vIP_GPIO_Mode_AF                 0x00000002U
#define vIP_GPIO_Mode_AN                 0x00000003U

#define vIP_GPIO_OutType_PP              0x00000000U
#define vIP_GPIO_OutType_OD              0x00000001U

#define vIP_GPIO_Speed_LOW               0x00000000U
#define vIP_GPIO_Speed_MEDIUM            0x00000001U
#define vIP_GPIO_Speed_HIGH              0x00000002U
#define vIP_GPIO_Speed_VERYHIGH          0x00000003U

#define vIP_GPIO_PuPd_PU                 0x00000001U
#define vIP_GPIO_PuPd_PD                 0x00000002U
#define vIP_GPIO_PuPd_NP                 0x00000000U


//设置GPIO模式
#define oIP_GPIO_SetMode(GPIOx,PinNum,Mode)        GPIOx->MODER   =(GPIOx->MODER   &(~(0x00000003U<< (PinNum*2)))) |(Mode<<(PinNum*2))

//设置GPIO输出类型
#define oIP_GPIO_SetOutType(GPIOx,PinNum,OutType)  GPIOx->OTYPER  =(GPIOx->OTYPER  &(~(0x00000001U<< PinNum)))     |(OutType<<PinNum)

//设置GPIO速度
#define oIP_GPIO_SetSpeed(GPIOx,PinNum,Speed)      GPIOx->OSPEEDR =(GPIOx->OSPEEDR &(~(0x00000003U<< (PinNum*2)))) |(Speed<<(PinNum*2))

//设置GPIO上下拉
#define oIP_GPIO_SetPuPd(GPIOx,PinNum,PuPd)        GPIOx->PUPDR   =(GPIOx->PUPDR   &(~(0x00000003U<< (PinNum*2)))) |(PuPd<<(PinNum*2))


//设置复用功能
#define oIP_GPIO_SetAF(GPIOx,PinNum,AFx)	GPIOx->AFR[PinNum>>3]=(GPIOx->AFR[PinNum>>3]&(~(0x0FU<<((PinNum&0x07U)*4U))))|(AFx<<((PinNum&0x07U)*4U))

//配置GPIO
#define oIP_GPIO_Config(GPIOx,PinNum,Mode,OutType,Speed,PuPd)   oIP_GPIO_SetMode(GPIOx,PinNum,Mode);\
                                                                oIP_GPIO_SetOutType(GPIOx,PinNum,OutType);\
                                                                oIP_GPIO_SetSpeed(GPIOx,PinNum,Speed);\
                                                                oIP_GPIO_SetPuPd(GPIOx,PinNum,PuPd)


//io号转低16位
#define cIP_GPIO_PinToLMsk(PinNum)                  (0x1U<<(PinNum))

//io号转高16位
#define cIP_GPIO_PinToHMsk(PinNum)                  (0x10000U<<(PinNum))


//获取引脚输入状态
#define oIP_GPIO_GetInputBit(GPIOx,PinNum)          (GPIOx->IDR&cIP_GPIO_PinToLMsk(PinNum))

//获取引脚输出状态
#define oIP_GPIO_GetOutputBit(GPIOx,PinNum)         (GPIOx->ODR&cIP_GPIO_PinToLMsk(PinNum))


//设置输出高
#define oIP_GPIO_SetOutput_High(GPIOx,PinNum)       GPIOx->BSRR=cIP_GPIO_PinToLMsk(PinNum)

//设置输出低
#define oIP_GPIO_SetOutput_Low(GPIOx,PinNum)        GPIOx->BSRR=cIP_GPIO_PinToHMsk(PinNum)


//输出电平翻转
#define oIP_GPIO_OutputBitReverse(GPIOx,PinNum)     GPIOx->BSRR=oIP_GPIO_GetOutputBit(GPIOx,PinNum)?cIP_GPIO_PinToHMsk(PinNum):cIP_GPIO_PinToLMsk(PinNum)






/**
 * @brief GPIO设置复用
 * @param GPIOx GPIO外设实例
 * @param GPIO_Num GPIO号 0~15
 * @param AFx 复用号 0~15
 * @return 无
 */
void IP_GPIO_SetAF(GPIO_TypeDef* GPIOx,unsigned char GPIO_Num,unsigned int AFx);

/**
 * @brief 配置GPIO
 * @param GPIOx GPIO外设实例
 * @param BITs GPIO端口掩码 可设置多个IO(掩码或运算 低16位)
 * @param Mode GPIO模式
 * @param OutType GPIO输出类型
 * @param Speed GPIO速度
 * @param PuPd GPIO上下拉类型
 * @return 无
 */
void IP_GPIO_Config(GPIO_TypeDef* GPIOx,unsigned int BITs,unsigned int Mode,unsigned int OutType,unsigned int Speed,unsigned int PuPd);


#endif //__IP_GPIO_H_



