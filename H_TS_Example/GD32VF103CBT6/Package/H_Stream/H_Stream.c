/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-19 21:00:58
 * @LastEditTime: 2022-01-06 21:23:36
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
#include "./H_Stream.h"




/**
 * @brief ????????????
 * @param stream ?????????
 * @param Data ??????????????????
 * @param Length ????????????
 * @param Timeout ???????????? ????????????????????????
 * @return 0:?????? -1:?????? ??????:?????????
 */
int H_Stream_Wirte(H_Stream* stream,void* Data,int Length,int Timeout){

  return H_Writer_Write(stream->Writer,Data,Length,Timeout);
}

/**
 * @brief ??????
 * @param stream ?????????
 * @return ???
 */
void H_Stream_WriteLock(H_Stream* stream){
  H_Writer_Lock(stream->Writer);
}

/**
 * @brief ????????????
 * @param stream ?????????
 * @return ???
 */
void H_Stream_WriteUnlock(H_Stream* stream){
  H_Writer_Unlock(stream->Writer);
}

/**
 * @brief ???????????? ???????????????
 * @param stream ?????????
 * @param Data ???????????????
 * @param Length ??????????????????
 * @param Timeout ???????????? ????????????????????????
 * @return ????????????????????? -1:?????? ??????:?????????
 */
int H_Stream_Read(H_Stream* stream,void* Data,int Length,int Timeout){

  return H_Reader_Read(stream->Reader,Data,Length,Timeout);
}





//???????????????????????? ?????????
void H_Stream_LL_Input_Thread(void* stream,void* Buffer,int Length){

  H_Reader_LL_Input_Thread(((H_Stream*)stream)->Reader,Buffer,Length);
}

//???????????????????????? ?????????
void H_Stream_LL_Input_ISR(void* stream,void* Buffer,int Length){

  H_Reader_LL_Input_ISR(((H_Stream*)stream)->Reader,Buffer,Length);
}

//????????????????????????
void H_Stream_LL_Input(void* stream,void* Buffer,int Length,int IsISR){

  H_Reader_LL_Input(((H_Stream*)stream)->Reader,Buffer,Length,IsISR);
}

//??????????????? ?????????????????????????????? ?????????
void H_Stream_LL_TransmitDone_Thread(void* stream){

  H_Writer_LL_TransmitDone_Thread(((H_Stream*)stream)->Writer);
}

//??????????????? ?????????????????????????????? ?????????
void H_Stream_LL_TransmitDone_ISR(void* stream){

  H_Writer_LL_TransmitDone_ISR(((H_Stream*)stream)->Writer);
}

//??????????????? ??????????????????????????????
void H_Stream_LL_TransmitDone(void* stream,int IsISR){

  H_Writer_LL_TransmitDone(((H_Stream*)stream)->Writer,IsISR);
}
