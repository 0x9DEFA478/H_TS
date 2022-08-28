/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-19 01:07:07
 * @LastEditTime: 2021-12-01 23:08:30
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
#include "H_MemoryMalloc.h"
#include "H_Memory.h"




/**
 * @brief 重新分配内存
 * @param memoryMalloc 内存管理方法集合
 * @param Ptr 要重新分配的内存
 * @param NewSize 重新分配的大小
 * @return 新的内存 如果为NULL 则为分配失败
 */
void* H_MemoryMalloc_Realloc(H_MemoryMalloc* memoryMalloc,void* Ptr,H_Malloc_SizeType NewSize){

  void* r;

  r=memoryMalloc->Malloc(NewSize);

  if(r==NULL){
    return NULL;
  }

  H_MemoryCopy_byte(r,Ptr,NewSize);
  memoryMalloc->Free(Ptr);

  return r;
}

/**
 * @brief 申请内存 附带申请一些内存区
 * @param memoryMalloc 内存管理方法集合
 * @param MasterSize 主内存大小
 * @param AdditionalPtrArray 容纳额外内存指针
 * @param AdditionalMemorySizeArray 额外内存大小数组
 * @param NumOfAdditionalPtr 额外内存数量
 * @return 主指针
 */
void* H_MemoryMalloc_ArrayMalloc(H_MemoryMalloc* memoryMalloc,H_Malloc_SizeType MasterSize,void** AdditionalPtrArray,H_Malloc_SizeType* AdditionalMemorySizeArray,H_Malloc_SizeType NumOfAdditionalPtr){

  void* r;
  void* v;
  H_Malloc_SizeType i;
  H_Malloc_SizeType index;

  r=memoryMalloc->Malloc(MasterSize+sizeof(H_MemoryMalloc*)+sizeof(H_Malloc_SizeType)+sizeof(void*)*NumOfAdditionalPtr);

  if(r==NULL){
    return NULL;
  }

  for(i=0;i<NumOfAdditionalPtr;i++){
    v=memoryMalloc->Malloc(AdditionalMemorySizeArray[i]);

    if(v==NULL){
      
      while(i>0){
        i--;
        memoryMalloc->Free(AdditionalPtrArray[i]);
      }
      memoryMalloc->Free(r);
      return NULL;
    }

    AdditionalPtrArray[i]=v;
  }

  index=MasterSize;
  H_MemoryCopy_byte(&((Hbyte*)r)[index],&memoryMalloc,sizeof(H_MemoryMalloc*));
  index+=sizeof(H_MemoryMalloc*);
  H_MemoryCopy_byte(&((Hbyte*)r)[index],&NumOfAdditionalPtr,sizeof(H_Malloc_SizeType));
  index+=sizeof(H_Malloc_SizeType);
  H_MemoryCopy_byte(&((Hbyte*)r)[index],AdditionalPtrArray,sizeof(void*)*NumOfAdditionalPtr);

  return r;
}

/**
 * @brief 释放内存集
 * @param MasterPtr 主指针
 * @return 无
 */
void H_MemoryMalloc_ArrayFree(void* MasterPtr,H_Malloc_SizeType MasterSize){
  H_MemoryMalloc* memoryMalloc;
  void* AdditionalPtr;
  H_Malloc_SizeType NumOfAdditionalPtr;
  H_Malloc_SizeType i;
  H_Malloc_SizeType index;

  index=MasterSize;
  H_MemoryCopy_byte(&memoryMalloc,&((Hbyte*)MasterPtr)[index],sizeof(H_MemoryMalloc*));
  index+=sizeof(H_MemoryMalloc*);
  H_MemoryCopy_byte(&NumOfAdditionalPtr,&((Hbyte*)MasterPtr)[index],sizeof(H_Malloc_SizeType));
  index+=sizeof(H_Malloc_SizeType);
  
  for(i=0;i<NumOfAdditionalPtr;i++){
    H_MemoryCopy_byte(&AdditionalPtr,&((Hbyte*)MasterPtr)[index],sizeof(void*));
    memoryMalloc->Free(AdditionalPtr);
    index+=sizeof(void*);
  }

}
