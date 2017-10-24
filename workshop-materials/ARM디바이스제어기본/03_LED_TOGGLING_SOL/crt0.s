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
	ldr		r1,=0x5
	str		r1,[r0]

	ldr		r0,=MPLLCON
   	ldr		r1,= 0x6e031
	str		r1,[r0]

@@@@@@@@@@@@@@@ I-Cache 속도 차이 @@@@@@@@@@@@@@@@@
	
@	ldr  	r0,=0xc000007a		@ [1] I Cache OFF
	ldr  	r0,=0xc000107a		@ [2] I Cache ON

	mcr  	p15,0,r0,c1,c0,0	

@@@@@@@@@@@@@@@ Led Toggling #1 @@@@@@@@@@@@@@@@@

	.if 1

	ldr 	r0, =GPBCON
	ldr		r1, [r0]
	bic		r1, r1, #0x3fc000
	orr		r1, r1, #0x154000
	str		r1, [r0]

	ldr 	r0, =GPBDAT
	ldr		r1, [r0]
	orr		r1, r1, #0x780
	str		r1, [r0]

LOOP1:
	ldr		r1, [r0]
	bic		r1, r1, #0x780
	str		r1, [r0]

	ldr		r3, =0x1000000
1:
	subs	r3, r3, #1
	bne		1b

	ldr		r1, [r0]
	orr		r1, r1, #0x780
	str		r1, [r0]

	ldr		r3, =0x1000000
1:
	subs	r3, r3, #1
	bne		1b

	b		LOOP1

	.endif

@@@@@@@@@@@@@@@ Led Toggling #2 @@@@@@@@@@@@@@@@@

	.if 0

	ldr 	r0, =GPBCON
	ldr		r1, [r0]
	bic		r1, r1, #0x3fc000
	orr		r1, r1, #0x154000
	str		r1, [r0]

	ldr 	r0, =GPBDAT
	ldr		r1, [r0]
	orr		r1, r1, #0x780
	str		r1, [r0]

LOOP2:
	ldr		r1, [r0]
	eor		r1, r1, #0x780
	str		r1, [r0]

	ldr		r3, =0x1000000
1:
	subs	r3, r3, #1
	bne		1b

	b		LOOP2

	.endif

@@@@@@@@@@@@@@@ Led 5회 Toggling @@@@@@@@@@@@@@@@@

	.if 0

	ldr 	r0, =GPBCON
	ldr		r1, [r0]
	bic		r1, r1, #0x3fc000
	orr		r1, r1, #0x154000
	str		r1, [r0]

	ldr 	r0, =GPBDAT
	ldr		r1, [r0]
	orr		r1, r1, #0x780
	str		r1, [r0]
	mov		r2, #10

LOOP3:
	ldr		r1, [r0]
	eor		r1, r1, #0x780
	str		r1, [r0]

	ldr		r3, =0x1000000
1:
	subs	r3, r3, #1
	bne		1b

	subs	r2, r2, #1
	bne		LOOP3

	b		.

	.endif

 .end
