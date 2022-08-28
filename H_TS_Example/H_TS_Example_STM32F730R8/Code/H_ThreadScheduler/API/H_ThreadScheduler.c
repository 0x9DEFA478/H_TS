/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-01 19:48:00
 * @LastEditTime: 2021-10-24 22:02:03
 * @LastEditors: 0x9DEFA478
 * @Description:
 * 线程调度器实现
 * 线程与调度的基本方法
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



#if vH_TS_isEnableCPULoadCalculate != 0
extern H_TS_Tick H_TS_GetDT(void);

static void CPULoadCalculate(int IsIdleThread){
  H_TS_Tick dt;
  H_TS_Tick RunTickCnt;
  H_TS_Tick IdleTickCnt;

  dt=H_TS_GetDT();
  RunTickCnt=H_TS_Core->CPULoadCalculator.RunTickCnt;
  IdleTickCnt=H_TS_Core->CPULoadCalculator.IdleTickCnt;

  RunTickCnt+=dt;
  if(IsIdleThread!=0){
    IdleTickCnt+=dt;
  }
  if(RunTickCnt>vH_TS_CPULoad_T){
    H_TS_Core->CPU_Load=1000-(1000*IdleTickCnt/RunTickCnt);
    H_TS_Core->CPULoadCalculator.RunTickCnt=0;
    H_TS_Core->CPULoadCalculator.IdleTickCnt=0;
  }else{
    H_TS_Core->CPULoadCalculator.RunTickCnt=RunTickCnt;
    H_TS_Core->CPULoadCalculator.IdleTickCnt=IdleTickCnt;
  }
}
#endif


//Tick中断回调
void H_TS_TickCallback(void* v){
  H_TS_Thread* p;
  int isScheduler;
  H_TS_Tick t_tick;

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
      p->Status=vH_TS_Thread_Status_Ready;
      *((H_TS_Thread* volatile *)p->BlockObject.TimeBlock.BlockObject)=NULL;

      p=p->List.TimeBlock.Next;
      while(p!=NULL){
        t_tick=p->BlockObject.TimeBlock.SleepTick;
        if(t_tick<1){
          p->Status=vH_TS_Thread_Status_Ready;
          *((H_TS_Thread* volatile *)p->BlockObject.TimeBlock.BlockObject)=NULL;
        }else{
          break;
        }
        p=p->List.Sleep.Next;
      }

      H_TS_Core->TimeBlockThreadsEvent=-1;
      isScheduler=-1;

    }else{
      p->BlockObject.TimeBlock.SleepTick=t_tick-1;
    }
  }

  if(isScheduler){
    oH_TS_CallPendSV();
  }

}

//线程调度
void* H_TS_RunThreadPtr_Refresh(){
  H_TS_Thread* p;
  H_TS_Thread* p_last;
  H_TS_Thread* p_next;
  int SleepThreadsEvent;
  int TimeBlockThreadsEvent;
  int i;


  oH_TS_ResumeSchedulerDone();
  if(H_TS_Core->SchedulerSuspend!=0){
    return (void*)0;//调度已挂起
  }


  SleepThreadsEvent=H_TS_Core->SleepThreadsEvent;
  TimeBlockThreadsEvent=H_TS_Core->TimeBlockThreadsEvent;
  if((SleepThreadsEvent!=0)||(TimeBlockThreadsEvent!=0)){
    oH_TS_TickSuspend();//挂起Tick响应

    //睡眠线程处理
    if(SleepThreadsEvent!=0){
      H_TS_Core->SleepThreadsEvent=0;

      p=H_TS_Core->SleepThreads;
      while(p!=NULL){
        if(p->Status==vH_TS_Thread_Status_Ready){

          //从睡眠线程链表中移除(第一个节点)
          p_next=p->List.Sleep.Next;
          if(p_next!=NULL){
            p_next->List.Sleep.Last=NULL;
          }

          //添加到就绪链表
          H_TS_ThreadAddToReadyList(p);
					
          p=p_next;

        }else{
          break;
        }
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

          //从时间限制线程链表中移除(第一个节点)
          p_next=p->List.TimeBlock.Next;
          if(p_next!=NULL){
            p_next->BlockObject.TimeBlock.SleepTick+=p->BlockObject.TimeBlock.SleepTick;
            p_next->List.TimeBlock.Last=NULL;
          }
					
					//添加到就绪链表
          H_TS_ThreadAddToReadyList(p);
					
          p=p_next;

        }else{
          break;
        }
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
            H_TS_ThreadAddToReadyList(p);

          }

          p=p_next;
        }

      }
      
    }

    oH_TS_ReusmeTick();//恢复Tick响应
  }
  

  //阻塞线程处理
  if(H_TS_Core->BlockThreadsEvent!=0){
    H_TS_Core->BlockThreadsEvent=0;

    for(i=0;i<vH_TS_BlockThreadListNum;i++){
      if(H_TS_Core->BlockThreadsEvents[i]!=0){
        H_TS_Core->BlockThreadsEvents[i]=0;

        //处理直到第一个节点不为等待就绪线程或者链表空
        p=H_TS_Core->BlockThreads[i];
        while(p!=NULL){
          if(p->Status==vH_TS_Thread_Status_Ready){

            //从阻塞线程链表中移除(第一个节点)
            p_next=p->List.Block.Next;
            if(p_next!=NULL){
              p_next->List.Block.Last=NULL;
            }

            H_TS_Core->BlockThreadsItemCnt[i]--;
            
            //添加到就绪链表
            H_TS_ThreadAddToReadyList(p);
            
            p=p_next;

          }else{
            break;
          }
        }
        H_TS_Core->BlockThreads[i]=p;

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

              H_TS_Core->BlockThreadsItemCnt[i]--;

              //添加到就绪链表
              H_TS_ThreadAddToReadyList(p);

            }

            p=p_next;
          }
        }

      }
    }
  }


  p_last=H_TS_Core->RunThreadPtr[0][1];
  p_next=H_TS_Core->ReadyThreads;
  //下次运行的线程更新
  if(p_last!=p_next){
    H_TS_Core->RunThreadPtr[0]=p_next->StackPointer;

#if (vH_TS_isEnableStackUsageStatistics != 0)||(vH_TS_isEnableCPULoadCalculate != 0)
    p=H_TS_Core->IdleThread;
    if(p_next==p){
#if vH_TS_isEnableStackUsageStatistics != 0
      H_TS_Core->IdleThreadResume=-1;
#endif

#if vH_TS_isEnableCPULoadCalculate != 0
      if(p_last!=p){
        CPULoadCalculate(0);
      }
    }else{
      if(p_last==p){
        CPULoadCalculate(-1);
      }
#endif
    }
#endif

    return vH_TS_VoidPtrMaxValue;
  }
  
  return (void*)0;
}
