	.include "2440addr.inc"
	.include "Option.inc"

	.code	32
	.text

	.extern FIQ_ISR_Vector

	.global	__start
__start:
	b	ResetHandler
	b	HandlerUndef		@ HandlerUndef
	b	HandlerSWI			@ handler for SWI interrupt
	b	HandlerPabort 		@ HandlerPabort
	b	HandlerDabort		@ HandlerDabort
	b	.					@ reserved
	b	HandlerIRQ			@ HandlerIRQ
	b	.					@ HandlerFIQ

@--------------------------------------------------
@ Reset Handler Routine
@--------------------------------------------------

	.extern	ISR_Vectors

	.global HandlerIRQ
HandlerIRQ:
	sub		sp, sp, #4
	stmfd	sp!, {r8-r9}
	ldr		r9, =INTOFFSET
	ldr		r9, [r9]
	ldr		r8, =ISR_Vectors
	ldr		r8, [r8, r9, lsl #2]
	str		r8, [sp, #8]
	ldmfd	sp!, {r8-r9, pc}

	.global	HandlerUndef
HandlerUndef:
	sub 	r0, lr, #4
	mrs		r1, spsr
	and		r1, r1, #0x1f
	ldr		pc, =Undef_Handler


	.global	HandlerPabort
HandlerPabort:
	sub 	r0, lr, #4
	mrs		r1, spsr
	and		r1, r1, #0x1f
	ldr		pc, =Pabort_Handler


	.global	HandlerDabort
HandlerDabort:
	sub 	r0, lr, #8
	mrs		r1, spsr
	and		r1, r1, #0x1f
	ldr		pc, =Dabort_Handler


@ SWI Handler

	.global HandlerSWI
HandlerSWI:
	stmfd  	sp!,{r4, r5, lr}

	@ Nested SWI Call이 가능한 구조로 설계하시오

	ldmfd	sp!,{r4, r5, pc}^

STR:
	.string "pino %x\n"
	.align 4

ResetHandler:
	ldr		r0,=WTCON				
	ldr		r1,=0x0         
	str		r1,[r0]

	ldr		r0,=INTSUBMSK
	ldr		r1,=0x7fff				
	str		r1,[r0]
	
	ldr		r0,=INTMSK
	ldr		r1,=0xffffffff			
	str		r1,[r0]

	ldr		r0,=LOCKTIME
	ldr		r1,=0xffffffff			
	str		r1,[r0]

	ldr		r0,=CAMDIVN
	ldr		r1,=0x0 			
	str		r1,[r0]

	ldr		r0,=CLKDIVN

	ldr		r1,=0x5 			
	str		r1,[r0]

	ldr		r0,=MPLLCON
   	ldr		r1,= 0x6e031 		@ 399.65MHz(About 400MHz) @ 16.9344MHz
	str		r1,[r0]
	
	ldr  	r0,=0xc000107e		@ Async mode, I,D Cache ON
		

	mcr  	p15,0,r0,c1,c0,0	
	
@@@@@@@@@@@@@@@@@@@ Led On-Off @@@@@@@@@@@@@@@@@@

	ldr 	r0, =GPBCON			@ LED=GPB[10:7]
	ldr		r1, [r0]
	bic		r1, r1, #0x3fc000
	orr		r1, r1, #0x154000
	str		r1, [r0]

	ldr 	r0, =GPBDAT
	ldr		r1, [r0]
	bic		r1, r1, #0x780
	str		r1, [r0]
	mov		r2, #2

	ldr		r3, =0x1000000		@ delay
1:
	subs	r3, r3, #1
	bne		1b

	ldr		r1, [r0]
	orr		r1, r1, #0x780
	str		r1, [r0]


@@@@@@@@@@@@@@@ Memory Configuration @@@@@@@@@@@@@@@@@

    ldr		r0,=SMRDATA
	ldr		r1,=BWSCON			
	add		r2, r0, #52			
1:
	ldr		r3, [r0], #4     	
	str		r3, [r1], #4  		
	cmp		r2, r0
	bne		1b

@@@@@@@@@@@@@@@ C Runtime Startup @@@@@@@@@@@@@@@@@

	.extern __RO_LIMIT__
	.extern __RW_BASE__

	.extern __ZI_BASE__
	.extern __ZI_LIMIT__

	ldr		r3, =__ZI_BASE__
	ldr		r1, =__ZI_LIMIT__
	mov		r2, #0x0
3:
	cmp		r3, r1
	strlo	r2, [r3], #4
	blo		3b

@;;;;;;;;;;;;;; Stack Configuration ;;;;;;;;;;;;;;;;;

	msr		cpsr_c, #(IRQ_BIT|FIQ_BIT|IRQ_MODE)
	ldr		sp, =IRQ_STACK_BASE

	msr		cpsr_c, #(IRQ_BIT|FIQ_BIT|SVC_MODE)
	ldr		sp, =SVC_STACK_BASE

	msr		cpsr_c, #(IRQ_BIT|FIQ_BIT|FIQ_MODE)
	ldr		sp, =FIQ_STACK_BASE

	msr		cpsr_c, #(IRQ_BIT|FIQ_BIT|UNDEF_MODE)
	ldr		sp, =UNDEF_STACK_BASE

	msr		cpsr_c, #(IRQ_BIT|FIQ_BIT|ABORT_MODE)
	ldr		sp, =ABORT_STACK_BASE

	msr		cpsr_c, #(SYS_MODE)
	ldr		sp, =SYS_STACK_BASE


	.extern Main

  	bl	Main

   	b		.

	@ 133MHz, Winbond BGA SDRAM (W9825G2DB), 32MB(8M*4B)

SMRDATA:

	.word 	0x02000000	@ BWSCON
	.word 	0x0
	.word 	0x0
	.word 	0x0
	.word 	0x0
	.word 	0x0
	.word 	0x0
	.word 	0x00018001	@ BANKCON6 100MHz
	.word 	0x0
	.word 	0x008401E7 	@ REFRESH 100MHz
	.word 	0x90
 	.word 	0x20		@ MRSRB6 100MHz
 	.word 	0x30
 	 	
  .end
