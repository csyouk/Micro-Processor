#include "device_driver.h"
#include "2440addr.h"

void Sys_Init(char * msg)
{
	Run_Time_Config();
	MMU_Init();
	Uart_Init(115200);
	Led_Init();
	Uart_Printf(msg);
}

extern volatile int Key_Value;
extern volatile int Uart0_Rx_In;
extern volatile int Uart0_Rx_Data;

extern void FIQ_Select(int id, int fiq);

void Main(void)
{
	Sys_Init("\nIRQ-FIQ Nesting Test\n");

	Uart_Printf("\nKEY = IRQ & UART-RX = FIQ\n");

	FIQ_Select(28, 1);
	Key_ISR_Init();
	Key_ISR_Enable(1);
	Uart_ISR_Enable(1,0,0);

	for(;;);
}
