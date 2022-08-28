/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-17 16:31:09
 * @LastEditTime: 2021-11-19 18:05:19
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
 * @brief 新建一个流
 * @param InitParam 流参数
 * @param memoryMalloc 对象来源内存池
 * @return 流对象
 */
H_Stream* new_H_Stream(H_Stream_InitParam* InitParam,H_MemoryMalloc* memoryMalloc){
  H_Stream* r;
  H_Writer_InitParam Writer_InitParam;

  r=memoryMalloc->Malloc(sizeof(H_Stream));

  if(r==NULL){
    goto Error0;
  }

  r->Reader=new_H_Reader(InitParam->Type,InitParam->Read.BufferSize,memoryMalloc);
  if(r->Reader==NULL){
    goto Error1;
  }

  Writer_InitParam.Type=InitParam->Type;
  Writer_InitParam.IsUseTransmitter=InitParam->Write.IsUseTransmitter;
  Writer_InitParam.TransmitterParam.BufferSize=InitParam->Write.TransmitterParam.BufferSize;
  Writer_InitParam.TransmitterParam.ThreadStackSize=InitParam->Write.TransmitterParam.ThreadStackSize;
  Writer_InitParam.TransmitterParam.Priority=InitParam->Write.TransmitterParam.Priority;
  Writer_InitParam.TransmitterParam.TransmitThreadName=InitParam->Write.TransmitterParam.TransmitThreadName;
  Writer_InitParam.LL_Function=InitParam->Write.LL_Function;
  r->Writer=new_H_Writer(&Writer_InitParam,memoryMalloc);
  if(r->Writer==NULL){
    goto Error2;
  }


  r->memoryMalloc=memoryMalloc;


  return r;

Error2:
  delete_H_Reader(r->Reader);
Error1:
  memoryMalloc->Free(r);
Error0:
  return NULL;
}

/**
 * @brief 释放流对象
 * @param writer 流对象
 * @return 无
 */
void delete_H_Stream(H_Stream* stream){
  
  delete_H_Writer(stream->Writer);
  delete_H_Reader(stream->Reader);
  stream->memoryMalloc->Free(stream);
}


