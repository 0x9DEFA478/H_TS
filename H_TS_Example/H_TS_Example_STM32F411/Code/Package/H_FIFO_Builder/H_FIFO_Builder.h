/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-16 00:19:12
 * @LastEditTime: 2021-11-19 01:13:06
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
#ifndef __Package_H_FIFO_Builder_H_
#define __Package_H_FIFO_Builder_H_
#include "H_FIFO.h"
#include "H_MemoryMalloc.h"


/**
 * @brief 创建H_FIFO
 * @param Type 等同于H_FIFO_Init()参数中的Type
 * @param BufferSize 等同于H_FIFO_Init()参数中的BufferSize
 * @return NULL:失败 其他:成功
 */
H_FIFO* new_H_FIFO(int Type,int BufferSize,H_MemoryMalloc* memoryMalloc);

/**
 * @brief 释放H_FIFO
 * @param fifo 要释放的H_FIFO
 * @return 无
 */
void delete_H_FIFO(H_FIFO* fifo);


#endif //__Package_H_FIFO_Builder_H_
