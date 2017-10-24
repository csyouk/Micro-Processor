@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ void DABT_Gen(void)
@ memory unaligned accee로 data-abort 발생
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  DABT_Gen
DABT_Gen:

	mov		r0, #1
	ldr		r0, [r0]
	mov		pc, lr
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ void PABT_Gen(void)
@ unmapped memory accee로 prefetch-abort 발생
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  PABT_Gen
PABT_Gen:

	ldr		pc, =0x5b000000
	mov		pc, lr
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ void UND_Gen(void)
@ undefine 명령 강제 수행으로 undef 발생
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  UND_Gen
UND_Gen:

	ldr		r0, =0x31000000
	ldr		r1, =0x56000010
	str		r1, [r0]
	ldr		pc, =0x31000000
	mov		pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ void SWI_Gen(void)
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  SWI_Gen
SWI_Gen:

	SWI		0x0
	mov		pc, lr
	



