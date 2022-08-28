/*
 * @Author: 0x9DEFA478
 * @Date: 2021-03-27 18:55:12
 * @LastEditTime: 2021-11-05 21:09:10
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




//长度使用的类型 无符号 (与cpu位数相同可获得更高的效率)
#define H_Malloc_SizeType                                          Husize

//必须为字节类型 无符号
#define H_Malloc_ByteType                                          Hbyte



#define vH_Malloc_Info_Result_None                                 0     //未发现问题
#define vH_Malloc_Info_Result_ExistsLast                           -1    //第一个块有上一个块
#define vH_Malloc_Info_Result_InexistentLast                       -2    //非第一个块无上一个块
#define vH_Malloc_Info_Result_BlockNoAlign                         -3    //出现问题(长度数据被修改 尾部不对齐)
#define vH_Malloc_Info_Result_IdleBlockIncontnuity                 -4    //存在连续未分配的块
#define vH_Malloc_Info_Result_NextBlockError                       -5    //与下一块信息不匹配
#define vH_Malloc_Info_Result_LastBlockError                       -6    //与上一块信息不匹配

typedef struct{
  int Result;//结果 0:代表未发现问题 其他: 见宏vH_Malloc_Info_Result_
  void* ErrorPtr;//错误地址 当Result不为0时 此段有效
  H_Malloc_SizeType UseSize;//使用内存量
  H_Malloc_SizeType FreeSize;//可申请量
  H_Malloc_SizeType OccupySize;//实际占用量
  H_Malloc_SizeType NoOccupySize;//空闲量
}H_Malloc_Info;




/**
 * @brief 初始化内存池
 * @param MemoryAddr 作为内存池数组的指针
 * @param MemorySize 数组大小(字节) 必须为MemoryAlign的倍数
 * @param MemoryAlign 用于生成对齐字节数 对齐字节数=2^MemoryAlign 并且2^MemoryAlign不小于两倍sizeof(H_Malloc_SizeType)
 * @return 无
 */
void H_Malloc_Init(void* MemoryAddr,H_Malloc_SizeType MemorySize,H_Malloc_SizeType MemoryAlign);

/**
 * @brief 向指定内存池申请内存
 * @param MemoryAddr 内存池地址
 * @param Size 要申请的内存大小
 * @return 申请到的内存指针 如果为NULL则为失败
 */
void* H_Malloc(void* MemoryAddr,H_Malloc_SizeType Size);

/**
 * @brief 将内存释放回内存池
 * @param MemoryAddr 内存池地址
 * @param Ptr 要释放的内存指针
 * @return 无
 */
void H_Free(void* MemoryAddr,void* Ptr);

/**
 * @brief 获取内存池状态
 * @param MemoryAddr 内存池地址
 * @param info 用于存放信息的指针
 * @return 无
 */
void H_Malloc_GetInfo(void* MemoryAddr,H_Malloc_Info* info);



#endif
