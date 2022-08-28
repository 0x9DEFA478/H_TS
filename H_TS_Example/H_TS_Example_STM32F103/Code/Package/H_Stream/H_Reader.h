/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-17 17:21:17
 * @LastEditTime: 2021-11-19 21:04:19
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
#ifndef __Package_H_Reader_H_
#define __Package_H_Reader_H_
#include "./H_FIFO_Builder/H_FIFO_Builder.h"
#include "H_ThreadScheduler.h"
#include "H_Memory.h"


//读取器定义
typedef struct _H_Reader{

  H_FIFO* fifo;
  void* Buffer;//FIFO读取缓存指针
  int fifo_LastLength;//FIFO输出块的剩余大小

  H_TS_BinarySemaphore* Sem_ReceiveDone;

  int (*Read)(struct _H_Reader*,void*,int,int);//等同于H_Reader_Read

  H_MemoryMalloc* memoryMalloc;
}H_Reader;




/**
 * @brief 新建读取器
 * @param Type 读取器数据类型 等同于H_FIFO_Init()参数中的Type
 * @param BufferSize 读取器缓存大小 等同于H_FIFO_Init()参数中的BufferSize
 * @param memoryMalloc 对象来源内存池
 * @return 读取器对象
 */
H_Reader* new_H_Reader(int Type,int BufferSize,H_MemoryMalloc* memoryMalloc);

/**
 * @brief 释放读取器对象
 * @param reader 读取器对象
 * @return 无
 */
void delete_H_Reader(H_Reader* reader);




/**
 * @brief 读取数据 流读取方式
 * @param reader 读取器对象
 * @param Data 数据缓冲区
 * @param Length 最大读取长度
 * @param Timeout 超时时间 负数代表无限等待
 * @return 读取的实际长度 -1:超时 其他:未定义
 */
int H_Reader_Read(H_Reader* reader,void* Data,int Length,int Timeout);

/**
 * @brief 获取读取指针
 * @param reader 读取器对象
 * @param LengthPtr 容纳数据长度 如果返回的是-1 代表读取超时 0代表无数据
 * @param Timeout 超时时间 负数代表无限等待
 * @return 数据指针
 */
void* H_Reader_GetReadBuffer(H_Reader* reader,int* LengthPtr,int Timeout);

/**
 * @brief 当H_Reader_GetReadBuffer()成功获取缓存后 此方法用来释放获取的缓存
 * @param reader 读取器对象
 * @return 无
 */
void H_Reader_FreeReadBuffer(H_Reader* reader);




//供底层传入数据用 线程用
void H_Reader_LL_Input_Thread(void* reader,void* Buffer,int Length);

//供底层传入数据用 中断用
void H_Reader_LL_Input_ISR(void* reader,void* Buffer,int Length);

//供底层传入数据用
void H_Reader_LL_Input(void* reader,void* Buffer,int Length,int IsISR);


#endif //__Package_H_Reader_H_
