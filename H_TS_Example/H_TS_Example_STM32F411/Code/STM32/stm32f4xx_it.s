




	AREA |.text|,CODE,READONLY
	THUMB
	PRESERVE8

	;对外提供的接口
	EXPORT HardFault_Handler
	EXPORT MemManage_Handler
	EXPORT BusFault_Handler
	EXPORT UsageFault_Handler
	EXPORT SVC_Handler
	EXPORT SVC_Call

	;引入声明
	EXTERN HardFault_IRQ
	EXTERN MemManage_IRQ
	EXTERN BusFault_IRQ
	EXTERN UsageFault_IRQ

Fault_Execute PROC

	MOV       R3,R0
	LDR       R2,=0x00000000

	TST       LR,#0x00000004             ;判断使用的堆栈指针
	ITE EQ
	MRSEQ     R0,MSP
	MRSNE     R0,PSP

	;判断线程是否使用了FPU 如果使用了 保存浮点寄存器
	TST       LR,#0x00000010
	ITTE EQ
	VSTMDBEQ  R0!, {S16-S31}
	MVNEQ     R1,R2
	MOVNE     R1,R2
	
	STMDB     R0!, {R4-R11}              ;保存现场

	BX        R3

	ENDP

HardFault_Handler PROC

	LDR       R0,=HardFault_IRQ
	B         Fault_Execute

	ENDP

MemManage_Handler PROC

	LDR       R0,=MemManage_IRQ
	B         Fault_Execute
	
	ENDP

BusFault_Handler PROC

	LDR       R0,=BusFault_IRQ
	B         Fault_Execute
	
	ENDP

UsageFault_Handler PROC

	LDR       R0,=UsageFault_IRQ
	B         Fault_Execute
	
	ENDP

SVC_Handler PROC

	TST       LR,#0x00000004             ;判断使用的堆栈指针
	ITE EQ
	MRSEQ     R2,MSP
	MRSNE     R2,PSP

	LDR       R1,[R2]
	LDR       R0,[R2,#4]
	PUSH      {R2,LR}
	BLX       R1
	POP       {R2,LR}
	STR       R0,[R2]
	
	BX        LR
	
	ENDP

SVC_Call PROC

	SVC       0
	BX        LR

	ENDP

	NOP

	END

