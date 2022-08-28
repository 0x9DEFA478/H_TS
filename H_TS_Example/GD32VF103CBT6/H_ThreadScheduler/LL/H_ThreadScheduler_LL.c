/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-01 20:56:08
 * @LastEditTime: 2022-01-12 23:24:31
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
#include "./H_ThreadScheduler_LL.h"
#include "../API/H_ThreadScheduler_Core.h"

extern H_TS* H_TS_Core;


void* volatile * volatile H_TS_RunThread;//调度中断调度判据 指向 (void*)[2] 的内存空间 [0]指向栈顶内存 [1]
void* H_TS_ScheduleISR_Call;//供 LL_ASM使用 调度中断将调用此段指向的方法



#if vH_TS_IsEnableCPULoadCalculate != 0

H_TS_Time H_TS_GetDT_LastT=0;

#endif




/**
 * @brief 初始化堆栈
 * 根据平台修改
 * @param StackPtr 为堆栈内存首地址指针的指针 初始化完毕后用于返回初始化好的堆栈指针
 * @param StackSize 堆栈大小
 * @param Code 线程执行的代码
 * @param v 线程传入参数
 * @return 无
 */
void H_TS_ThreadStackInit(void* StackPtr,Hsize StackSize,int (*Code)(void*),void* v){
  //该方法在线程开始时被调用

#if vH_TS_StackPtrDirection == 0

  Huint32 _StackPtr;

  //栈指针指向末尾(刚好超出栈内存空间 代表栈为空)
  _StackPtr=((Huint32*)StackPtr)[0]+StackSize;

  _StackPtr-=sizeof(void*)*32;
  // ((void**)_StackPtr)[0]=(void*)0U;//s0
  // ((void**)_StackPtr)[1]=(void*)0U;//s1
  // ((void**)_StackPtr)[2]=(void*)0U;//s2
  // ((void**)_StackPtr)[3]=(void*)0U;//s3
  // ((void**)_StackPtr)[4]=(void*)0U;//s4
  // ((void**)_StackPtr)[5]=(void*)0U;//s5
  // ((void**)_StackPtr)[6]=(void*)0U;//s6
  // ((void**)_StackPtr)[7]=(void*)0U;//s7
  // ((void**)_StackPtr)[8]=(void*)0U;//s8
  // ((void**)_StackPtr)[9]=(void*)0U;//s9
  // ((void**)_StackPtr)[10]=(void*)0U;//s10
  // ((void**)_StackPtr)[11]=(void*)0U;//s11
  
  void ThreadReturnCallback(int RetVal);
  ((void**)_StackPtr)[12]=(void*)ThreadReturnCallback;//ra 当线程使用return返回 跳转到ReturnCallback
  ((void**)_StackPtr)[13]=(void*)&H_TS_RunThread;//tp
  // ((void**)_StackPtr)[14]=(void*)0U;//t0
  // ((void**)_StackPtr)[15]=(void*)0U;//t1
  // ((void**)_StackPtr)[16]=(void*)0U;//t2
  ((void**)_StackPtr)[17]=v;//a0
  // ((void**)_StackPtr)[18]=(void*)0U;//a1
  // ((void**)_StackPtr)[19]=(void*)0U;//a2
  // ((void**)_StackPtr)[20]=(void*)0U;//a3
  // ((void**)_StackPtr)[21]=(void*)0U;//a4
  // ((void**)_StackPtr)[22]=(void*)0U;//a5
  // ((void**)_StackPtr)[23]=(void*)0U;//a6
  // ((void**)_StackPtr)[24]=(void*)0U;//a7
  // ((void**)_StackPtr)[25]=(void*)0U;//t3
  // ((void**)_StackPtr)[26]=(void*)0U;//t4
  // ((void**)_StackPtr)[27]=(void*)0U;//t5
  // ((void**)_StackPtr)[28]=(void*)0U;//t6
  ((void**)_StackPtr)[29]=(void*)((0x1U<<31)|(0x3U<<28)|(0x1U<<27)|(0x003U<<0));//mcause
  ((void**)_StackPtr)[30]=Code;//mepc PC
  ((void**)_StackPtr)[31]=(void*)(0x1U<6);//msubm


  ((Huint32*)StackPtr)[0]=_StackPtr;
#else
#error "未编写"
#endif


}

/**
 * @brief 线程通过return退出时会触发调用的方法
 * @param RetVal 线程的返回值
 * @return 无
 */
void ThreadReturnCallback(int RetVal){
  // H_TS_Thread* _this;

  // _this=cH_TS_RunThread(H_TS_Core)[1];

  //虽然线程通过return返回会转到这里 但其实线程还在运行 只是在执行这里的代码而已
  
  //可以在此截获线程退出信息

  H_TS_ThreadExit(RetVal);//实际上还是要调用线程退出方法
}






