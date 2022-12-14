/*
 * @Author: 0x9DEFA478
 * @Date: 2021-04-29 14:38:31
 * @LastEditTime: 2022-01-13 20:05:05
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
#include "gd32vf103.h"
#include "Peripheral.h"


volatile Hsize TimeTick;
volatile Hint32 SysTick_Freq;


extern void SysTick_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);


/**
 * @brief ?????????SysTick
 * @param _SysTick_Freq SysTick????????????
 * @param IP_IRQ_Callback SysTick??????????????????
 * @param v ????????????????????????
 * @return ???
 */
void TimeTick_Init(Hint32 _SysTick_Freq,void (*IP_IRQ_Callback)(void*),void* v){

  SysTick_Freq=_SysTick_Freq;

  eclic_disable_interrupt(CLIC_INT_TMR);
  asm volatile("fence":::"memory");

  SysTick_SetIrqCallback(IP_IRQ_Callback,v);

  eclic_set_irq_lvl_abs(CLIC_INT_TMR,1U);
  eclic_set_irq_priority(CLIC_INT_TMR,0U);
  eclic_set_nonvmode(CLIC_INT_TMR);
  eclic_set_level_trig(CLIC_INT_TMR);
  
  //???????????????
  ((volatile unsigned int*)(TIMER_CTRL_ADDR+TIMER_MTIME))[0]=0;//mtime_lo
  ((volatile unsigned int*)(TIMER_CTRL_ADDR+TIMER_MTIME))[1]=0;//mtime_hi
  //???????????????
  ((volatile unsigned int*)(TIMER_CTRL_ADDR+TIMER_MTIMECMP))[0]=_SysTick_Freq/1000;//mtimecmp_lo
  ((volatile unsigned int*)(TIMER_CTRL_ADDR+TIMER_MTIMECMP))[1]=0;//mtimecmp_hi

  asm volatile("fence":::"memory");
  eclic_enable_interrupt(CLIC_INT_TMR);
}

/**
 * @brief ??????SysTick????????????
 * @param IP_IRQ_Callback SysTick??????????????????
 * @param v ????????????????????????
 * @return ???
 */
void TimeTick_Set_IRQ_Callback(void (*IP_IRQ_Callback)(void*),void* v){
  eclic_disable_interrupt(CLIC_INT_TMR);
  asm volatile("fence":::"memory");
  SysTick_SetIrqCallback(IP_IRQ_Callback,v);
  asm volatile("fence":::"memory");
  eclic_enable_interrupt(CLIC_INT_TMR);
}

/**
 * @brief ????????????
 * @param timeMs ????????????????????????
 * @return ???
 */
void Delay_ms(int timeMs){
  int tickstart;
  
  tickstart = Time_GetTimeMs();

  while((Time_GetTimeMs()-tickstart) < timeMs){}
}

/**
 * @brief ????????????
 * @param timeUs ????????????????????????
 * @return ???
 */
void Delay_us(unsigned int timeUs){

  unsigned int t,nowT;
  unsigned int lastT;
  unsigned int cenT;
  //unsigned int SysReLoad;
#define SysReLoad 0xFFFFFFFFU

  //SysReLoad=SysTick->LOAD;
  lastT=mtime_lo();
  t=timeUs*(SysTick_Freq/1000000U);
  cenT=0;
  for(;;){
    nowT=mtime_lo();
    if(nowT!=lastT){
      if(nowT<lastT){
        cenT+=SysReLoad-lastT+nowT;
      }else{
        cenT+=nowT-lastT;
      }
      if(cenT>=t){
        break;
      }
      lastT=nowT;
    }
  }
#undef SysReLoad
}



