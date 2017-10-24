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

@@@@@@@@@@@@@@@ Led ON #1 @@@@@@@@@@@@@@@@@

	.if	1

	ldr 	r0, =GPBCON			@ LED=GPB[10:7]
	ldr		r1, =0x154000
	str		r1, [r0]

	ldr 	r0, =GPBDAT
	ldr		r1, =0x300
	str		r1, [r0]

	.endif

@@@@@@@@@@@@@@@ Led ON #2 @@@@@@@@@@@@@@@@@

	.if	0

	ldr 	r0, =GPBCON			@ LED=GPB[10:7]
	ldr		r1, [r0]
	bic		r1, r1, #0x2a8000
	orr		r1, r1, #0x154000
	str		r1, [r0]

	ldr 	r0, =GPBDAT
	ldr		r1, [r0]
	bic		r1, r1, #0x480
	orr		r1, r1, #0x300
	str		r1, [r0]

	.endif

@@@@@@@@@@@@@@@ Led ON #3 @@@@@@@@@@@@@@@@@

	.if	0

	ldr 	r0, =GPBCON			@ LED=GPB[10:7]
	ldr		r1, [r0]
	bic		r1, r1, #0x3fc000
	orr		r1, r1, #0x154000
	str		r1, [r0]

	ldr 	r0, =GPBDAT
	ldr		r1, [r0]
	bic		r1, r1, #0x780
	orr		r1, r1, #0x300
	str		r1, [r0]

	.endif

@@@@@@@@@@@@@@@ Led ON #5 @@@@@@@@@@@@@@@@@

	.if	0

	ldr 	r0, =GPBCON			@ LED=GPB[10:7]
	ldr		r1, [r0]
	bic		r1, r1, #(0xFF:SHL:14)
	orr		r1, r1, #(0x55:SHL:14)
	str		r1, [r0]

	ldr 	r0, =GPBDAT
	ldr		r1, [r0]
	bic		r1, r1, #(0xF:SHL:7)
	orr		r1, r1, #(0x6:SHL:7)
	str		r1, [r0]

	.endif

@@@@@@@@@@@@@@@ Led ON #4 @@@@@@@@@@@@@@@@@

	.if	0

	ldr 	r0, =GPBCON			@ LED=GPB[10:7]
	ldr		r1, [r0]
	bic		r1, r1, #(0xFF << 14)
	orr		r1, r1, #(0x55 << 14)
	str		r1, [r0]

	ldr 	r0, =GPBDAT
	ldr		r1, [r0]
	bic		r1, r1, #(0xF<<7)
	orr		r1, r1, #(0x6<<7)
	str		r1, [r0]

	.endif

	b	.

 .end
