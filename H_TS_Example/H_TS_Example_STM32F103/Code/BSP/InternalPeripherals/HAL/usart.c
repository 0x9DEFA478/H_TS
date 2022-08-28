#include "usart.h"
#include "Peripherals_Instence.h"
#include "MemoryMalloc.h"


#define USART_Handle_MemoryMalloc                   MemoryMalloc_SRAM
#define USART_Peripheral_MemoryMalloc               MemoryMalloc_SRAM

#define USART_Handle_TxBufferSize                   512
#define USART_Handle_RxBufferSize                   1024

#define USART_Stream_IOThread_StackSize             512
#define USART_Stream_IOThread_Priority              32767



USART_Handle* usart1_Handle;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;



//为串口新建流
USART_Handle* USART_CreateStream(IP_USART* ip_usart,int ReadBufferSize,int WriteBufferSize,H_MemoryMalloc* memoryMalloc){

  USART_Handle* r;
  H_Stream_InitParam InitParam;


  r=memoryMalloc->Malloc(sizeof(USART_Handle));
  if(r==NULL){
    goto Error0;
  }
  
  r->Buffer=USART_Peripheral_MemoryMalloc.Malloc(USART_Handle_RxBufferSize+USART_Handle_TxBufferSize);
  if(r->Buffer==NULL){
    goto Error1;
  }

  InitParam.Type=vH_FIFO_Type_byte;
  InitParam.Read.BufferSize=ReadBufferSize;
  if(WriteBufferSize>0){
    InitParam.Write.IsUseTransmitter=-1;

    InitParam.Write.TransmitterParam.BufferSize=WriteBufferSize;
    InitParam.Write.TransmitterParam.ThreadStackSize=USART_Stream_IOThread_StackSize;
    InitParam.Write.TransmitterParam.Priority=USART_Stream_IOThread_Priority;
    InitParam.Write.TransmitterParam.TransmitThreadName="USART_Stream";
  }else{
    InitParam.Write.IsUseTransmitter=0;
  }
  InitParam.Write.LL_Function.Transmit=(void (*)(void*,void*,int))IP_USART_Transmit;
  InitParam.Write.LL_Function.v=ip_usart;
  r->Stream=new_H_Stream(&InitParam,memoryMalloc);

  if(r->Stream==NULL){
    goto Error2;
  }
  
  r->ip_usart=ip_usart;
  r->memoryMalloc=memoryMalloc;

  //句柄新建成功

  IP_USART_Start(ip_usart
    ,&((Hbyte*)r->Buffer)[USART_Handle_RxBufferSize],USART_Handle_TxBufferSize,r->Buffer,USART_Handle_RxBufferSize
    ,H_Stream_LL_TransmitDone_ISR,H_Stream_LL_Input_ISR,r->Stream);

  return r;

Error2:
  USART_Peripheral_MemoryMalloc.Free(r->Buffer);
Error1:
  memoryMalloc->Free(r);
Error0:
  return NULL;
}

//为串口关闭流
void USART_DisposeStream(USART_Handle* usart_handle){

  IP_USART_Stop(usart_handle->ip_usart);//先停止底层
  delete_H_Stream(usart_handle->Stream);
  USART_Peripheral_MemoryMalloc.Free(usart_handle->Buffer);
  ((H_MemoryMalloc*)usart_handle->memoryMalloc)->Free(usart_handle);
}


void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

  ip_usart1=USART_Handle_MemoryMalloc.Malloc(sizeof(IP_USART));
  ip_usart1_txdma=USART_Handle_MemoryMalloc.Malloc(sizeof(IP_DMA));
  ip_usart1_rxdma=USART_Handle_MemoryMalloc.Malloc(sizeof(IP_DMA));

  if(cH_IsNull(ip_usart1)||cH_IsNull(ip_usart1_txdma)||cH_IsNull(ip_usart1_rxdma)){
    for(;;){
    }
  }

	extern void USART1_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
  USART1_SetIrqCallback((void(*)(void*))IP_USART_IRQ,ip_usart1);

  IP_DMA_Init(ip_usart1_txdma,huart1.hdmatx->Instance);
  IP_DMA_Init(ip_usart1_rxdma,huart1.hdmarx->Instance);
  IP_USART_Init(ip_usart1,ip_usart1_txdma,ip_usart1_rxdma,huart1.Instance);

  usart1_Handle=USART_CreateStream(ip_usart1,2048,2048,(H_MemoryMalloc*)&USART_Handle_MemoryMalloc);
  if(cH_IsNull(usart1_Handle)){
    for(;;){
    }
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
