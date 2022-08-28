/*
 * @Author: 0x9DEFA478
 * @Date: 2021-08-21 20:10:31
 * @LastEditTime: 2021-10-25 00:11:52
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
#include "H_ThreadScheduler_Core.h"
#include "..\LL\H_ThreadScheduler_LL.h"

extern H_TS* volatile H_TS_Core;


extern void* H_TS_Malloc(Hsize Size);
extern void H_TS_Free(void* v);
extern void H_TS_MemoryInit(void);
extern void H_TS_MemoryErrorCallback(void);

extern void H_TS_ThreadStackInit(void* StackPtr,int StackSize,int (*Code)(void*),void* v,void (*ReturnCallback)(int));
extern void ThreadReturnCallback(int retVal);



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

#define vH_TS_FunctionId_None                 0
#define vH_TS_FunctionId_getMinValidPriority  1
#define vH_TS_FunctionId_getMaxValidPriority  2
#define vH_TS_FunctionId_getLowValidPriority  3
#define vH_TS_FunctionId_getHighValidPriority 4

//获取最接近且比最低优先级低至少一级的可用优先级号(如果用完 将返回最低的那个优先级)
static int GetMinValidPriority(){
  H_TS_Thread* Thread;
  int r;

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
static int getMaxValidPriority(){
  H_TS_Thread* Thread;
  int r;

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
static int GetLowValidPriority(int refPriority){
  H_TS_Thread* Thread;
  int r;

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
static int GetHighValidPriority(int refPriority){
  H_TS_Thread* Thread;
  int r;

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

static int GetValidPriority(int Priority,int FunctionId){
  int r;

  switch(FunctionId){
    case vH_TS_FunctionId_getMinValidPriority:
      r=GetMinValidPriority();
      break;
    case vH_TS_FunctionId_getMaxValidPriority:
      r=getMaxValidPriority();
      break;
    case vH_TS_FunctionId_getLowValidPriority:
      r=GetLowValidPriority(Priority);
      break;
    case vH_TS_FunctionId_getHighValidPriority:
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

#if vH_TS_isEnableStackUsageStatistics != 0
  StackMark((void**)Stack,Thread->StackSize/sizeof(void*));
#endif

  H_TS_ThreadStackInit(&Stack,Thread->StackSize,Code,v,ThreadReturnCallback);
  ((void**)Thread->StackPointer)[0]=Stack;

#if vH_TS_isEnableStackUsageStatistics != 0
  Thread->StackMaxUsage=Thread->StackSize-sizeof(void*)*StackGetMinFree((void**)Thread->Stack,Thread->StackSize/sizeof(void*));
#endif

}

//将线程添加到H_TS调度链表中 
static void ThreadStart(H_TS_Thread* Thread,int Priority,int FunctionId){
  H_TS_Thread* p;

  if(H_TS_Core->RunThreadPtr==NULL){

    Thread->Status=vH_TS_Thread_Status_Ready;
    if(FunctionId!=vH_TS_FunctionId_None){
      Thread->Priority=GetValidPriority(Priority,FunctionId);
    }else{
      Thread->Priority=Priority;
    }
    
    //添加到线程链表
    p=H_TS_Core->Threads;
    Thread->AllList.Last=NULL;
    Thread->AllList.Next=p;
    p->AllList.Last=Thread;
    H_TS_Core->Threads=Thread;

    H_TS_ThreadAddToReadyList(Thread);

    return;
  }

  oH_TS_SchedulerSuspend();//挂起线程调度

  Thread->Status=vH_TS_Thread_Status_Ready;
  if(FunctionId!=vH_TS_FunctionId_None){
    Thread->Priority=GetValidPriority(Priority,FunctionId);
  }else{
    Thread->Priority=Priority;
  }

  //添加到线程链表
  p=H_TS_Core->Threads;
  Thread->AllList.Last=NULL;
  Thread->AllList.Next=p;
  p->AllList.Last=Thread;
  H_TS_Core->Threads=Thread;

  if(H_TS_ThreadAddToReadyList(Thread)==0){
    oH_TS_ResumeSchedulerAuto();
  }else{
    oH_TS_ResumeSchedulerFixed();
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
  H_TS_Thread* p_Exit;
  H_TS_Thread* p_exit;
  H_TS_Thread* p_next;
  H_TS_Thread* ap_last;
  H_TS_Thread* ap_next;

  for(;;){

    
    if(H_TS_Core->ExitThreads!=NULL){
      //有线程需要退出处理

      oH_TS_SchedulerSuspend();
      p_Exit=H_TS_Core->ExitThreads;
      H_TS_Core->ExitThreads=NULL;

      //将链表中的线程全部处理
      p_exit=p_Exit;
      while(p_exit!=NULL){
        p_next=p_exit->List.Exit.Next;

        p_exit->Status=vH_TS_Thread_Status_Stop;

        //从总链表中移除
        ap_last=p_exit->AllList.Last;
        ap_next=p_exit->AllList.Next;
        if(ap_last==NULL){
          H_TS_Core->Threads=ap_next;
        }else{
          ap_last->AllList.Next=ap_next;
        }
        if(ap_next!=NULL){
          ap_next->AllList.Last=ap_last;
        }
        
        p_exit=p_next;
      }
      //Exit链表不需要调度保护
      oH_TS_ResumeSchedulerAuto();

      //尝试释放链表中的线程占用的资源
      p_exit=p_Exit;
      while(p_exit!=NULL){
        p_next=p_exit->List.Exit.Next;

        if((p_exit->Feature&vH_TS_Thread_Feature_NoAutoFree)!=vH_TS_Thread_Feature_NoAutoFree){
          //自动回收资源
          delete_H_TS_Thread(p_exit);
        }
        
        p_exit=p_next;
      }

    }
    

#if vH_TS_isEnableStackUsageStatistics != 0
    if(H_TS_Core->IdleThreadResume!=0){
      H_TS_Core->IdleThreadResume=0;
      //检测到线程回到空闲线程

      p=H_TS_Core->Threads;
      while(p!=NULL){

        p->StackMaxUsage=p->StackSize-sizeof(void*)*StackGetMinFree((void**)p->Stack,p->StackSize/sizeof(void*));
        p=p->AllList.Next;
      }
    }
#endif

    IdleProcess=(void(*)(void*))H_TS_Core->IdleProcess;
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
H_TS_Thread* new_H_TS_Thread(int StackSize,Huint32 Feature){

  H_TS_Thread* r;

  StackSize+=(sizeof(void*)-1);
  StackSize-=StackSize%sizeof(void*);

  oH_TS_SchedulerSuspend();//挂起线程调度

  r=H_TS_Core->Memory.Malloc(sizeof(H_TS_Thread));
  if(r==NULL)
  {
    oH_TS_ResumeSchedulerAuto();//允许线程调度
    return NULL;
  }
  
  r->Stack=H_TS_Core->Memory.Malloc(StackSize);
  if(r->Stack==NULL)
  {
    H_TS_Core->Memory.Free(r);
    oH_TS_ResumeSchedulerAuto();//允许线程调度
    return NULL;
  }

  r->StackPointer=H_TS_Core->Memory.Malloc(2*sizeof(void*));
  if(r->StackPointer==NULL){
    H_TS_Core->Memory.Free(r->Stack);
    H_TS_Core->Memory.Free(r);
    oH_TS_ResumeSchedulerAuto();//允许线程调度
    return NULL;
  }

  oH_TS_ResumeSchedulerAuto();//允许线程调度

  ((void**)r->StackPointer)[1]=(void*)r;
  r->Status=vH_TS_Thread_Status_Stop;
  r->StackSize=StackSize;
  r->Feature=Feature;

  return r;
}

/**
 * @brief 释放一个线程占用的资源
 * @param Thread 要释放的线程
 * @return 0:成功 -1:失败 线程未完全停止
 */
int delete_H_TS_Thread(H_TS_Thread* Thread){
  int Status;

  Status=Thread->Status;
  if(Status!=vH_TS_Thread_Status_Stop){
    return -1;
  }

  oH_TS_SchedulerSuspend();//挂起线程调度

  H_TS_Core->Memory.Free(Thread->StackPointer);
  H_TS_Core->Memory.Free(Thread->Stack);
  H_TS_Core->Memory.Free(Thread);

  oH_TS_ResumeSchedulerAuto();//允许线程调度

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
int H_TS_ThreadStart(H_TS_Thread* Thread,int (*Code)(void*),void* v,int Priority){

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

  ThreadStart(Thread,0,vH_TS_FunctionId_getMaxValidPriority);

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

  ThreadStart(Thread,0,vH_TS_FunctionId_getMinValidPriority);

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
int H_TS_ThreadStart_HighPriority(H_TS_Thread* Thread,int (*Code)(void*),void* v,int refPriority){

  if(Thread->Status!=vH_TS_Thread_Status_Stop){
    return -1;
  }

  Init_Stack(Thread,Code,v);

  ThreadStart(Thread,refPriority,vH_TS_FunctionId_getHighValidPriority);

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
int H_TS_ThreadStart_LowPriority(H_TS_Thread* Thread,int (*Code)(void*),void* v,int refPriority){

  if(Thread->Status!=vH_TS_Thread_Status_Stop){
    return -1;
  }
  
  Init_Stack(Thread,Code,v);

  ThreadStart(Thread,refPriority,vH_TS_FunctionId_getLowValidPriority);

  return 0;
}

/**
 * @brief 新建并开始一个新线程 如果未开始线程调度 线程将在开始线程调度后运行
 * @param Code 线程代码
 * @param v 线程传入参数
 * @param StackSize 堆栈大小(如果不是sizeof(void*)的倍数 会被增加到最近的sizeof(void*)倍数)
 * @param Priority 优先级
 * @param Feature 一些特性信息
 * @return 线程句柄 如果为NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread(int (*Code)(void*),void* v,int StackSize,int Priority,Huint32 Feature){

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
 * @return 线程句柄 如果为NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_MaxPriority(int (*Code)(void*),void* v,int StackSize,Huint32 Feature){

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
 * @return 线程句柄 如果为NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_MinPriority(int (*Code)(void*),void* v,int StackSize,Huint32 Feature){

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
 * @return 线程句柄 如果为NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_HighPriority(int (*Code)(void*),void* v,int StackSize,int refPriority,Huint32 Feature){

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
 * @return 线程句柄 如果为NULL:失败 其他:成功
 */
H_TS_Thread* H_TS_StartNewThread_LowPriority(int (*Code)(void*),void* v,int StackSize,int refPriority,Huint32 Feature){

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
 * @return 无
 */
void H_TS_ThreadExit(){
  H_TS_Thread* _this;
  H_TS_Thread* p_last;
  H_TS_Thread* p_next;

  _this=H_TS_Core->RunThreadPtr[0][1];

  oH_TS_SchedulerSuspend();//挂起线程调度

  _this->Status=vH_TS_Thread_Status_RequestStop;//修改状态
  
  //将_this从就绪链表里删除
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

  //将_this添加到结束链表尾部
  p_last=H_TS_Core->ExitThreads;
  if(p_last==NULL){
    _this->List.Exit.Next=NULL;
    H_TS_Core->ExitThreads=_this;
  }else{
    p_next=p_last->List.Exit.Next;
    while(p_next!=NULL){
      p_last=p_next;
      p_next=p_last->List.Exit.Next;
    }
    _this->List.Exit.Next=NULL;
    p_last->List.Exit.Next=_this;
  }

  oH_TS_ResumeSchedulerFixed();//允许线程调度

  for(;;){}

}

/**
 * @brief 由线程调用, 获取线程本身的句柄
 * @return 线程本身的句柄
 */
H_TS_Thread* H_TS_GetThisThread(){

  return H_TS_Core->RunThreadPtr[0][1];
}

/**
 * @brief 获取所有线程的信息
 * @return 线程信息链表
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
#if vH_TS_isUseThreadName != 0
    p->ThreadName=Thread->ThreadName;
#else
    p->ThreadName="<nonsupport>";
#endif
    p->Priority=Thread->Priority;
#if vH_TS_isEnableStackUsageStatistics != 0
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
 * @return 0:成功 -1,-2,-3,-4:内存申请失败
 */
int H_TS_Init(void* v,Hsize IdleThreadStackSize){

  void* Stack;
  int i;

  H_TS_MemoryInit();
  H_TS_Core=Malloc(sizeof(H_TS));
  if(H_TS_Core==NULL){
    return -1;
  }
  H_TS_Core->Memory.Malloc=Malloc;
  H_TS_Core->Memory.Free=Free;

  H_TS_Core->RunThreadPtr=NULL;
  H_TS_Core->SchedulerSuspend=0;
  H_TS_Core->SysTickSuspend=0;
  H_TS_Core->SchedulerForIrq=0;
  H_TS_Core->ResumeSchedulerDone=0;
#if vH_TS_isEnableStackUsageStatistics != 0
  H_TS_Core->IdleThreadResume=0;
#endif
  H_TS_Core->SleepThreads=NULL;
  H_TS_Core->SleepThreadsEvent=0;
  H_TS_Core->BlockThreadsEvent=0;
  for(i=0;i<vH_TS_BlockThreadListNum;i++){
    H_TS_Core->BlockThreads[i]=NULL;
    H_TS_Core->BlockThreadsEvents[i]=0;
    H_TS_Core->BlockThreadsItemCnt[i]=0;
  }
  H_TS_Core->TimeBlockThreads=NULL;
  H_TS_Core->TimeBlockThreadsEvent=0;
  H_TS_Core->ExitThreads=NULL;
  H_TS_Core->IdleProcess=NULL;

#if vH_TS_isEnableCPULoadCalculate != 0
  H_TS_Core->CPULoadCalculator.RunTickCnt=0;
  H_TS_Core->CPULoadCalculator.IdleTickCnt=0;
#endif
  H_TS_Core->CPU_Load=-1;


  
  //生成空闲线程

  H_TS_Core->IdleThread=H_TS_Core->Memory.Malloc(sizeof(H_TS_Thread));
  if(H_TS_Core->IdleThread==NULL){
    return -2;
  }

  IdleThreadStackSize+=(sizeof(void*)-1);
  IdleThreadStackSize-=IdleThreadStackSize%sizeof(void*);

  H_TS_Core->IdleThread->Stack=H_TS_Core->Memory.Malloc(IdleThreadStackSize);
  if(H_TS_Core->IdleThread->Stack==NULL){
    H_TS_Core->Memory.Free(H_TS_Core);
    return -3;
  }

  H_TS_Core->IdleThread->StackPointer=H_TS_Core->Memory.Malloc(2*sizeof(void*));
  if(H_TS_Core->IdleThread->StackPointer==NULL){
    H_TS_Core->Memory.Free(H_TS_Core->IdleThread->Stack);
    H_TS_Core->Memory.Free(H_TS_Core);
    return -4;
  }

  
  ((void**)H_TS_Core->IdleThread->StackPointer)[1]=H_TS_Core->IdleThread;
  H_TS_Core->IdleThread->Status=vH_TS_Thread_Status_Ready;

  H_TS_Core->IdleThread->StackSize=IdleThreadStackSize;
  H_TS_Core->IdleThread->Priority=vH_TS_ThreadMaxPriorityValue;
  H_TS_Core->IdleThread->List.Ready.Last=NULL;
  H_TS_Core->IdleThread->List.Ready.Next=NULL;

  //初始化空闲线程

#if vH_TS_isEnableStackUsageStatistics != 0
  StackMark(H_TS_Core->IdleThread->Stack,IdleThreadStackSize/sizeof(void*));
#endif

  Stack=H_TS_Core->IdleThread->Stack;

  H_TS_ThreadStackInit(&Stack,H_TS_Core->IdleThread->StackSize,H_TS_IdleProcess,v,ThreadReturnCallback);
  ((void**)H_TS_Core->IdleThread->StackPointer)[0]=Stack;

#if vH_TS_isEnableStackUsageStatistics != 0
  H_TS_Core->IdleThread->StackMaxUsage=H_TS_Core->IdleThread->StackSize-sizeof(void*)*StackGetMinFree(H_TS_Core->IdleThread->Stack,IdleThreadStackSize/sizeof(void*));
#endif

  

  H_TS_Core->ReadyThreads=H_TS_Core->IdleThread;
  H_TS_Core->IdleThread->AllList.Next=NULL;
  H_TS_Core->IdleThread->AllList.Last=NULL;
  H_TS_Core->Threads=H_TS_Core->IdleThread;

  return 0;
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





