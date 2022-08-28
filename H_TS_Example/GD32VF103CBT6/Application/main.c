/*!
    \file  main.c
    \brief running led demo

    \version 2019-06-05, V1.0.0, demo for GD32VF103
*/

/*
    Copyright (c) 2019, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32vf103.h"
#include "Peripheral.h"
#include "H_ThreadScheduler.h"
#include "System/System.h"


extern void Debug_Execute(void* Buffer,int MaxLength);
static Hbyte Buffer[2048];

int Thread_R(void* v){

  for(;;){
    H_TS_ThreadSleep((int)v);
    oIP_GPIO_OutputBitReverse(GPIOC,13);
  }
}

int Thread_G(void* v){

  for(;;){
    H_TS_ThreadSleep((int)v);
    oIP_GPIO_OutputBitReverse(GPIOA,1);
  }
}

int Thread_B(void* v){

  for(;;){
    H_TS_ThreadSleep((int)v);
    oIP_GPIO_OutputBitReverse(GPIOA,2);
  }
}




//应用线程堆栈优先级
#define vapp_main_Priority         0

//应用线程堆栈大小
#define vapp_main_StackSize        1024


//主线程函数, 不要在其他地方调用这个
int app_main(void* v){

  H_TS_GetThisThread()->ThreadName="app_main";

  extern int stressTest(void);
  if(stressTest()!=0){
    return 0;
  }

  H_TS_StartNewThread(Thread_R,(void*)400,512,-1,0)->ThreadName="Thread_R";
  H_TS_StartNewThread(Thread_G,(void*)800,512,-1,0)->ThreadName="Thread_G";
  H_TS_StartNewThread(Thread_B,(void*)1600,512,1,0)->ThreadName="Thread_B";

  for(;;){
    Debug_Execute(Buffer,2048);
  }
}







/**
 * @brief 开始线程方法
 * @param v ...
 * @return 0
 */
int Thread_Start(void* v){

  //初始化外设
  Peripherals_Init();
    
  H_TS_StartNewThread(app_main,NULL,vapp_main_StackSize,vapp_main_Priority,0);
    
  //结束线程
  return 0;
}


static void IdleProcess(void* v){
  //__WFI();
}


int main(void){
  
  H_TS_Init(NULL,1024);

  //需要在此将系统初始化到可以开始任务调度的状态
  //此时H_TS已经完成了基本初始化(已经使用了H_TS_Init()) 此时可以新建线程 但线程不会立即运行
  //此函数退出后H_TS_StartScheduler()被调用 开始任务调度
  //建议此时不要开启其他涉及到H_TS的中断 在开始任务调度后再打开这些中断

  //传入的函数会在H_TS空闲时被循环调用
  H_TS_SetIdleProcess(IdleProcess);

  //系统初始化
  Peripherals_SysInit();

  //开始一个线程
  H_TS_StartNewThread(Thread_Start,NULL,1024,vH_TS_ThreadMaxPriorityValue-1,0);

  H_TS_StartScheduler();
  for(;;){
  }
}


void eclic_bwei_handler(){
  for(;;){
    //noEdit
  }
}



