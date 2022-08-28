/*
 * @Author: 0x9DEFA478
 * @Date: 2021-04-29 14:38:31
 * @LastEditTime: 2021-09-29 13:54:59
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


#include "stm32f1xx_hal.h"
#include "Peripheral.h"


volatile Hsize TimeTick;
volatile Hint32 SysTick_Freq;


extern void SysTick_SetIrqCallback(void (*IP_IRQ_Callback)(void*),void* v);


/**
 * @brief 初始化SysTick
 * @param _SysTick_Freq SysTick自增频率
 * @param IP_IRQ_Callback SysTick中断调用回调
 * @param v 中断回调传入参数
 * @return 无
 */
void TimeTick_Init(Hint32 _SysTick_Freq,void (*IP_IRQ_Callback)(void*),void* v){

  SysTick_Freq=_SysTick_Freq;

  SysTick->CTRL&=(~SysTick_CTRL_ENABLE_Msk);

  SysTick_SetIrqCallback(IP_IRQ_Callback,v);

  HAL_NVIC_SetPriority(SysTick_IRQn, 14U, 0U);
  
  SysTick->LOAD=(Huint32)((_SysTick_Freq/1000)-1UL);
  SysTick->VAL=0U;
  SysTick->CTRL=SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk;  
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
}

/**
 * @brief 设置SysTick中断回调
 * @param IP_IRQ_Callback SysTick中断调用回调
 * @param v 中断回调传入参数
 * @return 无
 */
void TimeTick_Set_IRQ_Callback(void (*IP_IRQ_Callback)(void*),void* v){
  SysTick->CTRL&=(~SysTick_CTRL_ENABLE_Msk);
  SysTick_SetIrqCallback(IP_IRQ_Callback,v);
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
}

/**
 * @brief 毫秒延时
 * @param timeMs 毫秒为单位的时间
 * @return 无
 */
void Delay_ms(int timeMs){
  int tickstart;
  
  tickstart = Time_GetTimeMs();

  while((Time_GetTimeMs()-tickstart) < timeMs){}
}

/**
 * @brief 微秒延时
 * @param timeUs 微秒为单位的时间
 * @return 无
 */
void Delay_us(unsigned int timeUs){

  unsigned int t,nowT;
  unsigned int lastT;
  unsigned int cenT;
  unsigned int SysReLoad;

  SysReLoad=SysTick->LOAD;
  lastT=SysTick->VAL;
  t=timeUs*(SysTick_Freq/1000000U);
  cenT=0;
  for(;;){
    nowT=SysTick->VAL;
    if(nowT!=lastT){
      if(nowT<lastT){
        cenT+=lastT-nowT;
      }else{
        cenT+=SysReLoad-nowT+lastT;
      }
      if(cenT>=t){
        break;
      }
      lastT=nowT;
    }
  }
}





HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  return HAL_OK;
}

void HAL_IncTick(void){
}

uint32_t HAL_GetTick(void){
  return TimeTick;
}

void HAL_SuspendTick(void){
}

void HAL_ResumeTick(void){
}

void HAL_Delay(uint32_t Delay){
  Delay_ms(Delay+1);
}


















