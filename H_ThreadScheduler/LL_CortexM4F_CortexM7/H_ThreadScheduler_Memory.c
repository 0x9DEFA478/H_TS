/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-01 23:22:55
 * @LastEditTime: 2022-01-12 15:56:26
 * @LastEditors: 0x9DEFA478
 * @Description: 线程调度器需要的内存管理
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
#include "H_Malloc.h"




#define vH_TS_MemSize (32*1024)

__attribute__((aligned(8))) static Hbyte Mem[vH_TS_MemSize];

#define vH_TS_Mem_IsDebug                                         0




/**
 * @brief 分配内存
 * @param Size 目标大小
 * @return 内存指针
 */
void* H_TS_Malloc(Hsize Size){
#if vH_TS_Mem_IsDebug != 0
  H_Malloc_Info info;
#endif
  void* r;
   
  r=H_Malloc(Mem,Size);
  
#if vH_TS_Mem_IsDebug != 0
  H_Malloc_GetInfo(Mem,&info);
  if(info.Result==0){
    return r;
  }
  
  for(;;){}
#else
  
  return r;
#endif
}

/**
 * @brief 释放内存
 * @param v 要释放的内存指针
 * @return 无
 */
void H_TS_Free(void* v){

#if vH_TS_Mem_IsDebug != 0
  H_Malloc_Info info;
#endif

  H_Free(Mem,v);
  

#if vH_TS_Mem_IsDebug != 0
  H_Malloc_GetInfo(Mem,&info);
  if(info.Result==0){
    return;
  }
  
  for(;;){}
#endif
}

/**
 * @brief 获取内存信息
 * @param FreeSize 用于返回剩余栈空间
 * @param AllSize 返回所有的栈空间
 * @return 无
 */
void H_TS_MemoryGetInfo(Hsize* FreeSize,Hsize* AllSize){
  H_Malloc_Info info;
  H_Malloc_GetInfo(Mem,&info);

  
  if(info.Result==0){
    AllSize[0]=vH_TS_MemSize;
    FreeSize[0]=info.FreeSize;
  }else{
    AllSize[0]=100;
    FreeSize[0]=info.Result;
  }
  
}

/**
 * @brief 初始化内存 此方法在H_TS初始化时被调用(H_TS_Init())
 * @return 无
 */
void H_TS_MemoryInit(){

  H_Malloc_Init(Mem,vH_TS_MemSize,3);
}

/**
 * @brief 内存错误回调
 * @return 无
 */
void H_TS_MemoryErrorCallback(){
  H_Malloc_Info info;
  H_Malloc_GetInfo(Mem,&info);
  if(info.Result==0){
    return;
  }
  for(;;){}
}



