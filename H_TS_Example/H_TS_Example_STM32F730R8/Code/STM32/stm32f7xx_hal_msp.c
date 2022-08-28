#include "main.h"

void HAL_MspInit(void)
{


  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  __NVIC_SetPriorityGrouping((0x7U-__NVIC_PRIO_BITS));
  
}
