/*
 * @Author: 0x9DEFA478
 * @Date: 2021-08-21 20:10:31
 * @LastEditTime: 2022-01-14 23:56:58
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
#include "./H_ThreadScheduler_Core.h"
#include "../LL/H_ThreadScheduler_LL.h"

extern H_TS* H_TS_Core;


extern void* H_TS_Malloc(Hsize Size);
extern void H_TS_Free(void* v);
extern void H_TS_MemoryInit(void);
extern void H_TS_MemoryErrorCallback(void);

extern void H_TS_ThreadStackInit(void* StackPtr,Hsize StackSize,int (*Code)(void*),void* v);




static void* Malloc(Hsize Size);
static void Free(void* v);




static void StackMark(void** Ptr,Hsize NumOfPtr){
  //NumOfPtr 单位为sizeof(void*)字节

  while(NumOfPtr>7){
    NumOfPtr-=8;
    Ptr[0]=(void*)vH_TS_StackMarkValue;
    Ptr[1]=(void*)vH_TS_StackMarkValue;
    Ptr[2]=(void*)vH_TS_StackMarkValue;
    Ptr[3]=(void*)vH_TS_StackMarkValue;
    Ptr[4]=(void*)vH_TS_StackMarkValue;
    Ptr[5]=(void*)vH_TS_StackMarkValue;
    Ptr[6]=(void*)vH_TS_StackMarkValue;
    Ptr[7]=(void*)vH_TS_StackMarkValue;
    Ptr=&Ptr[8];
  }
  while(NumOfPtr>0){
    NumOfPtr--;
    Ptr[0]=(void*)vH_TS_StackMarkValue;
    Ptr=&Ptr[1];
  }
}

//获取堆栈最小剩余空间
static Hsize StackGetMinFree(void** Ptr,Hsize NumOfPtr){

#if vH_TS_StackPtrDirection == 0
  Hsize r;

  r=0;
  while(NumOfPtr){
    NumOfPtr--;
    if(Ptr[0]==(void*)vH_TS_StackMarkValue){
      Ptr=&Ptr[1];
      r++;
    }else{
      return r;
    }
  }

  return r;

#else
  Husize ptr;
  Hsize r;

  ptr=(Husize)&Ptr[NumOfPtr-1];
  r=0;
  while(NumOfPtr){
    NumOfPtr--;
    if(((void**)ptr)[0]==(void*)vH_TS_StackMarkValue){
      ptr-=sizeof(void*);
      r++;
    }else{
      return r;
    }
  }

  return r;
#endif
}

#define vH_TS_FunctionId_None                                     0
#define vH_TS_FunctionId_GetMinValidPriority                      1
#define vH_TS_FunctionId_GetMaxValidPriority                      2
#define vH_TS_FunctionId_GetLowValidPriority                      3
#define vH_TS_FunctionId_GetHighValidPriority                     4

//获取最接近且比最低优先级低至少一级的可用优先级号(如果用完 将返回最低的那个优先级)
static Hsize GetMinValidPriority(){
  H_TS_Thread* Thread;
  Hsize r;

  r=0;
  Thread=H_TS_Core->Threads;

  while(Thread!=NULL){
    if(Thread!=H_TS_Core->IdleThread){
      r=Thread->Priority;
      
      Thread=Thread->AllList.Next;
      while(Thread!=NULL){
        if(Thread!=H_TS_Core->IdleThread){
          if(Thread->Priority>r){
            r=Thread->Priority;
          }
        }
        Thread=Thread->AllList.Next;
      }
      r+=1;
      if(r>=vH_TS_ThreadMaxPriorityValue){
        r=vH_TS_ThreadMaxPriorityValue-1;
      }
      return r;
    }
    Thread=Thread->AllList.Next;
  }

  return r;
}

//获取最接近且比最高优先级高至少一级的可用优先级号(如果用完 将返回最高的那个优先级)
static Hsize GetMaxValidPriority(){
  H_TS_Thread* Thread;
  Hsize r;

  r=0;
  Thread=H_TS_Core->Threads;

  while(Thread!=NULL){
    if(Thread!=H_TS_Core->IdleThread){
      r=Thread->Priority;
      
      Thread=Thread->AllList.Next;
      while(Thread!=NULL){
        if(Thread!=H_TS_Core->IdleThread){
          if(Thread->Priority<r){
            r=Thread->Priority;
          }
        }
        Thread=Thread->AllList.Next;
      }
      r+=1;
      if(r>=vH_TS_ThreadMaxPriorityValue){
        r=vH_TS_ThreadMaxPriorityValue-1;
      }
      return r;
    }
    Thread=Thread->AllList.Next;
  }

  return r;
}

//获取等于或接近refPriority的可用优先级号, 如果被占用, 将寻找更低优先级的优先级号(如果用完 将返回最低的那个优先级)
static Hsize GetLowValidPriority(Hsize refPriority){
  H_TS_Thread* Thread;
  Hsize r;

  r=refPriority;
  if(r>=vH_TS_ThreadMaxPriorityValue){
    r=vH_TS_ThreadMaxPriorityValue-1;
    return r;
  }

  Thread=H_TS_Core->Threads;

  while(Thread!=NULL){
    if(Thread!=H_TS_Core->IdleThread){
      if(Thread->Priority==r){
        r++;
        Thread=H_TS_Core->Threads;
        continue;
      }
    }
    Thread=Thread->AllList.Next;
  }


  if(r>=vH_TS_ThreadMaxPriorityValue){
    r=vH_TS_ThreadMaxPriorityValue-1;
  }
  return r;
}

//获取等于或接近refPriority的可用优先级号, 如果被占用, 将寻找更高优先级的优先级号(如果用完 将返回最高的那个优先级)
static Hsize GetHighValidPriority(Hsize refPriority){
  H_TS_Thread* Thread;
  Hsize r;

  r=refPriority;

  Thread=H_TS_Core->Threads;

  while(Thread!=NULL){
    if(Thread!=H_TS_Core->IdleThread){
      if(Thread->Priority==r){
        r--;
        Thread=H_TS_Core->Threads;
        continue;
      }
    }
    Thread=Thread->AllList.Next;
  }

  return r;
}

static Hsize GetValidPriority(Hsize Priority,Hsize FunctionId){
  Hsize r;

  switch(FunctionId){
    case vH_TS_FunctionId_GetMinValidPriority:
      r=GetMinValidPriority();
      break;
    case vH_TS_FunctionId_GetMaxValidPriority:
      r=GetMaxValidPriority();
      break;
    case vH_TS_FunctionId_GetLowValidPriority:
      r=GetLowValidPriority(Priority);
      break;
    case vH_TS_FunctionId_GetHighValidPriority:
      r=GetHighValidPriority(Priority);
      break;
    default:
      for(;;){
        //H_TS出现错误
      }
  }

  return r;
}

//初始化线程堆栈
static void Init_Stack(H_TS_Thread* Thread,int (*Code)(void*),void* v){

  void* Stack;

  Stack=Thread->Stack;

#if vH_TS_IsEnableStackUsageStatistics != 0
  StackMark((void**)Stack,Thread->StackSize/sizeof(void*));
#endif

  H_TS_ThreadStackInit(&Stack,Thread->StackSize,Code,v);
  ((void**)Thread->StackPointer)[0]=Stack;

#if vH_TS_IsEnableStackUsageStatistics != 0
  Thread->StackMaxUsage=Thread->StackSize-sizeof(void*)*StackGetMinFree((void**)Thread->Stack,Thread->StackSize/sizeof(void*));
#endif

}

//将线程添加到H_TS调度链表中 
static void ThreadStart(H_TS_Thread* Thread,Hsize Priority,Hsize FunctionId){
  H_TS_Thread* p;
  int H_TS_IsStart;
  
  int retVal;

  if(H_TS_Core->RunThreadPtr==NULL){
    H_TS_IsStart=0;
  }else{
    H_TS_IsStart=-1;

    oH_TS_SchedulerSuspend();//挂起线程调度
  }

  Thread->Status=vH_TS_Thread_Status_Ready;
  if(FunctionId!=vH_TS_FunctionId_None){
    Thread->Priority=GetValidPriority(Priority,FunctionId);
  }else{
    Thread->Priority=Priority;
  }

  //添加到待添加线程链表
  p=H_TS_Core->WaitAddThreads;
  Thread->AllList.Last=NULL;
  Thread->AllList.Next=p;
  if(p!=NULL){
    p->AllList.Last=Thread;
  }
  H_TS_Core->WaitAddThreads=Thread;

  H_TS_ThreadAddToReadyList(Thread,p,Priority,retVal);
  if(H_TS_IsStart!=0){
    if(retVal==0){
      oH_TS_ResumeSchedulerAuto();
    }else{
      oH_TS_ResumeSchedulerFixed();
    }
  }

}

/**
 * @brief H_TS空闲线程
 * @param v 初始化H_TS时传入的参数v
 * @return 不会返回
 */
static int H_TS_IdleProcess(void* v){
  void (*IdleProcess)(void*);
  H_TS_Thread* p;
  H_TS_Thread* p_Add;
  H_TS_Thread* p_Exit;
  H_TS_Thread* p_next;
  H_TS_Thread* ap_last;
  H_TS_Thread* ap_next;
  H_TS_Time CPU_Load_RunningTime;

  for(;;){


    //通过速度比较块的方法判断是否需要处理
    if((H_TS_Core->WaitAddThreads!=NULL)||(H_TS_Core->ExitThreads!=NULL)){

      //读取要处理的线程链表 线程可能同时出现在H_TS_Core->WaitAddThreads和H_TS_Core->ExitThreads中 读取要同时读取 并且要先处理p_Add
      //需要保证同步 需要屏蔽调度后再读取一次(顺便标记为NULL)
      oH_TS_SchedulerSuspend();
      p_Add=H_TS_Core->WaitAddThreads;
      H_TS_Core->WaitAddThreads=NULL;
      p_Exit=H_TS_Core->ExitThreads;
      H_TS_Core->ExitThreads=NULL;
      oH_TS_ResumeSchedulerAuto();

      //添加线程
      p=p_Add;
      if(p!=NULL){

        //找到最后一个节点
        ap_next=p->AllList.Next;
        while(ap_next!=NULL){
          p=ap_next;
          ap_next=p->AllList.Next;
        }

        //将链表添加到头部
        p_next=H_TS_Core->Threads;
        p_Add->AllList.Last=NULL;
        p->AllList.Next=p_next;
        p_next->AllList.Last=p;
        H_TS_Core->Threads=p_Add;
      }

      //移除线程
      p=p_Exit;
      while(p!=NULL){
        p_next=p->List.Exit.Next;

        p->Status=vH_TS_Thread_Status_Stop;

        //从总链表中移除
        ap_last=p->AllList.Last;
        ap_next=p->AllList.Next;
        if(ap_last==NULL){
          H_TS_Core->Threads=ap_next;
        }else{
          ap_last->AllList.Next=ap_next;
        }
        if(ap_next!=NULL){
          ap_next->AllList.Last=ap_last;
        }
        
        p=p_next;
      }

      //尝试释放链表中的线程占用的资源
      p=p_Exit;
      while(p!=NULL){
        p_next=p->List.Exit.Next;

        if((p->Feature&vH_TS_Thread_Feature_NoAutoFree)!=vH_TS_Thread_Feature_NoAutoFree){
          //自动回收资源
          delete_H_TS_Thread(p);
        }
        
        p=p_next;
      }

    }
    



    if(H_TS_Core->IdleThreadResume!=0){
      H_TS_Core->IdleThreadResume=0;
      //检测到线程回到空闲线程

#if vH_TS_IsEnableStackUsageStatistics != 0
      p=H_TS_Core->Threads;
      while(p!=NULL){

        p->StackMaxUsage=p->StackSize-sizeof(void*)*StackGetMinFree((void**)p->Stack,p->StackSize/sizeof(void*));
        p=p->AllList.Next;
      }
#endif


      oH_TS_DebugAssert(H_TS_Core->SubReadyThreads!=NULL,"此时 SubReadyThreads不能为NULL");

    }




    //计算各个线程的CPU占用率
    CPU_Load_RunningTime=H_TS_Core->CPU_Load_RunningTime;
    if(CPU_Load_RunningTime>=vH_TS_CPULoad_T){

      p=H_TS_Core->Threads;

      while(p!=NULL){
        
        p->CPU_Load=p->CPU_Load_RunningTime*1000/CPU_Load_RunningTime;
        p->CPU_Load_RunningTime=0;

        p=p->AllList.Next;
      }

      H_TS_Core->CPU_Load=1000-H_TS_Core->IdleThread->CPU_Load;

      H_TS_Core->CPU_Load_RunningTime=0;
    }




    IdleProcess=H_TS_Core->IdleProcess;
    if(IdleProcess!=NULL){
      IdleProcess(v);
    }

  }
}










/**
 * @brief 创建一个线程句柄 分配栈空间 (此方法并不会开始运行线程,需使用H_TS_ThreadStart()线程才会运行)
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param Feature 一些特性信息
 * @return 线程句柄 如果为NULL 代表失败
 */
H_TS_Thread* new_H_TS_Thread(Hsize StackSize,Huint32 Feature){
  H_TS_Thread* r;
  int H_TS_IsStart;

  StackSize+=(sizeof(void*)-1);
  StackSize-=StackSize%sizeof(void*);

  if(H_TS_Core->RunThreadPtr==NULL){
    H_TS_IsStart=0;
  }else{
    H_TS_IsStart=-1;

    oH_TS_SchedulerSuspend();//挂起线程调度
  }

  r=H_TS_Core->Memory.Malloc(sizeof(H_TS_Thread));
  if(r==NULL){
    goto Error0;
  }
  
  r->Stack=H_TS_Core->Memory.Malloc(StackSize);
  if(r->Stack==NULL){
    goto Error1;
  }

  r->StackPointer=H_TS_Core->Memory.Malloc(2*sizeof(void*));
  if(r->StackPointer==NULL){
    goto Error2;
  }

  if(H_TS_IsStart!=0){
    oH_TS_ResumeSchedulerAuto();//允许线程调度
  }

  ((void**)r->StackPointer)[1]=(void*)r;
  r->Status=vH_TS_Thread_Status_Stop;
  r->CPU_Load=-1;
#if vH_TS_IsEnableCPULoadCalculate != 0
  r->CPU_Load_RunningTime=0;
#endif
  r->StackSize=StackSize;
  r->Feature=Feature;
  r->ThreadName=NULL;

  return r;

Error2:
  H_TS_Core->Memory.Free(r->Stack);
Error1:
  H_TS_Core->Memory.Free(r);
Error0:
  if(H_TS_IsStart!=0){
    oH_TS_ResumeSchedulerAuto();//允许线程调度
  }
  return NULL;
}

/**
 * @brief 释放一个线程占用的资源
 * @param Thread 要释放的线程
 * @return 0:成功 -1:失败 线程未完全停止
 */
int delete_H_TS_Thread(H_TS_Thread* Thread){
  Hsize Status;
  int H_TS_IsStart;

  Status=Thread->Status;
  if(Status!=vH_TS_Thread_Status_Stop){
    return -1;
  }

  if(H_TS_Core->RunThreadPtr==NULL){
    H_TS_IsStart=0;
  }else{
    H_TS_IsStart=-1;

    oH_TS_SchedulerSuspend();//挂起线程调度
  }

  H_TS_Core->Memory.Free(Thread->StackPointer);
  H_TS_Core->Memory.Free(Thread->Stack);
  H_TS_Core->Memory.Free(Thread);

  if(H_TS_IsStart!=0){
    oH_TS_ResumeSchedulerAuto();//允许线程调度
  }

  return 0;
}

/**
 * @brief 开始运行一个线程 如果未开始线程调度 线程将在开始线程调度后运行
 * @param Thread 线程句柄 可以是已停止的线程句柄
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param Priority 优先级
 * @return 0:成功 -1:Thread不为停止状态 -2:优先级无效
 */
int H_TS_ThreadStart(H_TS_Thread* Thread,int (*Code)(void*),void* v,Hsize Priority){

  if(Priority>=vH_TS_ThreadMaxPriorityValue){
    return -2;
  }
  if(Thread->Status!=vH_TS_Thread_Status_Stop){
    return -1;
  }
  
  Init_Stack(Thread,Code,v);

  ThreadStart(Thread,Priority,vH_TS_FunctionId_None);

  return 0;
}

/**
 * @brief 开始运行一个线程 这个方法于H_TS_ThreadStart()不同的地方在于:
 *          它不需要传入优先级, 它会自动将线程优先级设置为当前所有线程的最高级还要高一级 即Priority为现有线程的最小值减1
 *          在存在空闲的高优先级的情况下(一般int有32位, 几乎不用考虑优先级用完的情况) 它不需要去避免优先级冲突的问题 这个方法很适合需要临时新建一个线程的情况
 * @param Thread 线程句柄 可以是已停止的线程句柄
 * @param Code 线程代码
 * @param v 线程传入参数
 * @return 0:成功 -1:Thread不为停止状态
 */
int H_TS_ThreadStart_MaxPriority(H_TS_Thread* Thread,int (*Code)(void*),void* v){

  if(Thread->Status!=vH_TS_Thread_Status_Stop){
    return -1;
  }

  Init_Stack(Thread,Code,v);

  ThreadStart(Thread,0,vH_TS_FunctionId_GetMaxValidPriority);

  return 0;
}

/**
 * @brief 开始运行一个线程 这个方法于H_TS_ThreadStart()不同的地方在于:
 *          它不需要传入优先级, 它会自动将线程优先级设置为当前所有线程的最低级还要低一级 即Priority为现有线程的最小值加1
 *          (这个"当前所有线程"不包括空闲线程 以保证空闲线程优先级是最低的)
 * @param Thread 线程句柄 可以是已停止的线程句柄
 * @param Code 线程代码
 * @param v 线程传入参数
 * @return 0:成功 -1:Thread不为停止状态
 */
int H_TS_ThreadStart_MinPriority(H_TS_Thread* Thread,int (*Code)(void*),void* v){

  if(Thread->Status!=vH_TS_Thread_Status_Stop){
    return -1;
  }

  Init_Stack(Thread,Code,v);

  ThreadStart(Thread,0,vH_TS_FunctionId_GetMinValidPriority);

  return 0;
}

/**
 * @brief 开始运行一个线程 这个方法于H_TS_ThreadStart()不同的地方在于:
 *          它不需要传入优先级, 它会自动将线程优先级设置为等于高于接近refPriority且空闲的一个优先级
 * @param Thread 线程句柄 可以是已停止的线程句柄
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param refPriority 参考优先级
 * @return 0:成功 -1:Thread不为停止状态
 */
int H_TS_ThreadStart_HighPriority(H_TS_Thread* Thread,int (*Code)(void*),void* v,Hsize refPriority){

  if(Thread->Status!=vH_TS_Thread_Status_Stop){
    return -1;
  }

  Init_Stack(Thread,Code,v);

  ThreadStart(Thread,refPriority,vH_TS_FunctionId_GetHighValidPriority);

  return 0;
}

/**
 * @brief 开始运行一个线程 这个方法于H_TS_ThreadStart()不同的地方在于:
 *          它不需要传入优先级, 它会自动将线程优先级设置为等于低于接近refPriority且空闲的一个优先级 但不会比空闲线程低
 * @param Thread 线程句柄 可以是已停止的线程句柄
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param refPriority 参考优先级
 * @return 0:成功 -1:Thread不为停止状态
 */
int H_TS_ThreadStart_LowPriority(H_TS_Thread* Thread,int (*Code)(void*),void* v,Hsize refPriority){

  if(Thread->Status!=vH_TS_Thread_Status_Stop){
    return -1;
  }
  
  Init_Stack(Thread,Code,v);

  ThreadStart(Thread,refPriority,vH_TS_FunctionId_GetLowValidPriority);

  return 0;
}

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param Priority 优先级
 * @param Feature 一些特性信息
 * @return 线程句柄 NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread(int (*Code)(void*),void* v,Hsize StackSize,Hsize Priority,Huint32 Feature){

  H_TS_Thread* r;

  r=new_H_TS_Thread(StackSize,Feature);
  if(r==NULL){
    return NULL;
  }

  if(H_TS_ThreadStart(r,Code,v,Priority)!=0){
    //开始失败
    delete_H_TS_Thread(r);
    return NULL;
  }
  //开始成功

  return r;
}

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 *        它不需要传入优先级, 它会自动将线程优先级设置为当前所有线程的最高级还要高一级 即Priority为现有线程的最小值减1
 *          在存在空闲的高优先级的情况下(一般int有32位, 几乎不用考虑优先级用完的情况) 它不需要去避免优先级冲突的问题 这个方法很适合需要临时新建一个线程的情况
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param Feature 一些特性信息
 * @return 线程句柄 NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_MaxPriority(int (*Code)(void*),void* v,Hsize StackSize,Huint32 Feature){

  H_TS_Thread* r;

  r=new_H_TS_Thread(StackSize,Feature);
  if(r==NULL){
    return NULL;
  }

  if(H_TS_ThreadStart_MaxPriority(r,Code,v)!=0){
    //开始失败
    delete_H_TS_Thread(r);
    return NULL;
  }
  //开始成功

  return r;
}

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 *          它不需要传入优先级, 它会自动将线程优先级设置为当前所有线程的最低级还要低一级 即Priority为现有线程的最小值加1
 *          (这个"当前所有线程"不包括空闲线程 以保证空闲线程优先级是最低的)
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param Feature 一些特性信息
 * @return 线程句柄 NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_MinPriority(int (*Code)(void*),void* v,Hsize StackSize,Huint32 Feature){

  H_TS_Thread* r;

  r=new_H_TS_Thread(StackSize,Feature);
  if(r==NULL){
    return NULL;
  }

  if(H_TS_ThreadStart_MinPriority(r,Code,v)!=0){
    //开始失败
    delete_H_TS_Thread(r);
    return NULL;
  }
  //开始成功

  return r;
}

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 *          它不需要传入优先级, 它会自动将线程优先级设置为等于高于接近refPriority且空闲的一个优先级
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param refPriority 参考优先级
 * @param Feature 一些特性信息
 * @return 线程句柄 NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_HighPriority(int (*Code)(void*),void* v,Hsize StackSize,Hsize refPriority,Huint32 Feature){

  H_TS_Thread* r;

  r=new_H_TS_Thread(StackSize,Feature);
  if(r==NULL){
    return NULL;
  }

  if(H_TS_ThreadStart_HighPriority(r,Code,v,refPriority)!=0){
    //开始失败
    delete_H_TS_Thread(r);
    return NULL;
  }
  //开始成功

  return r;
}

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 *          它不需要传入优先级, 它会自动将线程优先级设置为等于低于接近refPriority且空闲的一个优先级 但不会比空闲线程低
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param refPriority 参考优先级
 * @param Feature 一些特性信息
 * @return 线程句柄 NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_LowPriority(int (*Code)(void*),void* v,Hsize StackSize,Hsize refPriority,Huint32 Feature){

  H_TS_Thread* r;

  r=new_H_TS_Thread(StackSize,Feature);
  if(r==NULL){
    return NULL;
  }

  if(H_TS_ThreadStart_LowPriority(r,Code,v,refPriority)!=0){
    //开始失败
    delete_H_TS_Thread(r);
    return NULL;
  }
  //开始成功

  return r;
}

/**
 * @brief 结束线程自身
 * @param RetVal 线程返回值
 * @return 无
 */
void H_TS_ThreadExit(int RetVal){
  H_TS_Thread* _this;
  H_TS_Thread* p_last;
  H_TS_Thread* p_next;

  _this=cH_TS_RunThread(H_TS_Core)[1];

#if vH_TS_IsStoreThreadRetVal != 0
  _this->RetVal=RetVal;
#endif

  oH_TS_SchedulerSuspend();//挂起线程调度

  _this->Status=vH_TS_Thread_Status_RequestStop;//修改状态
  
  //从就绪链表里移除
  H_TS_ThreadRemoteThisFromReadyList(_this,p_next,p_last);

  //将_this添加到结束链表头部
  p_next=H_TS_Core->ExitThreads;
  _this->List.Exit.Next=p_next;
  H_TS_Core->ExitThreads=_this;

  oH_TS_ResumeSchedulerFixed();//允许线程调度

  for(;;){}

}

/**
 * @brief 由线程调用, 获取线程本身的句柄
 * @return 线程本身的句柄
 */
H_TS_Thread* H_TS_GetThisThread(){

  return cH_TS_RunThread(H_TS_Core)[1];
}

/**
 * @brief 获取所有线程的信息
 * @return 线程信息链表, 如果为NULL则代表失败
 */
H_TS_ThreadInfo* H_TS_ThreadInfoGet(){
  H_TS_Thread* Thread;
  H_TS_ThreadInfo* r;
  H_TS_ThreadInfo* p;

  r=NULL;
  p=NULL;

  oH_TS_SchedulerSuspend();

  Thread=H_TS_Core->Threads;

  while(Thread!=NULL){
    
    if(p==NULL){
      r=H_TS_Core->Memory.Malloc(sizeof(H_TS_ThreadInfo));
      p=r;
    }else{
      p->Next=H_TS_Core->Memory.Malloc(sizeof(H_TS_ThreadInfo));
      p=p->Next;
    }

    if(p==NULL){

      p=r;

      while(p!=NULL){
        r=p->Next;
        H_TS_Core->Memory.Free(p);
        p=r;
      }

      r=NULL;
      break;
    }

    p->Next=NULL;
    p->ThreadName=Thread->ThreadName;
    p->CPU_Load=Thread->CPU_Load;
    p->Priority=Thread->Priority;
#if vH_TS_IsEnableStackUsageStatistics != 0
    p->StackMaxUsage=Thread->StackMaxUsage;
#else
    p->StackMaxUsage=-1;
#endif
    p->StackSize=Thread->StackSize;
    
    Thread=Thread->AllList.Next;
  }
  
  oH_TS_ResumeSchedulerAuto();

  return r;
}

/**
 * @brief 释放线程信息链表
 * @param Info 线程信息
 * @return 无
 */
void H_TS_ThreadInfoFree(H_TS_ThreadInfo* Info){
  H_TS_ThreadInfo* p;

  if(Info==NULL){
    return;
  }

  p=Info;

  oH_TS_SchedulerSuspend();
  
  while(p!=NULL){
    Info=p->Next;
    H_TS_Core->Memory.Free(p);
    p=Info;
  }

  oH_TS_ResumeSchedulerAuto();
}

/**
 * @brief 初始化线程调度器
 * @param v 空闲线程传入参数
 * @param IdleThreadStackSize 空闲线程堆栈大小
 * @return 0:成功 其他:内存申请失败
 */
int H_TS_Init(void* v,Hsize IdleThreadStackSize){

  void* Stack;
  Hsize i;

  H_TS_MemoryInit();
  H_TS_Core=Malloc(sizeof(H_TS));
  oH_TS_CompileBarrier();
  if(H_TS_Core==NULL){
    goto Error0;
  }
  H_TS_Core->Memory.Malloc=Malloc;
  H_TS_Core->Memory.Free=Free;


  H_TS_Core->RunThreadPtr=NULL;

  H_TS_Core->SchedulerSuspend=0;
  H_TS_Core->SysTickSuspend=0;
  H_TS_Core->SchedulerForISR=0;
  H_TS_Core->ResumeSchedulerDone=0;
  
  H_TS_Core->CPU_Load=-1;
#if vH_TS_IsEnableCPULoadCalculate != 0
  H_TS_Core->CPU_Load_RunningTime=0;
#endif
  
  H_TS_Core->IdleProcess=NULL;
  H_TS_Core->IdleThreadResume=0;
  
  H_TS_Core->ExitThreads=NULL;
  H_TS_Core->WaitAddThreads=NULL;

  H_TS_Core->SubReadyThreads=NULL;
  H_TS_Core->SubReadyThreadsHighestPriority=vH_TS_ThreadMaxPriorityValue;

  H_TS_Core->SleepThreads=NULL;
  H_TS_Core->SleepThreadsEvent=0;

  H_TS_Core->TimeBlockThreads=NULL;
  H_TS_Core->TimeBlockThreadsEvent=0;

  H_TS_Core->BlockThreadsEvent=0;
#if vH_TS_BlockThreadListNum > 1
  H_TS_Core->BlockThreadsNextAddIndex=0;
  for(i=0;i<vH_TS_BlockThreadListNum;i++){
    H_TS_Core->BlockThreadsEvents[i]=0;
    H_TS_Core->BlockThreads[i]=NULL;
  }
#else
  H_TS_Core->BlockThread=NULL;
#endif

  H_TS_Core->UnblockThread.InIsThread1=0;
  H_TS_Core->UnblockThread.OutIsThread1=0;
  H_TS_Core->UnblockThread.ReEnter=0;
  H_TS_Core->TimeUnblockThread.InIsThread1=0;
  H_TS_Core->TimeUnblockThread.OutIsThread1=0;
  H_TS_Core->TimeUnblockThread.ReEnter=0;


  
  //生成空闲线程

  H_TS_Core->IdleThread=H_TS_Core->Memory.Malloc(sizeof(H_TS_Thread));
  if(H_TS_Core->IdleThread==NULL){
    goto Error1;
  }

  IdleThreadStackSize+=(sizeof(void*)-1);
  IdleThreadStackSize-=IdleThreadStackSize%sizeof(void*);

  H_TS_Core->IdleThread->Stack=H_TS_Core->Memory.Malloc(IdleThreadStackSize);
  if(H_TS_Core->IdleThread->Stack==NULL){
    goto Error2;
  }

  H_TS_Core->IdleThread->StackPointer=H_TS_Core->Memory.Malloc(2*sizeof(void*));
  if(H_TS_Core->IdleThread->StackPointer==NULL){
    goto Error3;
  }

  
  ((void**)H_TS_Core->IdleThread->StackPointer)[1]=H_TS_Core->IdleThread;

  //初始化空闲线程

  H_TS_Core->IdleThread->Status=vH_TS_Thread_Status_Ready;

  H_TS_Core->IdleThread->CPU_Load=-1;
#if vH_TS_IsEnableCPULoadCalculate != 0
  H_TS_Core->CPU_Load_RunningTime=0;
#endif

  H_TS_Core->IdleThread->StackSize=IdleThreadStackSize;
  H_TS_Core->IdleThread->Priority=vH_TS_ThreadMaxPriorityValue;

#if vH_TS_IsEnableStackUsageStatistics != 0
  StackMark(H_TS_Core->IdleThread->Stack,IdleThreadStackSize/sizeof(void*));
#endif

  Stack=H_TS_Core->IdleThread->Stack;

  H_TS_ThreadStackInit(&Stack,H_TS_Core->IdleThread->StackSize,H_TS_IdleProcess,v);
  ((void**)H_TS_Core->IdleThread->StackPointer)[0]=Stack;

#if vH_TS_IsEnableStackUsageStatistics != 0
  H_TS_Core->IdleThread->StackMaxUsage=H_TS_Core->IdleThread->StackSize-sizeof(void*)*StackGetMinFree(H_TS_Core->IdleThread->Stack,IdleThreadStackSize/sizeof(void*));
#endif

  H_TS_Core->IdleThread->ThreadName="<IDLE_THREAD>";
  
  H_TS_Core->IdleThread->IsNotReady=0;
  H_TS_Core->IdleThread->List.Ready.IsSubReady=0;
  H_TS_Core->IdleThread->List.Ready.Last=NULL;
  H_TS_Core->IdleThread->List.Ready.Next=NULL;
  H_TS_Core->ReadyThreads=H_TS_Core->IdleThread;

  H_TS_Core->IdleThread->AllList.Next=NULL;
  H_TS_Core->IdleThread->AllList.Last=NULL;
  H_TS_Core->Threads=H_TS_Core->IdleThread;


  (void)i;

  return 0;



Error3:
  H_TS_Core->Memory.Free(H_TS_Core->IdleThread->Stack);
Error2:
  H_TS_Core->Memory.Free(H_TS_Core->IdleThread);
Error1:
  Free(H_TS_Core);
Error0:
  return -1;
}








static void* Malloc(Hsize Size){
  void* r;

  r=H_TS_Malloc(Size);
  if(r==NULL){
    H_TS_MemoryErrorCallback();
  }

  return r;
}

static void Free(void* v){
  H_TS_Free(v);
}





