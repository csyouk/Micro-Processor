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

@@@@@@@@@@@@@@@ Conditional Assembling @@@@@@@@@@@@@@@@@

	.if 1

	.equiv DELAY_SEL1, 1

	.equ   DELAY2, 0
	.set   DELAY2, 1

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
	eor		r1, r1, #0x100
	str		r1, [r0]

	.if DELAY_SEL1 == 0
		BL		SHORT_DELAY
	.else
		BL		LONG_DELAY
	.endif

	b		LOOP2

SHORT_DELAY:
	MOV		r2, #0x800000
1:	SUBS	r2, r2, #1
	BGT		1b
	MOV		PC, LR

LONG_DELAY:
	.if DELAY2 == 0
		MOV		r2, #0x3200000
	.else
		MOV		r2, #0x1600000
	.endif
1:	SUBS	r2, r2, #1
	BGT		1b
	MOV		PC, LR

	.endif

@@@@@@@@@@@@@@@ Macro Example #1 @@@@@@@@@@@@@@@@@

	.if 0

	.macro	DELAY

	LDR		r9, =0x800000
1:	SUBS	r9, r9, #1
	BNE		1b

	.endm

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
	eor		r1, r1, #0x200
	str		r1, [r0]

	DELAY

	b		LOOP2

	.endif

@@@@@@@@@@@@@@@ Macro Example #2 @@@@@@@@@@@@@@@@@

	.if 0

	.macro	DELAY	value

	LDR		r9, =\value
1:	SUBS	r9, r9, #1
	BNE		1b

	.endm

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
	eor		r1, r1, #0x200
	str		r1, [r0]

	DELAY	0x1600000

	b		LOOP2

	.endif

@@@@@@@@@@@@@@@ Macro Example #3 @@@@@@@@@@@@@@@@@

	.if 0

	.macro	MACRO_DELAY	reg, value

	LDR		\reg, =\value
1:	SUBS	\reg, \reg, #1
	BNE		1b

	.endm

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
	eor		r1, r1, #0x080
	str		r1, [r0]

	MACRO_DELAY  r3, 0x1000000

	b		LOOP2

	.endif

@@@@@@@@@@@@@@@ Macro Example #4 @@@@@@@@@@@@@@@@@

	.if 0

	.altmacro
	.macro MACRO_FUNC reg, value

	LDR		&reg, =&value
1:	SUBS	&reg, &reg, #1
	BGT		1b
	MOV		PC, LR

	.endm

	.equ	DELAY_SEL1, 0
	.set 	DELAY_SEL1, 1

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
	eor		r1, r1, #0x400
	str		r1, [r0]

	.if DELAY_SEL1 == 0
		BL		SHORT_DELAY
	.else
		BL		LONG_DELAY
	.endif

	b		LOOP2

SHORT_DELAY: MACRO_FUNC r2, 0x800000
LONG_DELAY:  MACRO_FUNC r2, 0x1600000

	.endif
	.end

