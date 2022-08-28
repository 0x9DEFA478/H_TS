/*
 * @Author: 0x9DEFA478
 * @Date: 2021-08-19 19:00:44
 * @LastEditTime: 2021-10-24 21:36:32
 * @LastEditors: 0x9DEFA478
 * @Description: 一个特殊的部件 MultipleQueue将MessageQueue与Semaphore合并在一起
 *               可以一定程度上解决一个MessageQueue或Semaphore只能同时接收一个中断的局限
 *               一个MultipleQueue中可用有多个MessageQueue和多个Semaphore (新建MultipleQueue时 这些数量将被确定)
 *               线程因MultipleQueue阻塞时，只要其中一个MessageQueue或Semaphore接受了数据(有信号量或队列不为空)，线程将解阻塞，并且将得知是那些组件获得了数据
 *               一个MessageQueue或Semaphore只能同时接收一个中断的数据，所以多个中断操作MultipleQueue时，多个中断不能操作同一个MessageQueue或Semaphore
 *               这样一个线程若要接收多个中断的信息时，只需要创建一个MultipleQueue 不同的中断向MultipleQueue中的不同MessageQueue或Semaphore发送数据，从而不需要为每个中断都创建一个线程
 * 
 *               与MessageQueue或Semaphore不同的是 MultipleQueue的释放信号量和向队列添加数据的方法不区分中断与线程 都是使用同一个方法(每个线程都视为不同的中断)
 *               H_TS不保证MultipleQueue内部的MessageQueue或Semaphore之间的先后顺序(但同一个队列或信号量内部的顺序是保证的)
 *               (一个例子，2队列接收到了消息，1队列后接收到消息，但接收线程可能先得到1队列的消息)
 *               (如果MultipleQueue内部有多个信号量可接收/多个队列有数据, 会优先读取Semaphore 优先读取id小的)
 * 
 *               更新增加:
 *                 增加H_TS_MultipleQueueGive_ReEnter()和H_TS_MultipleQueueSend_ReEnter()方法
 *                 分别对应H_TS_MultipleQueueGive()和H_TS_MultipleQueueSend().
 *               区别是:
 *                 _ReEnter结尾的可以不用指定Id, 由其内部进行重入计数并选择Id
 *               注意:
 *                 对于同一个消息/信号量, 带_ReEnter与不带的不能混用
 *                 例如: 对于H_TS_MultipleQueue* MultipleQueue
 *                   1. 对MultipleQueue使用H_TS_MultipleQueueGive_ReEnter和H_TS_MultipleQueueSend_ReEnter
 *                   2. 对MultipleQueue使用H_TS_MultipleQueueGive和H_TS_MultipleQueueSend
 *                   3. 对MultipleQueue使用H_TS_MultipleQueueGive_ReEnter和H_TS_MultipleQueueSend
 *                   4. 对MultipleQueue使用H_TS_MultipleQueueGive和H_TS_MultipleQueueSend_ReEnter
 *                 以上的情况都可正常运行, 除这些情况外, 后果不可预料。
 *                   例如:
 *                     对MultipleQueue使用H_TS_MultipleQueueGive和H_TS_MultipleQueueGive_ReEnter和H_TS_MultipleQueueSend
 *                    H_TS_MultipleQueueGive与H_TS_MultipleQueueGive_ReEnter是冲突的, 对于Send也是类似的
 *                 _ReEnter被调用时, 低Id的 信号量/队列 优先被写入, 如果发生重入抢占, 则抢占者将从更高的Id写入
 *                 因此, 即使如此, H_TS_MultipleQueue内部的MessageQueue/Semaphore的数量要足够, 至少为 要释放信号量/发送消息队列 的 线程/中断 的数量
 *                   否则, 抢占者可能写入失败。
 * 
 *                 由于这些特性, H_TS_MultipleQueue所能容纳的 消息/信号量 的最大数量是不确定的。但最小数量是可以确定的。
 *                  为 (消息/信号量个数-1)+(消息/信号量容纳数量的最小值)
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




static int H_TS_MultipleQueue_IsNoEmpty(H_TS_MultipleQueue* MultipleQueue){
  int i;

  for(i=0;i<MultipleQueue->Semaphore_List.NumOfSemaphores;i++){
    if(0!=H_TS_Semaphore_IsNoEmpty(MultipleQueue->Semaphore_List.Semaphores[i])){
      
      return -1;
    }
  }

  for(i=0;i<MultipleQueue->MessageQueue_List.NumOfMessageFIFOs;i++){
    if(0!=H_TS_MessageFIFO_IsNoEmpty(MultipleQueue->MessageQueue_List.MessageFIFOs[i])){
      return -1;
    }
  }

  return 0;
}




static void MultipleQueue_RequestReady(H_TS_MultipleQueue* MultipleQueue){
  H_TS_Thread* Owner;

  Owner=MultipleQueue->Owner;
  if(Owner!=NULL){
    //有线程正在等待消息队列

    switch(Owner->Status){
      case vH_TS_Thread_Status_Block:

        Owner->Status=vH_TS_Thread_Status_Ready;//线程就绪
        MultipleQueue->Owner=NULL;//清除占用标记

        //让PendSV处理链表

        H_TS_Core->BlockThreadsEvents[Owner->List.Block.ListId]=-1;
        H_TS_Core->BlockThreadsEvent=-1;

        oH_TS_ISR_CallPendSV();

        break;
      case vH_TS_Thread_Status_TimeBlock:

        Owner->Status=vH_TS_Thread_Status_Ready;//线程就绪
        MultipleQueue->Owner=NULL;//清除占用标记

        //让PendSV处理链表
        
        H_TS_Core->TimeBlockThreadsEvent=-1;

        oH_TS_ISR_CallPendSV();

        break;
      case vH_TS_Thread_Status_Ready:
        //这是重入导致的 虽然会导致一些重复操作 但不影响结果
        //直接返回即可
        break;

      default:
        for(;;){
          //H_TS出BUG了!!
        }
    }
  }
}

/**
 * @brief 向复合队列释放信号量 中断与线程均可调用(不同的线程等同于不同的中断 每个线程访问的ID不能相同)
 * @param MultipleQueue 要操作的复合队列
 * @param SemaphoreIndex 信号量ID
 * @return 0:成功 -1:信号量已满
 */
int H_TS_MultipleQueueGive(H_TS_MultipleQueue* MultipleQueue,int SemaphoreIndex){
  
  if(0!=H_TS_Semaphore_Add(MultipleQueue->Semaphore_List.Semaphores[SemaphoreIndex])){
    return -1;
  }

  MultipleQueue_RequestReady(MultipleQueue);

  return 0;
}

/**
 * @brief 向复合队列释放信号量 中断与线程均可调用(不同的线程等同于不同的中断 每个线程访问的ID不能相同)
 *        此方法无需输入ID 内部有重入统计自动选择ID
 *        对同一个MultipleQueue, 此方法不能与H_TS_MultipleQueueGive混用
 * @param MultipleQueue 要操作的复合队列
 * @return 0:成功 -1:信号量已满
 */
int H_TS_MultipleQueueGive_ReEnter(H_TS_MultipleQueue* MultipleQueue){
  int Result;
  int SemaphoreIndex;
  int SemaphoreNextIndex;
  int AddCnt;
  int NumOfSemaphores;

  Result=-1;
  AddCnt=0;
  NumOfSemaphores=MultipleQueue->Semaphore_List.NumOfSemaphores;

  SemaphoreIndex=MultipleQueue->Semaphore_List.ReEnter;//ReEnter每次调用后都会复原, 无需线程(中断)保护. 因为即使被抢占了, 抢占的线程(中断)在退出之前会将ReEnter复原
  if(SemaphoreIndex<NumOfSemaphores){
    do{

      SemaphoreNextIndex=SemaphoreIndex+1;
      MultipleQueue->Semaphore_List.ReEnter=SemaphoreNextIndex;//标记占用
      AddCnt++;

      //尝试释放信号量
      if(0==H_TS_Semaphore_Add(MultipleQueue->Semaphore_List.Semaphores[SemaphoreIndex])){
        Result=0;
      }

      SemaphoreIndex=SemaphoreNextIndex;

    }while((Result!=0)&&(SemaphoreIndex<NumOfSemaphores));//如果信号量释放成功或者所有信号量满退出循环
    
    SemaphoreNextIndex-=AddCnt;
    MultipleQueue->Semaphore_List.ReEnter=SemaphoreNextIndex;

    if(Result==0){
      //写入成功

      MultipleQueue_RequestReady(MultipleQueue);
    }
  }
  
  return Result;
}

/**
 * @brief 向复合队列发送消息 由中断线程均可调用(不同的线程等同于不同的中断 每个线程访问的ID不能相同)
 * @param MultipleQueue 要操作的复合队列
 * @param MessageQueueIndex 消息队列ID
 * @param v0 消息指针0
 * @param v1 消息指针1
 * @return 0:成功 -1:队列已满
 */
int H_TS_MultipleQueueSend(H_TS_MultipleQueue* MultipleQueue,int MessageQueueIndex,void* v0,void* v1){
  
  if(0!=H_TS_MessageFIFO_Write(MultipleQueue->MessageQueue_List.MessageFIFOs[MessageQueueIndex],v0,v1)){
    return -1;
  }

  MultipleQueue_RequestReady(MultipleQueue);

  return 0;
}

/**
 * @brief 向复合队列发送消息 由中断线程均可调用(不同的线程等同于不同的中断 每个线程访问的ID不能相同)
 *        此方法无需输入ID 内部有重入统计自动选择ID
 *        对同一个MultipleQueue, 此方法不能与H_TS_MultipleQueueSend混用
 * @param MultipleQueue 要操作的复合队列
 * @param v0 消息指针0
 * @param v1 消息指针1
 * @return 0:成功 -1:队列已满
 */
int H_TS_MultipleQueueSend_ReEnter(H_TS_MultipleQueue* MultipleQueue,void* v0,void* v1){
  int Result;
  int MessageQueueIndex;
  int MessageQueueNextIndex;
  int AddCnt;
  int NumOfMessageFIFOs;

  Result=-1;
  AddCnt=0;
  NumOfMessageFIFOs=MultipleQueue->MessageQueue_List.NumOfMessageFIFOs;

  MessageQueueIndex=MultipleQueue->MessageQueue_List.ReEnter;//ReEnter每次调用后都会复原, 无需线程(中断)保护. 因为即使被抢占了, 抢占的线程(中断)在退出之前会将ReEnter复原
  
  if(MessageQueueIndex<NumOfMessageFIFOs){
    do{

      MessageQueueNextIndex=MessageQueueIndex+1;
      MultipleQueue->MessageQueue_List.ReEnter=MessageQueueNextIndex;//标记占用
      AddCnt++;

      //尝试发送消息
      if(0==H_TS_MessageFIFO_Write(MultipleQueue->MessageQueue_List.MessageFIFOs[MessageQueueIndex],v0,v1)){
        Result=0;
      }

      MessageQueueIndex=MessageQueueNextIndex;

    }while((Result!=0)&&(MessageQueueIndex<NumOfMessageFIFOs));//如果消息发送成功或者所有消息满退出循环

    MessageQueueNextIndex-=AddCnt;
    MultipleQueue->MessageQueue_List.ReEnter=MessageQueueNextIndex;

    if(Result==0){
      //写入成功

      MultipleQueue_RequestReady(MultipleQueue);
    }
  }
  
  return Result;
}

/**
 * @brief 从复合队列中接收信号量/读取消息
 * @param MultipleQueue 要操作的复合队列
 * @param Param 读取参数指针, 用于返回读取到的数据
 * @return 如果无消息 将一直阻塞 直到获取成功
 */
void H_TS_MultipleQueueTakeReceive(H_TS_MultipleQueue* MultipleQueue,H_TS_MultipleQueueTakeReceive_Param* Param){
  H_TS_Thread* _this;
  H_TS_Thread* p_last;
  H_TS_Thread* p_next;
  
  _this=H_TS_Core->RunThreadPtr[0][1];

  //由于线程与中断都能解阻塞H_TS_MultipleQueue 中断会在线程写入FIFO之后,触发调度之前触发调度,使线程触发调度时FIFO内已经无数据 这里要循环阻塞 直到读取成功
  while(0!=H_TS_MultipleQueueTryTakeReceive(MultipleQueue,Param)){

    //无信号量 开始阻塞

    oH_TS_SchedulerSuspend();

    _this->Status=vH_TS_Thread_Status_Block;
    _this->BlockObject.Block.BlockObject=(void*)&MultipleQueue->Owner;

    //从就绪链表里移除
    p_last=_this->List.Ready.Last;
    p_next=_this->List.Ready.Next;
    if(p_last==NULL){
      H_TS_Core->ReadyThreads=p_next;
      while(p_next==NULL){
        //至少存在一个空闲线程 H_TS_Core->ReadyThreads不会为NULL
      }
    }else{
      p_last->List.Ready.Next=p_next;
    }
    if(p_next!=NULL){
      p_next->List.Ready.Last=p_last;
    }

    //添加到阻塞链表
    H_TS_ThreadAddToBlockList(_this);

    //标记信号量拥有线程
    MultipleQueue->Owner=_this;

    if(0!=H_TS_MultipleQueue_IsNoEmpty(MultipleQueue)){
      if(MultipleQueue->Owner!=NULL){
        //在修改链表期间有信号量释放 中断不会触发PendSV
        
        _this->Status=vH_TS_Thread_Status_Ready;//线程就绪
        MultipleQueue->Owner=NULL;//清除占用标记

        //让PendSV处理链表

        H_TS_Core->BlockThreadsEvents[_this->List.Block.ListId]=-1;
        H_TS_Core->BlockThreadsEvent=-1;

      }
    }

    oH_TS_ResumeScheduler();//允许线程调度

  }
  
}

/**
 * @brief 尝试从队列中读取消息/获取信号量
 * @param MultipleQueue 要操作的复合队列
 * @param Param 读取参数指针, 用于返回读取到的数据
 * @return 0:成功 其他:获取失败
 */
int H_TS_MultipleQueueTryTakeReceive(H_TS_MultipleQueue* MultipleQueue,H_TS_MultipleQueueTakeReceive_Param* Param){
  int i;

  for(i=0;i<MultipleQueue->Semaphore_List.NumOfSemaphores;i++){
    if(0==H_TS_Semaphore_Sub(MultipleQueue->Semaphore_List.Semaphores[i])){
      Param->IsMessage=0;
      return 0;
    }
  }

  for(i=0;i<MultipleQueue->MessageQueue_List.NumOfMessageFIFOs;i++){
    if(0==H_TS_MessageFIFO_Read(MultipleQueue->MessageQueue_List.MessageFIFOs[i],&Param->v0,&Param->v1)){
      Param->IsMessage=-1;
      return 0;
    }
  }

  return -1;
}

/**
 * @brief 从队列中读取消息 时间限制
 * @param MultipleQueue 要操作的复合队列
 * @param Param 读取参数指针, 用于返回读取到的数据
 * @param timeOut 最大等待时间
 * @return 0:成功 其他:获取失败
 */
int H_TS_MultipleQueueTakeReceive_Time(H_TS_MultipleQueue* MultipleQueue,H_TS_MultipleQueueTakeReceive_Param* Param,H_TS_Tick timeOut){
  H_TS_Thread* _this;
  H_TS_Thread* p_last;
  H_TS_Thread* p_next;

  _this=H_TS_Core->RunThreadPtr[0][1];

  if(0==H_TS_MultipleQueueTryTakeReceive(MultipleQueue,Param)){
    return 0;
  }

  //无信号量 开始阻塞

  oH_TS_SchedulerSuspend();

  _this->Status=vH_TS_Thread_Status_TimeBlock;
  _this->BlockObject.TimeBlock.BlockObject=(void*)&MultipleQueue->Owner;

  //从就绪链表里移除
  p_last=_this->List.Ready.Last;
  p_next=_this->List.Ready.Next;
  if(p_last==NULL){
    H_TS_Core->ReadyThreads=p_next;
    while(p_next==NULL){
      //至少存在一个空闲线程 H_TS_Core->ReadyThreads不会为NULL
    }
  }else{
    p_last->List.Ready.Next=p_next;
  }
  if(p_next!=NULL){
    p_next->List.Ready.Last=p_last;
  }

  //添加到阻塞链表
  H_TS_ThreadAddToTimeBlockList(_this,timeOut);

  //标记消息队列拥有线程
  MultipleQueue->Owner=_this;

  if(0!=H_TS_MultipleQueue_IsNoEmpty(MultipleQueue)){
    if(MultipleQueue->Owner!=NULL){
      //在修改链表期间有信号量释放 中断不会触发PendSV
      
      _this->Status=vH_TS_Thread_Status_Ready;//线程就绪
      MultipleQueue->Owner=NULL;//清除占用标记

      //让PendSV处理链表
      H_TS_Core->TimeBlockThreadsEvent=-1;
      
    }
  }

  oH_TS_ResumeScheduler();//允许线程调度

  if(0!=H_TS_MultipleQueueTryTakeReceive(MultipleQueue,Param)){
    //超时
    return -1;
  }

  return 0;
}

