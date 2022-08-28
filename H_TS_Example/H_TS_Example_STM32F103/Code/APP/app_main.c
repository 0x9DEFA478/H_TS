/*
 * @Author: 0x9DEFA478
 * @Date: 2021-10-23 23:45:32
 * @LastEditTime: 2022-01-14 00:42:30
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
#include "app_main.h"
#include "H_ThreadScheduler.h"

#include "Peripheral.h"

static int IsCMD(Hbyte* Buffer,int BufferLength,char* CMD){

  while(BufferLength>0){
    BufferLength--;

    if(CMD[0]=='\0'){
      return 0;
    }

    if(CMD[0]!=Buffer[0]){
      return 0;
    }

    CMD=&CMD[1];
    Buffer=&Buffer[1];
  }

  return -1;
}

//捕捉一个指令 并作出响应
void Debug_Execute(void* Buffer,int MaxLength){
  int length;
  int i;
  int Time;
  Hbyte* str;

  H_Malloc_Info info;
  H_TS_ThreadInfo* thread_info;
  H_TS_ThreadInfo* thread_info_item;

  str=Buffer;

  
  length=I_DebugPort_Read(Buffer,1);
  if(length<=0){
    return;
  }

  if(str[0]==' '){//指令标识符

    length=I_DebugPort_Read(Buffer,MaxLength);
    if(length<=0){
      return;
    }


    if(0!=IsCMD(str,length,"GetStackInfo")){

      MemoryMalloc_SRAM.GetInfo(&info);
      H_Printf("SRAM:{f.2}%\r\n  Result:{i}\r\n  可使用:{i}Byte\r\n  总共:{i}Byte\r\n",100.0f-(float)info.FreeSize*100/(info.NoOccupySize+info.OccupySize)
        ,info.Result,info.FreeSize,info.NoOccupySize+info.OccupySize);
      
      
      H_Printf("RTOS堆栈占用率:{f.2}%\r\n",(float)H_TS_GetStackUtilization()/10.0f);
      

      H_Printf("线程堆栈:{i}Byte\r\n",H_TS_GetThisThread()->StackMaxUsage);


      H_Printf("CPU占用率:{f.2}%\r\n",(float)H_TS_GetCPU_Load()/10.0f);

    }else if(0!=IsCMD(str,length,"GetThreadInfo")){

      thread_info=H_TS_ThreadInfoGet();

      thread_info_item=thread_info;
      while(thread_info_item!=NULL){
        
        str=thread_info_item->ThreadName;
        if(str==NULL){
          str="<未命名>";
        }

        H_Printf("线程信息:\r\n  线程名:{s}\r\n  CPU占用率:{f.2}%\r\n  优先级:{i}\r\n  堆栈占用:{i}/{i}={f.2}%\r\n\r\n",str,(float)thread_info_item->CPU_Load/10.0f,thread_info_item->Priority,thread_info_item->StackMaxUsage,thread_info_item->StackSize,100.0*(float)thread_info_item->StackMaxUsage/thread_info_item->StackSize);
        
        thread_info_item=thread_info_item->Next;
      }

      H_TS_ThreadInfoFree(thread_info);


    }else if(0!=IsCMD(str,length,"Block")){
      H_Printf("阻塞一段时间...\r\n");
      HAL_Delay(1024);
      H_Printf("阻塞停止\r\n");
    }else{
      H_Printf("无效指令\r\n");
    }


  }

  
}




#define vStressTest_USE                                           1

#if vStressTest_USE != 0
void stressTest();
#endif






//主线程函数, 不要在其他地方调用这个
int app_main(void* v){
  void* Buffer;
  
  H_TS_GetThisThread()->ThreadName="app_main";


#if vStressTest_USE != 0
  stressTest();
#endif
  
  Buffer=MemoryMalloc_SRAM.Malloc(2048);
  for(;;){
    
    Debug_Execute(Buffer,2048);
  }


  return 0;
}





#if vStressTest_USE != 0

#define Stress_Check_T                                            100
//如果Stress_IsTestISR不为0 则将释放信号量操作让SVC中断完成 这可以模拟中断操作信号量的情况
#define Stress_IsTestISR                                          0
//占位线程数量 将会创建Stress_IsExtThread个线程, 这些线程保持阻塞状态, 模拟阻塞链表超长的情况
#define Stress_IsExtThread                                        0
//每多少计数后睡眠1ms 这用与防止两个线程之间调度过于频繁导致Tick中断无从插手 导致无法输出统计信息(可设为0代表不使用)
#define Stress_IsSleepPerCnt                                      1000000

typedef struct _StressData{
  H_TS_BinarySemaphore* bsem0;
  H_TS_BinarySemaphore* bsem1;
  volatile int cnt0;
  volatile int cnt1;
}StressData;

//用于调度测试
int stress0_Thread(void* v){

#if Stress_IsSleepPerCnt > 0
  int cnt0;
#endif

  StressData* stressData;

  stressData=v;

#if Stress_IsSleepPerCnt > 0
  cnt0=0;
#endif
  for(;;){
    H_TS_BinarySemaphoreTake(stressData->bsem0);
    stressData->cnt0++;

#if Stress_IsSleepPerCnt > 0
    cnt0++;
    if(cnt0>Stress_IsSleepPerCnt){
      cnt0=0;
      H_TS_ThreadSleep(1);//防止因调度过于频繁导致的Tick中断不能生效
    }
#endif

#if Stress_IsTestISR == 0
    H_TS_BinarySemaphoreGive(stressData->bsem1);
#else
    SVC_Call((void* (*)(void *))H_TS_BinarySemaphoreGive_ISR,stressData->bsem1);
#endif
  }
}

//用于调度测试
int stress1_Thread(void* v){

  StressData* stressData;

  stressData=v;

  for(;;){
    H_TS_BinarySemaphoreTake(stressData->bsem1);
    stressData->cnt1++;
#if Stress_IsTestISR == 0
    H_TS_BinarySemaphoreGive(stressData->bsem0);
#else
    SVC_Call((void* (*)(void *))H_TS_BinarySemaphoreGive_ISR,stressData->bsem0);
#endif
  }
}


//用于统计
int stress_Thread(void* v){

  StressData* stressData;
  int cnt0;
  int cnt1;
  int speed0;
  int speed1;
  int sum;
  float avg;
  int lastT;
  int dt;

  stressData=v;

  cnt0=0;
  cnt1=0;
  avg=0.0f;

  //万恶之源
#if Stress_IsTestISR == 0
  H_TS_BinarySemaphoreGive(stressData->bsem0);
#else
  SVC_Call((void* (*)(void *))H_TS_BinarySemaphoreGive_ISR,stressData->bsem0);
#endif

  lastT=System_GetTime_ms();
  for(;;){
    H_TS_ThreadSleep(5);

    dt=System_GetTime_ms()-lastT;
    if(dt>99){
      lastT+=dt;

      speed0=stressData->cnt0-cnt0;
      cnt0+=speed0;

      speed1=stressData->cnt1-cnt1;
      cnt1+=speed1;

      sum=speed0+speed1;
      
      avg+=0.1f*((float)(sum*Stress_Check_T)/dt-avg);//低通滤波
      sum=sum*Stress_Check_T/dt;

      Printf("CPU占用率:%.2f%%\r\nspeed:%d/%dms\r\navg:%.2f/%dms\r\n",(float)H_TS_GetCPU_Load()/10.0f,sum,Stress_Check_T,avg,Stress_Check_T);
      //H_Printf("CPU占用率:{f.2}%\r\nspeed:{i}/{i}ms\r\navg:{f.2}/{i}ms\r\n",(float)H_TS_GetCPU_Load()/10.0f,speed0+speed1,Stress_Check_T,avg,Stress_Check_T);
    
    }

    
  }
}

//占位线程 创建多个这样的线程模拟多线程阻塞情况
int stress_OccThread(void* v){
  
  H_TS_BinarySemaphore* sem;

  sem=new_H_TS_BinarySemaphore();
  H_TS_BinarySemaphoreTake(sem);//阻塞就完事了

  return 0;
}

void stressTest(){
  int i;

  StressData* stressData;

  stressData=MemoryMalloc_SRAM.Malloc(sizeof(StressData));

  stressData->cnt0=0;
  stressData->cnt1=0;
  stressData->bsem0=new_H_TS_BinarySemaphore();
  stressData->bsem1=new_H_TS_BinarySemaphore();

  H_TS_StartNewThread(stress0_Thread,stressData,768,vH_TS_ThreadMaxPriorityValue-1,0U)->ThreadName="stress0_Thread";
  H_TS_StartNewThread(stress1_Thread,stressData,768,vH_TS_ThreadMaxPriorityValue-2,0U)->ThreadName="stress1_Thread";
  H_TS_StartNewThread(stress_Thread,stressData,1536,vH_TS_ThreadMaxPriorityValue-3,0U)->ThreadName="stress_Thread";

  for(i=0;i<Stress_IsExtThread;i++){
    H_TS_StartNewThread(stress_OccThread,NULL,256,vH_TS_ThreadMaxPriorityValue-1,0U)->ThreadName="Occ";
  }

}

/*

芯片条件: stm32f103 72MHz Flash使能预取

  ARM编译器V5

  配置条件:
    阻塞链表数量vH_TS_BlockThreadListNum==1
    占位线程数量Stress_IsExtThread==0

                                       -O3优化+时间优化                  -O0优化

    Thread(Stress_IsTestISR==0)    avg:19122.60/100ms 5.23us     avg:11200.14/100ms 8.93us
    
    SVC_ISR(Stress_IsTestISR!=0)   avg:16534.39/100ms 6.05us     avg:9994.09/100ms 10.0us
    *后面us为单位的为每次调度所用的时间 100000us/cnt

  

*/

#endif






