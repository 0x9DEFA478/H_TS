/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-01 19:48:00
 * @LastEditTime: 2022-01-12 15:42:32
 * @LastEditors: 0x9DEFA478
 * @Description: 中断处理
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




//Tick中断回调
void H_TS_TickCallback(void* v){
  H_TS_Thread* p;
  int isScheduler;
  H_TS_Tick t_tick;
  Hsize InIsThread1;
  Hsize OutIsThread1;

  int retVal;

  isScheduler=0;

  if((H_TS_Core->SchedulerSuspend!=0)||(H_TS_Core->SysTickSuspend!=0)){
    //调度已挂起
    return;
  }


  //睡眠线程判定
  p=H_TS_Core->SleepThreads;
  if(p!=NULL){
    t_tick=p->BlockObject.SleepTick;
    
    if(t_tick<2){
      p->BlockObject.SleepTick=0;
      p->Status=vH_TS_Thread_Status_Ready;

      p=p->List.Sleep.Next;
      while(p!=NULL){
        t_tick=p->BlockObject.SleepTick;
        if(t_tick<1){
          p->Status=vH_TS_Thread_Status_Ready;
        }else{
          break;
        }
        p=p->List.Sleep.Next;
      }

      H_TS_Core->SleepThreadsEvent=-1;
      isScheduler=-1;

    }else{
      p->BlockObject.SleepTick=t_tick-1;
    }
  }

  //时间限制阻塞判定
  p=H_TS_Core->TimeBlockThreads;
  if(p!=NULL){
    t_tick=p->BlockObject.TimeBlock.SleepTick;
    
    if(t_tick<2){
      p->BlockObject.TimeBlock.SleepTick=0;
      p->Status=vH_TS_Thread_Status_Ready;//线程就绪
      ((H_TS_Thread* volatile *)p->BlockObject.TimeBlock.BlockObject)[0]=NULL;//清除占用标记

      //尝试添加到快速通道
      H_TS_ThreadAddToUnblockThread(p,H_TS_Core->TimeUnblockThread,InIsThread1,OutIsThread1,retVal);

      p=p->List.TimeBlock.Next;
      while(p!=NULL){
        t_tick=p->BlockObject.TimeBlock.SleepTick;
        if(t_tick<1){
          p->Status=vH_TS_Thread_Status_Ready;//线程就绪
          ((H_TS_Thread* volatile *)p->BlockObject.TimeBlock.BlockObject)[0]=NULL;//清除占用标记

          retVal=-1;//这里的添加到快速通道总是失败的

        }else{
          break;
        }
        p=p->List.Sleep.Next;
      }

      if(retVal!=0){
        H_TS_Core->TimeBlockThreadsEvent=-1;
      }
      isScheduler=-1;

    }else{
      p->BlockObject.TimeBlock.SleepTick=t_tick-1;
    }
  }

  if(isScheduler){
    oH_TS_CallScheduleISR();
  }

}

//线程调度 
void* H_TS_RunThread_Refresh(){
  H_TS_Thread* p;
  H_TS_Thread* p_last;
  H_TS_Thread* p_next;
  Hsize SleepThreadsEvent;
  Hsize TimeBlockThreadsEvent;
  Hsize InIsThread1;
  Hsize OutIsThread1;
  Hsize i;
  
  H_TS_Thread* p_temp;
  Hsize Priority;
  int retVal;


  if(H_TS_Core->SchedulerSuspend!=0){
    return (void*)0;//调度已挂起
  }
  oH_TS_ResumeSchedulerDone();




  oH_TS_TickSuspend();//挂起Tick响应
  SleepThreadsEvent=H_TS_Core->SleepThreadsEvent;
  TimeBlockThreadsEvent=H_TS_Core->TimeBlockThreadsEvent;
  if((SleepThreadsEvent!=0)||(TimeBlockThreadsEvent!=0)){

    //睡眠线程处理
    if(SleepThreadsEvent!=0){
      H_TS_Core->SleepThreadsEvent=0;

      p=H_TS_Core->SleepThreads;
      while(p!=NULL){
        if(p->Status==vH_TS_Thread_Status_Ready){

          p_next=p->List.Sleep.Next;

          //从睡眠线程链表中移除(第一个节点)
          if(p_next!=NULL){
            p_next->List.Sleep.Last=NULL;
          }

          //添加到就绪链表
          H_TS_ThreadAddToReadyList(p,p_temp,Priority,retVal);

        }else{
          //对于睡眠链表, 就绪线程不会分散排列
          break;
        }

        p=p_next;
      }
      H_TS_Core->SleepThreads=p;
      
    }

    //时间限制阻塞处理
    if(TimeBlockThreadsEvent!=0){
      H_TS_Core->TimeBlockThreadsEvent=0;

      //处理直到第一个节点不为等待就绪线程或者链表空
      p=H_TS_Core->TimeBlockThreads;
      while(p!=NULL){
        if(p->Status==vH_TS_Thread_Status_Ready){

          p_next=p->List.TimeBlock.Next;

          //从时间限制线程链表中移除(第一个节点)
          if(p_next!=NULL){
            p_next->BlockObject.TimeBlock.SleepTick+=p->BlockObject.TimeBlock.SleepTick;
            p_next->List.TimeBlock.Last=NULL;
          }

          //添加到就绪链表
          H_TS_ThreadAddToReadyList(p,p_temp,Priority,retVal);

        }else{
          break;
        }
        
        p=p_next;
      }
      H_TS_Core->TimeBlockThreads=p;

      //处理后续的节点
      if(p!=NULL){

        p=p->List.TimeBlock.Next;
        while(p!=NULL){

          p_next=p->List.TimeBlock.Next;

          if(p->Status==vH_TS_Thread_Status_Ready){

            //从时间限制线程链表中移除
            p_last=p->List.TimeBlock.Last;
            p_last->List.TimeBlock.Next=p_next;//p是后续的节点 p_last一定存在
            if(p_next!=NULL){//p_next不一定存在
              p_next->BlockObject.TimeBlock.SleepTick+=p->BlockObject.TimeBlock.SleepTick;
              p_next->List.TimeBlock.Last=p_last;
            }

            //添加到就绪链表
            H_TS_ThreadAddToReadyList(p,p_temp,Priority,retVal);

          }

          p=p_next;
        }

      }
      
    }

  }

  InIsThread1=H_TS_Core->TimeUnblockThread.InIsThread1;
  OutIsThread1=H_TS_Core->TimeUnblockThread.OutIsThread1;
  if(InIsThread1!=OutIsThread1){

    //读取指针
    if(OutIsThread1==0){
      p=H_TS_Core->TimeUnblockThread.Thread0;
      H_TS_Core->TimeUnblockThread.OutIsThread1=-1;
    }else{
      p=H_TS_Core->TimeUnblockThread.Thread1;
      H_TS_Core->TimeUnblockThread.OutIsThread1=0;
    }

    if(p->IsNotReady!=0){

      //从时间限制线程链表中移除
      p_next=p->List.TimeBlock.Next;
      p_last=p->List.TimeBlock.Last;
      if(p_last==NULL){
        H_TS_Core->TimeBlockThreads=p_next;
      }else{
        p_last->List.TimeBlock.Next=p_next;
      }
      if(p_next!=NULL){
        p_next->BlockObject.TimeBlock.SleepTick+=p->BlockObject.TimeBlock.SleepTick;
        p_next->List.TimeBlock.Last=p_last;
      }

      //添加到就绪链表
      H_TS_ThreadAddToReadyList(p,p_temp,Priority,retVal);
      
    }

  }
  oH_TS_ReusmeTick();//恢复Tick响应
  



  //阻塞线程处理
  if(H_TS_Core->BlockThreadsEvent!=0){
    H_TS_Core->BlockThreadsEvent=0;

#if vH_TS_BlockThreadListNum > 1
    for(i=0;i<vH_TS_BlockThreadListNum;i++){
      if(H_TS_Core->BlockThreadsEvents[i]!=0){
        H_TS_Core->BlockThreadsEvents[i]=0;

        //处理直到第一个节点不为等待就绪线程或者链表空
        p=H_TS_Core->BlockThreads[i];
#else
        p=H_TS_Core->BlockThread;
#endif
        while(p!=NULL){
          if(p->Status==vH_TS_Thread_Status_Ready){

            //从阻塞线程链表中移除(第一个节点)
            p_next=p->List.Block.Next;
            if(p_next!=NULL){
              p_next->List.Block.Last=NULL;
            }
            
            //添加到就绪链表
            H_TS_ThreadAddToReadyList(p,p_temp,Priority,retVal);

          }else{
            break;
          }

          p=p_next;
        }
#if vH_TS_BlockThreadListNum > 1
        H_TS_Core->BlockThreads[i]=p;
#else
        H_TS_Core->BlockThread=p;
#endif

        //处理后续的节点
        if(p!=NULL){

          p=p->List.Block.Next;
          while(p!=NULL){

            p_next=p->List.Block.Next;
            
            if(p->Status==vH_TS_Thread_Status_Ready){

              //从阻塞线程链表中移除
              p_last=p->List.Block.Last;
              p_last->List.Block.Next=p_next;//p是后续的节点 p_last一定存在
              if(p_next!=NULL){//p_next不一定存在
                p_next->List.Block.Last=p_last;
              }

              //添加到就绪链表
              H_TS_ThreadAddToReadyList(p,p_temp,Priority,retVal);

            }

            p=p_next;
          }
        }

#if vH_TS_BlockThreadListNum > 1
      }
    }
#endif

  }

  InIsThread1=H_TS_Core->UnblockThread.InIsThread1;
  OutIsThread1=H_TS_Core->UnblockThread.OutIsThread1;
  if(InIsThread1!=OutIsThread1){

    //读取指针
    if(OutIsThread1==0){
      p=H_TS_Core->UnblockThread.Thread0;
      H_TS_Core->UnblockThread.OutIsThread1=-1;
    }else{
      p=H_TS_Core->UnblockThread.Thread1;
      H_TS_Core->UnblockThread.OutIsThread1=0;
    }

    if(p->IsNotReady!=0){

      //从阻塞线程链表中移除
      p_next=p->List.Block.Next;
      p_last=p->List.Block.Last;
      if(p_last==NULL){
#if vH_TS_BlockThreadListNum > 1
        H_TS_Core->BlockThreads[p->List.Block.ListId]=p_next;
#else
        H_TS_Core->BlockThread=p_next;
#endif
      }else{
        p_last->List.Block.Next=p_next;
      }
      if(p_next!=NULL){
        p_next->List.Block.Last=p_last;
      }

      //添加到就绪链表
      H_TS_ThreadAddToReadyList(p,p_temp,Priority,retVal);

    }

  }
  



  (void)i;
  (void)retVal;


  p_last=cH_TS_RunThread(H_TS_Core)[1];
  p_next=H_TS_Core->ReadyThreads;
  if(p_last!=p_next){
    
#if vH_TS_IsEnableCPULoadCalculate != 0
    H_TS_Time DT;
    H_TS_Time CPU_Load_RunningTime;

    CPU_Load_RunningTime=H_TS_Core->CPU_Load_RunningTime;
    if(CPU_Load_RunningTime<vH_TS_CPULoad_T){
      H_TS_GetDT(DT);
      H_TS_Core->CPU_Load_RunningTime=CPU_Load_RunningTime+DT;
      p_last->CPU_Load_RunningTime+=DT;
    }
#endif

    //下次运行的线程更新
    cH_TS_RunThread(H_TS_Core)=p_next->StackPointer;

#if vH_TS_IsEnableStackUsageStatistics != 0
    p=H_TS_Core->IdleThread;
    if(p_next==p){
      H_TS_Core->IdleThreadResume=-1;
    }
#endif

    return vH_TS_VoidPtrMaxValue;
  }
  
  return (void*)0;
}



