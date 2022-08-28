/*
 * @Author: 0x9DEFA478
 * @Date: 2021-08-14 17:06:45
 * @LastEditTime: 2021-11-18 23:15:57
 * @LastEditors: 0x9DEFA478
 * @Description: fifo相关方法头文件
 *                 H_FIFO特性:
 *                   1.一次写入支持任意长度的数据(只要长度足够)
 *                   2.同一个数据包在H_FIFO中内存是连续的(这造成了如果FIFO缓冲区分布在头和尾, 即使总空闲长度足够, 但还是无法写入(头或尾空闲长度足够才能写入))
 *                   3.读取时能获取到数据包大小, 即输输出的数据与输入的数据是一致的(数据一致,大小一致)
 *                 注意: 由于特性2, 初始化H_FIFO时可能需要额外分配有些空间, 否则可能造成包剩余数量足够而缓存不足的情况, 这种情况将缓存空间增加一个最大包(使用时可能出现的)大小就可以解决
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
#ifndef __H_FIFO_H_
#define __H_FIFO_H_
#include "H_Type.h"


//FIFO数据类型有关

#define vH_FIFO_Type_byte      1  //字节 长度1
#define vH_FIFO_Type_uint16    2  //字节 长度2
#define vH_FIFO_Type_uint32    4  //字节 长度4
#define vH_FIFO_Type_uint64    8  //字节 长度8


//用于表示长度的数据类型位宽 2 4 8 字节 可选 最好不要超过机器位宽(如果机器没有一次性将这种数据类型存储到内存的指令的化 就不能超过)
#define H_FIFO_SizeOfSizeType  4


#if   H_FIFO_SizeOfSizeType == 2
//用于表示长度的数据类型 字节长度必须为2
#define H_FIFO_SizeType        Hint16
//存储长度占用的大小
#define vH_FIFO_byte_SizeSize    2
//存储长度占用的大小
#define vH_FIFO_uint16_SizeSize  1
//存储长度占用的大小
#define vH_FIFO_uint32_SizeSize  1
//存储长度占用的大小
#define vH_FIFO_uint64_SizeSize  1
#elif H_FIFO_SizeOfSizeType == 4
//用于表示长度的数据类型 字节长度必须为4
#define H_FIFO_SizeType        Hint32
//存储长度占用的大小
#define vH_FIFO_byte_SizeSize    4
//存储长度占用的大小
#define vH_FIFO_uint16_SizeSize  2
//存储长度占用的大小
#define vH_FIFO_uint32_SizeSize  1
//存储长度占用的大小
#define vH_FIFO_uint64_SizeSize  1
#elif H_FIFO_SizeOfSizeType == 8
//用于表示长度的数据类型 字节长度必须为8
#define H_FIFO_SizeType        Hint64
//存储长度占用的大小
#define vH_FIFO_byte_SizeSize    8
//存储长度占用的大小
#define vH_FIFO_uint16_SizeSize  4
//存储长度占用的大小
#define vH_FIFO_uint32_SizeSize  2
//存储长度占用的大小
#define vH_FIFO_uint64_SizeSize  1
#else
#error "请选择可用的位宽"
#endif


typedef struct _H_FIFO{

	struct{
		void* Buffer;//用于存放数据缓存
		H_FIFO_SizeType BufferSize;//缓存大小
		H_FIFO_SizeType SizeSize;//长度数据需要的存储空间大小
		volatile H_FIFO_SizeType InId;//缓存写入偏移 单位与FIFO数据类型有关
		volatile H_FIFO_SizeType OutId;//缓存输出偏移
	}FIFO;
  
  int (*Write)(struct _H_FIFO*,void*,H_FIFO_SizeType);
  int (*Write_Data0)(struct _H_FIFO*,void*,void*,H_FIFO_SizeType);
  void* (*Read)(struct _H_FIFO*,H_FIFO_SizeType*);
	void (*Next)(struct _H_FIFO*);

}H_FIFO;


#define oH_FIFO_isEmpty(fifo)  ((fifo)->FIFO.InId!=(fifo)->FIFO.OutId)



/**
 * @brief 初始化FIFO
 * @param fifo 未使用过的FIFO句柄
 * @param Type FIFO输入/输出的数据类型 vH_FIFO_Type_byte vH_FIFO_Type_uint16 vH_FIFO_Type_uint32 vH_FIFO_Type_uint64
 * @param Buffer 指向用于缓存数据的内存的指针
 * @param BufferSize 缓存大小, 单位为Type指定的数据类型宽度
 * @return 无
 */
void H_FIFO_Init(H_FIFO* fifo,H_FIFO_SizeType Type,H_FIFO_SizeType* Buffer,H_FIFO_SizeType BufferSize);

/**
 * @brief FIFO是否不为空
 * @param fifo FIFO句柄
 * @return 0:FIFO空 其他:FIFO不为空
 */
int H_FIFO_isEmpty(H_FIFO* fifo);


/**
 * @brief 获取FIFO的当前最大可写入长度
 * @param fifo FIFO句柄
 * @return 当前可写入的最大数据长度(以fifo输入的数据类型位宽为单位)
 */
int H_FIFO_GetEmptySize(H_FIFO* fifo);

/**
 * @brief 读取数据指针切换到下一个 (必须在FIFO中有数据时使用, 即H_FIFO_Read()能调用成功的情况下)
 * @param fifo FIFO句柄
 * @return 无
 */
void H_FIFO_Next(H_FIFO* fifo);

/**
 * @brief 向FIFO中添加包
 * @param fifo FIFO句柄
 * @param Data 要添加的数据
 * @param Length 长度(以fifo输入的数据类型位宽为单位)
 * @return 0:成功 其他:失败 空间或剩余数量不足
 */
int H_FIFO_Write(H_FIFO* fifo,void* Data,H_FIFO_SizeType Length);

/**
 * @brief 向FIFO中添加包(头部插入一个数据)
 * @param fifo FIFO句柄
 * @param Data0 头部插入的数据(位宽与fifo输入的数据类型一致)
 * @param Data 要添加的数据
 * @param Length 长度(以fifo输入的数据类型位宽为单位)
 * @return 0:成功 其他:失败 空间或剩余数量不足
 */
int H_FIFO_Write_Data0(H_FIFO* fifo,void* Data0,void* Data,H_FIFO_SizeType Length);

/**
 * @brief 读取输出数据指针 处理完数据后 调用H_FIFO_Next()来释放读取到的数据包
 * @param fifo FIFO句柄
 * @param LengthPtr 用于返回数据包大小
 * @return 读取数据的指针 如果为NULL((void*)0)表示无可用数据
 */
void* H_FIFO_Read(H_FIFO* fifo,H_FIFO_SizeType* LengthPtr);



#endif //__H_FIFO_H_
