/*
 * @Author: 0x9DEFA478
 * @Date: 2022-01-13 22:53:14
 * @LastEditTime: 2022-01-13 23:39:56
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
#include "System/System.h"
#include "H_String.h"
#include "H_ThreadScheduler.h"
#include "MemoryMalloc.h"
#include "I_DebugPort.h"
#include "H_Printf.h"
#include "Peripherals_Instence.h"
#include "IP_GPIO.h"
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

//?????????????????? ???????????????
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

  if(str[0]==' '){//???????????????

    length=I_DebugPort_Read(Buffer,MaxLength);
    if(length<=0){
      return;
    }


    if(0!=IsCMD(str,length,"GetStackInfo")){

      MemoryMalloc_SRAM.GetInfo(&info);
      H_Printf("SRAM:{f.2}%\r\n  Result:{i}\r\n  ?????????:{i}Byte\r\n  ??????:{i}Byte\r\n",100.0f-(float)info.FreeSize*100/(info.NoOccupySize+info.OccupySize)
        ,info.Result,info.FreeSize,info.NoOccupySize+info.OccupySize);
      
      H_Printf("RTOS???????????????:{f.2}%\r\n",(float)H_TS_GetStackUtilization()/10.0f);
      

      H_Printf("????????????:{i}Byte\r\n",H_TS_GetThisThread()->StackMaxUsage);


      H_Printf("CPU?????????:{f.2}%\r\n",(float)H_TS_GetCPU_Load()/10.0f);

    }else if(0!=IsCMD(str,length,"GetThreadInfo")){

      thread_info=H_TS_ThreadInfoGet();

      thread_info_item=thread_info;
      while(thread_info_item!=NULL){
        
        str=thread_info_item->ThreadName;
        if(str==NULL){
          str="<?????????>";
        }

        H_Printf("????????????:\r\n  ?????????:{s}\r\n  CPU?????????:{f.2}%\r\n  ?????????:{i}\r\n  ????????????:{i}/{i}={f.2}%\r\n\r\n",str,(float)thread_info_item->CPU_Load/10.0f,thread_info_item->Priority,thread_info_item->StackMaxUsage,thread_info_item->StackSize,100.0*(float)thread_info_item->StackMaxUsage/thread_info_item->StackSize);
        
        thread_info_item=thread_info_item->Next;
      }

      H_TS_ThreadInfoFree(thread_info);


    }else if(0!=IsCMD(str,length,"Block")){
      H_Printf("??????????????????...\r\n");
      Delay_ms(1024);
      H_Printf("????????????\r\n");
    }else{
      H_Printf("????????????\r\n");
    }


  }

  
}





#define SVC_Call Env_Call


#define vStressTest_USE 1


#if vStressTest_USE != 0

#define Stress_Check_T                                            100
//??????Stress_IsTestISR??????0 ??????????????????????????????SVC???????????? ?????????????????????????????????????????????
#define Stress_IsTestISR                                          0
//?????????????????? ????????????Stress_IsExtThread?????????, ??????????????????????????????, ?????????????????????????????????
#define Stress_IsExtThread                                        0
//????????????????????????1ms ?????????????????????????????????????????????????????????Tick?????????????????? ??????????????????????????????(?????????0???????????????)
#define Stress_IsSleepPerCnt                                      1000000

typedef struct _StressData{
  H_TS_BinarySemaphore* bsem0;
  H_TS_BinarySemaphore* bsem1;
  volatile int cnt0;
  volatile int cnt1;
}StressData;

//??????????????????
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
      H_TS_ThreadSleep(1);//????????????????????????????????????Tick??????????????????
    }
#endif

#if Stress_IsTestISR == 0
    H_TS_BinarySemaphoreGive(stressData->bsem1);
#else
    SVC_Call((void* (*)(void *))H_TS_BinarySemaphoreGive_ISR,stressData->bsem1);
#endif
  }
}

//??????????????????
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


//????????????
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

  //????????????
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
      
      avg+=0.1f*((float)(sum*Stress_Check_T)/dt-avg);//????????????
      sum=sum*Stress_Check_T/dt;

      Printf("CPU?????????:%.2f%%\r\nspeed:%d/%dms\r\navg:%.2f/%dms\r\n",(float)H_TS_GetCPU_Load()/10.0f,sum,Stress_Check_T,avg,Stress_Check_T);
      //H_Printf("CPU?????????:{f.2}%\r\nspeed:{i}/{i}ms\r\navg:{f.2}/{i}ms\r\n",(float)H_TS_GetCPU_Load()/10.0f,speed0+speed1,Stress_Check_T,avg,Stress_Check_T);
    
    }

    
  }
}

//???????????? ??????????????????????????????????????????????????????
int stress_OccThread(void* v){
  
  H_TS_BinarySemaphore* sem;

  sem=new_H_TS_BinarySemaphore();
  H_TS_BinarySemaphoreTake(sem);//??????????????????

  return 0;
}



/*


????????????: gd32vf103cbt 108MHz 

  

  ????????????:
    ??????????????????vH_TS_BlockThreadListNum==1
    ??????????????????Stress_IsExtThread==0

                                          Ofast??????                      Og??????

    Thread(Stress_IsTestISR==0)    avg:36144.78/100ms 2.77us     avg:34336.73/100ms 2.91us
    
    SVC_ISR(Stress_IsTestISR!=0)   avg:26381.21/100ms 3.79us     avg:25270.41/100ms 3.96us
    *??????us?????????????????????????????????????????? 100000us/cnt

*/

#endif

int stressTest(){
#if vStressTest_USE != 0
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

  return -1;
#else
  return 0;
#endif
}

