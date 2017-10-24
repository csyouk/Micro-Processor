	.include "2440addr.inc"

	.code	32
	.text

	.global	__start
__start:

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
@	ldr		r1,=0x7				@ 1:3:6
	ldr		r1,=0x5				@ 1:4:8
	str		r1,[r0]

	ldr		r0,=MPLLCON
   	ldr		r1,= 0x6e031 		@ 399.65MHz(About 400MHz) @ 16.9344MHz
	str		r1,[r0]
	
	@@@ Coprocessor 동작 모드에 따른 속도차이를 확인하시오

@	ldr  	r0,=0x0000007a		@ [1] default, Fast mode, I Cache OFF, D Cache OFF
@	ldr  	r0,=0x0000107a		@ [2] Fast mode, I Cache ON, D Cache OFF
@	ldr  	r0,=0xc000007a		@ [3] Async mode, I Cache OFF, D Cache OFF
	ldr  	r0,=0xc000107a		@ [4] Async mode, I Cache ON, D Cache OFF

	mcr  	p15,0,r0,c1,c0,0	

@@@@@@@@@@@@@@@ Led Toggling @@@@@@@@@@@@@@@@@

	@@@ LED=GPB[10:7], LED Toggling
	@@@ Delay => 0x1000000

LOOP:


	b		LOOP

 .end
