/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-30 16:22:04
 * @LastEditTime: 2021-10-30 21:47:42
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
 * @brief 初始化 IP_SPI
 * @param ip_spi 空的IP_SPI对象(已分配内存的)
 * @param DMA_Tx 已配置好的dma对象(已分配内存的 如果不使用可为NULL)
 * @param DMA_Rx 已配置好的dma对象(已分配内存的 如果不使用可为NULL)
 * @param _spi spi句柄
 * @param Feature 类型
 * @return 0:成功 其他:不支持的操作
 */
int IP_SPI_Init(IP_SPI* ip_spi,IP_DMA* DMA_Tx,IP_DMA* DMA_Rx,SPI_TypeDef* _spi,unsigned int Feature){

  if((Feature&vIP_SPI_Feature_isMaster)==0U){
    //暂时不支持从模式
    return -1;
  }
  if((Feature&vIP_SPI_Feature_Tx)!=vIP_SPI_Feature_Tx){
    //暂时不支持只接收模式
    return -2;
  }

  cIP_SPI_SPI(ip_spi)=_spi;//实例传入

  switch (Feature&(vIP_SPI_Feature_Tx|vIP_SPI_Feature_Rx|vIP_SPI_Feature_DMA)){

    case vIP_SPI_Feature_Tx:
      //仅发送
      ip_spi->isBusy=IP_SPI_isBusy_M_onlyTx;
      break;

    case vIP_SPI_Feature_Tx|vIP_SPI_Feature_Rx:
      //全双工
      ip_spi->isBusy=IP_SPI_isBusy_M_TxRx;
      break;

    case vIP_SPI_Feature_Tx|vIP_SPI_Feature_DMA:
      //仅发送 dma
      ip_spi->isBusy=IP_SPI_isBusy_M_onlyTx_dma;

      DMA_Tx->Instance.Stream->PAR=(unsigned int)&cIP_SPI_SPI(ip_spi)->DR;

      cIP_SPI_SPI(ip_spi)->CR2|=SPI_CR2_TXDMAEN;//使能spi的tx dma请求
      break;

    case vIP_SPI_Feature_Tx|vIP_SPI_Feature_Rx|vIP_SPI_Feature_DMA:
      //全双工 dma
      ip_spi->isBusy=IP_SPI_isBusy_M_TxRx_dma;

      DMA_Tx->Instance.Stream->PAR=(unsigned int)&cIP_SPI_SPI(ip_spi)->DR;

      DMA_Rx->Instance.Stream->PAR=(unsigned int)&cIP_SPI_SPI(ip_spi)->DR;

      cIP_SPI_SPI(ip_spi)->CR2|=SPI_CR2_TXDMAEN;//使能spi的tx dma请求
      cIP_SPI_SPI(ip_spi)->CR2|=SPI_CR2_RXDMAEN;//使能spi的rx dma请求
      break;
    
    default:
      //意外的情况
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

  cIP_SPI_SPI(ip_spi)->CR1|=SPI_CR1_SPE;//使能SPI

  return 0;
}

/**
 * @brief 设置SCK时钟频率
 * 在spi空闲时才能调用
 * @param ip_spi IP_SPI对象
 * @param Freq 频率 0为最大频率 数值越大频率越低
 * @return 无
 */
void IP_SPI_SetSckFreq(IP_SPI* ip_spi,int Freq){
  unsigned int div;

  if(Freq>7){
    Freq=7;
  }

  if(ip_spi->SckFreq==Freq){
    //无需修改
    return;
  }

  div=(unsigned int)Freq;

  div=(div<<SPI_CR1_BR_Pos)&SPI_CR1_BR_Msk;

  cIP_SPI_SPI(ip_spi)->CR1&=(~SPI_CR1_SPE);//失能SPI

  cIP_SPI_SPI(ip_spi)->CR1=(cIP_SPI_SPI(ip_spi)->CR1&(~SPI_CR1_BR_Msk))|div;

  ip_spi->Instance.SPI->CR1|=SPI_CR1_SPE;//使能SPI
  
  ip_spi->SckFreq=Freq;
}

/**
 * @brief 设置中断模式下的传输完成回调
 * 在spi空闲时才能调用
 * @param ip_spi IP_SPI对象
 * @param Callback 回调函数 在仅发送模式下 此回调被调用时可能spi仍在传输(虽然dma已经传输完毕 但还可能有一些数据在spi发送缓存中没有发送完毕) 需调用IP_SPI_isBusy()来确定真正的状态
 * @param v 回调函数传入的参数
 * @return 0:成功 其他:不支持的操作
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


