/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-17 16:31:16
 * @LastEditTime: 2021-11-19 21:02:31
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
#ifndef __Package_H_Stream_H_
#define __Package_H_Stream_H_
#include "./H_Reader.h"
#include "./H_Writer.h"




//流初始化参数
typedef struct{

  int Type;//流数据类型 等同于H_FIFO_Init()参数中的Type

  struct{
    int BufferSize;//读取缓冲区大小 等同于H_FIFO_Init()参数中的BufferSize
  }Read;

  struct{
    int IsUseTransmitter;//是否启用线程发送器 启用后 可使用发送缓存
    struct{
      int BufferSize;//写入缓冲区大小 等同于H_FIFO_Init()参数中的BufferSize
      int ThreadStackSize;//线程堆栈大小
      int Priority;//线程优先级
      char* TransmitThreadName;//线程名
    }TransmitterParam;

    H_Writer_LL_Function LL_Function;//底层方法
  }Write;

}H_Stream_InitParam;

//流定义
typedef struct{
  
  H_Reader* Reader;
  H_Writer* Writer;

  H_MemoryMalloc* memoryMalloc;
}H_Stream;





/**
 * @brief 新建一个流
 * @param InitParam 流参数
 * @param memoryMalloc 对象来源内存池
 * @return 流对象
 */
H_Stream* new_H_Stream(H_Stream_InitParam* InitParam,H_MemoryMalloc* memoryMalloc);

/**
 * @brief 释放流对象
 * @param writer 流对象
 * @return 无
 */
void delete_H_Stream(H_Stream* stream);




/**
 * @brief 写入数据
 * @param stream 流对象
 * @param Data 要写入的数据
 * @param Length 数据长度
 * @param Timeout 超时时间 负数代表无限等待
 * @return 0:成功 -1:超时 其他:未定义
 */
inline int H_Stream_Wirte(H_Stream* stream,void* Data,int Length,int Timeout){

  return H_Writer_Write(stream->Writer,Data,Length,Timeout);
}

/**
 * @brief 读取数据 流读取方式
 * @param stream 流对象
 * @param Data 数据缓冲区
 * @param Length 最大读取长度
 * @param Timeout 超时时间 负数代表无限等待
 * @return 读取的实际长度 -1:超时 其他:未定义
 */
inline int H_Stream_Read(H_Stream* stream,void* Data,int Length,int Timeout){

  return H_Reader_Read(stream->Reader,Data,Length,Timeout);
}




//供底层传入数据用 线程用
void H_Stream_LL_Input_Thread(void* stream,void* Buffer,int Length);

//供底层传入数据用 中断用
void H_Stream_LL_Input_ISR(void* stream,void* Buffer,int Length);

//供底层传入数据用
void H_Stream_LL_Input(void* stream,void* Buffer,int Length,int IsISR);

//供底层调用 通知写入器写入以完成 线程用
void H_Stream_LL_TransmitDone_Thread(void* stream);

//供底层调用 通知写入器写入以完成 中断用
void H_Stream_LL_TransmitDone_ISR(void* stream);

//供底层调用 通知写入器写入以完成
void H_Stream_LL_TransmitDone(void* stream,int IsISR);


#endif //__Package_H_Stream_H_
