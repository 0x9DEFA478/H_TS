/*
 * @Author: 0x9DEFA478
 * @Date: 2021-03-27 18:55:12
 * @LastEditTime: 2021-08-01 20:53:01
 * @LastEditors: 0x9DEFA478
 * @Description: Malloc定义
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

#ifndef __H_Malloc_H_
#define __H_Malloc_H_

#include "H_Type.h"




//长度使用的类型(与cpu位数相同可获得更高的效率)
#define H_Malloc_Size Huint32

//对齐字节数 必须为2^n 并且不小于H_Malloc_Size类型的储存字节数的两倍
#define H_Malloc_Align 8



typedef struct
{
  int Result;//结果 0:未发现问题 -1:头块不应有上一个块 -2:出现问题(长度数据被修改 尾部不对齐) -3:存在连续未分配的块 -4:上下块信息不匹配 -5:与下一块信息不匹配 -6:与上一块信息不匹配
  void* ErrPtr;//错误地址
  H_Malloc_Size UseSize;//使用内存量
  H_Malloc_Size FreeSize;//可申请量
  H_Malloc_Size OccupySize;//实际占用量
  H_Malloc_Size NoOccupySize;//空闲量
}H_Malloc_Info;




/**
 * @brief 初始化内存池
 * @param MemAddr 作为内存池数组的指针
 * @param MemSize 数组大小(字节)
 * @return 无
 */
void H_Malloc_Init(void* MemAddr,H_Malloc_Size MemSize);

/**
 * @brief 向指定内存池申请内存
 * @param MemAddr 内存池地址
 * @param Size 要申请的内存大小
 * @return 申请到的内存指针 如果为NULL则为失败
 */
void* H_Malloc(void* MemAddr,H_Malloc_Size Size);

/**
 * @brief 将内存释放回内存池
 * @param MemAddr 内存池地址
 * @param Ptr 要释放的内存指针
 * @return 无
 */
void H_Free(void* MemAddr,void* Ptr);

/**
 * @brief 获取内存池状态
 * @param MemAddr 内存池地址
 * @param info 用于存放信息的指针
 * @return 无
 */
void H_Malloc_GetInfo(void* MemAddr,H_Malloc_Info* info);



#endif