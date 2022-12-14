/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-17 16:31:16
 * @LastEditTime: 2022-01-13 21:25:34
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
#ifndef __Package_H_Stream_H_
#define __Package_H_Stream_H_
#include "./H_Reader.h"
#include "./H_Writer.h"




//??????????????????
typedef struct{

  int Type;//??????????????? ?????????H_FIFO_Init()????????????Type

  struct{
    int BufferSize;//????????????????????? ?????????H_FIFO_Init()????????????BufferSize
  }Read;

  struct{
    int IsUseTransmitter;//??????????????????????????? ????????? ?????????????????????
    struct{
      int BufferSize;//????????????????????? ?????????H_FIFO_Init()????????????BufferSize
      int ThreadStackSize;//??????????????????
      int Priority;//???????????????
      char* TransmitThreadName;//?????????
    }TransmitterParam;

    H_Writer_LL_Function LL_Function;//????????????
  }Write;

}H_Stream_InitParam;

//?????????
typedef struct{
  
  H_Reader* Reader;
  H_Writer* Writer;

  H_MemoryMalloc* memoryMalloc;
}H_Stream;





/**
 * @brief ???????????????
 * @param InitParam ?????????
 * @param memoryMalloc ?????????????????????
 * @return ?????????
 */
H_Stream* new_H_Stream(H_Stream_InitParam* InitParam,H_MemoryMalloc* memoryMalloc);

/**
 * @brief ???????????????
 * @param writer ?????????
 * @return ???
 */
void delete_H_Stream(H_Stream* stream);




/**
 * @brief ????????????
 * @param stream ?????????
 * @param Data ??????????????????
 * @param Length ????????????
 * @param Timeout ???????????? ????????????????????????
 * @return 0:?????? -1:?????? ??????:?????????
 */
int H_Stream_Wirte(H_Stream* stream,void* Data,int Length,int Timeout);

/**
 * @brief ??????
 * @param stream ?????????
 * @return ???
 */
void H_Stream_WriteLock(H_Stream* stream);

/**
 * @brief ????????????
 * @param stream ?????????
 * @return ???
 */
void H_Stream_WriteUnlock(H_Stream* stream);

/**
 * @brief ???????????? ???????????????
 * @param stream ?????????
 * @param Data ???????????????
 * @param Length ??????????????????
 * @param Timeout ???????????? ????????????????????????
 * @return ????????????????????? -1:?????? ??????:?????????
 */
int H_Stream_Read(H_Stream* stream,void* Data,int Length,int Timeout);




//???????????????????????? ?????????
void H_Stream_LL_Input_Thread(void* stream,void* Buffer,int Length);

//???????????????????????? ?????????
void H_Stream_LL_Input_ISR(void* stream,void* Buffer,int Length);

//????????????????????????
void H_Stream_LL_Input(void* stream,void* Buffer,int Length,int IsISR);

//??????????????? ?????????????????????????????? ?????????
void H_Stream_LL_TransmitDone_Thread(void* stream);

//??????????????? ?????????????????????????????? ?????????
void H_Stream_LL_TransmitDone_ISR(void* stream);

//??????????????? ??????????????????????????????
void H_Stream_LL_TransmitDone(void* stream,int IsISR);


#endif //__Package_H_Stream_H_
