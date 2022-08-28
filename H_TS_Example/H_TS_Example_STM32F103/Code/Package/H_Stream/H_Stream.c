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
 * @brief 写入数据
 * @param stream 流对象
 * @param Data 要写入的数据
 * @param Length 数据长度
 * @param Timeout 超时时间 负数代表无限等待
 * @return 0:成功 -1:超时 其他:未定义
 */
int H_Stream_Wirte(H_Stream* stream,void* Data,int Length,int Timeout){

  return H_Writer_Write(stream->Writer,Data,Length,Timeout);
}

/**
 * @brief 锁定
 * @param stream 流对象
 * @return 无
 */
void H_Stream_WriteLock(H_Stream* stream){
  H_Writer_Lock(stream->Writer);
}

/**
 * @brief 解除锁定
 * @param stream 流对象
 * @return 无
 */
void H_Stream_WriteUnlock(H_Stream* stream){
  H_Writer_Unlock(stream->Writer);
}

/**
 * @brief 读取数据 流读取方式
 * @param stream 流对象
 * @param Data 数据缓冲区
 * @param Length 最大读取长度
 * @param Timeout 超时时间 负数代表无限等待
 * @return 读取的实际长度 -1:超时 其他:未定义
 */
int H_Stream_Read(H_Stream* stream,void* Data,int Length,int Timeout){

  return H_Reader_Read(stream->Reader,Data,Length,Timeout);
}





//供底层传入数据用 线程用
void H_Stream_LL_Input_Thread(void* stream,void* Buffer,int Length){

  H_Reader_LL_Input_Thread(((H_Stream*)stream)->Reader,Buffer,Length);
}

//供底层传入数据用 中断用
void H_Stream_LL_Input_ISR(void* stream,void* Buffer,int Length){

  H_Reader_LL_Input_ISR(((H_Stream*)stream)->Reader,Buffer,Length);
}

//供底层传入数据用
void H_Stream_LL_Input(void* stream,void* Buffer,int Length,int IsISR){

  H_Reader_LL_Input(((H_Stream*)stream)->Reader,Buffer,Length,IsISR);
}

//供底层调用 通知写入器写入以完成 线程用
void H_Stream_LL_TransmitDone_Thread(void* stream){

  H_Writer_LL_TransmitDone_Thread(((H_Stream*)stream)->Writer);
}

//供底层调用 通知写入器写入以完成 中断用
void H_Stream_LL_TransmitDone_ISR(void* stream){

  H_Writer_LL_TransmitDone_ISR(((H_Stream*)stream)->Writer);
}

//供底层调用 通知写入器写入以完成
void H_Stream_LL_TransmitDone(void* stream,int IsISR){

  H_Writer_LL_TransmitDone(((H_Stream*)stream)->Writer,IsISR);
}
