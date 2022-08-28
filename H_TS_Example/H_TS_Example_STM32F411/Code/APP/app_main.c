/*
 * @Author: 0x9DEFA478
 * @Date: 2021-10-23 23:45:32
 * @LastEditTime: 2022-01-14 17:14:52
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

H_TS_Thread* app_main_ThreadHandle;


H_TS_MessageQueue* LedQueue;


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


H_TS_Thread* Serial_Thread_Handle;
int Serial_Thread(void* v){
  int length;
  int MaxLength;
  Hbyte* str;

  unsigned char Buffer[128];
  H_Malloc_Info info;
  H_TS_ThreadInfo* thread_info;
  H_TS_ThreadInfo* thread_info_item;

  MaxLength=sizeof(Buffer);

  

  for(;;){

    str=Buffer;
    
    length=I_DebugPort_Read(Buffer,1);
    if(length<=0){
      continue;
    }

    if(str[0]==' '){//指令标识符

      length=I_DebugPort_Read(Buffer,MaxLength);
      if(length<=0){
        continue;
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

          H_Printf("线程信息:\r\n  线程名:{s}\r\n  优先级:{i}\r\n  堆栈占用:{i}/{i}={f.2}%\r\n\r\n",str,thread_info_item->Priority,thread_info_item->StackMaxUsage,thread_info_item->StackSize,100.0f*(float)thread_info_item->StackMaxUsage/thread_info_item->StackSize);
          
          thread_info_item=thread_info_item->Next;
        }

        H_TS_ThreadInfoFree(thread_info);


      }else if(0!=IsCMD(str,length,"ReverseLED0")){

        H_TS_MessageQueueSend(LedQueue,GPIOA,(void*)9);

      }else if(0!=IsCMD(str,length,"ReverseLED1")){

        H_TS_MessageQueueSend(LedQueue,GPIOA,(void*)10);

      }else{
        H_Printf("无效指令\r\n");
      }


    }

  }
}

H_TS_Thread* Led_Thread0_Handle;
int Led_Thread0(void* v){
  void* v0;
  void* v1;

  RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;

  //GPIO初始化
  oIP_GPIO_Config(GPIOA,10,vIP_GPIO_Mode_OUT,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_LOW,vIP_GPIO_PuPd_NP);

  for(;;){
    H_TS_MessageQueueReceive(LedQueue,&v0,&v1);
    oIP_GPIO_OutputBitReverse(((GPIO_TypeDef*)v0),((int)v1));//强制转型 反转LED
  }

}

H_TS_Thread* Led_Thread1_Handle;
int Led_Thread1(void* v){
  int i;

  RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;

  //GPIO初始化
  oIP_GPIO_Config(GPIOA,9,vIP_GPIO_Mode_OUT,vIP_GPIO_OutType_PP,vIP_GPIO_Speed_LOW,vIP_GPIO_PuPd_NP);

  i=10;
  while(i>0){
    i--;

    H_TS_ThreadSleep(500);
    oIP_GPIO_OutputBitReverse(GPIOA,9);
  }

  //线程退出
  return 0;
}


void stressTest();

//主线程函数, 不要在其他地方调用这个
int app_main(void* v){

  app_main_ThreadHandle=H_TS_GetThisThread();
  if(cH_IsNull(app_main_ThreadHandle)){
    for(;;){
    }
  }
  app_main_ThreadHandle->ThreadName="app_main";

  LedQueue=new_H_TS_MessageQueue(8);//新建队列
  
  Led_Thread0_Handle=H_TS_StartNewThread_HighPriority(Led_Thread0,NULL,256,0,0U);
  Led_Thread1_Handle=H_TS_StartNewThread_HighPriority(Led_Thread1,NULL,256,0,0U);
  Serial_Thread_Handle=H_TS_StartNewThread_HighPriority(Serial_Thread,NULL,2048,0,0U);
  if(cH_IsNull(Led_Thread0_Handle)||cH_IsNull(Led_Thread1_Handle)||cH_IsNull(Serial_Thread_Handle)){
    for(;;){
    }
  }
  Led_Thread0_Handle->ThreadName="Led_Thread0";
  Led_Thread1_Handle->ThreadName="Led_Thread1";
  Serial_Thread_Handle->ThreadName="Serial_Thread";


  stressTest();


  return 0;
}




#define Stress_Check_T                                            100
//如果Stress_IsTestISR不为0 则将释放信号量操作让SVC中断完成 这可以模拟中断操作信号量的情况
#define Stress_IsTestISR                                          1
//占位线程数量 将会创建Stress_IsExtThread个线程, 这些线程保持阻塞状态, 模拟阻塞链表超长的情况
#define Stress_IsExtThread                                        0

typedef struct _StressData{
  H_TS_BinarySemaphore* bsem0;
  H_TS_BinarySemaphore* bsem1;
  volatile int cnt0;
  volatile int cnt1;
}StressData;

//用于调度测试
int stress0_Thread(void* v){

  StressData* stressData;

  stressData=v;

  for(;;){
    H_TS_BinarySemaphoreTake(stressData->bsem0);
    stressData->cnt0++;
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
  float avg;
  int lastT;

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

    if((System_GetTime_ms()-lastT)>99){
      lastT+=100;

      speed0=stressData->cnt0-cnt0;
      cnt0+=speed0;

      speed1=stressData->cnt1-cnt1;
      cnt1+=speed1;

      avg+=0.1f*((float)(speed0+speed1)-avg);//低通滤波

      Printf("CPU占用率:%.2f%%\r\nspeed:%d/%dms\r\navg:%.2f/%dms\r\n",(float)H_TS_GetCPU_Load()/10.0f,speed0+speed1,Stress_Check_T,avg,Stress_Check_T);
      //H_Printf("CPU占用率:{f.2}%\r\nspeed:{i}/{i}ms\r\navg:{f.2}/{i}ms\r\n",(float)H_TS_GetCPULoad()/10.0f,speed0+speed1,Stress_Check_T,avg,Stress_Check_T);
    
    }

    
  }
}

//占位线程 创建多个这样的线程模拟多线程阻塞情况
int stress_OccThread(void* v){
  
  H_TS_BinarySemaphore* sem;

  sem=new_H_TS_BinarySemaphore();
  H_TS_BinarySemaphoreTake(sem);//阻塞就完事了

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

芯片条件: stm32f411 96MHz Flash配置: 指令预取开 指令缓存开 数据缓存开

  ARM编译器V5

  配置条件:
    解阻塞FIFO vH_TS_NumOfUnblockFIFOs==0
    阻塞链表数量vH_TS_BlockThreadListNum==2
    占位线程数量Stress_IsExtThread==0

                                       -O3优化+时间优化                  -O0优化

    Thread(Stress_IsTestISR==0)    avg:31472.17/100ms 3.18us     avg:22439.97/100ms 4.45us
    
    SVC_ISR(Stress_IsTestISR!=0)   avg:26834.33/100ms 3.73us     avg:18998.03/100ms 5.26us
    *后面us为单位的为每次调度所用的时间 100000us/cnt

  基础配置下的结果用于后面的对比，仅本条有-O0的测试结果(每次都测-O0优化太麻烦了)


  配置条件:
    解阻塞FIFO vH_TS_NumOfUnblockFIFOs==0
    阻塞链表数量vH_TS_BlockThreadListNum==8
    占位线程数量Stress_IsExtThread==0

                                       -O3优化+时间优化 
    			
    Thread(Stress_IsTestISR==0)    avg:31327.76/100ms 3.19us
    
    SVC_ISR(Stress_IsTestISR!=0)   avg:25189.82/100ms 3.97us

  vH_TS_BlockThreadListNum的增加对线程触发的调度的速度无影响, 中断触发的调度延迟(调度延迟是遍历链表导致的，下同)变大


  配置条件:
    解阻塞FIFO vH_TS_NumOfUnblockFIFOs==0
    阻塞链表数量vH_TS_BlockThreadListNum==2
    占位线程数量Stress_IsExtThread==48

                                       -O3优化+时间优化
    			
    Thread(Stress_IsTestISR==0)    avg:31561.75/100ms 3.17us
    
    SVC_ISR(Stress_IsTestISR!=0)   avg:20720.91/100ms 4.82us

  增加占位线程后, 中断触发的调度延迟变大, 线程触发的调度延迟不变


  配置条件:
    解阻塞FIFO vH_TS_NumOfUnblockFIFOs==1
    阻塞链表数量vH_TS_BlockThreadListNum==2
    占位线程数量Stress_IsExtThread==48

                                       -O3优化+时间优化
    			
    Thread(Stress_IsTestISR==0)    avg:30602.68/100ms 3.27us
    
    SVC_ISR(Stress_IsTestISR!=0)   avg:23858.25/100ms 4.19us

  启用解阻塞FIFO(vH_TS_NumOfUnblockFIFOs不为0)后, 线程之间的调度延迟略微变大(调度中断多了对FIFO是否为空的判断)
  但是, 大量阻塞线程导致的中断触发的调度延迟被减轻了


  配置条件:
    解阻塞FIFO vH_TS_NumOfUnblockFIFOs==2
    阻塞链表数量vH_TS_BlockThreadListNum==2
    占位线程数量Stress_IsExtThread==48

                                       -O3优化+时间优化
    			
    Thread(Stress_IsTestISR==0)    avg:30675.94/100ms 3.25us
    
    SVC_ISR(Stress_IsTestISR!=0)   avg:23169.62/100ms 4.31us

  vH_TS_NumOfUnblockFIFOs变为2后, 中断调度延迟略微变大, 
  实际情况中, 多个中断重入访问FIFO(在中断访问部件时可能发生)的概率很低的情况下，vH_TS_NumOfUnblockFIFOs设为2或以上就有点浪费了
  在有大量中断频繁调用部件时令vH_TS_NumOfUnblockFIFOs为2较好
  *vH_TS_NumOfUnblockFIFOs 解阻塞FIFO数,为了对中断嵌套访问, 可以有多个FIFO,发生中断嵌套访问时每个中断访问不同的FIFO
  *如果FIFO不足, 更高级嵌套的中断将使用原始的解阻塞方式(即相当于vH_TS_NumOfUnblockFIFOs==0)


更新后的测试 该版本已经取消了解阻塞FIFO 取而代之的是快速通道(快速通道总是使能的)

  配置条件:
    阻塞链表数量vH_TS_BlockThreadListNum==1
    占位线程数量Stress_IsExtThread==0

                                       -O3优化+时间优化
    			
    Thread(Stress_IsTestISR==0)    avg:31375.51/100ms 3.18us
    
    SVC_ISR(Stress_IsTestISR!=0)   avg:27373.31/100ms 3.65us


*/






