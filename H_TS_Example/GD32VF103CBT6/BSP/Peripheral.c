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
 * @brief GD32???????????????
 * @return ???
 */
void SystemClock_Config(void);

/**
 * @brief ??????????????????????????????(??????PLL ??????HSI???????????????)
 * @return ???
 */
void SystemClock_DeInit(void);

//void MPU_Config(void);


static void PeripheralClkSelConfig(void);



void systick_irq_callback(void* v){
}


/**
 * @brief ??????????????? (MPU ?????? ??????)
 * @return ???
 */
void Peripherals_SysInit(){

  eclic_set_nlbits(((*(volatile unsigned int*)(ECLIC_ADDR_BASE+ECLIC_INFO_OFFSET))>>21)&0x0000000FU);
  eclic_global_interrupt_enable();

  //???????????????????????????
  eclic_set_irq_lvl_abs(CLIC_INT_BWEI,0xF);
  eclic_set_irq_priority(CLIC_INT_BWEI,0);
  eclic_set_nonvmode(CLIC_INT_BWEI);
  eclic_set_level_trig(CLIC_INT_BWEI);
  eclic_enable_interrupt(CLIC_INT_BWEI);

  SystemClock_Config();
  TimeTick_Init(TIMER_FREQ,systick_irq_callback,NULL);
  
}

/**
 * @brief ???????????????
 * @return ???
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
 * @brief ??????????????????(????????????bootloader??????)
 * @return ???
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
 * @brief ??????????????????????????????(??????PLL ??????HSI???????????????)
 * @return ???
 */
void SystemClock_DeInit(){
  
  

}

/**
 * @brief STM32???????????????
 * @return ???
 */
void SystemClock_Config(void){

  SystemClock_DeInit();

  


  PeripheralClkSelConfig();
  
}

/**
 * @brief ???????????????????????????
 * @return ???
 */
static void PeripheralClkSelConfig(){

  

}

















