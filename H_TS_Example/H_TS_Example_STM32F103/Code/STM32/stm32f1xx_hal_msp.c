#include "main.h"

void HAL_MspInit(void){
  

  RCC->APB2ENR|=RCC_APB2ENR_AFIOEN;
  RCC->APB1ENR|=RCC_APB1ENR_PWREN;

  /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
  */
  __HAL_AFIO_REMAP_SWJ_NOJTAG();

  __NVIC_SetPriorityGrouping((0x7U-__NVIC_PRIO_BITS));
  
}

void HAL_MspDeInit(void){
}

