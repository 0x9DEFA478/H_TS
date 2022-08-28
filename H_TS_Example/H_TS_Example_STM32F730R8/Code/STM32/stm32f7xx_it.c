#include "main.h"
#include "stm32f7xx_it.h"
#include "Peripheral.h"


static volatile IP_IRQ_Call_Def SysTick_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};

static volatile IP_IRQ_Call_Def DMA1_Stream0_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA1_Stream1_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA1_Stream2_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA1_Stream3_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA1_Stream4_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA1_Stream5_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA1_Stream6_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA1_Stream7_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};

static volatile IP_IRQ_Call_Def DMA2_Stream0_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA2_Stream1_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA2_Stream2_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA2_Stream3_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA2_Stream4_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA2_Stream5_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA2_Stream6_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA2_Stream7_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};


/**
 * @brief 设置SysTick中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void SysTick_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  SysTick_IRQ_Call.v=v;
  SysTick_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}



/**
 * @brief 设置DMA1数据流0中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream0_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream0_IRQ_Call.v=v;
  DMA1_Stream0_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置DMA1数据流1中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream1_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream1_IRQ_Call.v=v;
  DMA1_Stream1_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置DMA1数据流2中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream2_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream2_IRQ_Call.v=v;
  DMA1_Stream2_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置DMA1数据流3中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream3_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream3_IRQ_Call.v=v;
  DMA1_Stream3_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置DMA1数据流4中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream4_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream4_IRQ_Call.v=v;
  DMA1_Stream4_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置DMA1数据流5中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream5_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream5_IRQ_Call.v=v;
  DMA1_Stream5_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置DMA1数据流6中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream6_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream6_IRQ_Call.v=v;
  DMA1_Stream6_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置DMA1数据流7中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream7_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream7_IRQ_Call.v=v;
  DMA1_Stream7_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}



/**
 * @brief 设置DMA2数据流0中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream0_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream0_IRQ_Call.v=v;
  DMA2_Stream0_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置DMA2数据流1中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream1_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream1_IRQ_Call.v=v;
  DMA2_Stream1_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置DMA2数据流2中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream2_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream2_IRQ_Call.v=v;
  DMA2_Stream2_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置DMA2数据流3中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream3_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream3_IRQ_Call.v=v;
  DMA2_Stream3_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置DMA2数据流4中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream4_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream4_IRQ_Call.v=v;
  DMA2_Stream4_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置DMA2数据流5中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream5_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream5_IRQ_Call.v=v;
  DMA2_Stream5_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置DMA2数据流6中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream6_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream6_IRQ_Call.v=v;
  DMA2_Stream6_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置DMA2数据流7中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream7_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream7_IRQ_Call.v=v;
  DMA2_Stream7_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}






void DMA1_Stream0_IRQHandler(void){
  DMA1_Stream0_IRQ_Call.IP_IRQ_Callback(DMA1_Stream0_IRQ_Call.v);
}
void DMA1_Stream1_IRQHandler(void){
  DMA1_Stream1_IRQ_Call.IP_IRQ_Callback(DMA1_Stream1_IRQ_Call.v);
}
void DMA1_Stream2_IRQHandler(void){
  DMA1_Stream2_IRQ_Call.IP_IRQ_Callback(DMA1_Stream2_IRQ_Call.v);
}
void DMA1_Stream3_IRQHandler(void){
  DMA1_Stream3_IRQ_Call.IP_IRQ_Callback(DMA1_Stream3_IRQ_Call.v);
}
void DMA1_Stream4_IRQHandler(void){
  DMA1_Stream4_IRQ_Call.IP_IRQ_Callback(DMA1_Stream4_IRQ_Call.v);
}
void DMA1_Stream5_IRQHandler(void){
  DMA1_Stream5_IRQ_Call.IP_IRQ_Callback(DMA1_Stream5_IRQ_Call.v);
}
void DMA1_Stream6_IRQHandler(void){
  DMA1_Stream6_IRQ_Call.IP_IRQ_Callback(DMA1_Stream6_IRQ_Call.v);
}
void DMA1_Stream7_IRQHandler(void){
  DMA1_Stream7_IRQ_Call.IP_IRQ_Callback(DMA1_Stream7_IRQ_Call.v);
}


void DMA2_Stream0_IRQHandler(void){
  DMA2_Stream0_IRQ_Call.IP_IRQ_Callback(DMA2_Stream0_IRQ_Call.v);
}
void DMA2_Stream1_IRQHandler(void){
  DMA2_Stream1_IRQ_Call.IP_IRQ_Callback(DMA2_Stream1_IRQ_Call.v);
}
void DMA2_Stream2_IRQHandler(void){
  DMA2_Stream2_IRQ_Call.IP_IRQ_Callback(DMA2_Stream2_IRQ_Call.v);
}
void DMA2_Stream3_IRQHandler(void){
  DMA2_Stream3_IRQ_Call.IP_IRQ_Callback(DMA2_Stream3_IRQ_Call.v);
}
void DMA2_Stream4_IRQHandler(void){
  DMA2_Stream4_IRQ_Call.IP_IRQ_Callback(DMA2_Stream4_IRQ_Call.v);
}
void DMA2_Stream5_IRQHandler(void){
  DMA2_Stream5_IRQ_Call.IP_IRQ_Callback(DMA2_Stream5_IRQ_Call.v);
}
void DMA2_Stream6_IRQHandler(void){
  DMA2_Stream6_IRQ_Call.IP_IRQ_Callback(DMA2_Stream6_IRQ_Call.v);
}
void DMA2_Stream7_IRQHandler(void){
  DMA2_Stream7_IRQ_Call.IP_IRQ_Callback(DMA2_Stream7_IRQ_Call.v);
}




void NMI_Handler(void){
}
void HardFault_Handler(void)
{
  while(1){}
}
void MemManage_Handler(void)
{
  while(1){}
}
void BusFault_Handler(void)
{
  while(1){}
}
void UsageFault_Handler(void)
{
  while(1){}
}
void SVC_Handler(void){
}
void DebugMon_Handler(void){
}
//void PendSV_Handler(void){
//}
void SysTick_Handler(void){
  TimeTick++;
  SysTick_IRQ_Call.IP_IRQ_Callback(SysTick_IRQ_Call.v);
}







