/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-30 16:22:04
 * @LastEditTime: 2021-09-29 15:10:07
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
#include "IP_SPI.h"

extern int IP_SPI_isBusy_M_onlyTx(IP_SPI* ip_spi);
extern int IP_SPI_isBusy_M_TxRx(IP_SPI* ip_spi);
extern int IP_SPI_isBusy_M_onlyTx_dma(IP_SPI* ip_spi);
extern int IP_SPI_isBusy_M_TxRx_dma(IP_SPI* ip_spi);



/**
 * @brief ????????? IP_SPI
 * @param ip_spi ??????IP_SPI??????(??????????????????)
 * @param DMA_Tx ??????dma??????(?????????????????? ?????????????????????NULL)
 * @param DMA_Rx ??????dma??????(?????????????????? ?????????????????????NULL)
 * @param _hspi hspi??????
 * @param Feature ??????
 * @return 0:?????? ??????:??????????????????
 */
int IP_SPI_Init(IP_SPI* ip_spi,IP_DMA* DMA_Tx,IP_DMA* DMA_Rx,SPI_HandleTypeDef* _hspi,unsigned int Feature){

  if((Feature&vIP_SPI_Feature_isMaster)==0U){
    //????????????????????????
    return -1;
  }
  if((Feature&vIP_SPI_Feature_Tx)!=vIP_SPI_Feature_Tx){
    //??????????????????????????????
    return -2;
  }

  cIP_SPI_SPI(ip_spi)=_hspi->Instance;//????????????

  switch (Feature&(vIP_SPI_Feature_Tx|vIP_SPI_Feature_Rx|vIP_SPI_Feature_DMA)){

    case vIP_SPI_Feature_Tx:
      //?????????
      ip_spi->isBusy=IP_SPI_isBusy_M_onlyTx;
      break;

    case vIP_SPI_Feature_Tx|vIP_SPI_Feature_Rx:
      //?????????
      ip_spi->isBusy=IP_SPI_isBusy_M_TxRx;
      break;

    case vIP_SPI_Feature_Tx|vIP_SPI_Feature_DMA:
      //????????? dma
      ip_spi->isBusy=IP_SPI_isBusy_M_onlyTx_dma;

      IP_DMA_Init(DMA_Tx,_hspi->hdmatx->Instance);
      DMA_Tx->Instance.Channel->CPAR=(unsigned int)&cIP_SPI_SPI(ip_spi)->DR;

      cIP_SPI_SPI(ip_spi)->CR2|=SPI_CR2_TXDMAEN;//??????spi???tx dma??????
      break;

    case vIP_SPI_Feature_Tx|vIP_SPI_Feature_Rx|vIP_SPI_Feature_DMA:
      //????????? dma
      ip_spi->isBusy=IP_SPI_isBusy_M_TxRx_dma;

      IP_DMA_Init(DMA_Tx,_hspi->hdmatx->Instance);
      DMA_Tx->Instance.Channel->CPAR=(unsigned int)&cIP_SPI_SPI(ip_spi)->DR;

      IP_DMA_Init(DMA_Rx,_hspi->hdmarx->Instance);
      DMA_Rx->Instance.Channel->CPAR=(unsigned int)&cIP_SPI_SPI(ip_spi)->DR;

      cIP_SPI_SPI(ip_spi)->CR2|=SPI_CR2_TXDMAEN;//??????spi???tx dma??????
      cIP_SPI_SPI(ip_spi)->CR2|=SPI_CR2_RXDMAEN;//??????spi???rx dma??????
      break;
    
    default:
      //???????????????
      return -3;
  }

  
  ip_spi->SckFreq=(cIP_SPI_SPI(ip_spi)->CR1&SPI_CR1_BR_Msk)>>SPI_CR1_BR_Pos;

  if(cIP_SPI_SPI(ip_spi)->CR1&SPI_CR1_DFF)
  {
    ip_spi->BitWidth=16;
  }else{
    ip_spi->BitWidth=8;
  }
  

  ip_spi->TransStatus.Busy=0;
  ip_spi->Feature=Feature;
  ip_spi->Instance.DMA_Tx=DMA_Tx;
  ip_spi->Instance.DMA_Rx=DMA_Rx;

  cIP_SPI_SPI(ip_spi)->CR1|=SPI_CR1_SPE;//??????SPI

  return 0;
}

/**
 * @brief ??????SCK????????????
 * ???spi?????????????????????
 * @param ip_spi IP_SPI??????
 * @param Freq ?????? 0??????????????? ????????????????????????
 * @return ???
 */
void IP_SPI_SetSckFreq(IP_SPI* ip_spi,int Freq){
  unsigned int div;

  if(Freq>7){
    Freq=7;
  }

  if(ip_spi->SckFreq==Freq){
    //????????????
    return;
  }

  div=(unsigned int)Freq;

  div=(div<<SPI_CR1_BR_Pos)&SPI_CR1_BR_Msk;

  cIP_SPI_SPI(ip_spi)->CR1&=(~SPI_CR1_SPE);//??????SPI

  cIP_SPI_SPI(ip_spi)->CR1=(cIP_SPI_SPI(ip_spi)->CR1&(~SPI_CR1_BR_Msk))|div;

  ip_spi->Instance.SPI->CR1|=SPI_CR1_SPE;//??????SPI
  
  ip_spi->SckFreq=Freq;
}

/**
 * @brief ??????????????????????????????????????????
 * ???spi?????????????????????
 * @param ip_spi IP_SPI??????
 * @param Callback ???????????? ????????????????????? ???????????????????????????spi????????????(??????dma?????????????????? ??????????????????????????????spi?????????????????????????????????) ?????????IP_SPI_isBusy()????????????????????????
 * @param v ???????????????????????????
 * @return 0:?????? ??????:??????????????????
 */
int IP_SPI_SetTransDoneITCallback(IP_SPI* ip_spi,void (*Callback)(void*),void* v){
  if(ip_spi->Feature&(vIP_SPI_Feature_isITDMA)){
    ip_spi->TransDoneITCallback=Callback;
    ip_spi->v_TransDoneITCallback=v;
    return 0;
  }else{
    return -1;
  }
}


