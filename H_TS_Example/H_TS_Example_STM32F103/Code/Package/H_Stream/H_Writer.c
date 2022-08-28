/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-17 17:21:38
 * @LastEditTime: 2022-01-07 18:59:48
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
#include "./H_Writer.h"
#include "System/System.h"



//底层发送数据
static int LL_Write(H_Writer* writer,void* Data,int Length,int Timeout){
  int r;
  
  r=0;

  H_TS_LockLock(writer->WriteLock);

  H_TS_BinarySemaphoreTryTake(writer->Sem_OutputDone);//清空信号量

  if(writer->IsLLOutput!=0){
    //有正在发送的数据
    
    if(Timeout==0){
      r=-1;
      goto Return;
    }
    if(Timeout<0){
      H_TS_BinarySemaphoreTake(writer->Sem_OutputDone);
    }else{
      H_TS_BinarySemaphoreTake_Time(writer->Sem_OutputDone,Timeout);

      if(writer->IsLLOutput!=0){
        //发送仍未完成
        r=-1;
        goto Return;
      }
    }
    
  }

  //到这里说明发送完成

  writer->IsLLOutput=-1;//先标记
  writer->LL_Function.Transmit(writer->LL_Function.v,Data,Length);//底层发送数据

Return:
  H_TS_LockUnlock(writer->WriteLock);
  return r;
}


//发送数据线程
int H_Writer_TransmitThread(void* v){
  H_Writer* writer;
  void* Buffer;
  H_FIFO_SizeType length;

  writer=v;

  for(;;){
    if(writer->Transmitter.IsEnable==0){
      H_TS_BinarySemaphoreGive(writer->Transmitter.Sem_ThreadExitDone);
      return 0;//结束线程
    }
    
    //阻塞最长16ms 以有机会检查writer->Transmitter.IsEnable
    if(0==H_TS_BinarySemaphoreTake_Time(writer->Transmitter.Sem_fifo_InputDone,16)){

ReRead:
      Buffer=H_FIFO_Read(writer->Transmitter.fifo,&length);
      if(Buffer!=NULL){
        //读取到数据

        if(length>0){
          //发送数据到底层
          if(0!=LL_Write(writer,Buffer,length,-1)){
            for(;;){
              //这个不应该返回非0
            }
          }
        }
        H_FIFO_Next(writer->Transmitter.fifo);
        H_TS_BinarySemaphoreGive(writer->Transmitter.Sem_fifo_ReadDone);

        goto ReRead;
      }
    }


  }

  return 0;
}





/**
 * @brief 写入数据
 * @param writer 写入器对象
 * @param Data 要写入的数据
 * @param Length 数据长度
 * @param Timeout 超时时间 负数代表无限等待
 * @return 0:成功 -1:超时 其他:未定义
 */
int H_Writer_Write(H_Writer* writer,void* Data,int Length,int Timeout){
  int r;
  System_Time_ms_Type StartT;
  System_Time_ms_Type Time;

  if(writer->Transmitter.IsEnable==0){
    return LL_Write(writer,Data,Length,Timeout);
  }else{

    H_TS_BinarySemaphoreTryTake(writer->Transmitter.Sem_fifo_ReadDone);//清除标记

    r=H_FIFO_Write(writer->Transmitter.fifo,Data,Length);
    if(r!=0){
      //写入失败

      if(Timeout==0){
        return -1;
      }
      if(Timeout<0){
        while(r!=0){
          H_TS_BinarySemaphoreTake(writer->Transmitter.Sem_fifo_ReadDone);
          r=H_FIFO_Write(writer->Transmitter.fifo,Data,Length);
        }
      }else{

        StartT=System_GetTime_ms();
        r=-1;
        Time=System_GetTime_ms()-StartT;
        while((r!=0)&&(Time<Timeout)){//也在这里判定时间 因为即使FIFO有空间也不一定能写入数据 但这个不足却能使阻塞提前解除
          H_TS_BinarySemaphoreTake_Time(writer->Transmitter.Sem_fifo_ReadDone,Timeout-Time);
          Time=System_GetTime_ms()-StartT;
          r=H_FIFO_Write(writer->Transmitter.fifo,Data,Length);
        }
        
        if(r!=0){
          return -1;//超时
        }
      }
    }

    //写入成功

    H_TS_BinarySemaphoreGive(writer->Transmitter.Sem_fifo_InputDone);

    return 0;
  }
}




//供底层调用 通知写入器写入以完成 线程用
void H_Writer_LL_TransmitDone_Thread(void* writer){

  ((H_Writer*)writer)->IsLLOutput=0;
  H_TS_BinarySemaphoreGive(((H_Writer*)writer)->Sem_OutputDone);
}

//供底层调用 通知写入器写入以完成 中断用
void H_Writer_LL_TransmitDone_ISR(void* writer){

  ((H_Writer*)writer)->IsLLOutput=0;
  H_TS_BinarySemaphoreGive_ISR(((H_Writer*)writer)->Sem_OutputDone);
}

//供底层调用 通知写入器写入以完成
void H_Writer_LL_TransmitDone(void* writer,int IsISR){
  if(IsISR==0){
    H_Writer_LL_TransmitDone_Thread(writer);
  }else{
    H_Writer_LL_TransmitDone_ISR(writer);
  }
}



