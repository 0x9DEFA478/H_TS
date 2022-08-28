#include "gpio.h"
#include "IP_GPIO.h"

void MX_GPIO_Init(void)
{

  //GPIO_InitTypeDef GPIO_InitStruct = {0};

  RCC->AHB1ENR|=RCC_AHB1ENR_GPIOHEN;

  RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
  RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;


  

  
  //Button
  oIP_GPIO_Config(GPIOB,9,vIP_GPIO_Mode_IN,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_LOW,vIP_GPIO_PuPd_PU);
  oIP_GPIO_Config(GPIOB,6,vIP_GPIO_Mode_IN,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_LOW,vIP_GPIO_PuPd_PU);
  oIP_GPIO_Config(GPIOB,8,vIP_GPIO_Mode_IN,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_LOW,vIP_GPIO_PuPd_PU);
  oIP_GPIO_Config(GPIOA,0,vIP_GPIO_Mode_IN,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_LOW,vIP_GPIO_PuPd_PU);


}
