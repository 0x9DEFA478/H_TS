/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-30 16:16:39
 * @LastEditTime: 2021-09-30 21:47:54
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


int IP_SPI_isBusy_M_onlyTx(IP_SPI* ip_spi){
  //此模式下为阻塞传输 此方法总是返回0 即不忙碌
  return 0;
}

int IP_SPI_isBusy_M_TxRx(IP_SPI* ip_spi){
  //此模式下为阻塞传输 此方法总是返回0 即不忙碌
  return 0;
}

int IP_SPI_isBusy_M_onlyTx_dma(IP_SPI* ip_spi){

  if(cIP_SPI_txDMA(ip_spi)->CR&DMA_SxCR_EN){
    //dma正在传输
    return -1;
	}
	
  if(!(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_TXE)){
    return -1;
  }
	if(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_BSY){
		return -1;
	}
	
	return 0;
}

int IP_SPI_isBusy_M_TxRx_dma(IP_SPI* ip_spi){

  if(cIP_SPI_rxDMA(ip_spi)->CR&DMA_SxCR_EN){
    //dma正在传输
    return -1;
	}

  //全双工 接收完成即意味着通讯结束

  return 0;
}




/**
 * @brief spi是否忙碌
 * @param ip_spi IP_SPI对象
 * @return 0:空闲 spi可以开始一段新的传输 其他:spi忙碌
 */
int IP_SPI_isBusy(IP_SPI* ip_spi){
  if(ip_spi->TransStatus.Busy){
    return -1;
  }
  return ip_spi->isBusy(ip_spi);
}

/**
 * @brief 设置位宽
 * 在spi空闲时才能调用
 * @param ip_spi IP_SPI对象
 * @param BitWidth 位宽
 * @return 0:成功 其他:不支持的位宽
 */
int IP_SPI_SetBitWidth(IP_SPI* ip_spi,int BitWidth){

  if(ip_spi->BitWidth==BitWidth){
    //无需修改
    return 0;
  }

  switch(BitWidth){
    case 8:

      cIP_SPI_SPI(ip_spi)->CR1&=(~SPI_CR1_SPE);//失能SPI

      if(ip_spi->Feature&vIP_SPI_Feature_DMA)
      {
        if(ip_spi->Feature&vIP_SPI_Feature_Tx){
          cIP_SPI_txDMA(ip_spi)->CR=(cIP_SPI_txDMA(ip_spi)->CR&(~(DMA_SxCR_MSIZE_Msk|DMA_SxCR_PSIZE_Msk)))|((0U<<DMA_SxCR_MSIZE_Pos)|(0U<<DMA_SxCR_PSIZE_Pos));
        }
        if(ip_spi->Feature&vIP_SPI_Feature_Rx){
          cIP_SPI_rxDMA(ip_spi)->CR=(cIP_SPI_txDMA(ip_spi)->CR&(~(DMA_SxCR_MSIZE_Msk|DMA_SxCR_PSIZE_Msk)))|((0U<<DMA_SxCR_MSIZE_Pos)|(0U<<DMA_SxCR_PSIZE_Pos));
        }
      }

      cIP_SPI_SPI(ip_spi)->CR1&=(~SPI_CR1_DFF);

      ip_spi->BitWidth=BitWidth;
      ip_spi->Instance.SPI->CR1|=SPI_CR1_SPE;//使能SPI
     return 0;

    case 16:

      cIP_SPI_SPI(ip_spi)->CR1&=(~SPI_CR1_SPE);//失能SPI

      if(ip_spi->Feature&vIP_SPI_Feature_DMA)
      {
        if(ip_spi->Feature&vIP_SPI_Feature_Tx){
          cIP_SPI_txDMA(ip_spi)->CR=(cIP_SPI_txDMA(ip_spi)->CR&(~(DMA_SxCR_MSIZE_Msk|DMA_SxCR_PSIZE_Msk)))|((1U<<DMA_SxCR_MSIZE_Pos)|(1U<<DMA_SxCR_PSIZE_Pos));
        }
        if(ip_spi->Feature&vIP_SPI_Feature_Rx){
          cIP_SPI_rxDMA(ip_spi)->CR=(cIP_SPI_txDMA(ip_spi)->CR&(~(DMA_SxCR_MSIZE_Msk|DMA_SxCR_PSIZE_Msk)))|((1U<<DMA_SxCR_MSIZE_Pos)|(1U<<DMA_SxCR_PSIZE_Pos));
        }

      }

      cIP_SPI_SPI(ip_spi)->CR1|=SPI_CR1_DFF;
      
      ip_spi->BitWidth=BitWidth;
      ip_spi->Instance.SPI->CR1|=SPI_CR1_SPE;//使能SPI
      return 0;
    
    default:
      return -1;
  }
}

/**
 * @brief spi发送1字节 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送字节
 * @return 无
 */
void IP_SPI_Tx_byte(IP_SPI* ip_spi,unsigned char TxData){

  *((volatile unsigned char*)&cIP_SPI_SPI(ip_spi)->DR)=TxData;
  while(!(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_TXE)){
  }
  while(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_BSY){
	}
}

/**
 * @brief spi发送多字节 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param Len 发送数量 单位:字节
 * @return 无
 */
void IP_SPI_Tx_bytes(IP_SPI* ip_spi,unsigned char* TxData,int Len){
	volatile unsigned char* TxPtr;

  TxPtr=(volatile unsigned char*)&cIP_SPI_SPI(ip_spi)->DR;

  while(Len){
		if(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_TXE){//可写入
			TxPtr[0]=TxData[0];
			TxData=&TxData[1];
			Len--;
		}
	}

  while(!(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_TXE)){
  }
  while(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_BSY){
	}
}

/**
 * @brief spi发送接收1字节 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送字节
 * @return 接收的字节
 */
unsigned char IP_SPI_TxRx_byte(IP_SPI* ip_spi,unsigned char TxData){
  unsigned char RxData;

  *((volatile unsigned char*)&cIP_SPI_SPI(ip_spi)->DR)=TxData;

  while(!(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_RXNE)){
  }
  RxData=*((volatile unsigned char*)&cIP_SPI_SPI(ip_spi)->DR);

  return RxData;
}

/**
 * @brief spi发送接收多字节 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param RxData 接收数组的头指针
 * @param Len 发送数量 单位:字节
 * @return 无
 */
void IP_SPI_TxRx_bytes(IP_SPI* ip_spi,unsigned char* TxData,unsigned char* RxData,int Len){
	volatile unsigned char* TxPtr;
  volatile unsigned char* RxPtr;
  int rxLen;

  TxPtr=(volatile unsigned char*)&cIP_SPI_SPI(ip_spi)->DR;
  RxPtr=(volatile unsigned char*)&cIP_SPI_SPI(ip_spi)->DR;

  rxLen=Len;
  while((Len>0)||(rxLen>0)){
		if(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_TXE){//可写入
			TxPtr[0]=TxData[0];
			TxData=&TxData[1];
			Len--;
		}
    if(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_RXNE){//可读取
			RxData[0]=RxPtr[0];
			RxData=&RxData[1];
			rxLen--;
		}
	}
}

/**
 * @brief spi发送多字节 不阻塞线程 spi进入忙碌状态直到发送结束
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param Len 发送数量 单位:字节 0~65535
 * @return 无
 */
void IP_SPI_DMATx_bytes(IP_SPI* ip_spi,unsigned char* TxData,int Len){

  if(Len<=0){
    return;
  }

  oIP_DMA_ClearAllBit_IFCR(ip_spi->Instance.DMA_Tx);

  cIP_SPI_txDMA(ip_spi)->NDTR=(unsigned int)Len;
  cIP_SPI_txDMA(ip_spi)->M0AR=(unsigned int)TxData;

  cIP_SPI_txDMA(ip_spi)->CR|=DMA_SxCR_EN;//使能DMA
}

/**
 * @brief spi发送接收多字节 不阻塞线程 spi进入忙碌状态直到发送结束
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param RxData 接收数组的头指针
 * @param Len 发送数量 单位:字节 0~65535
 * @return 无
 */
void IP_SPI_DMATxRx_bytes(IP_SPI* ip_spi,unsigned char* TxData,unsigned char* RxData,int Len){

  if(Len<=0){
    return;
  }

  oIP_DMA_ClearAllBit_IFCR(ip_spi->Instance.DMA_Tx);
  oIP_DMA_ClearAllBit_IFCR(ip_spi->Instance.DMA_Rx);

  cIP_SPI_txDMA(ip_spi)->NDTR=(unsigned int)Len;
  cIP_SPI_txDMA(ip_spi)->M0AR=(unsigned int)TxData;

  cIP_SPI_rxDMA(ip_spi)->NDTR=(unsigned int)Len;
  cIP_SPI_rxDMA(ip_spi)->M0AR=(unsigned int)RxData;

  cIP_SPI_rxDMA(ip_spi)->CR|=DMA_SxCR_EN;//使能DMA
  cIP_SPI_txDMA(ip_spi)->CR|=DMA_SxCR_EN;//使能DMA
}

/**
 * @brief spi发送1半字 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送半字
 * @return 无
 */
void IP_SPI_Tx_uint16(IP_SPI* ip_spi,unsigned short TxData){

  *((volatile unsigned short*)&cIP_SPI_SPI(ip_spi)->DR)=TxData;
  while(!(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_TXE)){
  }
  while(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_BSY){
	}
}

/**
 * @brief spi发送多半字 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param Len 发送数量 单位:半字
 * @return 无
 */
void IP_SPI_Tx_uint16s(IP_SPI* ip_spi,unsigned short* TxData,int Len){
	volatile unsigned short* TxPtr;

  TxPtr=(volatile unsigned short*)&cIP_SPI_SPI(ip_spi)->DR;

  while(Len>0){
		if(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_TXE){//可写入
			TxPtr[0]=TxData[0];
			TxData=&TxData[1];
			Len--;
		}
	}

  while(!(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_TXE)){
  }
  while(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_BSY){
	}
}

/**
 * @brief spi发送接收1半字 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送半字
 * @return 接收的半字
 */
unsigned short IP_SPI_TxRx_uint16(IP_SPI* ip_spi,unsigned short TxData){
  unsigned short RxData;

  *((volatile unsigned short*)&cIP_SPI_SPI(ip_spi)->DR)=TxData;

  while(!(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_RXNE)){
  }
  RxData=*((volatile unsigned short*)&cIP_SPI_SPI(ip_spi)->DR);

  return RxData;
}

/**
 * @brief spi发送接收多半字 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param RxData 接收数组的头指针
 * @param Len 发送数量 单位:半字
 * @return 无
 */
void IP_SPI_TxRx_uint16s(IP_SPI* ip_spi,unsigned short* TxData,unsigned short* RxData,int Len){
	volatile unsigned short* TxPtr;
  volatile unsigned short* RxPtr;
  int rxLen;

  TxPtr=(volatile unsigned short*)&cIP_SPI_SPI(ip_spi)->DR;
  RxPtr=(volatile unsigned short*)&cIP_SPI_SPI(ip_spi)->DR;

  rxLen=Len;
  while((Len>0)||(rxLen>0)){
		if(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_TXE){//可写入
			TxPtr[0]=TxData[0];
			TxData=&TxData[1];
			Len--;
		}
    if(cIP_SPI_SPI(ip_spi)->SR&SPI_SR_RXNE){//可读取
			RxData[0]=RxPtr[0];
			RxData=&RxData[1];
			rxLen--;
		}
	}
}

/**
 * @brief spi发送多半字 不阻塞线程 spi进入忙碌状态直到发送结束
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param Len 发送数量 单位:半字 0~65535
 * @return 无
 */
void IP_SPI_DMATx_uint16s(IP_SPI* ip_spi,unsigned short* TxData,int Len){

  if(Len<=0){
    return;
  }

  oIP_DMA_ClearAllBit_IFCR(ip_spi->Instance.DMA_Tx);

  cIP_SPI_txDMA(ip_spi)->NDTR=(unsigned int)Len;
  cIP_SPI_txDMA(ip_spi)->M0AR=(unsigned int)TxData;

  cIP_SPI_txDMA(ip_spi)->CR|=DMA_SxCR_EN;//使能DMA
}

/**
 * @brief spi发送接收多半字 不阻塞线程 spi进入忙碌状态直到发送结束
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param RxData 接收数组的头指针
 * @param Len 发送数量 单位:半字 0~65535
 * @return 无
 */
void IP_SPI_DMATxRx_uint16s(IP_SPI* ip_spi,unsigned short* TxData,unsigned short* RxData,int Len){

  if(Len<=0){
    return;
  }

  oIP_DMA_ClearAllBit_IFCR(ip_spi->Instance.DMA_Tx);
  oIP_DMA_ClearAllBit_IFCR(ip_spi->Instance.DMA_Rx);

  cIP_SPI_txDMA(ip_spi)->NDTR=(unsigned int)Len;
  cIP_SPI_txDMA(ip_spi)->M0AR=(unsigned int)TxData;

  cIP_SPI_rxDMA(ip_spi)->NDTR=(unsigned int)Len;
  cIP_SPI_rxDMA(ip_spi)->M0AR=(unsigned int)RxData;

  cIP_SPI_rxDMA(ip_spi)->CR|=DMA_SxCR_EN;//使能DMA
  cIP_SPI_txDMA(ip_spi)->CR|=DMA_SxCR_EN;//使能DMA
}







//主模式 发送 txDMA中断回调
static void IP_SPI_DMA_IRQ_m_tx(void* v){
  IP_SPI* ip_spi;

  ip_spi=v;

  if(oIP_DMA_GetTC_ISR(ip_spi->Instance.DMA_Tx)){
    //发送完成

    oIP_DMA_ClearTC_IFCR(ip_spi->Instance.DMA_Tx);//清除标志位

    while(cIP_SPI_txDMA(ip_spi)->CR&DMA_SxCR_EN){
    }

    ip_spi->TransStatus.TransStatusRefresh(ip_spi);

  }else{
    for(;;){
      //未处理的中断
    }
  }
}

//主模式 发送接收 rxDMA中断回调
static void IP_SPI_DMA_IRQ_m_txrx(void* v){
  IP_SPI* ip_spi;

  ip_spi=v;

  if(oIP_DMA_GetTC_ISR(ip_spi->Instance.DMA_Rx)){
    //传输完成

    oIP_DMA_ClearTC_IFCR(ip_spi->Instance.DMA_Rx);//清除标志位

    while(cIP_SPI_txDMA(ip_spi)->CR&DMA_SxCR_EN){
    }
    while(cIP_SPI_rxDMA(ip_spi)->CR&DMA_SxCR_EN){
    }

    ip_spi->TransStatus.TransStatusRefresh(ip_spi);

  }else{
    for(;;){
      //未处理的中断
    }
  }
}

//中断数据刷新 发送字节
static void IP_SPI_DMATx_IT_bytes_TransStatusRefresh(IP_SPI* ip_spi){
  void (*TransDoneITCallback)(void*);

  if(ip_spi->TransStatus.RemainingSize==0){
    //传输完成

    cIP_SPI_txDMA(ip_spi)->CR&=(~DMA_SxCR_TCIE);//失能完成中断
    ip_spi->TransStatus.Busy=0;//解除忙碌状态
    TransDoneITCallback=(void (*)(void*))ip_spi->TransDoneITCallback;
    TransDoneITCallback(ip_spi->v_TransDoneITCallback);//调用用户完成回调

  }else{
    //传输未完成

    if(ip_spi->TransStatus.RemainingSize>65535){
      cIP_SPI_txDMA(ip_spi)->NDTR=65535U;
      cIP_SPI_txDMA(ip_spi)->M0AR=(unsigned int)ip_spi->TransStatus.Tx_Ptr;

      ip_spi->TransStatus.RemainingSize-=65535;
      ip_spi->TransStatus.Tx_Ptr=&((unsigned char*)ip_spi->TransStatus.Tx_Ptr)[65535];
    }else{
      cIP_SPI_txDMA(ip_spi)->NDTR=(unsigned int)ip_spi->TransStatus.RemainingSize;
      cIP_SPI_txDMA(ip_spi)->M0AR=(unsigned int)ip_spi->TransStatus.Tx_Ptr;

      ip_spi->TransStatus.RemainingSize=0;
    }

    oIP_DMA_ClearAllBit_IFCR(ip_spi->Instance.DMA_Tx);

    cIP_SPI_txDMA(ip_spi)->CR|=DMA_SxCR_EN;//使能DMA
  }
}

//中断数据刷新 发送接收字节
static void IP_SPI_DMATxRx_IT_bytes_TransStatusRefresh(IP_SPI* ip_spi){
  void (*TransDoneITCallback)(void*);
  
  if(ip_spi->TransStatus.RemainingSize==0){
    //传输完成

    cIP_SPI_rxDMA(ip_spi)->CR&=(~DMA_SxCR_TCIE);//失能完成中断
    ip_spi->TransStatus.Busy=0;//解除忙碌状态
    TransDoneITCallback=(void (*)(void*))ip_spi->TransDoneITCallback;
    TransDoneITCallback(ip_spi->v_TransDoneITCallback);//调用用户完成回调

  }else{
    //传输未完成

    if(ip_spi->TransStatus.RemainingSize>65535){
      cIP_SPI_txDMA(ip_spi)->NDTR=65535U;
      cIP_SPI_txDMA(ip_spi)->M0AR=(unsigned int)ip_spi->TransStatus.Tx_Ptr;
      cIP_SPI_rxDMA(ip_spi)->NDTR=65535U;
      cIP_SPI_rxDMA(ip_spi)->M0AR=(unsigned int)ip_spi->TransStatus.Rx_Ptr;

      ip_spi->TransStatus.RemainingSize-=65535;
      ip_spi->TransStatus.Tx_Ptr=&((unsigned char*)ip_spi->TransStatus.Tx_Ptr)[65535];
      ip_spi->TransStatus.Rx_Ptr=&((unsigned char*)ip_spi->TransStatus.Rx_Ptr)[65535];
    }else{
      cIP_SPI_txDMA(ip_spi)->NDTR=(unsigned int)ip_spi->TransStatus.RemainingSize;
      cIP_SPI_txDMA(ip_spi)->M0AR=(unsigned int)ip_spi->TransStatus.Tx_Ptr;
      cIP_SPI_rxDMA(ip_spi)->NDTR=(unsigned int)ip_spi->TransStatus.RemainingSize;
      cIP_SPI_rxDMA(ip_spi)->M0AR=(unsigned int)ip_spi->TransStatus.Rx_Ptr;

      ip_spi->TransStatus.RemainingSize=0;
    }

    oIP_DMA_ClearAllBit_IFCR(ip_spi->Instance.DMA_Tx);
    oIP_DMA_ClearAllBit_IFCR(ip_spi->Instance.DMA_Rx);

    cIP_SPI_rxDMA(ip_spi)->CR|=DMA_SxCR_EN;//使能DMA
    cIP_SPI_txDMA(ip_spi)->CR|=DMA_SxCR_EN;//使能DMA
  }
}

//中断数据刷新 发送半字
static void IP_SPI_DMATx_IT_uint16s_TransStatusRefresh(IP_SPI* ip_spi){
  void (*TransDoneITCallback)(void*);
  
  if(ip_spi->TransStatus.RemainingSize==0){
    //传输完成

    cIP_SPI_txDMA(ip_spi)->CR&=(~DMA_SxCR_TCIE);//失能完成中断
    ip_spi->TransStatus.Busy=0;//解除忙碌状态
    TransDoneITCallback=(void (*)(void*))ip_spi->TransDoneITCallback;
    TransDoneITCallback(ip_spi->v_TransDoneITCallback);//调用用户完成回调

  }else{
    //传输未完成

    if(ip_spi->TransStatus.RemainingSize>65535){
      cIP_SPI_txDMA(ip_spi)->NDTR=65535U;
      cIP_SPI_txDMA(ip_spi)->M0AR=(unsigned int)ip_spi->TransStatus.Tx_Ptr;

      ip_spi->TransStatus.RemainingSize-=65535;
      ip_spi->TransStatus.Tx_Ptr=&((unsigned short*)ip_spi->TransStatus.Tx_Ptr)[65535];
    }else{
      cIP_SPI_txDMA(ip_spi)->NDTR=(unsigned int)ip_spi->TransStatus.RemainingSize;
      cIP_SPI_txDMA(ip_spi)->M0AR=(unsigned int)ip_spi->TransStatus.Tx_Ptr;

      ip_spi->TransStatus.RemainingSize=0;
    }

    oIP_DMA_ClearAllBit_IFCR(ip_spi->Instance.DMA_Tx);

    cIP_SPI_txDMA(ip_spi)->CR|=DMA_SxCR_EN;//使能DMA
  }
}

//中断数据刷新 发送接收半字
static void IP_SPI_DMATxRx_IT_uint16s_TransStatusRefresh(IP_SPI* ip_spi){
  void (*TransDoneITCallback)(void*);
  
  if(ip_spi->TransStatus.RemainingSize==0){
    //传输完成

    cIP_SPI_rxDMA(ip_spi)->CR&=(~DMA_SxCR_TCIE);//失能完成中断
    ip_spi->TransStatus.Busy=0;//解除忙碌状态
    TransDoneITCallback=(void (*)(void*))ip_spi->TransDoneITCallback;
    TransDoneITCallback(ip_spi->v_TransDoneITCallback);//调用用户完成回调

  }else{
    //传输未完成

    if(ip_spi->TransStatus.RemainingSize>65535){
      cIP_SPI_txDMA(ip_spi)->NDTR=65535U;
      cIP_SPI_txDMA(ip_spi)->M0AR=(unsigned int)ip_spi->TransStatus.Tx_Ptr;
      cIP_SPI_rxDMA(ip_spi)->NDTR=65535U;
      cIP_SPI_rxDMA(ip_spi)->M0AR=(unsigned int)ip_spi->TransStatus.Rx_Ptr;

      ip_spi->TransStatus.RemainingSize-=65535;
      ip_spi->TransStatus.Tx_Ptr=&((unsigned short*)ip_spi->TransStatus.Tx_Ptr)[65535];
      ip_spi->TransStatus.Rx_Ptr=&((unsigned short*)ip_spi->TransStatus.Rx_Ptr)[65535];
    }else{
      cIP_SPI_txDMA(ip_spi)->NDTR=(unsigned int)ip_spi->TransStatus.RemainingSize;
      cIP_SPI_txDMA(ip_spi)->M0AR=(unsigned int)ip_spi->TransStatus.Tx_Ptr;
      cIP_SPI_rxDMA(ip_spi)->NDTR=(unsigned int)ip_spi->TransStatus.RemainingSize;
      cIP_SPI_rxDMA(ip_spi)->M0AR=(unsigned int)ip_spi->TransStatus.Rx_Ptr;

      ip_spi->TransStatus.RemainingSize=0;
    }

    oIP_DMA_ClearAllBit_IFCR(ip_spi->Instance.DMA_Tx);
    oIP_DMA_ClearAllBit_IFCR(ip_spi->Instance.DMA_Rx);

    cIP_SPI_rxDMA(ip_spi)->CR|=DMA_SxCR_EN;//使能DMA
    cIP_SPI_txDMA(ip_spi)->CR|=DMA_SxCR_EN;//使能DMA
  }
}




/**
 * @brief spi发送多字节 不阻塞线程 spi进入忙碌状态直到发送结束
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * 使用之前应调用 IP_SPI_SetTransDoneITCallback() 来设置传输完成回调
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param Len 发送数量 单位:字节 长度不限
 * @return 无
 */
void IP_SPI_DMATx_IT_bytes(IP_SPI* ip_spi,unsigned char* TxData,int Len){

  if(Len<=0){
    return;
  }

  ip_spi->TransStatus.Busy=-1;//进入忙碌状态
  ip_spi->TransStatus.RemainingSize=Len;
  ip_spi->TransStatus.Tx_Ptr=TxData;
  ip_spi->TransStatus.Rx_Ptr=NULL;

  ip_spi->TransStatus.TransStatusRefresh=IP_SPI_DMATx_IT_bytes_TransStatusRefresh;
  ip_spi->Instance.DMA_Tx->SetIrqCallback(IP_SPI_DMA_IRQ_m_tx,ip_spi);
  cIP_SPI_txDMA(ip_spi)->CR|=DMA_SxCR_TCIE;//使能完成中断

  ip_spi->TransStatus.TransStatusRefresh(ip_spi);
}

/**
 * @brief spi发送接收多字节 不阻塞线程 spi进入忙碌状态直到发送结束
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * 使用之前应调用 IP_SPI_SetTransDoneITCallback() 来设置传输完成回调
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param RxData 接收数组的头指针
 * @param Len 发送数量 单位:字节 长度不限
 * @return 无
 */
void IP_SPI_DMATxRx_IT_bytes(IP_SPI* ip_spi,unsigned char* TxData,unsigned char* RxData,int Len){

  if(Len<=0){
    return;
  }

  ip_spi->TransStatus.Busy=-1;//进入忙碌状态
  ip_spi->TransStatus.RemainingSize=Len;
  ip_spi->TransStatus.Tx_Ptr=TxData;
  ip_spi->TransStatus.Rx_Ptr=RxData;

  ip_spi->TransStatus.TransStatusRefresh=IP_SPI_DMATxRx_IT_bytes_TransStatusRefresh;
  ip_spi->Instance.DMA_Rx->SetIrqCallback(IP_SPI_DMA_IRQ_m_txrx,ip_spi);
  cIP_SPI_rxDMA(ip_spi)->CR|=DMA_SxCR_TCIE;//使能完成中断

  ip_spi->TransStatus.TransStatusRefresh(ip_spi);
}

/**
 * @brief spi发送多半字 不阻塞线程 spi进入忙碌状态直到发送结束
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * 使用之前应调用 IP_SPI_SetTransDoneITCallback() 来设置传输完成回调
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param Len 发送数量 单位:半字 长度不限
 * @return 无
 */
void IP_SPI_DMATx_IT_uint16s(IP_SPI* ip_spi,unsigned short* TxData,int Len){

  if(Len<=0){
    return;
  }

  ip_spi->TransStatus.Busy=-1;//进入忙碌状态
  ip_spi->TransStatus.RemainingSize=Len;
  ip_spi->TransStatus.Tx_Ptr=TxData;
  ip_spi->TransStatus.Rx_Ptr=NULL;

  ip_spi->TransStatus.TransStatusRefresh=IP_SPI_DMATx_IT_uint16s_TransStatusRefresh;
  ip_spi->Instance.DMA_Tx->SetIrqCallback(IP_SPI_DMA_IRQ_m_tx,ip_spi);
  cIP_SPI_txDMA(ip_spi)->CR|=DMA_SxCR_TCIE;//使能完成中断

  ip_spi->TransStatus.TransStatusRefresh(ip_spi);
}

/**
 * @brief spi发送接收多半字 不阻塞线程 spi进入忙碌状态直到发送结束
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * 使用之前应调用 IP_SPI_SetTransDoneITCallback() 来设置传输完成回调
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param RxData 接收数组的头指针
 * @param Len 发送数量 单位:半字 长度不限
 * @return 无
 */
void IP_SPI_DMATxRx_IT_uint16s(IP_SPI* ip_spi,unsigned short* TxData,unsigned short* RxData,int Len){

  if(Len<=0){
    return;
  }

  ip_spi->TransStatus.Busy=-1;//进入忙碌状态
  ip_spi->TransStatus.RemainingSize=Len;
  ip_spi->TransStatus.Tx_Ptr=TxData;
  ip_spi->TransStatus.Rx_Ptr=RxData;

  ip_spi->TransStatus.TransStatusRefresh=IP_SPI_DMATxRx_IT_uint16s_TransStatusRefresh;
  ip_spi->Instance.DMA_Rx->SetIrqCallback(IP_SPI_DMA_IRQ_m_txrx,ip_spi);
  cIP_SPI_rxDMA(ip_spi)->CR|=DMA_SxCR_TCIE;//使能完成中断

  ip_spi->TransStatus.TransStatusRefresh(ip_spi);
}









