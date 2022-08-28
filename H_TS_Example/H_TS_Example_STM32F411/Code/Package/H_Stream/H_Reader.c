/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-17 17:21:07
 * @LastEditTime: 2021-11-19 23:44:40
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
#include "./H_Reader.h"
#include "H_Memory.h"




int H_Reader_Read_byte(H_Reader* reader,void* Data,int Length,int Timeout){
  Hbyte* data;
  Hbyte* Buffer;
  int fifo_LastLength;
  int length;

  data=Data;
  length=0;

  fifo_LastLength=reader->fifo_LastLength;
  Buffer=reader->Buffer;

Start:
  if(fifo_LastLength>=Length){
    //剩余数据足够

    H_MemoryCopy_byte(data,Buffer,Length);

    Buffer=&Buffer[Length];
    length+=Length;
    fifo_LastLength-=Length;

    if(fifo_LastLength==0){
      H_Reader_FreeReadBuffer(reader);//释放读取完毕的内存
    }else{
      reader->Buffer=Buffer;
    }
    reader->fifo_LastLength=fifo_LastLength;
    return length;
  }else if(fifo_LastLength>0){
    //剩余数据不足

    H_MemoryCopy_byte(data,Buffer,fifo_LastLength);

    data=&data[fifo_LastLength];
    length+=fifo_LastLength;
    Length-=fifo_LastLength;

    do{
      H_Reader_FreeReadBuffer(reader);//释放读取完毕的内存
      Buffer=H_Reader_GetReadBuffer(reader,&fifo_LastLength,0);
    }while((Buffer!=NULL)&&(fifo_LastLength==0));

    if(Buffer!=NULL){
      goto Start;
    }

    //无可用数据 返回
    reader->fifo_LastLength=0;//剩余缓存已为空 标记为0
    return length;
  }
  //无剩余数据


  //无数据 阻塞读取

  Buffer=H_Reader_GetReadBuffer(reader,&fifo_LastLength,Timeout);
  while((Buffer!=NULL)&&(fifo_LastLength==0)){
    H_Reader_FreeReadBuffer(reader);//释放读取完毕的内存
    Buffer=H_Reader_GetReadBuffer(reader,&fifo_LastLength,0);
  }
  if(Buffer!=NULL){
    goto Start;
  }

  //如果是无限等待 不可能执行到这 只能是时间限制超时了

  reader->fifo_LastLength=0;
  return -1;//超时
}

int H_Reader_Read_uint16(H_Reader* reader,void* Data,int Length,int Timeout){
  Huint16* data;
  Huint16* Buffer;
  int fifo_LastLength;
  int length;

  data=Data;
  length=0;

  fifo_LastLength=reader->fifo_LastLength;
  Buffer=reader->Buffer;

Start:
  if(fifo_LastLength>=Length){
    //剩余数据足够

    H_MemoryCopy_uint16(data,Buffer,Length);

    Buffer=&Buffer[Length];
    length+=Length;
    fifo_LastLength-=Length;

    if(fifo_LastLength==0){
      H_Reader_FreeReadBuffer(reader);//释放读取完毕的内存
    }else{
      reader->Buffer=Buffer;
    }
    reader->fifo_LastLength=fifo_LastLength;
    return length;
  }else if(fifo_LastLength>0){
    //剩余数据不足

    H_MemoryCopy_uint16(data,Buffer,fifo_LastLength);

    data=&data[fifo_LastLength];
    length+=fifo_LastLength;
    Length-=fifo_LastLength;

    do{
      H_Reader_FreeReadBuffer(reader);//释放读取完毕的内存
      Buffer=H_Reader_GetReadBuffer(reader,&fifo_LastLength,0);
    }while((Buffer!=NULL)&&(fifo_LastLength==0));

    if(Buffer!=NULL){
      goto Start;
    }

    //无可用数据 返回
    reader->fifo_LastLength=0;//剩余缓存已为空 标记为0
    return length;
  }
  //无剩余数据


  //无数据 阻塞读取

  Buffer=H_Reader_GetReadBuffer(reader,&fifo_LastLength,Timeout);
  while((Buffer!=NULL)&&(fifo_LastLength==0)){
    H_Reader_FreeReadBuffer(reader);//释放读取完毕的内存
    Buffer=H_Reader_GetReadBuffer(reader,&fifo_LastLength,0);
  }
  if(Buffer!=NULL){
    goto Start;
  }

  //如果是无限等待 不可能执行到这 只能是时间限制超时了

  reader->fifo_LastLength=0;
  return -1;//超时
}

int H_Reader_Read_uint32(H_Reader* reader,void* Data,int Length,int Timeout){
  Huint32* data;
  Huint32* Buffer;
  int fifo_LastLength;
  int length;

  data=Data;
  length=0;

  fifo_LastLength=reader->fifo_LastLength;
  Buffer=reader->Buffer;

Start:
  if(fifo_LastLength>=Length){
    //剩余数据足够

    H_MemoryCopy_uint32(data,Buffer,Length);

    Buffer=&Buffer[Length];
    length+=Length;
    fifo_LastLength-=Length;

    if(fifo_LastLength==0){
      H_Reader_FreeReadBuffer(reader);//释放读取完毕的内存
    }else{
      reader->Buffer=Buffer;
    }
    reader->fifo_LastLength=fifo_LastLength;
    return length;
  }else if(fifo_LastLength>0){
    //剩余数据不足

    H_MemoryCopy_uint32(data,Buffer,fifo_LastLength);

    data=&data[fifo_LastLength];
    length+=fifo_LastLength;
    Length-=fifo_LastLength;

    do{
      H_Reader_FreeReadBuffer(reader);//释放读取完毕的内存
      Buffer=H_Reader_GetReadBuffer(reader,&fifo_LastLength,0);
    }while((Buffer!=NULL)&&(fifo_LastLength==0));
    
    if(Buffer!=NULL){
      goto Start;
    }

    //无可用数据 返回
    reader->fifo_LastLength=0;//剩余缓存已为空 标记为0
    return length;
  }
  //无剩余数据


  //无数据 阻塞读取

  Buffer=H_Reader_GetReadBuffer(reader,&fifo_LastLength,Timeout);
  while((Buffer!=NULL)&&(fifo_LastLength==0)){
    H_Reader_FreeReadBuffer(reader);//释放读取完毕的内存
    Buffer=H_Reader_GetReadBuffer(reader,&fifo_LastLength,0);
  }
  if(Buffer!=NULL){
    goto Start;
  }

  //如果是无限等待 不可能执行到这 只能是时间限制超时了

  reader->fifo_LastLength=0;
  return -1;//超时
}

int H_Reader_Read_uint64(H_Reader* reader,void* Data,int Length,int Timeout){
  Huint64* data;
  Huint64* Buffer;
  int fifo_LastLength;
  int length;

  data=Data;
  length=0;

  fifo_LastLength=reader->fifo_LastLength;
  Buffer=reader->Buffer;

Start:
  if(fifo_LastLength>=Length){
    //剩余数据足够

    H_MemoryCopy_uint64(data,Buffer,Length);

    Buffer=&Buffer[Length];
    length+=Length;
    fifo_LastLength-=Length;

    if(fifo_LastLength==0){
      H_Reader_FreeReadBuffer(reader);//释放读取完毕的内存
    }else{
      reader->Buffer=Buffer;
    }
    reader->fifo_LastLength=fifo_LastLength;
    return length;
  }else if(fifo_LastLength>0){
    //剩余数据不足

    H_MemoryCopy_uint64(data,Buffer,fifo_LastLength);

    data=&data[fifo_LastLength];
    length+=fifo_LastLength;
    Length-=fifo_LastLength;

    do{
      H_Reader_FreeReadBuffer(reader);//释放读取完毕的内存
      Buffer=H_Reader_GetReadBuffer(reader,&fifo_LastLength,0);
    }while((Buffer!=NULL)&&(fifo_LastLength==0));

    if(Buffer!=NULL){
      goto Start;
    }

    //无可用数据 返回
    reader->fifo_LastLength=0;//剩余缓存已为空 标记为0
    return length;
  }
  //无剩余数据


  //无数据 阻塞读取

  Buffer=H_Reader_GetReadBuffer(reader,&fifo_LastLength,Timeout);
  while((Buffer!=NULL)&&(fifo_LastLength==0)){
    H_Reader_FreeReadBuffer(reader);//释放读取完毕的内存
    Buffer=H_Reader_GetReadBuffer(reader,&fifo_LastLength,0);
  }
  if(Buffer!=NULL){
    goto Start;
  }

  //如果是无限等待 不可能执行到这 只能是时间限制超时了

  reader->fifo_LastLength=0;
  return -1;//超时
}




/**
 * @brief 读取数据 流读取方式
 * @param reader 读取器对象
 * @param Data 数据缓冲区
 * @param Length 最大读取长度
 * @param Timeout 超时时间 负数代表无限等待
 * @return 读取的实际长度 -1:超时 其他:未定义
 */
int H_Reader_Read(H_Reader* reader,void* Data,int Length,int Timeout){
  return reader->Read(reader,Data,Length,Timeout);
}

/**
 * @brief 获取读取指针
 * @param reader 读取器对象
 * @param LengthPtr 容纳数据长度 如果返回的是-1 代表读取超时 0代表无数据
 * @param Timeout 超时时间 负数代表无限等待
 * @return 数据指针
 */
void* H_Reader_GetReadBuffer(H_Reader* reader,int* LengthPtr,int Timeout){
  void* Buffer;
  H_FIFO_SizeType length;

  H_TS_BinarySemaphoreTryTake(reader->Sem_ReceiveDone);//清空信号量

  Buffer=H_FIFO_Read(reader->fifo,&length);
  if(Buffer!=NULL){
    LengthPtr[0]=length;
  }else{

    if(Timeout<0){

      H_TS_BinarySemaphoreTake(reader->Sem_ReceiveDone);//等待
      Buffer=H_FIFO_Read(reader->fifo,&length);
      if(Buffer!=NULL){
        LengthPtr[0]=length;
      }else{
        for(;;){
          //似乎出现了问题
        }
      }

    }else if (Timeout==0){
      //无操作 直接返回
      LengthPtr[0]=0;
    }else{

      H_TS_BinarySemaphoreTake_Time(reader->Sem_ReceiveDone,Timeout);//等待
      Buffer=H_FIFO_Read(reader->fifo,&length);
      if(Buffer!=NULL){
        LengthPtr[0]=length;
      }else{
        LengthPtr[0]=-1;
      }
      
    }
  }

  return Buffer;
}

/**
 * @brief 当H_Reader_GetReadBuffer()成功获取缓存后 此方法用来释放获取的缓存
 * @param reader 读取器对象
 * @return 无
 */
void H_Reader_FreeReadBuffer(H_Reader* reader){
  H_FIFO_Next(reader->fifo);//释放读取完毕的内存
}




//供底层传入数据用 线程用
void H_Reader_LL_Input_Thread(void* reader,void* Buffer,int Length){

  if(0==H_FIFO_Write(((H_Reader*)reader)->fifo,Buffer,Length)){
    H_TS_BinarySemaphoreGive(((H_Reader*)reader)->Sem_ReceiveDone);
  }
}

//供底层传入数据用 中断用
void H_Reader_LL_Input_ISR(void* reader,void* Buffer,int Length){

  if(0==H_FIFO_Write(((H_Reader*)reader)->fifo,Buffer,Length)){
    H_TS_BinarySemaphoreGive_ISR(((H_Reader*)reader)->Sem_ReceiveDone);
  }
}

//供底层传入数据用
void H_Reader_LL_Input(void* reader,void* Buffer,int Length,int IsISR){
  if(IsISR==0){
    H_Reader_LL_Input_Thread(reader,Buffer,Length);
  }else{
    H_Reader_LL_Input_ISR(reader,Buffer,Length);
  }
}






