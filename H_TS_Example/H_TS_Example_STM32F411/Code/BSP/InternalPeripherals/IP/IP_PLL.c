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
 * @brief 使能或关闭PLL
 * @param Status 0:关闭PLL 其他:打开PLL
 * @return 无
 */
void PLL_SetStatus(unsigned int Status){

  if(Status){
    RCC->CR|=RCC_CR_PLLON;//使能pll
    while(!(RCC->CR&RCC_CR_PLLRDY)){
    }
  }
  else{
    RCC->CR&=~RCC_CR_PLLON;//失能pll
    while(RCC->CR&RCC_CR_PLLRDY){
    }
  }
}

/**
 * @brief 使能或关闭PLLI2S
 * @param Status 0:关闭PLL 其他:打开PLL
 * @return 无
 */
void PLLI2S_SetStatus(unsigned int Status){


  if(Status){
    RCC->CR|=RCC_CR_PLLI2SON;//使能plli2s
    while(!(RCC->CR&RCC_CR_PLLI2SRDY)){
    }
  }
  else{
    RCC->CR&=~RCC_CR_PLLI2SON;//失能plli2s
    while(RCC->CR&RCC_CR_PLLI2SRDY){
    }
  }
}




/**
 * @brief 配置PLL 应该在PLL失能的情况下使用该方法
 * @param pllm 预分频m 2~63
 * @param plln 倍频系数n 50~432
 * @param pllp 输出p的分频系数 只可取2,4,6,8
 * @param pllq 输出q的分频系数 2~15
 * @param pllr 输出r的分频系数 无效
 * @return 无
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
 * @brief 配置PLLI2S 应该在PLLI2S失能的情况下使用该方法
 * @param pllm 预分频m 2~63
 * @param plln 倍频系数n 50~432
 * @param pllp 输出p的分频系数 无效
 * @param pllq 输出q的分频系数 无效
 * @param pllr 输出r的分频系数 2~7
 * @return 无
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



