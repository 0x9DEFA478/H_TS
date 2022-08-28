/*
 * @Author: 0x9DEFA478
 * @Date: 2021-04-21 18:51:32
 * @LastEditTime: 2021-12-18 20:27:53
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
#include "stm32f4xx_hal.h"


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
  HAL_NVIC_SetPriority(SVCall_IRQn,0U,0U);

  SystemClock_Config();
	TimeTick_Init(96000000,systick_irq_callback,NULL);
	
  MX_TIM5_Init();
  HAL_TIM_Base_Start(&htim5);

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
  // MX_SPI1_Init();
  // MX_I2S3_Init();
  MX_USART2_UART_Init();
  
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
  PLLI2S_SetStatus(0);

}

/**
 * @brief STM32时钟初始化
 * @return 无
 */
void SystemClock_Config(void){


  SystemClock_DeInit();

  RCC->APB1ENR|=RCC_APB1ENR_PWREN;

  PWR->CR=(PWR->CR&(~PWR_CR_VOS_Msk))|(0x3<<PWR_CR_VOS_Pos);//Scale1
  // while((PWR->CSR1&PWR_CSR1_VOSRDY)!=PWR_CSR1_VOSRDY){
  //   //等待电压等级就绪
  // }


  //PLL时钟选择为HSE
  RCC->PLLCFGR=(RCC->PLLCFGR&(~RCC_PLLCFGR_PLLSRC_Msk))|RCC_PLLCFGR_PLLSRC_HSE;

  PLL_SetDIV(25,192,2,4,2);
  //PLLI2S_SetDIV(15,188,2,2,3);

  PLL_SetStatus(1);
  //PLLI2S_SetStatus(1);

  //设置分频
  RCC->CFGR=(RCC->CFGR&(~(RCC_CFGR_HPRE_Msk|RCC_CFGR_PPRE1_Msk|RCC_CFGR_PPRE2_Msk)))
  |RCC_CFGR_HPRE_DIV1|RCC_CFGR_PPRE1_DIV2|RCC_CFGR_PPRE2_DIV1;

	//设置Flash延时
  FLASH->ACR=(FLASH->ACR&(~FLASH_ACR_LATENCY_Msk))|FLASH_ACR_LATENCY_3WS;
  //使能Flash预取 指令缓存 数据缓存
  FLASH->ACR|=FLASH_ACR_PRFTEN|FLASH_ACR_ICEN|FLASH_ACR_DCEN;

  //切换系统时钟到PLL
  RCC->CFGR = (RCC->CFGR&(~RCC_CFGR_SW_Msk))|RCC_CFGR_SW_PLL;
  while((RCC_CFGR_SWS_Msk & RCC->CFGR) != RCC_CFGR_SWS_PLL){
    //等待切换完毕
  }

  //更新SystemCoreClock
  //SystemCoreClockUpdate();
  SystemCoreClock=96000000U;


  PeripheralClkSelConfig();
  
}

/**
 * @brief 外设时钟源选择配置
 * @return 无
 */
static void PeripheralClkSelConfig(){

  RCC->DCKCFGR=

    //定时器 时钟源选择
    //0x0U：如果RCC_CFGR的 PPRE1 PPRE2 分频为1 则 TIMxCLK = PCLKx 否则 TIMxCLK = 2*PCLKx
    //0x1U：如果RCC_CFGR的 PPRE1 PPRE2 分频为1,2,4 则 TIMxCLK = HCLK 否则 TIMxCLK = 4*PCLKx
    (RCC_DCKCFGR_TIMPRE_Msk&(0x1U<<RCC_DCKCFGR_TIMPRE_Pos));


  RCC->CFGR&=(~RCC_CFGR_I2SSRC);//选择I2S时钟源

}

void MPU_Config(void){
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x00000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32B;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RO_URO;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected
  */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x08000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected
  */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.BaseAddress = 0x20000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_128KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}








