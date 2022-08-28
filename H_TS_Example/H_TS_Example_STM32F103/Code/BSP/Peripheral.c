/*
 * @Author: 0x9DEFA478
 * @Date: 2021-04-21 18:51:32
 * @LastEditTime: 2021-12-06 20:44:49
 * @LastEditors: 0x9DEFA478
 * @Description: 外设方法 在此进行实例化和初始化
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
#include "stm32f1xx_hal.h"


/**
 * @brief STM32时钟初始化
 * @return 无
 */
void SystemClock_Config(void);

/**
 * @brief 将时钟恢复到复位状态(关闭PLL 使用HSI为系统时钟)
 * @return 无
 */
void SystemClock_DeInit(void);

void MPU_Config(void);


static void PeripheralClkSelConfig(void);



void systick_irq_callback(void* v){
}


/**
 * @brief 系统初始化 (MPU 缓存 时钟)
 * @return 无
 */
void Peripherals_SysInit(){

  MPU_Config();
  // SCB_EnableICache();
  // SCB_EnableDCache();

  HAL_Init();

  SystemClock_Config();
	TimeTick_Init(72000000,systick_irq_callback,NULL);
	
  MX_TIM4_Init();
  HAL_TIM_Base_Start(&htim4);

}

/**
 * @brief 外设初始化
 * @return 无
 */
void Peripherals_Init(){

  Peripherals_Instence_LockInit();

  H_TS_LockLock(ip_rcc_Lock);

  MX_GPIO_Init();
  MemoryMalloc_Init();

  MX_DMA_Init();
  MX_USART1_UART_Init();

  I_DebugPort_Init();
  I_DebugPort_SetValid(-1);
  H_Printf_Init();


  H_TS_LockUnlock(ip_rcc_Lock);
  
}

/**
 * @brief 关闭所有外设(用于跳转bootloader之前)
 * @return 无
 */
void Peripherals_DeInit(){

  

  H_TS_LockLock(ip_rcc_Lock);

  

  

  MX_DMA_DeInit();

  SysTick->CTRL=0;

  SystemClock_DeInit();
  
}





/**
 * @brief 将时钟恢复到复位状态(关闭PLL 使用HSI为系统时钟)
 * @return 无
 */
void SystemClock_DeInit(){
  
  if((RCC->CFGR&RCC_CFGR_SWS_Msk)!=RCC_CFGR_SWS_HSI){
    //如果系统时钟源不为HSI

    if(RCC->CR&RCC_CR_HSION){
      RCC->CR&=(~RCC_CR_HSION);
    }
    while(RCC->CR&RCC_CR_HSIRDY){
      //等待HSI关闭
    }

    if((RCC->CR&RCC_CR_HSION)!=RCC_CR_HSION){
      RCC->CR|=RCC_CR_HSION;//使能HSI
      while(!(RCC->CR&RCC_CR_HSIRDY)){
        //等待HSI稳定
      }
    }

    RCC->CFGR=(RCC->CFGR&(~RCC_CFGR_SW_Msk))|RCC_CFGR_SW_HSI;
    while((RCC->CFGR&RCC_CFGR_SWS_Msk)!=RCC_CFGR_SWS_HSI){
      //等待时钟切换完成
    }
  }
  //此时系统时钟变为HSI

  //如果HSE未使能
  if(!(RCC->CR&RCC_CR_HSEON)){
    RCC->CR|=RCC_CR_HSEON;
  }
  while(!(RCC->CR&RCC_CR_HSERDY)){
    //等待HSE稳定
  }

  //关闭所有的PLL
  PLL_SetStatus(0);

}

/**
 * @brief STM32时钟初始化
 * @return 无
 */
void SystemClock_Config(void){


  SystemClock_DeInit();


  //PLL时钟选择为HSE
  RCC->CFGR=(RCC->CFGR&(~RCC_CFGR_PLLSRC_Msk))|(0x1U<<RCC_CFGR_PLLSRC_Pos);
  //HSE不分频
  RCC->CFGR=(RCC->CFGR&(~RCC_CFGR_PLLXTPRE_Msk))|(0x0U<<RCC_CFGR_PLLXTPRE_Pos);
  //USB 1.5分频
  RCC->CFGR=(RCC->CFGR&(~RCC_CFGR_USBPRE_Msk))|(0x0U<<RCC_CFGR_USBPRE_Pos);


  PLL_SetDIV(9);

  PLL_SetStatus(1);

  //设置分频
  RCC->CFGR=(RCC->CFGR&(~(RCC_CFGR_HPRE_Msk|RCC_CFGR_PPRE1_Msk|RCC_CFGR_PPRE2_Msk)))
  |RCC_CFGR_HPRE_DIV1|RCC_CFGR_PPRE1_DIV2|RCC_CFGR_PPRE2_DIV1;

	//设置Flash延时
  FLASH->ACR=(FLASH->ACR&(~FLASH_ACR_LATENCY_Msk))|(0x2U<<FLASH_ACR_LATENCY_Pos);
  //使能Flash预取 指令缓存 数据缓存
  FLASH->ACR|=FLASH_ACR_PRFTBE;

  //切换系统时钟到PLL
  RCC->CFGR = (RCC->CFGR&(~RCC_CFGR_SW_Msk))|RCC_CFGR_SW_PLL;
  while((RCC_CFGR_SWS_Msk & RCC->CFGR) != RCC_CFGR_SWS_PLL){
    //等待切换完毕
  }

  //更新SystemCoreClock
  //SystemCoreClockUpdate();
  SystemCoreClock=72000000U;


  PeripheralClkSelConfig();
  
}

/**
 * @brief 外设时钟源选择配置
 * @return 无
 */
static void PeripheralClkSelConfig(){
}

void MPU_Config(void){

}








