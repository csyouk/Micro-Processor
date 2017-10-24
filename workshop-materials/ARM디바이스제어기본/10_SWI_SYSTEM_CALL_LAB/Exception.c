#include "device_driver.h"

void Undef_Handler(int addr, int mode)
{
	Uart_Printf("UND-Exception @[0x%x], Mode=[0x%X]\n", addr, mode);
	for(;;);
}

void Pabort_Handler(int addr, int mode)
{
	Uart_Printf("PABT-Exception @[0x%x], Mode=[0x%X]\n", addr, mode);
	for(;;);
}

void Dabort_Handler(int addr, int mode)
{
	Uart_Printf("DABT-Exception @[0x%x], Mode=[0x%X]\n", addr, mode);
	for(;;);
}

// SWI 개별 서비스 루틴들

void _Print_Hello(void)
{
	Uart_Printf("SW0 Service...\n");
	Uart_Printf("Hello\n");
}

int _Sqr(int a)
{
	Uart_Printf("SW1 Service...\n");
	return a * a;
}

double _Double_Add(double a, double b)
{
	Uart_Printf("SW2 Service...\n");
	return a + b;
}

void * SWI_Handler[] = {(void *)_Print_Hello, (void *)_Sqr, (void *)_Double_Add};
