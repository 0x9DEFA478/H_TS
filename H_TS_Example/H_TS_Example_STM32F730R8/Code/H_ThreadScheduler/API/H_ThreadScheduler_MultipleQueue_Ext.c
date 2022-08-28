/*
 * @Author: 0x9DEFA478
 * @Date: 2021-08-19 19:00:44
 * @LastEditTime: 2021-10-24 18:40:37
 * @LastEditors: 0x9DEFA478
 * @Description: 实现说明详见H_ThreadScheduler_MultipleQueue.c
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
#include "H_ThreadScheduler_Core.h"
#include "..\LL\H_ThreadScheduler_LL.h"

extern H_TS* volatile H_TS_Core;



static void** H_TS_CreateObjects(void* (*Create)(int),void (*Delete)(void*),int* Param_Array,int Num){
  void** r;
  int i;

  r=H_TS_Core->Memory.Malloc(Num*sizeof(void*));
  if(r==NULL){
    return NULL;
  }

  for(i=0;i<Num;i++){
    r[i]=Create(Param_Array[i]);
    if(r[i]==NULL){
      while(i>0){
        i--;
        Delete(r[i]);
      }
      H_TS_Core->Memory.Free(r);
      return NULL;
    }
  }

  return r;
}

static void H_TS_DeleteObjects(void (*Delete)(void*),void** ObjectArray,int Num){
  while(Num>0){
    Num--;
    Delete(ObjectArray[Num]);
  }
  H_TS_Core->Memory.Free(ObjectArray);
}




/**
 * @brief 新建一个复合队列
 * @param MaxCount_Array 一个数组, 表示每一个信号量的最大计数值
 * @param NumOfSemaphores 信号量数量
 * @param NumOfMessages_Array 一个数组, 表示每一个消息队列容量大小(最多只能添加到(容量-1)个消息)
 * @param NumOfMessageFIFOs 消息队列数量
 * @return 复合队列对象 如果为NULL 则失败
 */
H_TS_MultipleQueue* new_H_TS_MultipleQueue(int* MaxCount_Array,int NumOfSemaphores,int* NumOfMessages_Array,int NumOfMessageFIFOs){
  H_TS_MultipleQueue* r;

  oH_TS_SchedulerSuspend();

  r=H_TS_Core->Memory.Malloc(sizeof(H_TS_MultipleQueue));
  if(r==NULL){
    oH_TS_ResumeSchedulerAuto();
    return NULL;
  }

  r->Semaphore_List.Semaphores=NULL;
  r->MessageQueue_List.MessageFIFOs=NULL;

  if(NumOfSemaphores>0){
    r->Semaphore_List.Semaphores=H_TS_CreateObjects(H_TS_Semaphore_Create,H_TS_Semaphore_Delete,MaxCount_Array,NumOfSemaphores);
    if(r->Semaphore_List.Semaphores==NULL){
      H_TS_Core->Memory.Free(r);
      oH_TS_ResumeSchedulerAuto();
      return NULL;
    }
  }

  if(NumOfMessageFIFOs>0){
    r->MessageQueue_List.MessageFIFOs=H_TS_CreateObjects(H_TS_MessageFIFO_Create,H_TS_MessageFIFO_Delete,NumOfMessages_Array,NumOfMessageFIFOs);
    if(r->MessageQueue_List.MessageFIFOs==NULL){
      if(r->Semaphore_List.Semaphores!=NULL){
        H_TS_DeleteObjects(H_TS_Semaphore_Delete,r->Semaphore_List.Semaphores,NumOfSemaphores);
      }
      H_TS_Core->Memory.Free(r);
      oH_TS_ResumeSchedulerAuto();
      return NULL;
    }
  }

  r->Semaphore_List.NumOfSemaphores=NumOfSemaphores;
  r->Semaphore_List.ReEnter=0;
  r->MessageQueue_List.NumOfMessageFIFOs=NumOfMessageFIFOs;
  r->MessageQueue_List.ReEnter=0;
  r->Owner=NULL;

  oH_TS_ResumeSchedulerAuto();

  return r;
}

/**
 * @brief 删除一个复合队列
 * @param MultipleQueue 要删除的复合队列对象
 * @return 0:成功 -1:有线程因此复合队列阻塞
 */
int delete_H_TS_MultipleQueue(H_TS_MultipleQueue* MultipleQueue){

  oH_TS_SchedulerSuspend();

  if(MultipleQueue->Owner!=NULL){
    oH_TS_ResumeSchedulerAuto();
    return -1;
  }

  if(MultipleQueue->Semaphore_List.NumOfSemaphores>0){
    H_TS_DeleteObjects(H_TS_Semaphore_Delete,MultipleQueue->Semaphore_List.Semaphores,MultipleQueue->Semaphore_List.NumOfSemaphores);
  }
  if(MultipleQueue->MessageQueue_List.NumOfMessageFIFOs>0){
    H_TS_DeleteObjects(H_TS_MessageFIFO_Delete,MultipleQueue->MessageQueue_List.MessageFIFOs,MultipleQueue->MessageQueue_List.NumOfMessageFIFOs);
  }
  H_TS_Core->Memory.Free(MultipleQueue);

  oH_TS_ResumeSchedulerAuto();

  return 0;
}

