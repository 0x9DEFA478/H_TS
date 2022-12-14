/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-17 17:21:38
 * @LastEditTime: 2021-11-21 16:13:27
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



//??????????????????
static int LL_Write(H_Writer* writer,void* Data,int Length,int Timeout){

  H_TS_BinarySemaphoreTryTake(writer->Sem_OutputDone);//???????????????

  if(writer->IsLLOutput!=0){
    //????????????????????????
    
    if(Timeout==0){
      return -1;
    }
    if(Timeout<0){
      H_TS_BinarySemaphoreTake(writer->Sem_OutputDone);
    }else{
      H_TS_BinarySemaphoreTake_Time(writer->Sem_OutputDone,Timeout);

      if(writer->IsLLOutput!=0){
        //??????????????????
        return -1;
      }
    }
    
  }

  //???????????????????????????

  writer->IsLLOutput=-1;//?????????
  writer->LL_Function.Transmit(writer->LL_Function.v,Data,Length);//??????????????????

  return 0;
}


//??????????????????
int H_Writer_TransmitThread(void* v){
  H_Writer* writer;
  void* Buffer;
  H_FIFO_SizeType length;

  writer=v;

  for(;;){
    if(writer->Transmitter.IsEnable==0){
      H_TS_BinarySemaphoreGive(writer->Transmitter.Sem_ThreadExitDone);
      return 0;//????????????
    }
    
    //????????????16ms ??????????????????writer->Transmitter.IsEnable
    if(0==H_TS_BinarySemaphoreTake_Time(writer->Transmitter.Sem_fifo_InputDone,16)){

ReRead:
      Buffer=H_FIFO_Read(writer->Transmitter.fifo,&length);
      if(Buffer!=NULL){
        //???????????????

        if(length>0){
          //?????????????????????
          if(0!=LL_Write(writer,Buffer,length,-1)){
            for(;;){
              //????????????????????????0
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
 * @brief ????????????
 * @param writer ???????????????
 * @param Data ??????????????????
 * @param Length ????????????
 * @param Timeout ???????????? ????????????????????????
 * @return 0:?????? -1:?????? ??????:?????????
 */
int H_Writer_Write(H_Writer* writer,void* Data,int Length,int Timeout){
  int r;
  System_Time_ms_Type StartT;
  System_Time_ms_Type Time;

  if(writer->Transmitter.IsEnable==0){
    return LL_Write(writer,Data,Length,Timeout);
  }else{

    H_TS_BinarySemaphoreTryTake(writer->Transmitter.Sem_fifo_ReadDone);//????????????

    r=H_FIFO_Write(writer->Transmitter.fifo,Data,Length);
    if(r!=0){
      //????????????

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
        while((r!=0)&&(Time<Timeout)){//???????????????????????? ????????????FIFO???????????????????????????????????? ??????????????????????????????????????????
          H_TS_BinarySemaphoreTake_Time(writer->Transmitter.Sem_fifo_ReadDone,Timeout-Time);
          Time=System_GetTime_ms()-StartT;
          r=H_FIFO_Write(writer->Transmitter.fifo,Data,Length);
        }
        
        if(r!=0){
          return -1;//??????
        }
      }
    }

    //????????????

    H_TS_BinarySemaphoreGive(writer->Transmitter.Sem_fifo_InputDone);

    return 0;
  }
}




//??????????????? ?????????????????????????????? ?????????
void H_Writer_LL_TransmitDone_Thread(void* writer){

  ((H_Writer*)writer)->IsLLOutput=0;
  H_TS_BinarySemaphoreGive(((H_Writer*)writer)->Sem_OutputDone);
}

//??????????????? ?????????????????????????????? ?????????
void H_Writer_LL_TransmitDone_ISR(void* writer){

  ((H_Writer*)writer)->IsLLOutput=0;
  H_TS_BinarySemaphoreGive_ISR(((H_Writer*)writer)->Sem_OutputDone);
}

//??????????????? ??????????????????????????????
void H_Writer_LL_TransmitDone(void* writer,int IsISR){
  if(IsISR==0){
    H_Writer_LL_TransmitDone_Thread(writer);
  }else{
    H_Writer_LL_TransmitDone_ISR(writer);
  }
}



