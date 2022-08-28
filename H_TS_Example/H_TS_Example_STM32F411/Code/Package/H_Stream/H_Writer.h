/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-17 17:21:27
 * @LastEditTime: 2021-11-19 21:04:18
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
#ifndef __Package_H_Writer_H_
#define __Package_H_Writer_H_
#include "./H_FIFO_Builder/H_FIFO_Builder.h"
#include "H_ThreadScheduler.h"
#include "H_Memory.h"



//写入器底层调用
typedef struct{
  void* v;//底层句柄
  void (*Transmit)(void*,void*,int);//底层发送方法 (底层句柄,数据指针,数据长度)
}H_Writer_LL_Function;

//初始化参数结构
typedef struct{

  int Type;//写入器数据类型 等同于H_FIFO_Init()参数中的Type

  int IsUseTransmitter;//是否启用线程发送器 启用后 可使用发送缓存
  struct{
    int BufferSize;//等同于H_FIFO_Init()参数中的BufferSize
    int ThreadStackSize;//线程堆栈大小
    int Priority;//线程优先级
    char* TransmitThreadName;//线程名
  }TransmitterParam;

  H_Writer_LL_Function LL_Function;//底层方法
}H_Writer_InitParam;

//写入器定义
typedef struct{

  volatile int IsLLOutput;//底层是否有正在输出的数据

  struct{
    volatile int IsEnable;//是否使能Transmitter 当此段清零时 Thread会退出
    H_FIFO* fifo;//发送器缓存
    H_TS_BinarySemaphore* Sem_fifo_InputDone;//通知线程fifo有数据
    H_TS_BinarySemaphore* Sem_fifo_ReadDone;//通知写入者fifo被读取
    H_TS_Thread* Thread;//线程句柄 用于发送缓存的数据
    H_TS_BinarySemaphore* Sem_ThreadExitDone;//当此段有效时 说明Thread正在退出,且不再占用写入器的资源
  }Transmitter;

  H_TS_BinarySemaphore* Sem_OutputDone;

  H_Writer_LL_Function LL_Function;

  H_MemoryMalloc* memoryMalloc;
}H_Writer;




/**
 * @brief 新建一个写入器
 * @param InitParam 写入器参数
 * @param memoryMalloc 对象来源内存池
 * @return 写入器对象
 */
H_Writer* new_H_Writer(H_Writer_InitParam* InitParam,H_MemoryMalloc* memoryMalloc);

/**
 * @brief 释放写入器对象
 * @param writer 写入器对象
 * @return 无
 */
void delete_H_Writer(H_Writer* writer);




/**
 * @brief 写入数据
 * @param writer 写入器对象
 * @param Data 要写入的数据
 * @param Length 数据长度
 * @param Timeout 超时时间 负数代表无限等待
 * @return 0:成功 -1:超时 其他:未定义
 */
int H_Writer_Write(H_Writer* writer,void* Data,int Length,int Timeout);




//供底层调用 通知写入器写入以完成 线程用
void H_Writer_LL_TransmitDone_Thread(void* writer);

//供底层调用 通知写入器写入以完成 中断用
void H_Writer_LL_TransmitDone_ISR(void* writer);

//供底层调用 通知写入器写入以完成
void H_Writer_LL_TransmitDone(void* writer,int IsISR);


#endif //__Package_H_Writer_H_
