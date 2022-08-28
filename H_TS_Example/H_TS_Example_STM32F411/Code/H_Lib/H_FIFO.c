/*
 * @Author: 0x9DEFA478
 * @Date: 2021-08-14 17:19:35
 * @LastEditTime: 2021-11-18 23:15:59
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
#include "H_FIFO.h"



extern int H_FIFO_byte_Write(H_FIFO* fifo,void* Data,H_FIFO_SizeType Length);
extern int H_FIFO_byte_Write_Data0(H_FIFO* fifo,void* Data0,void* Data,H_FIFO_SizeType Length);
extern void* H_FIFO_byte_Read(H_FIFO* fifo,H_FIFO_SizeType* LengthPtr);
extern void H_FIFO_byte_Next(H_FIFO* fifo);

extern int H_FIFO_uint16_Write(H_FIFO* fifo,void* Data,H_FIFO_SizeType Length);
extern int H_FIFO_uint16_Write_Data0(H_FIFO* fifo,void* Data0,void* Data,H_FIFO_SizeType Length);
extern void* H_FIFO_uint16_Read(H_FIFO* fifo,H_FIFO_SizeType* LengthPtr);
extern void H_FIFO_uint16_Next(H_FIFO* fifo);

extern int H_FIFO_uint32_Write(H_FIFO* fifo,void* Data,H_FIFO_SizeType Length);
extern int H_FIFO_uint32_Write_Data0(H_FIFO* fifo,void* Data0,void* Data,H_FIFO_SizeType Length);
extern void* H_FIFO_uint32_Read(H_FIFO* fifo,H_FIFO_SizeType* LengthPtr);
extern void H_FIFO_uint32_Next(H_FIFO* fifo);

extern int H_FIFO_uint64_Write(H_FIFO* fifo,void* Data,H_FIFO_SizeType Length);
extern int H_FIFO_uint64_Write_Data0(H_FIFO* fifo,void* Data0,void* Data,H_FIFO_SizeType Length);
extern void* H_FIFO_uint64_Read(H_FIFO* fifo,H_FIFO_SizeType* LengthPtr);
extern void H_FIFO_uint64_Next(H_FIFO* fifo);



/**
 * @brief 初始化FIFO
 * @param fifo 未使用过的FIFO句柄
 * @param Type FIFO输入/输出的数据类型 vH_FIFO_Type_byte vH_FIFO_Type_uint16 vH_FIFO_Type_uint32 vH_FIFO_Type_uint64
 * @param Buffer 指向用于缓存数据的内存的指针
 * @param BufferSize 缓存大小, 单位为Type指定的数据类型宽度
 * @return 无
 */
void H_FIFO_Init(H_FIFO* fifo,H_FIFO_SizeType Type,H_FIFO_SizeType* Buffer,H_FIFO_SizeType BufferSize){
  
  fifo->FIFO.InId=0;
	fifo->FIFO.OutId=0;

	fifo->FIFO.Buffer=Buffer;
	fifo->FIFO.BufferSize=BufferSize;

  switch(Type){
    case vH_FIFO_Type_byte:
    
#if   H_FIFO_SizeOfSizeType == 2
      fifo->FIFO.SizeSize=2;
#elif H_FIFO_SizeOfSizeType == 4
      fifo->FIFO.SizeSize=4;
#elif H_FIFO_SizeOfSizeType == 8
      fifo->FIFO.SizeSize=8;
#else
#error "请选择可用的位宽"
#endif

      fifo->Write=H_FIFO_byte_Write;
      fifo->Write_Data0=H_FIFO_byte_Write_Data0;
      fifo->Read=H_FIFO_byte_Read;
      fifo->Next=H_FIFO_byte_Next;
      break;
    case vH_FIFO_Type_uint16:

#if   H_FIFO_SizeOfSizeType == 2
      fifo->FIFO.SizeSize=1;
#elif H_FIFO_SizeOfSizeType == 4
      fifo->FIFO.SizeSize=2;
#elif H_FIFO_SizeOfSizeType == 8
      fifo->FIFO.SizeSize=4;
#else
#error "请选择可用的位宽"
#endif

      fifo->Write=H_FIFO_uint16_Write;
      fifo->Write_Data0=H_FIFO_uint16_Write_Data0;
      fifo->Read=H_FIFO_uint16_Read;
      fifo->Next=H_FIFO_uint16_Next;
      break;
    case vH_FIFO_Type_uint32:

#if   H_FIFO_SizeOfSizeType == 2
      fifo->FIFO.SizeSize=1;
#elif H_FIFO_SizeOfSizeType == 4
      fifo->FIFO.SizeSize=1;
#elif H_FIFO_SizeOfSizeType == 8
      fifo->FIFO.SizeSize=2;
#else
#error "请选择可用的位宽"
#endif

      fifo->Write=H_FIFO_uint32_Write;
      fifo->Write_Data0=H_FIFO_uint32_Write_Data0;
      fifo->Read=H_FIFO_uint32_Read;
      fifo->Next=H_FIFO_uint32_Next;
      break;
    case vH_FIFO_Type_uint64:

#if   H_FIFO_SizeOfSizeType == 2
      fifo->FIFO.SizeSize=1;
#elif H_FIFO_SizeOfSizeType == 4
      fifo->FIFO.SizeSize=1;
#elif H_FIFO_SizeOfSizeType == 8
      fifo->FIFO.SizeSize=1;
#else
#error "请选择可用的位宽"
#endif

      fifo->Write=H_FIFO_uint64_Write;
      fifo->Write_Data0=H_FIFO_uint64_Write_Data0;
      fifo->Read=H_FIFO_uint64_Read;
      fifo->Next=H_FIFO_uint64_Next;
      break;
    default:
      for(;;){
        //不支持的类型
      }
  }
}

/**
 * @brief FIFO是否不为空
 * @param fifo FIFO句柄
 * @return 0:FIFO空 其他:FIFO不为空
 */
int H_FIFO_isEmpty(H_FIFO* fifo){
  if(oH_FIFO_isEmpty(fifo)){
    return -1;
  }

  return 0;
}

/**
 * @brief 获取FIFO的当前最大可写入长度
 * @param fifo FIFO句柄
 * @return 当前可写入的最大数据长度(以fifo输入的数据类型位宽为单位)
 */
H_FIFO_SizeType H_FIFO_GetEmptySize(H_FIFO* fifo){
  H_FIFO_SizeType InId;
  H_FIFO_SizeType OutId;
  H_FIFO_SizeType BufferSize;
  H_FIFO_SizeType EmptySize;


  InId=fifo->FIFO.InId;
  OutId=fifo->FIFO.OutId;
  BufferSize=fifo->FIFO.BufferSize;

  //计算空间
  if(InId<OutId){
    EmptySize=OutId-InId;
  }else{
    EmptySize=BufferSize-InId;
    if(EmptySize<OutId){
      EmptySize=OutId;
    }
  }

  EmptySize-=fifo->FIFO.SizeSize;//去掉用于表示长度的大小
  if(EmptySize<0){
    EmptySize=0;
  }

  return EmptySize;
}

/**
 * @brief 向FIFO中添加包
 * @param fifo FIFO句柄
 * @param Data 要添加的数据
 * @param Length 长度(以fifo输入的数据类型位宽为单位)
 * @return 0:成功 其他:失败 空间或剩余数量不足
 */
int H_FIFO_Write(H_FIFO* fifo,void* Data,H_FIFO_SizeType Length){
  return fifo->Write(fifo,Data,Length);
}

/**
 * @brief 向FIFO中添加包(头部插入一个数据)
 * @param fifo FIFO句柄
 * @param Data0 头部插入的数据(位宽与fifo输入的数据类型一致)
 * @param Data 要添加的数据
 * @param Length 长度(以fifo输入的数据类型位宽为单位)
 * @return 0:成功 其他:失败 空间或剩余数量不足
 */
int H_FIFO_Write_Data0(H_FIFO* fifo,void* Data0,void* Data,H_FIFO_SizeType Length){
  return fifo->Write_Data0(fifo,Data0,Data,Length);
}

/**
 * @brief 读取输出数据指针 处理完数据后 调用H_FIFO_Next()来释放读取到的数据包
 * @param fifo FIFO句柄
 * @param LengthPtr 用于返回数据包大小
 * @return 读取数据的指针 如果为NULL((void*)0)表示无可用数据
 */
void* H_FIFO_Read(H_FIFO* fifo,H_FIFO_SizeType* LengthPtr){
  return fifo->Read(fifo,LengthPtr);
}

/**
 * @brief 读取数据指针切换到下一个 (必须在FIFO中有数据时使用, 即H_FIFO_Read()能调用成功的情况下)
 * @param fifo FIFO句柄
 * @return 无
 */
void H_FIFO_Next(H_FIFO* fifo){
  return fifo->Next(fifo);
}



