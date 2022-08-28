/*
 * @Author: 0x9DEFA478
 * @Date: 2021-08-23 22:53:01
 * @LastEditTime: 2021-10-30 23:08:19
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
#include "IP_I2S.h"


extern void IP_I2S_DMA_IRQ_tx_m(IP_I2S* ip_i2s);

/**
 * @brief 初始化 IP_I2S 仅发送模式
 * @param ip_i2s 空的IP_I2S对象(已分配内存的)
 * @param DMA_Tx 初始化好的dma对象
 * @param _i2s i2s句柄
 * @return 无
 */
void IP_I2S_TxOnly_Init(IP_I2S* ip_i2s,IP_DMA* DMA_Tx,SPI_TypeDef* _i2s){

  cIP_I2S_I2S(ip_i2s)=_i2s;//实例传入

  DMA_Tx->Instance.Stream->PAR=(unsigned int)&cIP_I2S_I2S(ip_i2s)->DR;

  cIP_I2S_I2S(ip_i2s)->CR2|=SPI_CR2_TXDMAEN;//使能spi的tx dma请求

  ip_i2s->I2SDiv=(cIP_I2S_I2S(ip_i2s)->I2SPR&SPI_I2SPR_I2SDIV)<<1;
  if((cIP_I2S_I2S(ip_i2s)->I2SPR&SPI_I2SPR_ODD)!=0){
    ip_i2s->I2SDiv+=1;
  }

  ip_i2s->Instance.DMA_Tx=DMA_Tx;

}

/**
 * @brief 开始I2S传输
 * @param ip_i2s IP_I2S句柄
 * @param SizeOfDataPack 每个数据包大小(单位:字节) 必须为4的倍数
 * @param GetData_Callback 获取数据的方法
 * @param Callback_v GetData_Callback传入的参数
 * @return 无
 */
void IP_I2S_TxOnly_Start(IP_I2S* ip_i2s,int SizeOfDataPack,void* (*GetData_Callback)(void*),void* Callback_v){

  
  ip_i2s->SizeOfDataPack=SizeOfDataPack;
  ip_i2s->GetData_Callback=GetData_Callback;
  ip_i2s->Callback_v=Callback_v;

  if(!(cIP_I2S_I2S(ip_i2s)->I2SCFGR&SPI_I2SCFGR_I2SE)){
    cIP_I2S_I2S(ip_i2s)->I2SCFGR|=SPI_I2SCFGR_I2SE;//使能I2S
  }

  if(!(cIP_I2S_txDMA(ip_i2s)->CR&DMA_SxCR_EN)){//dma没有使能

    ip_i2s->Instance.DMA_Tx->SetIrqCallback((void (*)(void*))IP_I2S_DMA_IRQ_tx_m,ip_i2s);

    cIP_I2S_txDMA(ip_i2s)->PAR=(unsigned int)&cIP_I2S_I2S(ip_i2s)->DR;
    cIP_I2S_txDMA(ip_i2s)->NDTR=ip_i2s->SizeOfDataPack/2;
    cIP_I2S_txDMA(ip_i2s)->M0AR=(unsigned int)ip_i2s->GetData_Callback(ip_i2s->Callback_v);
    cIP_I2S_txDMA(ip_i2s)->M1AR=(unsigned int)ip_i2s->GetData_Callback(ip_i2s->Callback_v);

    cIP_I2S_txDMA(ip_i2s)->CR&=(~DMA_SxCR_CT);//缓存0
    cIP_I2S_txDMA(ip_i2s)->CR|=DMA_SxCR_DBM;//使能双缓存
    
    cIP_I2S_txDMA(ip_i2s)->CR|=DMA_SxCR_TCIE;//使能完成中断

    oIP_DMA_ClearAllBit_IFCR(ip_i2s->Instance.DMA_Tx);

    cIP_I2S_txDMA(ip_i2s)->CR|=DMA_SxCR_EN;//使能DMA
  }

}

/**
 * @brief 停止I2S传输
 * @param ip_i2s IP_I2S句柄
 * @return 无
 */
void IP_I2S_TxOnly_Stop(IP_I2S* ip_i2s){

  if(cIP_I2S_txDMA(ip_i2s)->CR&DMA_SxCR_EN){
    cIP_I2S_txDMA(ip_i2s)->CR&=(~DMA_SxCR_EN);//关闭dma
  }

  if((cIP_I2S_I2S(ip_i2s)->I2SCFGR&SPI_I2SCFGR_I2SE)!=0){

    while(!(cIP_I2S_I2S(ip_i2s)->SR&SPI_SR_TXE)){
    }
    while(cIP_I2S_I2S(ip_i2s)->SR&SPI_SR_BSY){
    }
    cIP_I2S_I2S(ip_i2s)->I2SCFGR&=(~SPI_I2SCFGR_I2SE);//关闭i2s
  }
  
}

/**
 * @brief 设置I2S分频 在i2s关闭的情况下使用
 * @param ip_i2s IP_I2S句柄
 * @param I2SDiv 分频系数
 * @return 无
 */
void IP_I2S_TxOnly_SetI2SDiv(IP_I2S* ip_i2s,int I2SDiv){

  if(ip_i2s->I2SDiv==I2SDiv){
    return;
  }

  ip_i2s->I2SDiv=I2SDiv;

  if((I2SDiv&0x01U)==0x1U){
    cIP_I2S_I2S(ip_i2s)->I2SPR|=SPI_I2SPR_ODD;
  }
  else{
    cIP_I2S_I2S(ip_i2s)->I2SPR&=(~SPI_I2SPR_ODD);
  }

  cIP_I2S_I2S(ip_i2s)->I2SPR=(cIP_I2S_I2S(ip_i2s)->I2SPR&(~SPI_I2SPR_I2SDIV_Msk))|(SPI_I2SPR_I2SDIV_Msk&((I2SDiv>>1)<<SPI_I2SPR_I2SDIV_Pos));
  
}


