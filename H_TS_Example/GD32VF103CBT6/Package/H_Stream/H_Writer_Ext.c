/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-17 17:21:38
 * @LastEditTime: 2022-01-06 21:12:31
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


//??????????????????
extern int H_Writer_TransmitThread(void* v);


/**
 * @brief ?????????????????????
 * @param InitParam ???????????????
 * @param memoryMalloc ?????????????????????
 * @return ???????????????
 */
H_Writer* new_H_Writer(H_Writer_InitParam* InitParam,H_MemoryMalloc* memoryMalloc){
  H_Writer* r;

  r=memoryMalloc->Malloc(sizeof(H_Writer));

  if(r==NULL){
    goto Error0;
  }

  r->Sem_OutputDone=new_H_TS_BinarySemaphore();
  if(r->Sem_OutputDone==NULL){
    goto Error1;
  }

  r->WriteLock=new_H_TS_Lock();
  if(r->WriteLock==NULL){
    goto Error2;
  }


  r->LL_Function=InitParam->LL_Function;
  r->memoryMalloc=memoryMalloc;

  r->IsLLOutput=0;

  //??????????????????r->Transmitter???????????? ??????????????????????????????

  if(InitParam->IsUseTransmitter!=0){
    r->Transmitter.IsEnable=-1;

    r->Transmitter.fifo=new_H_FIFO(InitParam->Type,InitParam->TransmitterParam.BufferSize,memoryMalloc);
    if(r->Transmitter.fifo==NULL){
      goto Error3;
    }

    r->Transmitter.Sem_fifo_InputDone=new_H_TS_BinarySemaphore();
    if(r->Transmitter.Sem_fifo_InputDone==NULL){
      goto Error4;
    }

    r->Transmitter.Sem_fifo_ReadDone=new_H_TS_BinarySemaphore();
    if(r->Transmitter.Sem_fifo_ReadDone==NULL){
      goto Error5;
    }

    r->Transmitter.Sem_ThreadExitDone=new_H_TS_BinarySemaphore();
    if(r->Transmitter.Sem_ThreadExitDone==NULL){
      goto Error6;
    }

    // switch(InitParam->Type){
    //   case vH_FIFO_Type_byte:
    //     break;
    //   case vH_FIFO_Type_uint16:
    //     break;
    //   case vH_FIFO_Type_uint32:
    //     break;
    //   case vH_FIFO_Type_uint64:
    //     break;
    //   default:
    //     break;
    // }


    r->Transmitter.Thread=H_TS_StartNewThread_LowPriority(H_Writer_TransmitThread,r
      ,InitParam->TransmitterParam.ThreadStackSize,InitParam->TransmitterParam.Priority,0);
    if(r->Transmitter.Thread==NULL){
      goto Error7;
    }
    r->Transmitter.Thread->ThreadName=InitParam->TransmitterParam.TransmitThreadName;

  }else{
    r->Transmitter.IsEnable=0;
  }


  return r;


Error7:
  delete_H_TS_BinarySemaphore(r->Transmitter.Sem_ThreadExitDone);
Error6:
  delete_H_TS_BinarySemaphore(r->Transmitter.Sem_fifo_ReadDone);
Error5:
  delete_H_TS_BinarySemaphore(r->Transmitter.Sem_fifo_InputDone);
Error4:
  delete_H_FIFO(r->Transmitter.fifo);
Error3:
  delete_H_TS_Lock(r->WriteLock);
Error2:
  delete_H_TS_BinarySemaphore(r->Sem_OutputDone);
Error1:
  memoryMalloc->Free(r);
Error0:
  return NULL;
}

/**
 * @brief ?????????????????????
 * @param writer ???????????????
 * @return ???
 */
void delete_H_Writer(H_Writer* writer){

  if(writer->Transmitter.IsEnable!=0){
    writer->Transmitter.IsEnable=0;//???????????????????????????
    H_TS_BinarySemaphoreTake(writer->Transmitter.Sem_ThreadExitDone);//??????????????????

    delete_H_TS_BinarySemaphore(writer->Transmitter.Sem_ThreadExitDone);
    delete_H_TS_BinarySemaphore(writer->Transmitter.Sem_fifo_ReadDone);
    delete_H_TS_BinarySemaphore(writer->Transmitter.Sem_fifo_InputDone);
    delete_H_FIFO(writer->Transmitter.fifo);
  }
  
  delete_H_TS_BinarySemaphore(writer->Sem_OutputDone);
  writer->memoryMalloc->Free(writer);

}

/**
 * @brief ??????????????? ??????????????????????????????
 * @param writer ???????????????
 * @return ???
 */
void H_Writer_Lock(H_Writer* writer){

  H_TS_LockLock(writer->WriteLock);
}

/**
 * @brief ??????????????? ?????????????????????????????????
 * @param writer ???????????????
 * @return ???
 */
void H_Writer_Unlock(H_Writer* writer){

  H_TS_LockUnlock(writer->WriteLock);
}



