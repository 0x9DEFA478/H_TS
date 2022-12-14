/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-28 16:56:58
 * @LastEditTime: 2021-07-28 17:47:15
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




int LED_Thread(void* v){
  
  RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;
  oIP_GPIO_Config(GPIOC,6,vIP_GPIO_Mode_OUT,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_LOW,vIP_GPIO_PuPd_NP);

  for(;;){

    H_TS_ThreadSleep(100);

    oIP_GPIO_OutputBitReverse(GPIOC,6);
  }
}



int Thread_Start(void* v){

  //???????????????
  Peripherals_Init();


  H_TS_StartNewThread(LED_Thread,NULL,256,0,0);

  //????????????
  return 0;
}


static void IdleProcess(void* v){
  
	__WFI();
}

/**
 * @brief ???????????????
 * @return ???
 */
void Entry(){

  //??????????????????????????????????????????????????????????????????
  //??????H_TS??????????????????????????????(???????????????H_TS_Init()) ???????????????????????? ???????????????????????????
  //??????????????????H_TS_StartScheduler()????????? ??????????????????
  //???????????????????????????????????????H_TS????????? ?????????????????????????????????????????????

  //?????????????????????H_TS????????????????????????
  H_TS_SetIdleProcess(IdleProcess);

  //???????????????
  Peripherals_SysInit();

  //??????????????????
  H_TS_StartNewThread(Thread_Start,NULL,512,0,0);

}









