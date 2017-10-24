	.include "2440addr.inc"
	.include "Option.inc"

	.code	32
	.text

	.global	__start
__start:

@--------------------------------------------------
@ Reset Handler Routine
@--------------------------------------------------

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

@@@@@@@@@@@@@@@@ Stack Mounting @@@@@@@@@@@@@@@@@

	ldr		sp, =SYS_STACK_BASE

@@@@@@@@@@@@@@@@ Call Main @@@@@@@@@@@@@@@@@

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
