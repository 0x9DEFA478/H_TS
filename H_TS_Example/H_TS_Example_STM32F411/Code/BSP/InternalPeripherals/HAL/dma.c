#include "dma.h"


/**
 * DMA1_0
 * DMA1_1
 * DMA1_2
 * DMA1_3
 * DMA1_4
 * DMA1_5  USART2_Rx
 * DMA1_6  USART2_Tx
 * DMA1_7
 * 
 * DMA2_0
 * DMA2_1
 * DMA2_2
 * DMA2_3
 * DMA2_4
 * DMA2_5
 * DMA2_6
 * DMA2_7
 * 
 */


void MX_DMA_Init(void){

  RCC->AHB1ENR|=RCC_AHB1ENR_DMA1EN;
  //RCC->AHB1ENR|=RCC_AHB1ENR_DMA2EN;

  
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);


}

void MX_DMA_DeInit(){


  HAL_NVIC_DisableIRQ(DMA1_Stream5_IRQn);
  HAL_NVIC_DisableIRQ(DMA1_Stream6_IRQn);

  RCC->AHB1ENR&=(~RCC_AHB1ENR_DMA1EN);
  //RCC->AHB1ENR&=(~RCC_AHB1ENR_DMA2EN);

}
