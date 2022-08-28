/*
 * @Author: 0x9DEFA478
 * @Date: 2022-01-13 18:18:20
 * @LastEditTime: 2022-01-13 22:52:12
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
#include "usart.h"
#include "IP_GPIO.h"
#include "Peripherals_Instence.h"
#include "MemoryMalloc.h"


#define USART_Handle_MemoryMalloc                   MemoryMalloc_SRAM
#define USART_Peripheral_MemoryMalloc               MemoryMalloc_SRAM

#define USART_Handle_TxBufferSize                   512
#define USART_Handle_RxBufferSize                   1024

#define USART_Stream_IOThread_StackSize             512
#define USART_Stream_IOThread_Priority              32767



USART_Handle* usart0_Handle;



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




void USART0_Init(){
  dma_parameter_struct dma_init_struct;

  rcu_periph_clock_enable(RCU_USART0);
  rcu_periph_clock_enable(RCU_GPIOA);

  oIP_GPIO_SetOutput_High(GPIOA,9);
  oIP_GPIO_SetOutput_High(GPIOA,10);
  oIP_GPIO_Config(GPIOA,9,vIP_GPIO_Mode_AF_PP_xMHz|vIP_GPIO_Mode_OUTAF_50MHz);
  oIP_GPIO_Config(GPIOA,10,vIP_GPIO_Mode_IN_FLOAT);


  //TxDMA
  dma_deinit(DMA0,DMA_CH3);
  dma_struct_para_init(&dma_init_struct);
  dma_init_struct.direction=DMA_MEMORY_TO_PERIPHERAL;
  dma_init_struct.memory_addr=0U;
  dma_init_struct.memory_inc=DMA_MEMORY_INCREASE_ENABLE;
  dma_init_struct.memory_width=DMA_MEMORY_WIDTH_8BIT;
  dma_init_struct.number=0U;
  dma_init_struct.periph_addr=0U;
  dma_init_struct.periph_inc=DMA_PERIPH_INCREASE_DISABLE;
  dma_init_struct.periph_width=DMA_PERIPHERAL_WIDTH_8BIT;
  dma_init_struct.priority=DMA_PRIORITY_LOW;
  dma_init(DMA0,DMA_CH3,&dma_init_struct);

  //RxDMA
  dma_deinit(DMA0, DMA_CH4);
  dma_struct_para_init(&dma_init_struct);
  dma_init_struct.direction=DMA_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_addr=0U;
  dma_init_struct.memory_inc=DMA_MEMORY_INCREASE_ENABLE;
  dma_init_struct.memory_width=DMA_MEMORY_WIDTH_8BIT;
  dma_init_struct.number=0U;
  dma_init_struct.periph_addr=0U;
  dma_init_struct.periph_inc=DMA_PERIPH_INCREASE_DISABLE;
  dma_init_struct.memory_width=DMA_PERIPHERAL_WIDTH_8BIT;
  dma_init_struct.priority=DMA_PRIORITY_LOW;
  dma_init(DMA0,DMA_CH4,&dma_init_struct);


  

  usart_deinit(USART0);
  usart_baudrate_set(USART0,115200U);
  usart_word_length_set(USART0,USART_WL_8BIT);
  usart_stop_bit_set(USART0,USART_STB_1BIT);
  usart_parity_config(USART0,USART_PM_NONE);
  usart_hardware_flow_rts_config(USART0,USART_RTS_DISABLE);
  usart_hardware_flow_cts_config(USART0,USART_CTS_DISABLE);
  usart_receive_config(USART0,USART_RECEIVE_ENABLE);
  usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);
  
  eclic_set_irq_lvl_abs(USART0_IRQn,7U);
  eclic_set_irq_priority(USART0_IRQn,0U);
  eclic_set_nonvmode(USART0_IRQn);
  eclic_set_level_trig(USART0_IRQn);
  eclic_enable_interrupt(USART0_IRQn);




  ip_usart0=USART_Handle_MemoryMalloc.Malloc(sizeof(IP_USART));
  ip_usart0_txdma=USART_Handle_MemoryMalloc.Malloc(sizeof(IP_DMA));
  ip_usart0_rxdma=USART_Handle_MemoryMalloc.Malloc(sizeof(IP_DMA));

  if(cH_IsNull(ip_usart0)||cH_IsNull(ip_usart0_txdma)||cH_IsNull(ip_usart0_rxdma)){
    for(;;){
    }
  }

	extern void USART0_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);
  USART0_SetIrqCallback((void(*)(void*))IP_USART_IRQ,ip_usart0);

  IP_DMA_Init(ip_usart0_txdma,DMA0_Channel3);
  IP_DMA_Init(ip_usart0_rxdma,DMA0_Channel4);
  IP_USART_Init(ip_usart0,ip_usart0_txdma,ip_usart0_rxdma,(USART_TypeDef*)USART0);

  usart0_Handle=USART_CreateStream(ip_usart0,2048,2048,(H_MemoryMalloc*)&USART_Handle_MemoryMalloc);
  if(cH_IsNull(usart0_Handle)){
    for(;;){
    }
  }
}

void USART0_DeInit(){


  eclic_disable_interrupt(USART0_IRQn);

  usart_deinit(USART0);

  rcu_periph_clock_disable(RCU_USART0);

  dma_deinit(DMA0,DMA_CH3);
  dma_deinit(DMA0,DMA_CH4);
  
  oIP_GPIO_Config(GPIOA,10,vIP_GPIO_Mode_AN);
  oIP_GPIO_Config(GPIOA,9,vIP_GPIO_Mode_AN);


}




