/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-26 19:00:42
 * @LastEditTime: 2021-09-29 13:06:36
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
#include "IP_PLL.h"
#include "stm32f4xx.h"



/**
 * @brief ???????????????PLL
 * @param Status 0:??????PLL ??????:??????PLL
 * @return ???
 */
void PLL_SetStatus(unsigned int Status){

  if(Status){
    RCC->CR|=RCC_CR_PLLON;//??????pll
    while(!(RCC->CR&RCC_CR_PLLRDY)){
    }
  }
  else{
    RCC->CR&=~RCC_CR_PLLON;//??????pll
    while(RCC->CR&RCC_CR_PLLRDY){
    }
  }
}

/**
 * @brief ???????????????PLLI2S
 * @param Status 0:??????PLL ??????:??????PLL
 * @return ???
 */
void PLLI2S_SetStatus(unsigned int Status){


  if(Status){
    RCC->CR|=RCC_CR_PLLI2SON;//??????plli2s
    while(!(RCC->CR&RCC_CR_PLLI2SRDY)){
    }
  }
  else{
    RCC->CR&=~RCC_CR_PLLI2SON;//??????plli2s
    while(RCC->CR&RCC_CR_PLLI2SRDY){
    }
  }
}




/**
 * @brief ??????PLL ?????????PLL?????????????????????????????????
 * @param pllm ?????????m 2~63
 * @param plln ????????????n 50~432
 * @param pllp ??????p??????????????? ?????????2,4,6,8
 * @param pllq ??????q??????????????? 2~15
 * @param pllr ??????r??????????????? ??????
 * @return ???
 */
void PLL_SetDIV(unsigned int pllm,unsigned int plln,unsigned int pllp,unsigned int pllq,unsigned int pllr){

  if(RCC->CR&RCC_CR_PLLON){
    return;
  }

  switch (pllp){
    case 2U:
      pllp=0x00U;
      break;
    case 4U:
      pllp=0x01U;
      break;
    case 6U:
      pllp=0x02U;
      break;
    case 8U:
      pllp=0x03U;
      break;
    
    default:
      return;
  }

  RCC->PLLCFGR=(RCC->PLLCFGR&(~(RCC_PLLCFGR_PLLM_Msk|RCC_PLLCFGR_PLLN_Msk|RCC_PLLCFGR_PLLP_Msk|RCC_PLLCFGR_PLLQ_Msk)))
  |(
    ((pllm<<RCC_PLLCFGR_PLLM_Pos)&RCC_PLLCFGR_PLLM_Msk)|
    ((plln<<RCC_PLLCFGR_PLLN_Pos)&RCC_PLLCFGR_PLLN_Msk)|
    ((pllp<<RCC_PLLCFGR_PLLP_Pos)&RCC_PLLCFGR_PLLP_Msk)|
    ((pllq<<RCC_PLLCFGR_PLLQ_Pos)&RCC_PLLCFGR_PLLQ_Msk)
  );

}

/**
 * @brief ??????PLLI2S ?????????PLLI2S?????????????????????????????????
 * @param pllm ?????????m 2~63
 * @param plln ????????????n 50~432
 * @param pllp ??????p??????????????? ??????
 * @param pllq ??????q??????????????? ??????
 * @param pllr ??????r??????????????? 2~7
 * @return ???
 */
void PLLI2S_SetDIV(unsigned int pllm,unsigned int plln,unsigned int pllp,unsigned int pllq,unsigned int pllr){

  if(RCC->CR&RCC_CR_PLLI2SON){
    return;
  }

  RCC->PLLI2SCFGR=(RCC->PLLI2SCFGR&(~(RCC_PLLI2SCFGR_PLLI2SM_Msk|RCC_PLLI2SCFGR_PLLI2SN_Msk|/*RCC_PLLI2SCFGR_PLLI2SQ|*/RCC_PLLI2SCFGR_PLLI2SR_Msk)))
  |(
    ((pllm<<RCC_PLLI2SCFGR_PLLI2SM_Pos)&RCC_PLLI2SCFGR_PLLI2SM_Msk)|
    ((plln<<RCC_PLLI2SCFGR_PLLI2SN_Pos)&RCC_PLLI2SCFGR_PLLI2SN_Msk)|
    //((pllq<<RCC_PLLI2SCFGR_PLLI2SQ_Pos)&RCC_PLLI2SCFGR_PLLI2SQ_Msk)|
    ((pllr<<RCC_PLLI2SCFGR_PLLI2SR_Pos)&RCC_PLLI2SCFGR_PLLI2SR_Msk)
  );

  
}



