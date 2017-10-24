#include "device_driver.h"

void Undef_Handler(void) 
{
	Uart_Printf("UND Exception\n");
	for(;;);
}

void Pabort_Handler(void) 
{
	Uart_Printf("PABT Exception\n");
	for(;;);
}

void Dabort_Handler(void) 
{
	Uart_Printf("DABT Exception\n");
	for(;;);
}

void SWI_Handler(void)
{
	Uart_Printf("SWI Exception\n");
	for(;;);
}


