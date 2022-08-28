/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-03 14:30:17
 * @LastEditTime: 2021-09-15 09:56:56
 * @LastEditors: 0x9DEFA478
 * @Description: 定义了一些链表方法 由H_TS内部调用
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

//此段指向H_TS核心句柄 全局只有这一个，H_TS不能实例化(或只能有一个) 其他文件通过extern引用
H_TS* volatile H_TS_Core;



/**
 * @brief 信号量增加
 * @param Semaphore 要增加的信号量
 * @return 0:成功 其他:失败
 */
int H_TS_Semaphore_Add(struct _H_TS_Semaphore* Semaphore){
  int I;
  int O;
  int Empty;

  I=Semaphore->I;
  O=Semaphore->O;

  if(I<O){
    Empty=O-I;
  }else{
    Empty=Semaphore->Max+1-I+O;
  }

  if(Empty<2){
    return -1;
  }

  if(I==Semaphore->Max){
    Semaphore->I=0;
  }else{
    Semaphore->I=I+1;
  }

  return 0;
}

/**
 * @brief 信号量减少
 * @param Semaphore 要减少的信号量
 * @return 0:成功 其他:失败
 */
int H_TS_Semaphore_Sub(struct _H_TS_Semaphore* Semaphore){
  int I;
  int O;

  I=Semaphore->I;
  O=Semaphore->O;
  if(I!=O){
    //有信号量

    if(O==Semaphore->Max){
      Semaphore->O=0;
    }else{
      Semaphore->O=O+1;
    }
    return 0;
  }

  return -1;
}

/**
 * @brief Semaphore是否大于0
 * @param Semaphore 信号量
 * @return 0:空 其他:不为空
 */
int H_TS_Semaphore_IsNoEmpty(struct _H_TS_Semaphore* Semaphore){
  if(Semaphore->I!=Semaphore->O){
    return -1;
  }
  return 0;
}




/**
 * @brief 消息写入
 * @param MessageFIFO 消息FIFO
 * @return 0:成功 其他:失败
 */
int H_TS_MessageFIFO_Write(struct _H_TS_MessageFIFO* MessageFIFO,void* v0,void* v1){
  int I;
  int O;
  int Empty;
  void* volatile * Messages;

  I=MessageFIFO->I;
  O=MessageFIFO->O;

  if(I<O){
    Empty=O-I;
  }else{
    Empty=MessageFIFO->NumOfMessages-I+O;
  }

  if(Empty<2){
    return -1;
  }

  Messages=&MessageFIFO->Messages[I*2];
  Messages[0]=v0;
  Messages[1]=v1;

  if(I==(MessageFIFO->NumOfMessages-1)){
    MessageFIFO->I=0;
  }else{
    MessageFIFO->I=I+1;
  }

  return 0;
}

/**
 * @brief 读取消息
 * @param MessageFIFO 消息FIFO
 * @return 0:成功 其他:失败
 */
int H_TS_MessageFIFO_Read(struct _H_TS_MessageFIFO* MessageFIFO,void** v0,void** v1){
  int I;
  int O;
  void* volatile * Messages;

  I=MessageFIFO->I;
  O=MessageFIFO->O;
  if(I!=O){
    //有信号量

    Messages=&MessageFIFO->Messages[O*2];
    v0[0]=Messages[0];
    v1[0]=Messages[1];

    if(O==(MessageFIFO->NumOfMessages-1)){
      MessageFIFO->O=0;
    }else{
      MessageFIFO->O=O+1;
    }
    return 0;
  }

  return -1;
}

/**
 * @brief MessageFIFO是否有消息
 * @param MessageFIFO 消息FIFO
 * @return 0:空 其他:不为空
 */
int H_TS_MessageFIFO_IsNoEmpty(struct _H_TS_MessageFIFO* MessageFIFO){
  if(MessageFIFO->I!=MessageFIFO->O){
    return -1;
  }
  return 0;
}




/**
 * @brief 添加线程到就绪链表 
 * @param Thread 要添加到的线程
 * @return 0:添加到非头部 其他:添加到头部
 */
int H_TS_ThreadAddToReadyList(H_TS_Thread* Thread){

  H_TS_Thread* p_next;
  H_TS_Thread* p_next_next;
  H_TS_Thread* p_last;

  p_next=H_TS_Core->ReadyThreads;

  while(p_next==NULL){
    //不可能出现无就绪线程的情况(至少存在一个空闲线程)
  }

  if(p_next->Priority > Thread->Priority){

    Thread->List.Ready.Last=NULL;
    Thread->List.Ready.Next=p_next;
    p_next->List.Ready.Last=Thread;
    H_TS_Core->ReadyThreads=Thread;

    return -1;

  }else{

    p_next_next=p_next->List.Ready.Next;
    while(p_next_next!=NULL){
      p_next=p_next_next;

      if(p_next->Priority > Thread->Priority){
        //找到插入点

        p_last=p_next->List.Ready.Last;

        Thread->List.Ready.Next=p_next;
        Thread->List.Ready.Last=p_last;
        p_last->List.Ready.Next=Thread;
        p_next->List.Ready.Last=Thread;

        return 0;
      }

      p_next_next=p_next->List.Ready.Next;
    }
    //无插入点 添加到尾部

    Thread->List.Ready.Next=NULL;
    Thread->List.Ready.Last=p_next;
    p_next->List.Ready.Next=Thread;
  }

  return 0;
}

/**
 * @brief 添加线程到阻塞(Block)链表
 * @param Thread 要添加的线程
 * @return 无
 */
void H_TS_ThreadAddToBlockList(H_TS_Thread* Thread){

  H_TS_Thread* p;
  int ListId;
  int lastCnt;
  int newCnt;
  int i;

  //寻找占用最少的链表
  ListId=0;
  lastCnt=H_TS_Core->BlockThreadsItemCnt[0];
  for(i=1;i<vH_TS_BlockThreadListNum;i++){
    newCnt=H_TS_Core->BlockThreadsItemCnt[i];
    if(newCnt<lastCnt){
      lastCnt=newCnt;
      ListId=i;
    }
  }

  p=H_TS_Core->BlockThreads[ListId];
  Thread->List.Block.ListId=ListId;
  Thread->List.Block.Next=p;
  Thread->List.Block.Last=NULL;
  if(p!=NULL){
    p->List.Block.Last=Thread;
  }
  

  H_TS_Core->BlockThreads[ListId]=Thread;

  H_TS_Core->BlockThreadsItemCnt[ListId]=lastCnt+1;

}

/**
 * @brief 将线程添加到睡眠(Sleep)链表
 * @param Thread 要添加的线程
 * @param NumOfTick 睡眠tick数
 * @return 无
 */
void H_TS_ThreadAddToSleepList(H_TS_Thread* Thread,H_TS_Tick NumOfTick){
  H_TS_Thread* p_last;
  H_TS_Thread* p_next;
  H_TS_Tick t_tick;

  p_last=H_TS_Core->SleepThreads;
  if(p_last==NULL){
    //链表空 直接添加
    Thread->BlockObject.SleepTick=NumOfTick;
    Thread->List.Sleep.Next=NULL;
    Thread->List.Sleep.Last=NULL;
    H_TS_Core->SleepThreads=Thread;
  }else{
    //链表不为空 按解除阻塞顺序插入

    p_next=H_TS_Core->SleepThreads;
    p_last=NULL;

    while(p_next!=NULL){

      t_tick=p_next->BlockObject.SleepTick;

      if(t_tick > NumOfTick){
        //找到插入点

        Thread->BlockObject.SleepTick=NumOfTick;
        p_next->BlockObject.SleepTick=t_tick-NumOfTick;

        //插入节点
        Thread->List.Sleep.Last=p_last;
        Thread->List.Sleep.Next=p_next;
        p_next->List.Sleep.Last=Thread;
        if(p_last==NULL){
          H_TS_Core->SleepThreads=Thread;
        }else{
          p_last->List.Sleep.Next=Thread;
        }

        return;
      }else{
        NumOfTick-=t_tick;
      }
      
      p_last=p_next;
      p_next=p_last->List.Sleep.Next;
    }

    Thread->BlockObject.SleepTick=NumOfTick;

    //仍未找到插入点,插入到最后
    //此时p_last不可能为NULL
    Thread->List.Sleep.Last=p_last;
    Thread->List.Sleep.Next=NULL;
    p_last->List.Sleep.Next=Thread;
  }
}

/**
 * @brief 将线程添加到时间限制阻塞(TimeBlock)链表
 * @param Thread 要添加的线程
 * @param timeOut 最大等待时间
 * @return 无
 */
void H_TS_ThreadAddToTimeBlockList(H_TS_Thread* Thread,H_TS_Tick timeOut){
  H_TS_Thread* p_last;
  H_TS_Thread* p_next;
  H_TS_Tick t_tick;

  p_last=H_TS_Core->TimeBlockThreads;
  if(p_last==NULL){
    //链表空 直接添加
    Thread->BlockObject.TimeBlock.SleepTick=timeOut;
    Thread->List.TimeBlock.Next=NULL;
    Thread->List.TimeBlock.Last=NULL;
    H_TS_Core->TimeBlockThreads=Thread;
  }else{
    //链表不为空 按解除阻塞顺序插入

    p_next=H_TS_Core->TimeBlockThreads;
    p_last=NULL;

    while(p_next!=NULL){

      t_tick=p_next->BlockObject.TimeBlock.SleepTick;

      if(t_tick > timeOut){
        //找到插入点

        Thread->BlockObject.TimeBlock.SleepTick=timeOut;
        p_next->BlockObject.TimeBlock.SleepTick=t_tick-timeOut;

        //插入节点
        Thread->List.TimeBlock.Last=p_last;
        Thread->List.TimeBlock.Next=p_next;
        p_next->List.TimeBlock.Last=Thread;
        if(p_last==NULL){
          H_TS_Core->TimeBlockThreads=Thread;
        }else{
          p_last->List.TimeBlock.Next=Thread;
        }

        return;
      }else{
        timeOut-=t_tick;
      }
      
      p_last=p_next;
      p_next=p_last->List.TimeBlock.Next;
    }

    Thread->BlockObject.TimeBlock.SleepTick=timeOut;

    //仍未找到插入点,插入到最后
    //此时p_last不可能为NULL
    Thread->List.TimeBlock.Last=p_last;
    Thread->List.TimeBlock.Next=NULL;
    p_last->List.TimeBlock.Next=Thread;
  }
}

