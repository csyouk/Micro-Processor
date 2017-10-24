	.include "2440addr.inc"

	.code	32
	.text

	.global	Asm_Led_On
Asm_Led_On:

	ldr 	r0, =GPBDAT
	ldr		r1, [r0]
	bic		r1, r1, #(0xF<<7)
	str		r1, [r0]
	mov		pc, lr

	.global	Asm_Led_Off
Asm_Led_Off:

	ldr 	r0, =GPBDAT
	ldr		r1, [r0]
	orr		r1, r1, #(0xF<<7)
	str		r1, [r0]
	mov		pc, lr

	.extern c_func

	.global	Asm_Print_Err
Asm_Print_Err:

	ldr 	r0, =GPBCON
	ldr		r1, [r0]
	bic		r1, r1, #(0xFF << 14)
	orr		r1, r1, #(0x55 << 14)
	str		r1, [r0]

	bl		c_func

	ldr 	r0, =GPBDAT
	ldr		r1, [r0]
	orr		r1, r1, #(0xF<<7)
	str		r1, [r0]
	mov		pc, lr

	.global	Asm_Print_Good
Asm_Print_Good:

	str		lr, [sp, #-4]!

	ldr 	r0, =GPBCON
	ldr		r1, [r0]
	bic		r1, r1, #(0xFF << 14)
	orr		r1, r1, #(0x55 << 14)
	str		r1, [r0]

	bl		c_func

	ldr 	r0, =GPBDAT
	ldr		r1, [r0]
	orr		r1, r1, #(0xF<<7)
	str		r1, [r0]

	ldr		lr, [sp], #4
	mov		pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ int Asm_Add(int a, int b)@
@ r0, r3�� ȣ�������� ������ �����Ͽ��� ��
@ r0-r3�̳� ������ ���� leaf function��, �����ʿ����
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  Asm_Add
Asm_Add:
	@ int Asm_Add(int a, int b)@
	@ �Էµ� �μ��� ���� ����





@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ int Asm_Add_Sqr(int a, int b)@
@ Sqr�� ȣ���ϸ� r0-r3,r12�� ���� ���ɼ��� ����
@ �ı� ����� �ִ� ������ ���ǽ�Ŵ
@ leaf function�� �ƴϹǷ� lr�� �����Ѵ�
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.extern  	Sqr
	
	.global  Asm_Add_Sqr
Asm_Add_Sqr:
	@ int Asm_Add_Sqr(int a, int b)@
	@ {
	@	return (Sqr(a)+Sqr(b))@
	@ }
	@ C�� �Լ��� Sqr()�� �̿�








@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ int Asm_Add_7(int a, int b)@
@ Add_7 ȣ��� ������ 7���� (�����̿�)
@ stack����å�� : Caller(__cdecl)
@ Add_7�� ȣ���ϸ� r0-r3,r12�� ���� ���ɼ��� ����
@ �ı� ����� �ִ� ������ ���ǽ�Ŵ
@ leaf function�� �ƴϹǷ� lr�� �����Ѵ�
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.extern  	Add_7
	
	.global  Asm_Add_7
Asm_Add_7:
	@ int Asm_Add_7(int a, int b)@
	@ {
	@	return (Add_7(a,a,a,a,a,a,a)+b)@
	@ }
	@ C�� �Լ��� Add_7()�� �̿�










@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ int Asm_Add_7_Sqr(int a, int b)@
@ Add_7 ȣ��� ������ 7���� (�����̿�)
@ stack����å�� : Caller(__cdecl)
@ Add_7�� ȣ���ϸ� r0-r3,r12�� ���� ���ɼ��� ����
@ �ı� ����� �ִ� ������ ���ǽ�Ŵ
@ leaf function�� �ƴϹǷ� lr�� �����Ѵ�
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  Asm_Add_7_Sqr
Asm_Add_7_Sqr:
	@ int Asm_Add_7_Sqr(int a, int b)@
	@ {
	@	return (Add_6(a,a,a,a,a,a,a)+Sqr(b))@
	@ }
	@ C�� �Լ��� Add_7(), Sqr()�� �̿�












@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ void Asm_Arg1(char a, short b, int c)@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.extern	 print_int

	.global  Asm_Arg1
Asm_Arg1:
	@ void Asm_Arg1(char a, short b, int c)@
	@ {
	@	print_int(a)@ print_int(b)@ print_int(c)@
	@ }

	push	{r4-r6, lr}
	mov		r4, r1
	mov		r5, r2
	bl		print_int
	mov		r0, r4
	bl		print_int
	mov		r0, r5
	bl		print_int
	pop		{r4-r6, pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ void Asm_Arg2(int * d, int *e)@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.extern	 print_int

	.global  Asm_Arg2
Asm_Arg2:
	@ void Asm_Arg2(int * d, int *e)@
	@ {
	@	print_int(*d)@ print_int(e[0])@ print_int(e[1])@ print_int(e[2])@ print_int(e[3])@
	@ }

	push 	{r4-r6, lr}
	mov		r4, r1
	ldr		r0, [r0]
	bl		print_int

	mov		r5, #4
1:
	ldr		r0, [r4], #4
	bl		print_int
	subs	r5, r5, #1
	bgt		1b

	pop		{r4-r6, pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned char Asm_Ret_Unsigned_Char(unsigned char a);
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global	Asm_Ret_Unsigned_Char
Asm_Ret_Unsigned_Char:

	add		r0, r0, #1
	and		r0, r0, #0xff
	mov		pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ signed char Asm_Ret_Signed_Char(signed char a);
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global	Asm_Ret_Signed_Char
Asm_Ret_Signed_Char:

	add		r0, r0, #1
	lsl		r0, #24
	asr		r0, #24
	mov		pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned short Asm_Ret_Unsigned_Short(unsigned short a);
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global	Asm_Ret_Unsigned_Short
Asm_Ret_Unsigned_Short:

	add		r0, r0, #1
	lsl		r0, #16
	lsr		r0, #16
	mov		pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ signed short Asm_Ret_Signed_Short(signed short a);
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global	Asm_Ret_Signed_Short
Asm_Ret_Signed_Short:

	add		r0, r0, #1
	lsl		r0, #16
	asr		r0, #16
	mov		pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int Asm_Ret_Unsigned_Int(unsigned int a);
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ signed int Asm_Ret_Signed_Int(signed int a);
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global	Asm_Ret_Unsigned_Int
	.global	Asm_Ret_Signed_Int

Asm_Ret_Unsigned_Int:
Asm_Ret_Signed_Int:

	add		r0, r0, #1
	mov		pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ int Asm_Var_Unsigned_Char(void)@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.extern uc
	.extern	sc
	.extern	us
	.extern	ss

	.global 	Asm_Var_Unsigned_Char
Asm_Var_Unsigned_Char:

	ldr		r1, =uc
	ldrb	r0, [r1]
	add		r0, r0, #1
	strb	r0, [r1]
	ldrb	r0, [r1]
	mov		pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ int Asm_Var_Signed_Char(void)@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global 	Asm_Var_Signed_Char
Asm_Var_Signed_Char:

	ldr		r1, =sc
	ldrsb	r0, [r1]
	add		r0, r0, #1
	strb	r0, [r1]
	ldrsb	r0, [r1]
	mov		pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ int Asm_Var_Unsigned_Short(void)@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global 	Asm_Var_Unsigned_Short
Asm_Var_Unsigned_Short:

	ldr		r1, =us
	ldrh	r0, [r1]
	add		r0, r0, #1
	strh	r0, [r1]
	ldrh	r0, [r1]
	mov		pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ int Asm_Var_Signed_Short(void)@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global 	Asm_Var_Signed_Short
Asm_Var_Signed_Short:

	ldr		r1, =ss
	ldrsh	r0, [r1]
	add		r0, r0, #1
	strh	r0, [r1]
	ldrsh	r0, [r1]
	mov		pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ void Asm_Var_Access(void)@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.extern	a
	.extern	p
	.extern  x

	.global	Asm_Var_Access
Asm_Var_Access:

	ldr		r0, =a
	ldr		r1, [r0]
	add		r1, r1, #1
	str		r1, [r0]

	ldr		r0, =p
	ldr		r0, [r0]
	ldr		r1, [r0]
	add		r1, r1, #1
	str		r1, [r0]

	mov		r2, #4
	ldr		r0, =x
1:
	ldr		r1, [r0]
	add		r1, r1, #1
	str		r1, [r0], #4
	subs	r2, r2, #1
	bgt		1b

	mov		pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ void Asm_Tail_Call(int a)@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.extern 	print_int

	.global 	Asm_Tail_Call
Asm_Tail_Call:

	add		r0, r0, #1
	b		print_int

	.end
