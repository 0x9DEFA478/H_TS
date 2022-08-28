/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-17 17:21:07
 * @LastEditTime: 2021-11-19 16:40:08
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
#include "./H_Reader.h"



extern int H_Reader_Read_byte(H_Reader* reader,void* Data,int Length,int Timeout);
extern int H_Reader_Read_uint16(H_Reader* reader,void* Data,int Length,int Timeout);
extern int H_Reader_Read_uint32(H_Reader* reader,void* Data,int Length,int Timeout);
extern int H_Reader_Read_uint64(H_Reader* reader,void* Data,int Length,int Timeout);


/**
 * @brief 新建读取器
 * @param Type 读取器数据类型 等同于H_FIFO_Init()参数中的Type
 * @param BufferSize 读取器缓存大小 等同于H_FIFO_Init()参数中的BufferSize
 * @param memoryMalloc 对象来源内存池
 * @return 读取器对象
 */
H_Reader* new_H_Reader(int Type,int BufferSize,H_MemoryMalloc* memoryMalloc){
  H_Reader* r;
  
  r=memoryMalloc->Malloc(sizeof(H_Reader));

  if(r==NULL){
    goto Error0;
  }

  r->fifo=new_H_FIFO(Type,BufferSize,memoryMalloc);
  
  if(r->fifo==NULL){
    goto Error1;
  }

  r->Sem_ReceiveDone=new_H_TS_BinarySemaphore();
  if(r->Sem_ReceiveDone==NULL){
    goto Error2;
  }

  
  switch(Type){
    case vH_FIFO_Type_byte:
      r->Read=H_Reader_Read_byte;
      break;
    case vH_FIFO_Type_uint16:
      r->Read=H_Reader_Read_uint16;
      break;
    case vH_FIFO_Type_uint32:
      r->Read=H_Reader_Read_uint32;
      break;
    case vH_FIFO_Type_uint64:
      r->Read=H_Reader_Read_uint64;
      break;
    default:
      break;
  }

  r->memoryMalloc=memoryMalloc;
  
  r->fifo_LastLength=0;

  return r;

Error2:
  delete_H_FIFO(r->fifo);
Error1:
  memoryMalloc->Free(r);
Error0:
  return NULL;
}

/**
 * @brief 释放读取器对象
 * @param reader 读取器对象
 * @return 无
 */
void delete_H_Reader(H_Reader* reader){
  
  delete_H_TS_BinarySemaphore(reader->Sem_ReceiveDone);
  delete_H_FIFO(reader->fifo);
  reader->memoryMalloc->Free(reader);
}

