/*
 * @Author: 0x9DEFA478
 * @Date: 2021-04-21 20:23:19
 * @LastEditTime: 2021-04-21 22:18:36
 * @LastEditors: 0x9DEFA478
 * @Description: GPIO功能实现
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
#include "IP_GPIO.h"


/**
 * @brief GPIO设置复用
 * @param GPIOx GPIO外设实例
 * @param GPIO_Num GPIO号 0~15
 * @param AFx 复用号 0~15
 * @return 无
 */
void IP_GPIO_SetAF(GPIO_TypeDef* GPIOx,unsigned char GPIO_Num,unsigned int AFx){

	oIP_GPIO_SetAF(GPIOx,GPIO_Num,AFx);
} 

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
void IP_GPIO_Config(GPIO_TypeDef* GPIOx,unsigned int BITs,unsigned int Mode,unsigned int OutType,unsigned int Speed,unsigned int PuPd){
	unsigned int pinpos = 0, pos = 0;
	for (pinpos = 0; pinpos < 16; pinpos++){
		pos = 1 << pinpos;
		if (BITs & pos){
			oIP_GPIO_SetMode(GPIOx,pinpos,Mode);
			if ((Mode == vIP_GPIO_Mode_OUT) || (Mode == vIP_GPIO_Mode_AF)){
				oIP_GPIO_SetSpeed(GPIOx,pinpos,Speed);
				oIP_GPIO_SetOutType(GPIOx,pinpos,OutType);
			}
			oIP_GPIO_SetPuPd(GPIOx,pinpos,PuPd);
		}
	}
}
