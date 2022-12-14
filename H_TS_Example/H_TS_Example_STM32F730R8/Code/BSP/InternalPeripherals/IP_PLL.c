/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-26 19:00:42
 * @LastEditTime: 2021-07-26 22:00:52
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
#include "stm32f7xx.h"



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
 * @brief ???????????????PLLSAI
 * @param Status 0:??????PLL ??????:??????PLL
 * @return ???
 */
void PLLSAI_SetStatus(unsigned int Status){


  if(Status){

    RCC->CR|=RCC_CR_PLLSAION;//??????pllsai
    while(!(RCC->CR&RCC_CR_PLLSAIRDY)){
    }
  }
  else{
    RCC->CR&=~RCC_CR_PLLSAION;//??????pllsai
    while(RCC->CR&RCC_CR_PLLSAIRDY){
    }
  }
}




/**
 * @brief ??????PLL ?????????PLL?????????????????????????????????
 *        ????????????PLL?????????????????? ???????????????????????????
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

  RCC->PLLCFGR=(RCC->PLLCFGR&(~(RCC_PLLCFGR_PLLM|RCC_PLLCFGR_PLLN|RCC_PLLCFGR_PLLP|RCC_PLLCFGR_PLLQ)))
  |(
    ((pllm<<RCC_PLLCFGR_PLLM_Pos)&RCC_PLLCFGR_PLLM_Msk)|
    ((plln<<RCC_PLLCFGR_PLLN_Pos)&RCC_PLLCFGR_PLLN_Msk)|
    ((pllp<<RCC_PLLCFGR_PLLP_Pos)&RCC_PLLCFGR_PLLP_Msk)|
    ((pllq<<RCC_PLLCFGR_PLLQ_Pos)&RCC_PLLCFGR_PLLQ_Msk)
  );

}

/**
 * @brief ??????PLLI2S ?????????PLLI2S?????????????????????????????????
 *        ????????????PLL?????????????????? ???????????????????????????
 * @param plln ????????????n 50~432
 * @param pllp ??????p??????????????? ??????
 * @param pllq ??????q??????????????? 2~15
 * @param pllr ??????r??????????????? 2~7
 * @param divq ??????q?????????????????? 1~32
 * @return ???
 */
void PLLI2S_SetDIV(unsigned int plln,unsigned int pllp,unsigned int pllq,unsigned int pllr,unsigned int divq){

  if(RCC->CR&RCC_CR_PLLI2SON){
    return;
  }

  RCC->PLLI2SCFGR=(RCC->PLLI2SCFGR&(~(RCC_PLLI2SCFGR_PLLI2SN|RCC_PLLI2SCFGR_PLLI2SQ|RCC_PLLI2SCFGR_PLLI2SR)))
  |(
    ((plln<<RCC_PLLI2SCFGR_PLLI2SN_Pos)&RCC_PLLI2SCFGR_PLLI2SN_Msk)|
    ((pllq<<RCC_PLLI2SCFGR_PLLI2SQ_Pos)&RCC_PLLI2SCFGR_PLLI2SQ_Msk)|
    ((pllr<<RCC_PLLI2SCFGR_PLLI2SR_Pos)&RCC_PLLI2SCFGR_PLLI2SR_Msk)
  );

  RCC->DCKCFGR1|=(RCC->DCKCFGR1&(~RCC_DCKCFGR1_PLLI2SDIVQ_Msk))|(((divq-1U)<<RCC_DCKCFGR1_PLLI2SDIVQ_Pos)&(RCC_DCKCFGR1_PLLI2SDIVQ_Msk));

}

/**
 * @brief ??????PLLSAI ?????????PLLSAI?????????????????????????????????
 *        ????????????PLL?????????????????? ???????????????????????????
 * @param plln ????????????n
 * @param pllp ??????p??????????????? ?????????2,4,6,8
 * @param pllq ??????q??????????????? 2~15
 * @param pllr ??????r??????????????? ??????
 * @param divq ??????q?????????????????? 1~32
 * @return ???
 */
void PLLSAI_SetDIV(unsigned int plln,unsigned int pllp,unsigned int pllq,unsigned int pllr,unsigned int divq){

  if(RCC->CR&RCC_CR_PLLSAION){
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

  RCC->PLLSAICFGR=(RCC->PLLSAICFGR&(~(RCC_PLLSAICFGR_PLLSAIN|RCC_PLLSAICFGR_PLLSAIP|RCC_PLLSAICFGR_PLLSAIQ)))
  |(
    ((plln<<RCC_PLLSAICFGR_PLLSAIN_Pos)&RCC_PLLSAICFGR_PLLSAIN_Msk)|
    ((pllp<<RCC_PLLSAICFGR_PLLSAIP_Pos)&RCC_PLLSAICFGR_PLLSAIP_Msk)|
    ((pllq<<RCC_PLLSAICFGR_PLLSAIQ_Pos)&RCC_PLLSAICFGR_PLLSAIQ_Msk)
  );

  RCC->DCKCFGR1|=(RCC->DCKCFGR1&(~RCC_DCKCFGR1_PLLSAIDIVQ_Msk))|(((divq-1U)<<RCC_DCKCFGR1_PLLSAIDIVQ_Pos)&(RCC_DCKCFGR1_PLLSAIDIVQ_Msk));

}






