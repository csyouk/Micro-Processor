	.include "2440addr.inc"

	.code	32
	.text

	.global	__start
__start:

@--------------------------------------------------
@ WDT Disable
@--------------------------------------------------

	ldr		r0,=WTCON				
	ldr		r1,=0x0         
	str		r1,[r0]

	@ Interrupt Disable

	ldr		r0,=INTSUBMSK
	ldr		r1,=0x7fff				
	str		r1,[r0]
	
	ldr		r0,=INTMSK
	ldr		r1,=0xffffffff			
	str		r1,[r0]

	@ FCLK:HCLK:PCLK 설정

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
   	ldr		r1,= 0x6e031
	str		r1,[r0]

	@ Coprocessor Clock 설정
	
	ldr  	r0,=0xc000107a
	mcr  	p15,0,r0,c1,c0,0	



   	b		.

 .end
