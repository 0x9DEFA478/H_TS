/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-03 14:30:17
 * @LastEditTime: 2021-12-31 22:39:46
 * @LastEditors: 0x9DEFA478
 * @Description: 定义了一些链表方法和FIFO操作 由H_TS内部调用
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
#include "./H_ThreadScheduler_Core.h"




H_TS* H_TS_Core;//此段指向H_TS核心句柄 全局只有这一个 其他文件通过extern引用




/**
 * @brief 信号量增加
 * @param Semaphore 要增加的信号量
 * @return 0:成功 其他:失败
 */
int H_TS_Semaphore_Add(struct _H_TS_Semaphore* Semaphore){
  Hsize I;
  Hsize O;
  Hsize Empty;

  I=Semaphore->I;
  O=Semaphore->O;

  if(I<O){
    Empty=O-I;
  }else{
    Empty=Semaphore->Max+1-I+O;
  }

  if(Empty<=1){
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
  Hsize I;
  Hsize O;

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
 * @brief 消息写入
 * @param MessageFIFO 消息FIFO
 * @param v0 指针0
 * @param v1 指针1
 * @return 0:成功 其他:失败
 */
int H_TS_MessageFIFO_Write(struct _H_TS_MessageFIFO* MessageFIFO,void* v0,void* v1){
  Hsize I;
  Hsize O;
  Hsize Empty;
  void* volatile * Messages;

  I=MessageFIFO->I;
  O=MessageFIFO->O;

  if(I<O){
    Empty=O-I;
  }else{
    Empty=MessageFIFO->NumOfMessages-I+O;
  }

  if(Empty<=1){
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
 * @param v0 用于接收指针0
 * @param v1 用于接收指针1
 * @return 0:成功 其他:失败
 */
int H_TS_MessageFIFO_Read(struct _H_TS_MessageFIFO* MessageFIFO,void** v0,void** v1){
  Hsize I;
  Hsize O;
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
 * @brief 数据写入
 * @param Mailbox 邮箱
 * @param Data 数据
 * @return 0:成功 其他:失败
 */
int H_TS_Mailbox_Post(struct _H_TS_Mailbox* Mailbox,void* Data){
  Hsize I;
  Hsize O;
  Hsize Empty;
  volatile Hbyte* Mails;
  Hbyte* src;
  Hsize i;

  I=Mailbox->I;
  O=Mailbox->O;

  if(I<O){
    Empty=O-I;
  }else{
    Empty=Mailbox->Max-I+O;
  }

  if(Empty<=1){
    return -1;
  }

  src=Data;
  i=Mailbox->MailSize;
  Mails=&Mailbox->Mails[I*i];
  while(i>7){
    i-=8;
    
    Mails[0]=src[0];
    Mails[1]=src[1];
    Mails[2]=src[2];
    Mails[3]=src[3];
    Mails[4]=src[4];
    Mails[5]=src[5];
    Mails[6]=src[6];
    Mails[7]=src[7];

    src=&src[8];
    Mails=&Mails[8];
  }
  while(i>1){
    i-=2;
    
    Mails[0]=src[0];
    Mails[1]=src[1];

    src=&src[2];
    Mails=&Mails[2];
  }
  if(i>0){
    i--;
    
    Mails[0]=src[0];

    src=&src[1];
    Mails=&Mails[1];
  }

  if(I==(Mailbox->Max-1)){
    Mailbox->I=0;
  }else{
    Mailbox->I=I+1;
  }

  return 0;
}

/**
 * @brief 读取数据
 * @param Mailbox 邮箱
 * @param Data 指向容纳数据的内存
 * @return 0:成功 其他:失败
 */
int H_TS_Mailbox_Receive(struct _H_TS_Mailbox* Mailbox,void* Data){
  Hsize I;
  Hsize O;
  volatile Hbyte* Mails;
  Hbyte* src;
  Hsize i;

  I=Mailbox->I;
  O=Mailbox->O;

  if(I!=O){
    //有信号量

    src=Data;
    i=Mailbox->MailSize;
    Mails=&Mailbox->Mails[O*i];
    while(i>7){
      i-=8;
      
      src[0]=Mails[0];
      src[1]=Mails[1];
      src[2]=Mails[2];
      src[3]=Mails[3];
      src[4]=Mails[4];
      src[5]=Mails[5];
      src[6]=Mails[6];
      src[7]=Mails[7];

      src=&src[8];
      Mails=&Mails[8];
    }
    while(i>1){
      i-=2;
      
      src[0]=Mails[0];
      src[1]=Mails[1];

      src=&src[2];
      Mails=&Mails[2];
    }
    if(i>0){
      i--;
      
      src[0]=Mails[0];

      src=&src[1];
      Mails=&Mails[1];
    }

    if(O==(Mailbox->Max-1)){
      Mailbox->O=0;
    }else{
      Mailbox->O=O+1;
    }
    return 0;
  }

  return -1;
}




/**
 * @brief 固定添加线程链表到就绪链表
 * @param Thread 要添加的线程链表
 * @return 无
 */
void H_TS_ThreadListForceAddToReadyList(H_TS_Thread* Thread){
  H_TS_Thread* Thread_Next;
  H_TS_Thread* p_next;
  H_TS_Thread* p_last;
  H_TS_Thread* ReadyThreads;
  Hsize Priority;

  ReadyThreads=H_TS_Core->ReadyThreads;

  while(Thread!=NULL){
    Thread->List.Ready.IsSubReady=0;
    Thread_Next=Thread->List.Ready.Next;

    p_next=ReadyThreads;

    oH_TS_DebugAssert(p_next==NULL,"不可能出现无就绪线程的情况(至少存在一个空闲线程)");

    Priority=Thread->Priority;
    if(p_next->Priority<Priority){
      //优先级不是当前就绪最高的

      p_next=p_next->List.Ready.Next;
      while(p_next!=NULL){

        if(p_next->Priority>=Priority){
          //找到插入点

          p_last=p_next->List.Ready.Last;

          Thread->List.Ready.Next=p_next;
          Thread->List.Ready.Last=p_last;
          p_last->List.Ready.Next=Thread;
          p_next->List.Ready.Last=Thread;

          goto Continue;
        }

        p_next=p_next->List.Ready.Next;
      }
      //无插入点 添加到尾部

#if vH_TS_EnableDebug != 0
      oH_TS_DebugAssert(1,"由于空闲线程的存在 不可能无插入点");
#else
      for(;;){
      }
#endif
    }else{
      //高优先级 直接添加到头部

      Thread->List.Ready.Last=NULL;
      Thread->List.Ready.Next=p_next;
      p_next->List.Ready.Last=Thread;
      ReadyThreads=Thread;
    }

Continue:
    Thread=Thread_Next;
  }

  H_TS_Core->ReadyThreads=ReadyThreads;

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

      if(t_tick>NumOfTick){
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
 * @param Timeout 最大等待时间
 * @return 无
 */
void H_TS_ThreadAddToTimeBlockList(H_TS_Thread* Thread,H_TS_Tick Timeout){
  H_TS_Thread* p_last;
  H_TS_Thread* p_next;
  H_TS_Tick t_tick;

  p_last=H_TS_Core->TimeBlockThreads;
  if(p_last==NULL){
    //链表空 直接添加
    Thread->BlockObject.TimeBlock.SleepTick=Timeout;
    Thread->List.TimeBlock.Next=NULL;
    Thread->List.TimeBlock.Last=NULL;
    H_TS_Core->TimeBlockThreads=Thread;
  }else{
    //链表不为空 按解除阻塞顺序插入

    p_next=H_TS_Core->TimeBlockThreads;
    p_last=NULL;

    while(p_next!=NULL){

      t_tick=p_next->BlockObject.TimeBlock.SleepTick;

      if(t_tick>Timeout){
        //找到插入点

        Thread->BlockObject.TimeBlock.SleepTick=Timeout;
        p_next->BlockObject.TimeBlock.SleepTick=t_tick-Timeout;

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
        Timeout-=t_tick;
      }
      
      p_last=p_next;
      p_next=p_last->List.TimeBlock.Next;
    }

    Thread->BlockObject.TimeBlock.SleepTick=Timeout;

    //仍未找到插入点,插入到最后
    //此时p_last不可能为NULL
    Thread->List.TimeBlock.Last=p_last;
    Thread->List.TimeBlock.Next=NULL;
    p_last->List.TimeBlock.Next=Thread;
  }
}



