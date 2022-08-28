#include "usart.h"
#include "Peripherals_Instence.h"
#include "MemoryMalloc.h"


#define USART_Handle_MemoryMalloc                   MemoryMalloc_SRAM
#define USART_Peripheral_MemoryMalloc               MemoryMalloc_SRAM

#define USART_Handle_TxBufferSize                   512
#define USART_Handle_RxBufferSize                   1024

#define USART_Stream_IOThread_StackSize             512
#define USART_Stream_IOThread_Priority              32767



USART_Handle* usart2_Handle;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;



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


void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

  ip_usart2=USART_Handle_MemoryMalloc.Malloc(sizeof(IP_USART));
  ip_usart2_txdma=USART_Handle_MemoryMalloc.Malloc(sizeof(IP_DMA));
  ip_usart2_rxdma=USART_Handle_MemoryMalloc.Malloc(sizeof(IP_DMA));

  if(cH_IsNull(ip_usart2)||cH_IsNull(ip_usart2_txdma)||cH_IsNull(ip_usart2_rxdma)){
    for(;;){
    }
  }

	extern void USART2_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
  USART2_SetIrqCallback((void(*)(void*))IP_USART_IRQ,ip_usart2);

  IP_DMA_Init(ip_usart2_txdma,huart2.hdmatx->Instance);
  IP_DMA_Init(ip_usart2_rxdma,huart2.hdmarx->Instance);
  IP_USART_Init(ip_usart2,ip_usart2_txdma,ip_usart2_rxdma,huart2.Instance);

  usart2_Handle=USART_CreateStream(ip_usart2,4096,4096,(H_MemoryMalloc*)&USART_Handle_MemoryMalloc);
  if(cH_IsNull(usart2_Handle)){
    for(;;){
    }
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Stream5;
    hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);

    /* USART2_TX Init */
    hdma_usart2_tx.Instance = DMA1_Stream6;
    hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_usart2_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_usart2_tx.Init.MemBurst = DMA_MBURST_INC16;
    hdma_usart2_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart2_tx);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

