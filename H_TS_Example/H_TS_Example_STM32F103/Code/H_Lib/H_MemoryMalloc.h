/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-19 01:07:25
 * @LastEditTime: 2021-12-01 23:08:41
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
#ifndef __H_MemoryMalloc_H_
#define __H_MemoryMalloc_H_
#include "H_Malloc.h"





typedef struct{
  void* (*Malloc)(H_Malloc_SizeType);
  void (*Free)(void*);
  void (*GetInfo)(H_Malloc_Info*);
}H_MemoryMalloc;




/**
 * @brief 重新分配内存
 * @param memoryMalloc 内存管理方法集合
 * @param Ptr 要重新分配的内存
 * @param NewSize 重新分配的大小
 * @return 新的内存 如果为NULL 则为分配失败
 */
void* H_MemoryMalloc_Realloc(H_MemoryMalloc* memoryMalloc,void* Ptr,H_Malloc_SizeType NewSize);

/**
 * @brief 申请内存 附带申请一些内存区
 * @param memoryMalloc 内存管理方法集合
 * @param MasterSize 主内存大小
 * @param AdditionalPtrArray 容纳额外内存指针
 * @param AdditionalMemorySizeArray 额外内存大小数组
 * @param NumOfAdditionalPtr 额外内存数量
 * @return 主指针
 */
void* H_MemoryMalloc_ArrayMalloc(H_MemoryMalloc* memoryMalloc,H_Malloc_SizeType MasterSize,void** AdditionalPtrArray,H_Malloc_SizeType* AdditionalMemorySizeArray,H_Malloc_SizeType NumOfAdditionalPtr);

/**
 * @brief 释放内存集
 * @param MasterPtr 主指针
 * @return 无
 */
void H_MemoryMalloc_ArrayFree(void* MasterPtr,H_Malloc_SizeType MasterSize);




#endif //__H_MemoryMalloc_H_