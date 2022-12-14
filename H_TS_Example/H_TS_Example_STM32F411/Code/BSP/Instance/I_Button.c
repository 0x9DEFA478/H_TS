/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-30 18:13:02
 * @LastEditTime: 2021-12-05 20:52:56
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
  if(oIP_GPIO_GetInputBit(GPIOB,9)){
    return 0;
  }
  return -1;
}

static int isPress_B2(Huint32 KeyID){
  if(oIP_GPIO_GetInputBit(GPIOB,6)){
    return 0;
  }
  return -1;
}

static int isPress_B3(Huint32 KeyID){
  if(oIP_GPIO_GetInputBit(GPIOA,8)){
    return 0;
  }
  return -1;
}

static int isPress_B4(Huint32 KeyID){
  if(oIP_GPIO_GetInputBit(GPIOA,0)){
    return 0;
  }
  return -1;
}



/**
 * @brief ???????????????
 * @param EventCallback ??????????????????
 * @param v_EventCallback EventCallback??????????????????
 * @return ???
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
 * @brief ??????????????????????????????
 * @param KeyID ??????
 * @param LongPressTick ??????Tick??? 0???????????????????????????
 * @return 0:?????? -1:????????????????????????
 */
int I_Button_SetLongPressTick(Huint32 KeyID,int LongPressTick){

  return H_Key_SetLongPressTick(&b_root,KeyID,LongPressTick);
}

/**
 * @brief ????????????
 * @return ???
 */
void I_Button_Loop(){

  H_Key_Loop(&b_root);

}
