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

void SWI_Handler(int addr, int mode)
{
	Uart_Printf("SWI-Exception @[0x%x], Mode=[0x%X]\n", addr, mode);

	// SWI 번호(ID)를 인쇄하는 코드를 설계하라


	for(;;);
}



