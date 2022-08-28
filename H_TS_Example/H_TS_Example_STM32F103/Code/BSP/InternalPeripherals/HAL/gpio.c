#include "gpio.h"
#include "IP_GPIO.h"










void MX_GPIO_Init(void){

  //GPIO_InitTypeDef GPIO_InitStruct = {0};

  RCC->APB2ENR|=RCC_APB2ENR_IOPDEN;

  RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
  RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;


  
  // //OLED
  // oIP_GPIO_Config(GPIOB,0,vIP_GPIO_Mode_OUT_PP_xMHz|vIP_GPIO_Mode_OUTAF_2MHz);//RES
  // oIP_GPIO_Config(GPIOB,1,vIP_GPIO_Mode_OUT_PP_xMHz|vIP_GPIO_Mode_OUTAF_10MHz);//DC
  // oIP_GPIO_Config(GPIOB,10,vIP_GPIO_Mode_OUT_PP_xMHz|vIP_GPIO_Mode_OUTAF_10MHz);//CS

  // //RC522
  // oIP_GPIO_Config(GPIOA,9,vIP_GPIO_Mode_OUT_PP_xMHz|vIP_GPIO_Mode_OUTAF_2MHz);//RES
  // oIP_GPIO_SetOutput_High(GPIOA,6);
  // oIP_GPIO_Config(GPIOA,6,vIP_GPIO_Mode_IN_PUPD);//IRQ
  // oIP_GPIO_Config(GPIOB,12,vIP_GPIO_Mode_OUT_PP_xMHz|vIP_GPIO_Mode_OUTAF_10MHz);//CS


  // //Button
  // oIP_GPIO_SetOutput_High(GPIOB,3);
  // oIP_GPIO_Config(GPIOB,3,vIP_GPIO_Mode_IN_PUPD);
  // oIP_GPIO_SetOutput_High(GPIOB,4);
  // oIP_GPIO_Config(GPIOB,4,vIP_GPIO_Mode_IN_PUPD);
  // oIP_GPIO_SetOutput_High(GPIOB,5);
  // oIP_GPIO_Config(GPIOB,5,vIP_GPIO_Mode_IN_PUPD);
  // oIP_GPIO_SetOutput_High(GPIOB,6);
  // oIP_GPIO_Config(GPIOB,6,vIP_GPIO_Mode_IN_PUPD);

  // //USB
  // oIP_GPIO_SetOutput_Low(GPIOA,12);
  // oIP_GPIO_Config(GPIOA,12,vIP_GPIO_Mode_OUT_PP_xMHz|vIP_GPIO_Mode_OUTAF_50MHz);
  // HAL_Delay(1);
  // oIP_GPIO_SetOutput_High(GPIOA,12);


}
