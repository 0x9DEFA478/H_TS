/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-30 16:16:46
 * @LastEditTime: 2021-09-28 21:05:59
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
#ifndef __IP_SPI_H_
#define __IP_SPI_H_
#include "spi.h"
#include "IP_DMA.h"


//各种特性信息

#define vIP_SPI_Feature_isMaster  0x00000001U
#define vIP_SPI_Feature_Tx        0x00000002U
#define vIP_SPI_Feature_Rx        0x00000004U
#define vIP_SPI_Feature_DMA       0x00000008U
#define vIP_SPI_Feature_isITDMA   0x00000010U


//获取发送dma实例
#define cIP_SPI_txDMA(ip_spi) ((ip_spi)->Instance.DMA_Tx->Instance.Channel)

//获取接收dma实例
#define cIP_SPI_rxDMA(ip_spi) ((ip_spi)->Instance.DMA_Rx->Instance.Channel)

//获取spi实例
#define cIP_SPI_SPI(ip_spi)   ((ip_spi)->Instance.SPI)


typedef struct _IP_SPI{

  unsigned int Feature;

  struct
  {
    SPI_TypeDef* SPI;
    IP_DMA* DMA_Tx;
    IP_DMA* DMA_Rx;
  }Instance;

  int (*isBusy)(struct _IP_SPI*);

  volatile int SckFreq;
  volatile int BitWidth;

  void* volatile TransDoneITCallback;//在仅发送模式下 此方法被调用时可能spi仍在传输(虽然dma已经传输完毕 但还可能有一些数据在spi发送缓存中没有发送完毕) 需调用IP_SPI_isBusy()来确定真正的状态
  void* volatile v_TransDoneITCallback;

  struct{
    volatile int Busy;//此段用于 dma打开中断的情况下 标记dma忙碌状态 此段代表空闲时 spi仍有可能在传输 是否忙碌以 IP_SPI_isBusy() 方法得到的结果为准
    void* volatile Tx_Ptr;
    void* volatile Rx_Ptr;
    volatile int RemainingSize;
    void (*TransStatusRefresh)(struct _IP_SPI*);
  }TransStatus;
  
  
}IP_SPI;





/**
 * @brief 初始化 IP_SPI
 * @param ip_spi 空的IP_SPI对象(已分配内存的)
 * @param DMA_Tx 空的dma对象(已分配内存的 如果不使用可为NULL)
 * @param DMA_Rx 空的dma对象(已分配内存的 如果不使用可为NULL)
 * @param _hspi hspi句柄
 * @param Feature 类型
 * @return 0:成功 其他:不支持的操作
 */
int IP_SPI_Init(IP_SPI* ip_spi,IP_DMA* DMA_Tx,IP_DMA* DMA_Rx,SPI_HandleTypeDef* _hspi,unsigned int Feature);

/**
 * @brief 设置SCK时钟频率
 * 在spi空闲时才能调用
 * @param ip_spi IP_SPI对象
 * @param Freq 频率 0为最大频率 数值越大频率越低
 * @return 无
 */
void IP_SPI_SetSckFreq(IP_SPI* ip_spi,int Freq);

/**
 * @brief 设置中断模式下的传输完成回调
 * 在spi空闲时才能调用
 * @param ip_spi IP_SPI对象
 * @param Callback 回调函数 在仅发送模式下 此回调被调用时可能spi仍在传输(虽然dma已经传输完毕 但还可能有一些数据在spi发送缓存中没有发送完毕) 需调用IP_SPI_isBusy()来确定真正的状态
 * @param v 回调函数传入的参数
 * @return 0:成功 其他:不支持的操作
 */
int IP_SPI_SetTransDoneITCallback(IP_SPI* ip_spi,void (*Callback)(void*),void* v);







/**
 * @brief spi是否忙碌
 * @param ip_spi IP_SPI对象
 * @return 0:空闲 spi可以开始一段新的传输 其他:spi忙碌
 */
int IP_SPI_isBusy(IP_SPI* ip_spi);

/**
 * @brief 设置位宽
 * 在spi空闲时才能调用
 * @param ip_spi IP_SPI对象
 * @param BitWidth 位宽
 * @return 0:成功 其他:不支持的位宽
 */
int IP_SPI_SetBitWidth(IP_SPI* ip_spi,int BitWidth);

/**
 * @brief spi发送1字节 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送字节
 * @return 无
 */
void IP_SPI_Tx_byte(IP_SPI* ip_spi,unsigned char TxData);

/**
 * @brief spi发送多字节 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param Len 发送数量 单位:字节
 * @return 无
 */
void IP_SPI_Tx_bytes(IP_SPI* ip_spi,unsigned char* TxData,int Len);

/**
 * @brief spi发送接收1字节 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送字节
 * @return 接收的字节
 */
unsigned char IP_SPI_TxRx_byte(IP_SPI* ip_spi,unsigned char TxData);

/**
 * @brief spi发送接收多字节 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param RxData 接收数组的头指针
 * @param Len 发送数量 单位:字节
 * @return 无
 */
void IP_SPI_TxRx_bytes(IP_SPI* ip_spi,unsigned char* TxData,unsigned char* RxData,int Len);

/**
 * @brief spi发送多字节 不阻塞线程 spi进入忙碌状态直到发送结束
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param Len 发送数量 单位:字节 0~65535
 * @return 无
 */
void IP_SPI_DMATx_bytes(IP_SPI* ip_spi,unsigned char* TxData,int Len);

/**
 * @brief spi发送接收多字节 不阻塞线程 spi进入忙碌状态直到发送结束
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param RxData 接收数组的头指针
 * @param Len 发送数量 单位:字节 0~65535
 * @return 无
 */
void IP_SPI_DMATxRx_bytes(IP_SPI* ip_spi,unsigned char* TxData,unsigned char* RxData,int Len);

/**
 * @brief spi发送1半字 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送半字
 * @return 无
 */
void IP_SPI_Tx_uint16(IP_SPI* ip_spi,unsigned short TxData);

/**
 * @brief spi发送多半字 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param Len 发送数量 单位:半字
 * @return 无
 */
void IP_SPI_Tx_uint16s(IP_SPI* ip_spi,unsigned short* TxData,int Len);

/**
 * @brief spi发送接收1半字 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送半字
 * @return 接收的半字
 */
unsigned short IP_SPI_TxRx_uint16(IP_SPI* ip_spi,unsigned short TxData);

/**
 * @brief spi发送接收多半字 阻塞线程
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param RxData 接收数组的头指针
 * @param Len 发送数量 单位:半字
 * @return 无
 */
void IP_SPI_TxRx_uint16s(IP_SPI* ip_spi,unsigned short* TxData,unsigned short* RxData,int Len);

/**
 * @brief spi发送多半字 不阻塞线程 spi进入忙碌状态直到发送结束
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param Len 发送数量 单位:半字 0~65535
 * @return 无
 */
void IP_SPI_DMATx_uint16s(IP_SPI* ip_spi,unsigned short* TxData,int Len);

/**
 * @brief spi发送接收多半字 不阻塞线程 spi进入忙碌状态直到发送结束
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param RxData 接收数组的头指针
 * @param Len 发送数量 单位:半字 0~65535
 * @return 无
 */
void IP_SPI_DMATxRx_uint16s(IP_SPI* ip_spi,unsigned short* TxData,unsigned short* RxData,int Len);









/**
 * @brief spi发送多字节 不阻塞线程 spi进入忙碌状态直到发送结束
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * 使用之前应调用 IP_SPI_SetTransDoneITCallback() 来设置传输完成回调
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param Len 发送数量 单位:字节 长度不限
 * @return 无
 */
void IP_SPI_DMATx_IT_bytes(IP_SPI* ip_spi,unsigned char* TxData,int Len);

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
void IP_SPI_DMATxRx_IT_bytes(IP_SPI* ip_spi,unsigned char* TxData,unsigned char* RxData,int Len);

/**
 * @brief spi发送多半字 不阻塞线程 spi进入忙碌状态直到发送结束
 * 在spi空闲时才能调用 使用时应注意spi当前的位宽
 * 使用之前应调用 IP_SPI_SetTransDoneITCallback() 来设置传输完成回调
 * @param ip_spi IP_SPI对象
 * @param TxData 发送数组的头指针
 * @param Len 发送数量 单位:半字 长度不限
 * @return 无
 */
void IP_SPI_DMATx_IT_uint16s(IP_SPI* ip_spi,unsigned short* TxData,int Len);

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
void IP_SPI_DMATxRx_IT_uint16s(IP_SPI* ip_spi,unsigned short* TxData,unsigned short* RxData,int Len);


#endif //__IP_SPI_H_
