#include "main.h"
#include "stm32f4xx_it.h"
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
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void SysTick_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  SysTick_IRQ_Call.v=v;
  SysTick_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}



/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream0_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream0_IRQ_Call.v=v;
  DMA1_Stream0_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream1_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream1_IRQ_Call.v=v;
  DMA1_Stream1_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream2_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream2_IRQ_Call.v=v;
  DMA1_Stream2_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream3_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream3_IRQ_Call.v=v;
  DMA1_Stream3_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream4_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream4_IRQ_Call.v=v;
  DMA1_Stream4_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream5_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream5_IRQ_Call.v=v;
  DMA1_Stream5_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream6_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream6_IRQ_Call.v=v;
  DMA1_Stream6_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA1_Stream7_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Stream7_IRQ_Call.v=v;
  DMA1_Stream7_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}



/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream0_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream0_IRQ_Call.v=v;
  DMA2_Stream0_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream1_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream1_IRQ_Call.v=v;
  DMA2_Stream1_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream2_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream2_IRQ_Call.v=v;
  DMA2_Stream2_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream3_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream3_IRQ_Call.v=v;
  DMA2_Stream3_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream4_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream4_IRQ_Call.v=v;
  DMA2_Stream4_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream5_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream5_IRQ_Call.v=v;
  DMA2_Stream5_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置中断调用函数
 * @param IP_IRQ_Callback 中断调用
 * @param v 中断调用传入的参数
 * @return 无
 */
void DMA2_Stream6_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA2_Stream6_IRQ_Call.v=v;
  DMA2_Stream6_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

/**
 * @brief 设置中断调用函数
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












typedef union{

  struct{
    volatile int IsFP;
    volatile unsigned int R[13];
    volatile unsigned int LR;
    volatile unsigned int PC;
    volatile unsigned int xPSR;
  }NoFP;
  struct{
    volatile int IsFP;
    volatile unsigned int R[13];
    volatile unsigned int LR;
    volatile unsigned int PC;
    volatile unsigned int xPSR;

    volatile unsigned int S[32];
    volatile unsigned int FPSCR;
  }FP;

}CPU_Stack;

static void GetCPU_Stack(CPU_Stack* cpu_stack,unsigned int* Stack,int IsFP){
  int StackIndex;
  
  StackIndex=0;

  if(IsFP==0){

    cpu_stack->NoFP.IsFP=0;

    cpu_stack->NoFP.R[4]=Stack[StackIndex];StackIndex++;
    cpu_stack->NoFP.R[5]=Stack[StackIndex];StackIndex++;
    cpu_stack->NoFP.R[6]=Stack[StackIndex];StackIndex++;
    cpu_stack->NoFP.R[7]=Stack[StackIndex];StackIndex++;
    cpu_stack->NoFP.R[8]=Stack[StackIndex];StackIndex++;
    cpu_stack->NoFP.R[9]=Stack[StackIndex];StackIndex++;
    cpu_stack->NoFP.R[10]=Stack[StackIndex];StackIndex++;
    cpu_stack->NoFP.R[11]=Stack[StackIndex];StackIndex++;

    cpu_stack->NoFP.R[0]=Stack[StackIndex];StackIndex++;
    cpu_stack->NoFP.R[1]=Stack[StackIndex];StackIndex++;
    cpu_stack->NoFP.R[2]=Stack[StackIndex];StackIndex++;
    cpu_stack->NoFP.R[3]=Stack[StackIndex];StackIndex++;
    cpu_stack->NoFP.R[12]=Stack[StackIndex];StackIndex++;
    cpu_stack->NoFP.LR=Stack[StackIndex];StackIndex++;
    cpu_stack->NoFP.PC=Stack[StackIndex];StackIndex++;
    cpu_stack->NoFP.xPSR=Stack[StackIndex];StackIndex++;

  }else{

    cpu_stack->NoFP.IsFP=-1;

    cpu_stack->FP.R[4]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.R[5]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.R[6]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.R[7]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.R[8]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.R[9]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.R[10]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.R[11]=Stack[StackIndex];StackIndex++;

    cpu_stack->FP.S[16]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[17]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[18]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[19]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[20]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[21]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[22]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[23]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[24]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[25]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[26]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[27]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[28]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[29]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[30]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[31]=Stack[StackIndex];StackIndex++;

    cpu_stack->FP.R[0]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.R[1]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.R[2]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.R[3]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.R[12]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.LR=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.PC=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.xPSR=Stack[StackIndex];StackIndex++;

    cpu_stack->FP.S[0]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[1]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[2]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[3]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[4]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[5]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[6]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[7]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[8]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[9]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[10]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[11]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[12]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[13]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[14]=Stack[StackIndex];StackIndex++;
    cpu_stack->FP.S[15]=Stack[StackIndex];StackIndex++;

    cpu_stack->FP.FPSCR=Stack[StackIndex];StackIndex++;

  }

  (void)StackIndex;
  
}

CPU_Stack Fault_CPU_Stack;

void NMI_Handler(void){
}

// void HardFault_Handler(){
//   for(;;){
//   }
// }
void HardFault_IRQ(unsigned int* Stack,int IsFP){
  
  GetCPU_Stack(&Fault_CPU_Stack,Stack,IsFP);

  for(;;){
  }
}

// void MemManage_Handler(){
//   for(;;){
//   }
// }
void MemManage_IRQ(unsigned int* Stack,int IsFP){
  
  GetCPU_Stack(&Fault_CPU_Stack,Stack,IsFP);

  for(;;){
  }
}

// void BusFault_Handler(){
//   for(;;){
//   }
// }
void BusFault_IRQ(unsigned int* Stack,int IsFP){
  
  GetCPU_Stack(&Fault_CPU_Stack,Stack,IsFP);

  for(;;){
  }
}

// void UsageFault_Handler(){
//   for(;;){
//   }
// }
void UsageFault_IRQ(unsigned int* Stack,int IsFP){
  
  GetCPU_Stack(&Fault_CPU_Stack,Stack,IsFP);

  for(;;){
  }
}

// void SVC_Handler(void){
// }

void DebugMon_Handler(void){
}

// void PendSV_Handler(void){
// }

void SysTick_Handler(void){
  TimeTick++;
  SysTick_IRQ_Call.IP_IRQ_Callback(SysTick_IRQ_Call.v);
}



