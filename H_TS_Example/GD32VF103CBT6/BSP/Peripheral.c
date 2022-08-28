/*
 * @Author: 0x9DEFA478
 * @Date: 2022-01-11 18:09:07
 * @LastEditTime: 2022-01-13 20:49:26
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
#include "gd32vf103.h"


/**
 * @brief GD32时钟初始化
 * @return 无
 */
void SystemClock_Config(void);

/**
 * @brief 将时钟恢复到复位状态(关闭PLL 使用HSI为系统时钟)
 * @return 无
 */
void SystemClock_DeInit(void);

//void MPU_Config(void);


static void PeripheralClkSelConfig(void);



void systick_irq_callback(void* v){
}


/**
 * @brief 系统初始化 (MPU 缓存 时钟)
 * @return 无
 */
void Peripherals_SysInit(){

  eclic_set_nlbits(((*(volatile unsigned int*)(ECLIC_ADDR_BASE+ECLIC_INFO_OFFSET))>>21)&0x0000000FU);
  eclic_global_interrupt_enable();

  //打开相应的异常中断
  eclic_set_irq_lvl_abs(CLIC_INT_BWEI,0xF);
  eclic_set_irq_priority(CLIC_INT_BWEI,0);
  eclic_set_nonvmode(CLIC_INT_BWEI);
  eclic_set_level_trig(CLIC_INT_BWEI);
  eclic_enable_interrupt(CLIC_INT_BWEI);

  SystemClock_Config();
  TimeTick_Init(TIMER_FREQ,systick_irq_callback,NULL);
  
}

/**
 * @brief 外设初始化
 * @return 无
 */
void Peripherals_Init(){

  Peripherals_Instence_LockInit();

  H_TS_LockLock(ip_rcc_Lock);

  GPIO_Init();
  MemoryMalloc_Init();

  DMA_Init();
  USART0_Init();

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

  I_DebugPort_SetValid(0);

  H_TS_LockLock(ip_usart0_Lock);
  USART0_DeInit();

  DMA_DeInit();

  SystemClock_DeInit();
  
}





/**
 * @brief 将时钟恢复到复位状态(关闭PLL 使用HSI为系统时钟)
 * @return 无
 */
void SystemClock_DeInit(){
  
  

}

/**
 * @brief STM32时钟初始化
 * @return 无
 */
void SystemClock_Config(void){

  SystemClock_DeInit();

  


  PeripheralClkSelConfig();
  
}

/**
 * @brief 外设时钟源选择配置
 * @return 无
 */
static void PeripheralClkSelConfig(){

  

}

















