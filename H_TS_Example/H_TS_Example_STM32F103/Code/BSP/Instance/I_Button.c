/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-30 18:13:02
 * @LastEditTime: 2021-09-29 15:22:02
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
#include "I_Button.h"
#include "IP_GPIO.h"

static H_Key_Root_Def b_root;
static H_Key_Def b1;
static H_Key_Def b2;
static H_Key_Def b3;
static H_Key_Def b4;


static int isPress_B1(Huint32 KeyID){
  if(oIP_GPIO_GetInputBit(GPIOB,3)){
    return 0;
  }
  return -1;
}

static int isPress_B2(Huint32 KeyID){
  if(oIP_GPIO_GetInputBit(GPIOB,4)){
    return 0;
  }
  return -1;
}

static int isPress_B3(Huint32 KeyID){
  if(oIP_GPIO_GetInputBit(GPIOB,5)){
    return 0;
  }
  return -1;
}

static int isPress_B4(Huint32 KeyID){
  if(oIP_GPIO_GetInputBit(GPIOB,6)){
    return 0;
  }
  return -1;
}



/**
 * @brief 按键初始化
 * @param EventCallback 按键事件回调
 * @param v_EventCallback EventCallback回调传入参数
 * @return 无
 */
void I_Button_Init(void (*EventCallback)(void*,Huint32),void* v_EventCallback){


  H_Key_Root_Init(&b_root,1,65535,EventCallback,v_EventCallback);
  H_Key_Init(&b1,vButton_B1,240,isPress_B1);
  H_Key_Init(&b2,vButton_B2,240,isPress_B2);
  H_Key_Init(&b3,vButton_B3,240,isPress_B3);
  H_Key_Init(&b4,vButton_B4,240,isPress_B4);
  H_Key_Add(&b_root,&b1);
  H_Key_Add(&b_root,&b2);
  H_Key_Add(&b_root,&b3);
  H_Key_Add(&b_root,&b4);
}

/**
 * @brief 设置长按事件触发时长
 * @param KeyID 键值
 * @param LongPressTick 触发Tick数 0表示不使用长按事件
 * @return 0:成功 -1:未找到对应的按键
 */
int I_Button_SetLongPressTick(Huint32 KeyID,int LongPressTick){

  return H_Key_SetLongPressTick(&b_root,KeyID,LongPressTick);
}

/**
 * @brief 扫描循环
 * @return 无
 */
void I_Button_Loop(){

  H_Key_Loop(&b_root);

}
