

;底层汇编文件
;移植时,需要处理
;根据平台修改任务切换汇编代码
;将此文件的PendSV_Handler设置为PendSV中断调用



  AREA |.text|,CODE,READONLY
	THUMB
	PRESERVE8

	;对外接口
	EXPORT PendSV_Handler
	EXPORT H_TS_StartFirstThread

	;引入声明
	EXTERN H_TS_PendSV_Call
	EXTERN H_TS_RunThreadPtr
	EXTERN H_TS_RunThreadPtr_Refresh


H_TS_ThreadSwitch PROC
	LDR       R0,=H_TS_RunThreadPtr
	LDR       R12,[R0]                   ;读取栈指针地址

	PUSH      {R12,LR}
	BL        H_TS_RunThreadPtr_Refresh  ;切换Thread
	POP       {R12,LR}

	CMP       R0,#0                      ;结果是否为0
	BXEQ      LR                         ;如果为0 返回 不做任务切换

	;开始任务切换 保存和释放堆栈
	MRS       R0,PSP                     ;读线程栈指针到R0

	;判断线程是否使用了FPU 如果使用了 保存浮点寄存器
	TST       LR,#0x10
	VSTMDBEQ  R0!, {S16-S31}
	
	STMDB     R0!, {R4-R11,LR}           ;保存现场

	STR       R0,[R12]                   ;保存栈指针

	LDR       R0,=H_TS_RunThreadPtr
	LDR       R0,[R0]                    ;读取栈指针地址
	LDR       R0,[R0]                    ;读取栈指针

	LDMIA     R0!,{R4-R11,LR}            ;恢复现场

	;判断线程是否使用了FPU 如果使用了 恢复浮点寄存器
	TST       LR,#0x10
	VLDMIAEQ  R0!,{S16-S31}

	MSR       PSP,R0                     ;恢复堆栈指针

	BX        LR
	ENDP

H_TS_StartFirstThread PROC
	LDR       R0,=H_TS_PendSV_Call
	LDR       R1,=H_TS_ThreadSwitch
	STR       R1,[R0]
	
	MOV       R0,#0
	MSR       CONTROL,R0                 ;设置CONTROL 特权级,使用MSP (主要是为了清除FPCA)

	LDR       R1,=H_TS_RunThreadPtr      ;加载Thread地址
	LDR       R1,[R1]                    ;读取栈指针地址
	LDR       R0,[R1]                    ;读取栈指针

	LDMIA     R0!,{R4-R11,LR}            ;恢复现场
	MSR       PSP,R0                     ;恢复堆栈指针
	BX        LR
	ENDP

;需要将下面的方法设为PendSV中断调用
PendSV_Handler PROC
	LDR       R0,=H_TS_PendSV_Call
	LDR       R0,[R0]
	BX        R0
	ENDP

	END

