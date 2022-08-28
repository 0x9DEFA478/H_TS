

//底层汇编文件
//移植时,需要处理
//根据平台修改任务切换汇编代码
//将此文件的eclic_msip_handler设置为软件中断调用



  //宏包含
#include "riscv_encoding.h"

  //宏定义
.macro H_TS_PushContext0
  addi      sp,sp,4*-20

  sw        ra,4*0(sp)
  sw        tp,4*1(sp)
  sw        t0,4*2(sp)
  sw        t1,4*3(sp)
  sw        t2,4*4(sp)
  sw        a0,4*5(sp)
  sw        a1,4*6(sp)
  sw        a2,4*7(sp)
  sw        a3,4*8(sp)
  sw        a4,4*9(sp)
  sw        a5,4*10(sp)
  sw        a6,4*11(sp)
  sw        a7,4*12(sp)
  sw        t3,4*13(sp)
  sw        t4,4*14(sp)
  sw        t5,4*15(sp)
  sw        t6,4*16(sp)

  csrrwi    zero,CSR_PUSHMCAUSE,17
  csrrwi    zero,CSR_PUSHMEPC,18
  csrrwi    zero,CSR_PUSHMSUBM,19
.endm
.macro H_TS_PopContext0
  lw        t0,4*19(sp)
  csrw      CSR_MSUBM,t0
  lw        t0,4*18(sp)
  csrw      CSR_MEPC,t0
  lw        t0,4*17(sp)
  csrw      CSR_MCAUSE,t0

  lw        ra,4*0(sp)
  lw        tp,4*1(sp)
  lw        t0,4*2(sp)
  lw        t1,4*3(sp)
  lw        t2,4*4(sp)
  lw        a0,4*5(sp)
  lw        a1,4*6(sp)
  lw        a2,4*7(sp)
  lw        a3,4*8(sp)
  lw        a4,4*9(sp)
  lw        a5,4*10(sp)
  lw        a6,4*11(sp)
  lw        a7,4*12(sp)
  lw        t3,4*13(sp)
  lw        t4,4*14(sp)
  lw        t5,4*15(sp)
  lw        t6,4*16(sp)

  addi      sp,sp,4*20
.endm
.macro H_TS_PushContext1
  addi      sp,sp,4*-12
  
  sw        s0,4*0(sp)
  sw        s1,4*1(sp)
  sw        s2,4*2(sp)
  sw        s3,4*3(sp)
  sw        s4,4*4(sp)
  sw        s5,4*5(sp)
  sw        s6,4*6(sp)
  sw        s7,4*7(sp)
  sw        s8,4*8(sp)
  sw        s9,4*9(sp)
  sw        s10,4*10(sp)
  sw        s11,4*11(sp)
.endm
.macro H_TS_PopContext1
  lw        s0,4*0(sp)
  lw        s1,4*1(sp)
  lw        s2,4*2(sp)
  lw        s3,4*3(sp)
  lw        s4,4*4(sp)
  lw        s5,4*5(sp)
  lw        s6,4*6(sp)
  lw        s7,4*7(sp)
  lw        s8,4*8(sp)
  lw        s9,4*9(sp)
  lw        s10,4*10(sp)
  lw        s11,4*11(sp)

  addi      sp,sp,4*12
.endm

  //引入声明
  .extern H_TS_ScheduleISR_Call
  .extern H_TS_RunThread
  .extern H_TS_RunThread_Refresh

  .section .text
  .align 2
H_TS_ThreadSwitch:
  lw        t0,0(tp)                   //读取原来线程的栈指针地址 &H_TS_RunThread[0]

  addi      sp,sp,4*-1
  sw        t0,4*0(sp)
  call      H_TS_RunThread_Refresh     //更新当前线程信息
  lw        t0,4*0(sp)
  addi      sp,sp,4*1

  beqz      a0,1f                      //如果为0 跳转到返回 不做任务切换


  //开始任务切换 保存和释放堆栈
  
  H_TS_PushContext1                    //保存现场

  sw        sp,0(t0)                   //保存栈指针 到原来线程的H_TS_RunThread[0]

  lw        t0,0(tp)                   //读取新的栈指针地址 &H_TS_RunThread[0]
  lw        sp,0(t0)                   //读取栈指针 H_TS_RunThread[0]

0:
  H_TS_PopContext1                     //恢复现场

1:                                     //异常返回
  csrc      CSR_MSTATUS,MSTATUS_MIE
  H_TS_PopContext0
  mret

  .align 2
  .global H_TS_StartFirstThread
H_TS_StartFirstThread:
  la        t0,H_TS_ScheduleISR_Call
  la        t1,H_TS_ThreadSwitch
  sw        t1,0(t0)

  la        tp,H_TS_RunThread          //加载Thread地址
  lw        t1,0(tp)                   //读取栈指针地址 &H_TS_RunThread[0]
  lw        sp,0(t1)                   //读取栈指针 H_TS_RunThread[0] 顺便恢复堆栈指针

  j         0b                         //借用上面的代码来返回(话说riscv的寄存器可真多 现场保护代码又臭又长)

  .align 2
  .global eclic_msip_handler
//需要将下面的方法设为软件中断调用
eclic_msip_handler:
  H_TS_PushContext0
  csrs      CSR_MSTATUS,MSTATUS_MIE

  //清除软件中断触发位
  li        t0,0xD1000FFC
  sw        zero,0(t0)

  la        t0,H_TS_ScheduleISR_Call
  lw        t0,0(t0)
  jr        t0



