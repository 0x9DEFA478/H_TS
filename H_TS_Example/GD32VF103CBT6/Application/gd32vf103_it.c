/*
 * @Author: 0x9DEFA478
 * @Date: 2022-01-11 18:13:36
 * @LastEditTime: 2022-01-13 20:16:48
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
#include "Peripheral.h"


static volatile IP_IRQ_Call_Def SysTick_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};

static volatile IP_IRQ_Call_Def DMA0_Channel0_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA0_Channel1_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA0_Channel2_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA0_Channel3_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA0_Channel4_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA0_Channel5_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA0_Channel6_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA0_Channel7_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};

static volatile IP_IRQ_Call_Def DMA1_Channel0_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA1_Channel1_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA1_Channel2_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA1_Channel3_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA1_Channel4_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA1_Channel5_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA1_Channel6_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};
static volatile IP_IRQ_Call_Def DMA1_Channel7_IRQ_Call={.IP_IRQ_Callback=NULL,.v=NULL};




void SysTick_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  SysTick_IRQ_Call.v=v;
  SysTick_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}



void DMA0_Channel0_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA0_Channel0_IRQ_Call.v=v;
  DMA0_Channel0_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void DMA0_Channel1_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA0_Channel1_IRQ_Call.v=v;
  DMA0_Channel1_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void DMA0_Channel2_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA0_Channel2_IRQ_Call.v=v;
  DMA0_Channel2_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void DMA0_Channel3_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA0_Channel3_IRQ_Call.v=v;
  DMA0_Channel3_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void DMA0_Channel4_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA0_Channel4_IRQ_Call.v=v;
  DMA0_Channel4_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void DMA0_Channel5_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA0_Channel5_IRQ_Call.v=v;
  DMA0_Channel5_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void DMA0_Channel6_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA0_Channel6_IRQ_Call.v=v;
  DMA0_Channel6_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void DMA0_Channel7_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA0_Channel7_IRQ_Call.v=v;
  DMA0_Channel7_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}



void DMA1_Channel0_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Channel0_IRQ_Call.v=v;
  DMA1_Channel0_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void DMA1_Channel1_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Channel1_IRQ_Call.v=v;
  DMA1_Channel1_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void DMA1_Channel2_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Channel2_IRQ_Call.v=v;
  DMA1_Channel2_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void DMA1_Channel3_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Channel3_IRQ_Call.v=v;
  DMA1_Channel3_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void DMA1_Channel4_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Channel4_IRQ_Call.v=v;
  DMA1_Channel4_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void DMA1_Channel5_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Channel5_IRQ_Call.v=v;
  DMA1_Channel5_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void DMA1_Channel6_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Channel6_IRQ_Call.v=v;
  DMA1_Channel6_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}

void DMA1_Channel7_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v){
  DMA1_Channel7_IRQ_Call.v=v;
  DMA1_Channel7_IRQ_Call.IP_IRQ_Callback=IP_IRQ_Callback;
}






















void DMA0_Channel0_IRQHandler(void){
  DMA0_Channel0_IRQ_Call.IP_IRQ_Callback(DMA0_Channel0_IRQ_Call.v);
}
void DMA0_Channel1_IRQHandler(void){
  DMA0_Channel1_IRQ_Call.IP_IRQ_Callback(DMA0_Channel1_IRQ_Call.v);
}
void DMA0_Channel2_IRQHandler(void){
  DMA0_Channel2_IRQ_Call.IP_IRQ_Callback(DMA0_Channel2_IRQ_Call.v);
}
void DMA0_Channel3_IRQHandler(void){
  DMA0_Channel3_IRQ_Call.IP_IRQ_Callback(DMA0_Channel3_IRQ_Call.v);
}
void DMA0_Channel4_IRQHandler(void){
  DMA0_Channel4_IRQ_Call.IP_IRQ_Callback(DMA0_Channel4_IRQ_Call.v);
}
void DMA0_Channel5_IRQHandler(void){
  DMA0_Channel5_IRQ_Call.IP_IRQ_Callback(DMA0_Channel5_IRQ_Call.v);
}
void DMA0_Channel6_IRQHandler(void){
  DMA0_Channel6_IRQ_Call.IP_IRQ_Callback(DMA0_Channel6_IRQ_Call.v);
}
void DMA0_Channel7_IRQHandler(void){
  DMA0_Channel7_IRQ_Call.IP_IRQ_Callback(DMA0_Channel7_IRQ_Call.v);
}



void DMA1_Channel0_IRQHandler(void){
  DMA1_Channel0_IRQ_Call.IP_IRQ_Callback(DMA1_Channel0_IRQ_Call.v);
}
void DMA1_Channel1_IRQHandler(void){
  DMA1_Channel1_IRQ_Call.IP_IRQ_Callback(DMA1_Channel1_IRQ_Call.v);
}
void DMA1_Channel2_IRQHandler(void){
  DMA1_Channel2_IRQ_Call.IP_IRQ_Callback(DMA1_Channel2_IRQ_Call.v);
}
void DMA1_Channel3_IRQHandler(void){
  DMA1_Channel3_IRQ_Call.IP_IRQ_Callback(DMA1_Channel3_IRQ_Call.v);
}
void DMA1_Channel4_IRQHandler(void){
  DMA1_Channel4_IRQ_Call.IP_IRQ_Callback(DMA1_Channel4_IRQ_Call.v);
}
void DMA1_Channel5_IRQHandler(void){
  DMA1_Channel5_IRQ_Call.IP_IRQ_Callback(DMA1_Channel5_IRQ_Call.v);
}
void DMA1_Channel6_IRQHandler(void){
  DMA1_Channel6_IRQ_Call.IP_IRQ_Callback(DMA1_Channel6_IRQ_Call.v);
}
void DMA1_Channel7_IRQHandler(void){
  DMA1_Channel7_IRQ_Call.IP_IRQ_Callback(DMA1_Channel7_IRQ_Call.v);
}












uintptr_t handle_trap(uintptr_t mcause, uintptr_t sp){
  Huint32 Exc_Code;

  Exc_Code=mcause&0x00000FFFU;

  switch(Exc_Code){
    case 0x00BU://EnvCall M-mode
      //调用传入的回调
      ((void**)sp)[5]=((void* (*)(void*))((void**)sp)[5])((void*)((void**)sp)[6]);
      ((Huint32*)sp)[18]=((Huint32*)sp)[18]+4U;//PC(mepc)设置为ecall的下一条指令(不然ecall会被再次执行)
      return 0U;
    case 0x009U://EnvCall S-mode
      //暂时不支持 无限循环去
      break;
    case 0x008U://EnvCall U-mode
      //暂时不支持 无限循环去
      break;
    default:
      break;
  }

  for(;;){
  }
}

void SysTick_Handler(void){
  TimeTick++;
  SysTick_IRQ_Call.IP_IRQ_Callback(SysTick_IRQ_Call.v);
}

void eclic_mtip_handler(){
  unsigned long long mtimecmp;

  ((unsigned int*)&mtimecmp)[0]=((volatile unsigned int*)(TIMER_CTRL_ADDR+TIMER_MTIMECMP))[0];
  ((unsigned int*)&mtimecmp)[1]=((volatile unsigned int*)(TIMER_CTRL_ADDR+TIMER_MTIMECMP))[1];

extern volatile Huint32 SysTick_Freq;
  mtimecmp+=SysTick_Freq/1000;

  ((volatile unsigned int*)(TIMER_CTRL_ADDR+TIMER_MTIMECMP))[0]=((unsigned int*)&mtimecmp)[0];
  ((volatile unsigned int*)(TIMER_CTRL_ADDR+TIMER_MTIMECMP))[1]=((unsigned int*)&mtimecmp)[1];

  SysTick_Handler();
}



