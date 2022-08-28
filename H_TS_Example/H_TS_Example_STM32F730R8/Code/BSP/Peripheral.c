/*
 * @Author: 0x9DEFA478
 * @Date: 2021-04-21 18:51:32
 * @LastEditTime: 2021-07-29 15:49:34
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
#include "stm32f7xx_hal.h"


/**
 * @brief STM32时钟初始化
 * @return 无
 */
void SystemClock_Config(void);

void MPU_Config(void);


void systick_irq_callback(void* v){
}


/**
 * @brief 系统初始化 (MPU 缓存 时钟)
 * @return 无
 */
void Peripherals_SysInit(){

  MPU_Config();
  SCB_EnableICache();
  SCB_EnableDCache();
  FLASH->ACR|=FLASH_ACR_ARTEN|FLASH_ACR_PRFTEN;//使能FLASH ART 和指令预取

  HAL_Init();

  SystemClock_Config();
	TimeTick_Init(216000000,systick_irq_callback,NULL);
	
}

/**
 * @brief 外设初始化
 * @return 无
 */
void Peripherals_Init(){
	
}





static void PeripheralClkSelConfig(void);

/**
 * @brief STM32时钟初始化
 * @return 无
 */
void SystemClock_Config(void){

  RCC->APB1ENR|=RCC_APB1ENR_PWREN;
	
  PWR->CR1=(PWR->CR1&(~PWR_CR1_VOS_Msk))|(0x3<<PWR_CR1_VOS_Pos);//Scale1
  // while((PWR->CSR1&PWR_CSR1_VOSRDY)!=PWR_CSR1_VOSRDY){
  //   //等待电压等级就绪
  // }

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


  PWR->CR1|=PWR_CR1_ODEN;
  while((PWR->CSR1&PWR_CSR1_ODRDY)!=PWR_CSR1_ODRDY){
    //等待OverDrive准备好
  }
  
  PWR->CR1|=PWR_CR1_ODSWEN;
  while((PWR->CSR1&PWR_CSR1_ODSWRDY)!=PWR_CSR1_ODSWRDY){
    //等待OverDrive准备好
  }


  //关闭所有的PLL
  PLL_SetStatus(0);
  PLLI2S_SetStatus(0);
  PLLSAI_SetStatus(0);

  //PLL时钟选择为HSE
  RCC->PLLCFGR|=RCC_PLLCFGR_PLLSRC_HSE;

  PLL_SetDIV(25,432,2,2,2);
  PLLI2S_SetDIV(192,2,2,2,1);
  PLLSAI_SetDIV(192,2,2,2,1);

  PLL_SetStatus(1);
  //PLLI2S_SetStatus(0);
  //PLLSAI_SetStatus(0);

  //设置分频
  RCC->CFGR=(RCC->CFGR&(~(RCC_CFGR_HPRE_Msk|RCC_CFGR_PPRE1_Msk|RCC_CFGR_PPRE2_Msk)))
  |RCC_CFGR_HPRE_DIV1|RCC_CFGR_PPRE1_DIV4|RCC_CFGR_PPRE2_DIV2;

  //设置Flash延时
  FLASH->ACR=(FLASH->ACR&(~FLASH_ACR_LATENCY_Msk))|FLASH_ACR_LATENCY_7WS;

  //切换系统时钟到PLL
  RCC->CFGR = (RCC->CFGR&(~RCC_CFGR_SW_Msk))|RCC_CFGR_SW_PLL;
  while((RCC_CFGR_SWS_Msk & RCC->CFGR) != RCC_CFGR_SWS_PLL){
    //等待切换完毕
  }

  //更新SystemCoreClock
  //SystemCoreClockUpdate();
  SystemCoreClock=218000000U;


  PeripheralClkSelConfig();
  
}

/**
 * @brief 外设时钟源选择配置
 * @return 无
 */
static void PeripheralClkSelConfig(){

  RCC->DCKCFGR1=(RCC->DCKCFGR1&(RCC_DCKCFGR1_PLLI2SDIVQ_Msk|RCC_DCKCFGR1_PLLSAIDIVQ_Msk))|

    //定时器 时钟源选择
    //0x0U：如果RCC_CFGR的 PPRE1 PPRE2 分频为1 则 TIMxCLK = PCLKx 否则 TIMxCLK = 2*PCLKx
    //0x1U：如果RCC_CFGR的 PPRE1 PPRE2 分频为1,2,4 则 TIMxCLK = PCLKx 否则 TIMxCLK = 4*PCLKx
    (RCC_DCKCFGR1_TIMPRE_Msk&(0x0U<<RCC_DCKCFGR1_TIMPRE_Pos))|

    //SAI2 内核时钟源选择
    //0x0U：将 f(PLLSAI_Q)/PLLSAIDIVQ 时钟选作内核外设时钟
    //0x1U：将 f(PLLI2S_Q)/PLLI2SDIVQ 时钟选作内核外设时钟
    //0x2U：将 I2S_CKIN 时钟选作内核外设时钟
    (RCC_DCKCFGR1_SAI2SEL_Msk&(0x0U<<RCC_DCKCFGR1_SAI2SEL_Pos))|

    //SAI1 内核时钟源选择
    //0x0U：将 f(PLLSAI_Q)/PLLSAIDIVQ 时钟选作内核外设时钟
    //0x1U：将 f(PLLI2S_Q)/PLLI2SDIVQ 时钟选作内核外设时钟
    //0x2U：将 I2S_CKIN 时钟选作内核外设时钟
    (RCC_DCKCFGR1_SAI1SEL_Msk&(0x0U<<RCC_DCKCFGR1_SAI1SEL_Pos));


  RCC->DCKCFGR2=

    //SDMMC2 时钟源选择
    //0x0U：PLL48CLK
    //0x1U：SYSCLK
    (RCC_DCKCFGR2_SDMMC2SEL_Msk&(0x0U<<RCC_DCKCFGR2_SDMMC2SEL_Pos))|

    //SDMMC1 时钟源选择
    //0x0U：PLL48CLK
    //0x1U：SYSCLK
    (RCC_DCKCFGR2_SDMMC1SEL_Msk&(0x0U<<RCC_DCKCFGR2_SDMMC1SEL_Pos))|

    //PLL48CLK 来源选择
    //0x0U：PLLQ
    //0x1U：PLLSAIP
    (RCC_DCKCFGR2_CK48MSEL_Msk&(0x0U<<RCC_DCKCFGR2_CK48MSEL_Pos))|

    //LPTIM1 时钟源选择
    //0x0U：PCLK1
    //0x1U：LSI
    //0x2U：HSI
    //0x3U：LSE
    (RCC_DCKCFGR2_LPTIM1SEL_Msk&(0x0U<<RCC_DCKCFGR2_LPTIM1SEL_Pos))|

    //I2C3 时钟源选择
    //0x0U：PCLK1
    //0x1U：SYSCLK
    //0x2U：HSI
    (RCC_DCKCFGR2_I2C3SEL_Msk&(0x0U<<RCC_DCKCFGR2_I2C3SEL_Pos))|

    //I2C2 时钟源选择
    //0x0U：PCLK1
    //0x1U：SYSCLK
    //0x2U：HSI
    (RCC_DCKCFGR2_I2C2SEL_Msk&(0x0U<<RCC_DCKCFGR2_I2C2SEL_Pos))|

    //I2C1 时钟源选择
    //0x0U：PCLK1
    //0x1U：SYSCLK
    //0x2U：HSI
    (RCC_DCKCFGR2_I2C1SEL_Msk&(0x0U<<RCC_DCKCFGR2_I2C1SEL_Pos))|

    //UART8 时钟源选择
    //0x0U：PCLK1
    //0x1U：SYSCLK
    //0x2U：HSI
    //0x3U：LSE
    (RCC_DCKCFGR2_UART8SEL_Msk&(0x0U<<RCC_DCKCFGR2_UART8SEL_Pos))|

    //UART7 时钟源选择
    //0x0U：PCLK1
    //0x1U：SYSCLK
    //0x2U：HSI
    //0x3U：LSE
    (RCC_DCKCFGR2_UART7SEL_Msk&(0x0U<<RCC_DCKCFGR2_UART7SEL_Pos))|

    //USART6 时钟源选择
    //0x0U：PCLK2
    //0x1U：SYSCLK
    //0x2U：HSI
    //0x3U：LSE
    (RCC_DCKCFGR2_USART6SEL_Msk&(0x0U<<RCC_DCKCFGR2_USART6SEL_Pos))|

    //UART5 时钟源选择
    //0x0U：PCLK1
    //0x1U：SYSCLK
    //0x2U：HSI
    //0x3U：LSE
    (RCC_DCKCFGR2_UART5SEL_Msk&(0x0U<<RCC_DCKCFGR2_UART5SEL_Pos))|

    //UART4 时钟源选择
    //0x0U：PCLK1
    //0x1U：SYSCLK
    //0x2U：HSI
    //0x3U：LSE
    (RCC_DCKCFGR2_UART4SEL_Msk&(0x0U<<RCC_DCKCFGR2_UART4SEL_Pos))|

    //USART3 时钟源选择
    //0x0U：PCLK1
    //0x1U：SYSCLK
    //0x2U：HSI
    //0x3U：LSE
    (RCC_DCKCFGR2_USART3SEL_Msk&(0x0U<<RCC_DCKCFGR2_USART3SEL_Pos))|

    //USART2 时钟源选择
    //0x0U：PCLK1
    //0x1U：SYSCLK
    //0x2U：HSI
    //0x3U：LSE
    (RCC_DCKCFGR2_USART2SEL_Msk&(0x0U<<RCC_DCKCFGR2_USART2SEL_Pos))|

    //USART1 时钟源选择
    //0x0U：PCLK2
    //0x1U：SYSCLK
    //0x2U：HSI
    //0x3U：LSE
    (RCC_DCKCFGR2_USART1SEL_Msk&(0x0U<<RCC_DCKCFGR2_USART1SEL_Pos));

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
  MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
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
  MPU_InitStruct.BaseAddress = 0x00200000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER3;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RO_URO;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER4;
  MPU_InitStruct.BaseAddress = 0x20000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_64KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER5;
  MPU_InitStruct.BaseAddress = 0x20010000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}








//================================================================================================================================================================
//
//        quadspi
//
//================================================================================================================================================================

#define quadspi_BK1CS_GPIOx			GPIOB
#define quadspi_BK1CS_PinNum		6



#define quadspi_SCK_GPIOx				GPIOB
#define quadspi_SCK_PinNum			2

#define quadspi_BK1IO0_GPIOx		GPIOC
#define quadspi_BK1IO0_PinNum		9

#define quadspi_BK1IO1_GPIOx		GPIOC
#define quadspi_BK1IO1_PinNum		10

//#define quadspi_BK1IO2_GPIOx		GPIOE
//#define quadspi_BK1IO2_PinNum		2

//#define quadspi_BK1IO3_GPIOx		GPIOD
//#define quadspi_BK1IO3_PinNum		13


//#define BK2CS_quadspi_GPIOx			
//#define BK2CS_quadspi_PinNum		

//#define quadspi_BK2IO0_GPIOx		GPIOH
//#define quadspi_BK2IO0_PinNum		2

//#define quadspi_BK2IO1_GPIOx		GPIOH
//#define quadspi_BK2IO1_PinNum		3

//#define quadspi_BK2IO2_GPIOx		GPIOG
//#define quadspi_BK2IO2_PinNum		9

//#define quadspi_BK2IO3_GPIOx		GPIOG
//#define quadspi_BK2IO3_PinNum		14

/**
 * @brief quadspi初始化
 * @return 无
 */
static void quadspi_dualBank_init(){

  RCC->AHB3ENR|=RCC_AHB3ENR_QSPIEN;
  RCC->AHB1ENR|=(RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_GPIOCEN);

  oIP_GPIO_Config(quadspi_SCK_GPIOx,    quadspi_SCK_PinNum,   vIP_GPIO_Mode_AF,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_VERYHIGH,vIP_GPIO_PuPd_NP);

  oIP_GPIO_Config(quadspi_BK1CS_GPIOx,	quadspi_BK1CS_PinNum ,vIP_GPIO_Mode_AF,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_VERYHIGH,vIP_GPIO_PuPd_NP);
  oIP_GPIO_Config(quadspi_BK1IO0_GPIOx,	quadspi_BK1IO0_PinNum,vIP_GPIO_Mode_AF,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_VERYHIGH,vIP_GPIO_PuPd_NP);
  oIP_GPIO_Config(quadspi_BK1IO1_GPIOx,	quadspi_BK1IO1_PinNum,vIP_GPIO_Mode_AF,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_VERYHIGH,vIP_GPIO_PuPd_NP);
  // oIP_GPIO_Config(quadspi_BK1IO2_GPIOx,	quadspi_BK1IO2_PinNum,vIP_GPIO_Mode_AF,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_VERYHIGH,vIP_GPIO_PuPd_NP);
  // oIP_GPIO_Config(quadspi_BK1IO3_GPIOx,	quadspi_BK1IO3_PinNum,vIP_GPIO_Mode_AF,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_VERYHIGH,vIP_GPIO_PuPd_NP);

  // oIP_GPIO_Config(quadspi_BK2IO0_GPIOx,	quadspi_BK2IO0_PinNum,vIP_GPIO_Mode_AF,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_VERYHIGH,vIP_GPIO_PuPd_NP);
  // oIP_GPIO_Config(quadspi_BK2IO1_GPIOx,	quadspi_BK2IO1_PinNum,vIP_GPIO_Mode_AF,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_VERYHIGH,vIP_GPIO_PuPd_NP);
  // oIP_GPIO_Config(quadspi_BK2IO2_GPIOx,	quadspi_BK2IO2_PinNum,vIP_GPIO_Mode_AF,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_VERYHIGH,vIP_GPIO_PuPd_NP);
  // oIP_GPIO_Config(quadspi_BK2IO3_GPIOx,	quadspi_BK2IO3_PinNum,vIP_GPIO_Mode_AF,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_VERYHIGH,vIP_GPIO_PuPd_NP);


  oIP_GPIO_SetAF(quadspi_SCK_GPIOx,		quadspi_SCK_PinNum,	 9U);

  oIP_GPIO_SetAF(quadspi_BK1CS_GPIOx,	quadspi_BK1CS_PinNum,10U);
  oIP_GPIO_SetAF(quadspi_BK1IO0_GPIOx,quadspi_BK1IO0_PinNum,9U);
  oIP_GPIO_SetAF(quadspi_BK1IO1_GPIOx,quadspi_BK1IO1_PinNum,9U);
  // oIP_GPIO_SetAF(quadspi_BK1IO2_GPIOx,quadspi_BK1IO2_PinNum,9U);
  // oIP_GPIO_SetAF(quadspi_BK1IO3_GPIOx,quadspi_BK1IO3_PinNum,9U);

  // oIP_GPIO_SetAF(quadspi_BK2IO0_GPIOx,quadspi_BK2IO0_PinNum,9U);
  // oIP_GPIO_SetAF(quadspi_BK2IO1_GPIOx,quadspi_BK2IO1_PinNum,9U);
  // oIP_GPIO_SetAF(quadspi_BK2IO2_GPIOx,quadspi_BK2IO2_PinNum,9U);
  // oIP_GPIO_SetAF(quadspi_BK2IO3_GPIOx,quadspi_BK2IO3_PinNum,9U);



  QUADSPI->CR=0U;

  QUADSPI->CR|=((2U-1U)<<QUADSPI_CR_PRESCALER_Pos)    //设置分频 2分频
    |((4U-1U)<<QUADSPI_CR_FTHRES_Pos)                 //设置FIFO阈值为4
    |(0x0U<<QUADSPI_CR_FSEL_Pos)                      //设置bank bank(x+1)
    |(0x0U<<QUADSPI_CR_DFM_Pos)                       //双闪存模式
    |(0x1U<<QUADSPI_CR_SSHIFT_Pos)                    //延迟采样
    |(0x0U<<QUADSPI_CR_TCEN_Pos);                     //超时定时器是否使能

  QUADSPI->DCR=(23U<<QUADSPI_DCR_FSIZE_Pos)           //设置寻址大小 2^(x+1) 字节   25:64MB 20:2MB
    |((2U-1U)<<QUADSPI_DCR_CSHT_Pos)                  //设置cs最小高电平时间
    |(0U<<QUADSPI_DCR_CKMODE_Pos);                     //设置mode0 空闲低电平

  QUADSPI->CR|=QUADSPI_CR_EN;//使能qspi

}




/**
 * @brief 设置quadspi为内存映射模式
 * @return 无
 */
static void quadspi_SetMemoryMapped(){

  //设置交替字节
  QUADSPI->ABR=0xFFFFFFFF;

  //dataMode
  //0：无数据
  //1：单线传输数据
  //2：双线传输数据
  //3：四线传输数据

  //DummyCycles空闲时钟数

  //AlternateBytesSize
  //0：8 位交替字节
  //1：16 位交替字节
  //2：24 位交替字节
  //3：32 位交替字节

  //AlternateByteMode
  //0：无交替字节
  //1：单线传输交替字节
  //2：双线传输交替字节
  //3：四线传输交替字节

  //AddressSize
  //0：8 位地址
  //1：16 位地址
  //2：24 位地址
  //3：32 位地址

  //AddressMode
  //0：无地址
  //1：单线传输地址
  //2：双线传输地址
  //3：四线传输地址

  //cmdMode
  //0：无指令
  //1：单线传输指令
  //2：双线传输指令
  //3：四线传输指令

  QUADSPI->CCR=(0<<QUADSPI_CCR_DDRM_Pos)
    |(0<<QUADSPI_CCR_SIOO_Pos)
    |(QUADSPI_CCR_DMODE_Msk         &(0x02U <<QUADSPI_CCR_DMODE_Pos))         //dataMode
    |(QUADSPI_CCR_DCYC_Msk          &(0x00U <<QUADSPI_CCR_DCYC_Pos))          //DummyCycles
    |(QUADSPI_CCR_ABSIZE_Msk        &(0x00U <<QUADSPI_CCR_ABSIZE_Pos))        //AlternateBytesSize
    |(QUADSPI_CCR_ABMODE_Msk        &(0x02U <<QUADSPI_CCR_ABMODE_Pos))        //AlternateByteMode
    |(QUADSPI_CCR_ADSIZE_Msk        &(0x02U <<QUADSPI_CCR_ADSIZE_Pos))        //AddressSize
    |(QUADSPI_CCR_ADMODE_Msk        &(0x02U <<QUADSPI_CCR_ADMODE_Pos))        //AddressMode
    |(QUADSPI_CCR_IMODE_Msk         &(0x01U <<QUADSPI_CCR_IMODE_Pos))         //cmdMode
    |(QUADSPI_CCR_INSTRUCTION_Msk   &(0xBBU <<QUADSPI_CCR_INSTRUCTION_Pos))   //cmd
    |(QUADSPI_CCR_FMODE_Msk         &(0x03U <<QUADSPI_CCR_FMODE_Pos));        //内存映射模式 0:间接写入模式 1:间接读取模式 2:自动轮询模式 3:内存映射模式
                                                                      

}

extern void JumpBoot(void);

/**
 * @brief 系统预初始化 初始化Qspi 片外RAM等 在SystemInit中调用
 * @return 无
 */
void BspPreInit(){


  //跳转条件判断 如果满足跳转条件 下面的代码就不会执行
  JumpBoot();


  SCB->VTOR=0x08000000U;//设置中断向量表
	
  //Qspi
//  quadspi_dualBank_init();
//  quadspi_SetMemoryMapped();
//  __DSB();
//  __ISB();
}


