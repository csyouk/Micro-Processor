#include "device_driver.h"
#include "2440addr.h"

void Sys_Init(char * msg)
{
	Run_Time_Config();
	Uart_Init(115200);
	MMU_Init();
	Led_Init();
	Uart_Printf(msg);
}

#define TEST	1    // Test Select: 1~9

void c_func(void)
{
	Uart_Printf("C-Function!\n");
}

int Sqr(int a)
{
	return (a*a);
}

int Add_7(int a, int b, int c, int d, int e, int f, int g)
{
	return (a+b+c+d+e+f+g);
}

void print_int(int v)
{
	Uart_Printf("Value = %d\n", v);
}

void print_stack(long long int a, long long int b, int c, long long int d)
{
	Uart_Printf("a = 0x%llx\n", a);
	Uart_Printf("b = 0x%llx\n", b);
	Uart_Printf("c = 0x%x\n", c);
	Uart_Printf("d = 0x%llx\n", d);
}

unsigned char uc = 0x80;
signed char sc = 0x80;
unsigned short us = 0x8000;
signed short ss = 0x8000;

int a = 100;
int *p = &a;
int x[4] = {10,20,30,40};

#if TEST == 1

extern void Asm_Led_On(void);
extern void Asm_Led_Off(void);

int Main(void)
{
	int i;

	Sys_Init("\nAPCS, C-ASM Interface Test #1\n");

	// ASM �Լ� ȣ�� �⺻

	for(i=0; i<5; i++)
	{
		Asm_Led_On();
		Delay(1000);
		Asm_Led_Off();
		Delay(1000);
	}

	Uart_Printf("\nReturned!\n");
	return 0;
}

#elif TEST == 2

extern void Asm_Print_Err(void);

int Main(void)
{
	Sys_Init("\nAPCS, C-ASM Interface Test #2\n");

	// C -> ASM -> C �Լ� ȣ��

	Asm_Print_Err();
	Uart_Printf("\nReturned!\n");

	return 0;
}

#elif TEST == 3

extern void Asm_Print_Good(void);

int Main(void)
{
	Sys_Init("\nAPCS, C-ASM Interface Test #3\n");

	// C -> ASM -> C �Լ� ȣ�� ����

	Asm_Print_Good();
	Uart_Printf("\nReturned!\n");

	return 0;
}

#elif TEST == 4

int new_add(int a, int b, int c, int d, int e, int f)
{
	return (a+b+c+d+e+f);
}

int Main(void)
{
	Sys_Init("\nAPCS, C-ASM Interface Test #4\n");

	int a;

	a = new_add(1,2,3,4,5,6);
	Uart_Printf("%d\n", a);

	Uart_Printf("\nReturned!\n");
	return 0;
}

#elif TEST == 5

extern int Asm_Add(int a, int b);
extern int Asm_Add_Sqr(int a, int b);
extern int Asm_Add_7(int a, int b);
extern int Asm_Add_7_Sqr(int a, int b);

int Main(void)
{
	Sys_Init("\nAPCS, C-ASM Interface Test #5\n");

	{
		int a = 3;
		int b = 5;

		/* �ּ��� Ǯ�鼭 �ϳ��� ���� �Ѵ� */

		Uart_Printf("3+5=%d [Result  = 8]\n", Asm_Add(a,b));
		//Uart_Printf("3*3+5*5=%d [Result  = 34]\n", Asm_Add_Sqr(a,b));
		//Uart_Printf("3*7+5=%d [Result = 26]\n", Asm_Add_7(a,b));
		//Uart_Printf("3*7+5*5=%d [Result = 46]\n", Asm_Add_7_Sqr(a,b));
	}

	Uart_Printf("\nReturned!\n");
	return 0;
}

#elif TEST == 6

extern void Asm_Arg1(char a, short b, int c);
extern void Asm_Arg2(int * d, int *e);

int Main(void)
{
	Sys_Init("\nAPCS, C-ASM Interface Test #6\n");

	// �������� Ÿ���� argument ����

	{
		char a = 50;
		short b = 100;
		int c = 500;
		int * d = &c;
		int e[4] = {10,20,30,40};

		Asm_Arg1(a, b, c);
		Asm_Arg2(d, e);
	}

	Uart_Printf("\nReturned!\n");
	return 0;
}

#elif TEST == 7

extern unsigned char Asm_Ret_Unsigned_Char(unsigned char a);
extern signed char Asm_Ret_Signed_Char(signed char a);
extern unsigned short Asm_Ret_Unsigned_Short(unsigned short a);
extern signed short Asm_Ret_Signed_Short(signed short a);
extern unsigned int Asm_Ret_Unsigned_Int(unsigned int a);
extern signed int Asm_Ret_Signed_Int(signed int a);

unsigned char C_Ret_Unsigned_Char(unsigned char a){ return a + 1; }
signed char C_Ret_Signed_Char(signed char a){ return a + 1; }
unsigned short C_Ret_Unsigned_Short(unsigned short a){ return a + 1; }
signed short C_Ret_Signed_Short(signed short a){ return a + 1; }
unsigned int C_Ret_Unsigned_Int(unsigned int a){ return a + 1; }
signed int C_Ret_Signed_Int(signed int a){ return a + 1; }

int Main(void)
{
	Sys_Init("\nAPCS, C-ASM Interface Test #7\n");

	// �������� Ÿ���� return

	{
		// C ���� �Լ�
		Uart_Printf("C Version Functions\n");
		Uart_Printf("UC 0x80 => %#.8X\n", C_Ret_Unsigned_Char(0x80));
		Uart_Printf("SC 0x80 => %#.8X\n", C_Ret_Signed_Char(0x80));
		Uart_Printf("US 0x8000 => %#.8X\n", C_Ret_Unsigned_Short(0x8000));
		Uart_Printf("SS 0x8000 => %#.8X\n", C_Ret_Signed_Short(0x8000));
		Uart_Printf("UI 0x80000000 => %#.8X\n", C_Ret_Unsigned_Int(0x80000000));
		Uart_Printf("SI 0x80000000 => %#.8X\n", C_Ret_Signed_Int(0x80000000));

		// ASM ���� �Լ�
		Uart_Printf("\nASM Version Functions\n");
		Uart_Printf("UC 0x80 => %#.8X\n", Asm_Ret_Unsigned_Char(0x80));
		Uart_Printf("SC 0x80 => %#.8X\n", Asm_Ret_Signed_Char(0x80));
		Uart_Printf("US 0x8000 => %#.8X\n", Asm_Ret_Unsigned_Short(0x8000));
		Uart_Printf("SS 0x8000 => %#.8X\n", Asm_Ret_Signed_Short(0x8000));
		Uart_Printf("UI 0x80000000 => %#.8X\n", Asm_Ret_Unsigned_Int(0x80000000));
		Uart_Printf("SI 0x80000000 => %#.8X\n", Asm_Ret_Signed_Int(0x80000000));
	}

	Uart_Printf("\nReturned!\n");
	return 0;
}

#elif TEST == 8

int C_Var_Unsigned_Char(void) { return uc+=1; }
int C_Var_Signed_Char(void) { return sc+=1; }
int C_Var_Unsigned_Short(void) { return us+=1; }
int C_Var_Signed_Short(void) { return ss+=1; }

extern int Asm_Var_Unsigned_Char(void);
extern int Asm_Var_Signed_Char(void);
extern int Asm_Var_Unsigned_Short(void);
extern int Asm_Var_Signed_Short(void);
extern void Asm_Var_Access(void);

int Main(void)
{
	Sys_Init("\nAPCS, C-ASM Interface Test #8\n");

	// C�� ���������� �＼�� => ASM ���忡�� C���� ������������ ��� �޸� �ּ��̴�
	// ASM�� ������ EXPORT�� ������ �о���� C���� ������ Ÿ������ �Ǵ� ����

	{
		int result;

		// C ���� �Լ�
		Uart_Printf("C Version Functions\n");
		result = C_Var_Unsigned_Char();
		Uart_Printf("UC 0x80 => %#.8X, return => %#.8X\n", uc, result);
		result = C_Var_Signed_Char();
		Uart_Printf("SC 0x80 => %#.8X, return => %#.8X\n", sc, result);
		result = C_Var_Unsigned_Short();
		Uart_Printf("US 0x8000 => %#.8X, return => %#.8X\n", us, result);
		result = C_Var_Signed_Short();
		Uart_Printf("SS 0x8000 => %#.8X, return => %#.8X\n", ss, result);

		uc = 0x80;
		sc = 0x80;
		us = 0x8000;
		ss = 0x8000;

		// ASM ���� �Լ�
		Uart_Printf("\nASM Version Functions\n");
		result = Asm_Var_Unsigned_Char();
		Uart_Printf("UC 0x80 => %#.8X, return => %#.8X\n", uc, result);
		result = Asm_Var_Signed_Char();
		Uart_Printf("SC 0x80 => %#.8X, return => %#.8X\n", sc, result);
		result = Asm_Var_Unsigned_Short();
		Uart_Printf("US 0x8000 => %#.8X, return => %#.8X\n", us, result);
		result = Asm_Var_Signed_Short();
		Uart_Printf("SS 0x8000 => %#.8X, return => %#.8X\n", ss, result);

		// ����, ������, �迭 �＼�� : ������ ���� ���� 1�� ���� (�����ʹ� ����Ű�� �����͸� 1�� ����)
		Asm_Var_Access();
		Uart_Printf("\na (102) => %d\n", a);
		Uart_Printf("*p (102) => %d\n", *p);
		Uart_Printf("x[0]~x[3] (11,21,31,41) => %d,%d,%d,%d\n", x[0], x[1], x[2], x[3]);
	}

	Uart_Printf("\nReturned!\n");
	return 0;
}

#elif TEST == 9

extern void Asm_Tail_Call(int a);
extern void Asm_Sqr_Print(int a);

int Main(void)
{
	Sys_Init("\nAPCS, C-ASM Interface Test #9\n");

	// Tail Call�� ����

	Asm_Tail_Call(100);
	Uart_Printf("Returned\n\n");

	Uart_Printf("\nReturned!\n");
	return 0;
}

#endif

