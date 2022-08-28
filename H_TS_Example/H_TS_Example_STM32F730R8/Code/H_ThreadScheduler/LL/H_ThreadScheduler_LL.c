/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-01 20:56:08
 * @LastEditTime: 2021-10-24 21:33:03
 * @LastEditors: 0x9DEFA478
 * @Description: 底层实现 移植时部分需要处理
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
#include "H_ThreadScheduler_LL.h"
#include "..\API\H_ThreadScheduler_Core.h"

extern H_TS* volatile H_TS_Core;


void* volatile * volatile H_TS_RunThreadPtr;
void* H_TS_PendSV_Call;



#if vH_TS_isEnableCPULoadCalculate != 0

/**
 * @brief 返回上次调用该函数是多少时间之前 该方法在使能cpu占用率统计后被使用
 * 根据平台修改
 * @return 上次调用该函数是多少时间之前
 */
H_TS_Tick H_TS_GetDT(){
  static H_TS_Tick lastT=0;
  H_TS_Tick nowT;
  H_TS_Tick r;

  nowT=(H_TS_Tick)TIM5->CNT;

  r=nowT-lastT;
  lastT=nowT;

  return r;
}

#endif

/**
 * @brief 设置Systick回调 并且使能
 * 根据平台修改
 * @param callback 使能SysTick回调
 * @param v 回调传入参数
 * @return 无
 */
void H_TS_EnableTickCallback(void (*callback)(void*),void* v){
  //该方法在开始调度时被调用
  //在这里将callback设置为SysTick回调 使得每次Systick中断callback都会被调用一次

  TimeTick_Set_IRQ_Callback(callback,v);//Systick回调设置
}

/**
 * @brief 初始化堆栈
 * 根据平台修改
 * @param StackPtr 为堆栈内存首地址指针的指针 初始化完毕后用于返回初始化好的堆栈指针
 * @param StackSize 堆栈大小
 * @param Code 线程执行的代码
 * @param v 线程传入参数
 * @param ReturnCallback 线程方法返回后调用的方法
 * @return 无
 */
void H_TS_ThreadStackInit(void* StackPtr,int StackSize,int (*Code)(void*),void* v,void (*ReturnCallback)(int)){
  //该方法在线程开始时被调用

#if vH_TS_StackPtrDirection == 0

  unsigned int mStackPtr;

  //栈指针指向末尾(刚好超出栈内存空间 代表栈为空)
  ((void**)StackPtr)[0]=(void*)&((Hbyte_ptr)*((void**)StackPtr))[StackSize];

  mStackPtr=((unsigned int*)StackPtr)[0];

  
  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=(void*)0x01000000U;//xPSR

  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=Code;//PC
  
  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=ReturnCallback;//LR 当线程使用return返回 跳转到ReturnCallback

#if 0

  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=ReturnCallback;//R12

  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=ReturnCallback;//R3

  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=ReturnCallback;//R2

  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=ReturnCallback;//R1

  mStackPtr-=sizeof(void*);
#else

  //R1 R2 R3 R12
  mStackPtr-=5*sizeof(void*);

#endif
  
  (*((void**)mStackPtr))=v;//R0


  //下面模拟被保护的现场(实际线程还没开始运行 虚构一个现场)为了线程切换出栈用

  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=(void*)0xFFFFFFFDU;//LR 退回到线程模式 使用PSP

#if 0
  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=(void*)0;//R11

  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=(void*)0;//R10

  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=(void*)0;//R9

  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=(void*)0;//R8

  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=(void*)0;//R7

  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=(void*)0;//R6

  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=(void*)0;//R5

  mStackPtr-=sizeof(void*);
  (*((void**)mStackPtr))=(void*)0;//R4
#else
  mStackPtr-=8*sizeof(void*);
#endif

  ((unsigned int*)StackPtr)[0]=mStackPtr;
#else
#error "未编写"
#endif


}

/**
 * @brief 线程通过return退出时会触发调用的方法 (如果机器不支持return转到函数 此方法可能不可用)
 * @param retVal 线程的返回值
 * @return 无
 */
void ThreadReturnCallback(int retVal){
  H_TS_Thread* _this;

  _this=H_TS_Core->RunThreadPtr[0][1];

#if vH_TS_isStoreThreadRetVal != 0
  _this->RetVal=retVal;
#endif

  //可以在此截获线程退出信息

  H_TS_ThreadExit();
}






