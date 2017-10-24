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

	ldr  	r0,=0xc000107a
	mcr  	p15,0,r0,c1,c0,0	

@@@@@@@@@@@@@@@ Subroutine Call #1 @@@@@@@@@@@@@@@@@

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

LOOP:
	bl		LED_ON
	bl		DELAY
	bl		LED_OFF
	bl		DELAY
	b		LOOP

LED_ON:
	ldr 	r0, =GPBDAT
	ldr		r1, [r0]
	bic		r1, r1, #0x780
	str		r1, [r0]
	mov		pc, lr

LED_OFF:
	ldr 	r0, =GPBDAT
	ldr		r1, [r0]
	orr		r1, r1, #0x780
	str		r1, [r0]
	mov		pc, lr

DELAY:
	ldr		r0, =0x1000000
1:
	subs	r0, r0, #1
	bne		1b
	mov		pc, lr

	.endif

@@@@@@@@@@@@@@@ Subroutine Call #2 @@@@@@@@@@@@@@@@@

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

LOOP2:
	ldr		r1, [r0]
	eor		r1, r1, #0x780
	str		r1, [r0]


	@bl	DELAY
	@b LOOP2

	mov lr, pc
	B DELAY
	B LOOP2

DELAY:
	ldr		r0, =0x100000
1:
	subs	r0, r0, #1
	bne		1b
	mov		pc, lr


.endif

	.end
