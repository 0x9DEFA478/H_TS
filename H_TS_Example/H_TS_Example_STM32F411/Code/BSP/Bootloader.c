/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-27 23:01:22
 * @LastEditTime: 2021-12-05 20:50:31
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
#include "Peripheral.h"


#define vBootLoader_DFUAddr 0x1FFF0000U

static void Bootloader(void);



/**
 * @brief 系统预初始化 初始化Qspi 片外RAM等 在SystemInit中调用
 * @return 无
 */
void BspPreInit(){


  //跳转条件判断 如果满足跳转条件 下面的代码就不会执行
  Bootloader();


	SCB->VTOR=0x08000000U;//设置中断向量表
	
  __DSB();
  __ISB();
}

/**
 * @brief 跳转到DFU 跳转前请保证外设关闭妥当
 * @return 无
 */
void Bootloader_JumpDFU(){
  
  __set_MSP(((unsigned int *)vBootLoader_DFUAddr)[0]);
	__set_CONTROL(0);

  ((void (**)(void))vBootLoader_DFUAddr)[1]();

  for(;;){
  }
}

static void Bootloader(){
  volatile int cnt;

  RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
  RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;

  oIP_GPIO_Config(GPIOB,9,vIP_GPIO_Mode_IN,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_LOW,vIP_GPIO_PuPd_PU);
  oIP_GPIO_Config(GPIOB,6,vIP_GPIO_Mode_IN,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_LOW,vIP_GPIO_PuPd_PU);
  oIP_GPIO_Config(GPIOB,8,vIP_GPIO_Mode_IN,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_LOW,vIP_GPIO_PuPd_PU);
  oIP_GPIO_Config(GPIOA,0,vIP_GPIO_Mode_IN,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_LOW,vIP_GPIO_PuPd_PU);

  cnt=1000000;
  while(cnt>0){
    cnt--;
  }

  if(oIP_GPIO_GetInputBit(GPIOB,9)&&oIP_GPIO_GetInputBit(GPIOB,6)&&oIP_GPIO_GetInputBit(GPIOB,8)&&oIP_GPIO_GetInputBit(GPIOA,0)){
    return;
  }else{
    Bootloader_JumpDFU();
  }
  
}





