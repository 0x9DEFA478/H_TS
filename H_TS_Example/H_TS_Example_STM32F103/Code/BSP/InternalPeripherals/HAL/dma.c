#include "dma.h"


/**
 * DMA1_0
 * DMA1_1
 * DMA1_2
 * DMA1_3
 * DMA1_4  USART2_Tx
 * DMA1_5  USART2_Rx
 * DMA1_6
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

  RCC->AHBENR|=RCC_AHBENR_DMA1EN;


  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

void MX_DMA_DeInit(){

  HAL_NVIC_DisableIRQ(DMA1_Channel4_IRQn);
  HAL_NVIC_DisableIRQ(DMA1_Channel5_IRQn);

  RCC->AHBENR&=(~RCC_AHBENR_DMA1EN);

}
