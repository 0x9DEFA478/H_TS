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
 * @brief 使能或关闭PLLSAI
 * @param Status 0:关闭PLL 其他:打开PLL
 * @return 无
 */
void PLLSAI_SetStatus(unsigned int Status){


  if(Status){

    RCC->CR|=RCC_CR_PLLSAION;//使能pllsai
    while(!(RCC->CR&RCC_CR_PLLSAIRDY)){
    }
  }
  else{
    RCC->CR&=~RCC_CR_PLLSAION;//失能pllsai
    while(RCC->CR&RCC_CR_PLLSAIRDY){
    }
  }
}




/**
 * @brief 配置PLL 应该在PLL失能的情况下使用该方法
 *        需要提供PLL参考时钟频率 以进行频率范围配置
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

  RCC->PLLCFGR=(RCC->PLLCFGR&(~(RCC_PLLCFGR_PLLM|RCC_PLLCFGR_PLLN|RCC_PLLCFGR_PLLP|RCC_PLLCFGR_PLLQ)))
  |(
    ((pllm<<RCC_PLLCFGR_PLLM_Pos)&RCC_PLLCFGR_PLLM_Msk)|
    ((plln<<RCC_PLLCFGR_PLLN_Pos)&RCC_PLLCFGR_PLLN_Msk)|
    ((pllp<<RCC_PLLCFGR_PLLP_Pos)&RCC_PLLCFGR_PLLP_Msk)|
    ((pllq<<RCC_PLLCFGR_PLLQ_Pos)&RCC_PLLCFGR_PLLQ_Msk)
  );

}

/**
 * @brief 配置PLLI2S 应该在PLLI2S失能的情况下使用该方法
 *        需要提供PLL参考时钟频率 以进行频率范围配置
 * @param plln 倍频系数n 50~432
 * @param pllp 输出p的分频系数 无效
 * @param pllq 输出q的分频系数 2~15
 * @param pllr 输出r的分频系数 2~7
 * @param divq 输出q的再分频系数 1~32
 * @return 无
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
 * @brief 配置PLLSAI 应该在PLLSAI失能的情况下使用该方法
 *        需要提供PLL参考时钟频率 以进行频率范围配置
 * @param plln 倍频系数n
 * @param pllp 输出p的分频系数 只可取2,4,6,8
 * @param pllq 输出q的分频系数 2~15
 * @param pllr 输出r的分频系数 无效
 * @param divq 输出q的再分频系数 1~32
 * @return 无
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






