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


//??????????????????

#define vIP_SPI_Feature_isMaster  0x00000001U
#define vIP_SPI_Feature_Tx        0x00000002U
#define vIP_SPI_Feature_Rx        0x00000004U
#define vIP_SPI_Feature_DMA       0x00000008U
#define vIP_SPI_Feature_isITDMA   0x00000010U


//????????????dma??????
#define cIP_SPI_txDMA(ip_spi) ((ip_spi)->Instance.DMA_Tx->Instance.Channel)

//????????????dma??????
#define cIP_SPI_rxDMA(ip_spi) ((ip_spi)->Instance.DMA_Rx->Instance.Channel)

//??????spi??????
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

  void* volatile TransDoneITCallback;//????????????????????? ???????????????????????????spi????????????(??????dma?????????????????? ??????????????????????????????spi?????????????????????????????????) ?????????IP_SPI_isBusy()????????????????????????
  void* volatile v_TransDoneITCallback;

  struct{
    volatile int Busy;//???????????? dma???????????????????????? ??????dma???????????? ????????????????????? spi????????????????????? ??????????????? IP_SPI_isBusy() ???????????????????????????
    void* volatile Tx_Ptr;
    void* volatile Rx_Ptr;
    volatile int RemainingSize;
    void (*TransStatusRefresh)(struct _IP_SPI*);
  }TransStatus;
  
  
}IP_SPI;





/**
 * @brief ????????? IP_SPI
 * @param ip_spi ??????IP_SPI??????(??????????????????)
 * @param DMA_Tx ??????dma??????(?????????????????? ?????????????????????NULL)
 * @param DMA_Rx ??????dma??????(?????????????????? ?????????????????????NULL)
 * @param _hspi hspi??????
 * @param Feature ??????
 * @return 0:?????? ??????:??????????????????
 */
int IP_SPI_Init(IP_SPI* ip_spi,IP_DMA* DMA_Tx,IP_DMA* DMA_Rx,SPI_HandleTypeDef* _hspi,unsigned int Feature);

/**
 * @brief ??????SCK????????????
 * ???spi?????????????????????
 * @param ip_spi IP_SPI??????
 * @param Freq ?????? 0??????????????? ????????????????????????
 * @return ???
 */
void IP_SPI_SetSckFreq(IP_SPI* ip_spi,int Freq);

/**
 * @brief ??????????????????????????????????????????
 * ???spi?????????????????????
 * @param ip_spi IP_SPI??????
 * @param Callback ???????????? ????????????????????? ???????????????????????????spi????????????(??????dma?????????????????? ??????????????????????????????spi?????????????????????????????????) ?????????IP_SPI_isBusy()????????????????????????
 * @param v ???????????????????????????
 * @return 0:?????? ??????:??????????????????
 */
int IP_SPI_SetTransDoneITCallback(IP_SPI* ip_spi,void (*Callback)(void*),void* v);







/**
 * @brief spi????????????
 * @param ip_spi IP_SPI??????
 * @return 0:?????? spi?????????????????????????????? ??????:spi??????
 */
int IP_SPI_isBusy(IP_SPI* ip_spi);

/**
 * @brief ????????????
 * ???spi?????????????????????
 * @param ip_spi IP_SPI??????
 * @param BitWidth ??????
 * @return 0:?????? ??????:??????????????????
 */
int IP_SPI_SetBitWidth(IP_SPI* ip_spi,int BitWidth);

/**
 * @brief spi??????1?????? ????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????
 * @return ???
 */
void IP_SPI_Tx_byte(IP_SPI* ip_spi,unsigned char TxData);

/**
 * @brief spi??????????????? ????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????????????????
 * @param Len ???????????? ??????:??????
 * @return ???
 */
void IP_SPI_Tx_bytes(IP_SPI* ip_spi,unsigned char* TxData,int Len);

/**
 * @brief spi????????????1?????? ????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????
 * @return ???????????????
 */
unsigned char IP_SPI_TxRx_byte(IP_SPI* ip_spi,unsigned char TxData);

/**
 * @brief spi????????????????????? ????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????????????????
 * @param RxData ????????????????????????
 * @param Len ???????????? ??????:??????
 * @return ???
 */
void IP_SPI_TxRx_bytes(IP_SPI* ip_spi,unsigned char* TxData,unsigned char* RxData,int Len);

/**
 * @brief spi??????????????? ??????????????? spi????????????????????????????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????????????????
 * @param Len ???????????? ??????:?????? 0~65535
 * @return ???
 */
void IP_SPI_DMATx_bytes(IP_SPI* ip_spi,unsigned char* TxData,int Len);

/**
 * @brief spi????????????????????? ??????????????? spi????????????????????????????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????????????????
 * @param RxData ????????????????????????
 * @param Len ???????????? ??????:?????? 0~65535
 * @return ???
 */
void IP_SPI_DMATxRx_bytes(IP_SPI* ip_spi,unsigned char* TxData,unsigned char* RxData,int Len);

/**
 * @brief spi??????1?????? ????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????
 * @return ???
 */
void IP_SPI_Tx_uint16(IP_SPI* ip_spi,unsigned short TxData);

/**
 * @brief spi??????????????? ????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????????????????
 * @param Len ???????????? ??????:??????
 * @return ???
 */
void IP_SPI_Tx_uint16s(IP_SPI* ip_spi,unsigned short* TxData,int Len);

/**
 * @brief spi????????????1?????? ????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????
 * @return ???????????????
 */
unsigned short IP_SPI_TxRx_uint16(IP_SPI* ip_spi,unsigned short TxData);

/**
 * @brief spi????????????????????? ????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????????????????
 * @param RxData ????????????????????????
 * @param Len ???????????? ??????:??????
 * @return ???
 */
void IP_SPI_TxRx_uint16s(IP_SPI* ip_spi,unsigned short* TxData,unsigned short* RxData,int Len);

/**
 * @brief spi??????????????? ??????????????? spi????????????????????????????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????????????????
 * @param Len ???????????? ??????:?????? 0~65535
 * @return ???
 */
void IP_SPI_DMATx_uint16s(IP_SPI* ip_spi,unsigned short* TxData,int Len);

/**
 * @brief spi????????????????????? ??????????????? spi????????????????????????????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????????????????
 * @param RxData ????????????????????????
 * @param Len ???????????? ??????:?????? 0~65535
 * @return ???
 */
void IP_SPI_DMATxRx_uint16s(IP_SPI* ip_spi,unsigned short* TxData,unsigned short* RxData,int Len);









/**
 * @brief spi??????????????? ??????????????? spi????????????????????????????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * ????????????????????? IP_SPI_SetTransDoneITCallback() ???????????????????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????????????????
 * @param Len ???????????? ??????:?????? ????????????
 * @return ???
 */
void IP_SPI_DMATx_IT_bytes(IP_SPI* ip_spi,unsigned char* TxData,int Len);

/**
 * @brief spi????????????????????? ??????????????? spi????????????????????????????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * ????????????????????? IP_SPI_SetTransDoneITCallback() ???????????????????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????????????????
 * @param RxData ????????????????????????
 * @param Len ???????????? ??????:?????? ????????????
 * @return ???
 */
void IP_SPI_DMATxRx_IT_bytes(IP_SPI* ip_spi,unsigned char* TxData,unsigned char* RxData,int Len);

/**
 * @brief spi??????????????? ??????????????? spi????????????????????????????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * ????????????????????? IP_SPI_SetTransDoneITCallback() ???????????????????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????????????????
 * @param Len ???????????? ??????:?????? ????????????
 * @return ???
 */
void IP_SPI_DMATx_IT_uint16s(IP_SPI* ip_spi,unsigned short* TxData,int Len);

/**
 * @brief spi????????????????????? ??????????????? spi????????????????????????????????????
 * ???spi????????????????????? ??????????????????spi???????????????
 * ????????????????????? IP_SPI_SetTransDoneITCallback() ???????????????????????????
 * @param ip_spi IP_SPI??????
 * @param TxData ????????????????????????
 * @param RxData ????????????????????????
 * @param Len ???????????? ??????:?????? ????????????
 * @return ???
 */
void IP_SPI_DMATxRx_IT_uint16s(IP_SPI* ip_spi,unsigned short* TxData,unsigned short* RxData,int Len);


#endif //__IP_SPI_H_
