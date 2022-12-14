/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-16 00:19:06
 * @LastEditTime: 2021-11-19 01:17:06
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
#include "./H_FIFO_Builder.h"

/**
 * @brief ??????H_FIFO
 * @param Type ?????????H_FIFO_Init()????????????Type
 * @param BufferSize ?????????H_FIFO_Init()????????????BufferSize
 * @return NULL:?????? ??????:??????
 */
H_FIFO* new_H_FIFO(int Type,int BufferSize,H_MemoryMalloc* memoryMalloc){

  H_FIFO* fifo;
  void* Buffer;
  Huint32 i;

  switch(Type){
    case vH_FIFO_Type_byte:
      i=BufferSize;
      break;
    case vH_FIFO_Type_uint16:
      i=BufferSize*2;
      break;
    case vH_FIFO_Type_uint32:
      i=BufferSize*4;
      break;
    case vH_FIFO_Type_uint64:
      i=BufferSize*8;
      break;
    default:
      fifo=NULL;
      break;
  }

  fifo=H_MemoryMalloc_ArrayMalloc(memoryMalloc,sizeof(H_FIFO),&Buffer,&i,1);
  if(fifo==NULL){
    return NULL;
  }

  H_FIFO_Init(fifo,Type,Buffer,BufferSize);

  return fifo;
}

/**
 * @brief ??????H_FIFO
 * @param fifo ????????????H_FIFO
 * @return ???
 */
void delete_H_FIFO(H_FIFO* fifo){

  H_MemoryMalloc_ArrayFree(fifo,sizeof(H_FIFO));
}

