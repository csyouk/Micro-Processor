#include "device_driver.h"

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

void Main(void)
{
	Sys_Init("\nIRQ Test\n");

	Uart_Printf("\nKEY Interrupt & UART-RX Interrupt Test\n");

	Key_ISR_Init();
	Key_ISR_Enable(1);
	Uart_ISR_Enable(1,0,0);

	for(;;)
	{
		if(Key_Value)
		{
			Uart_Printf("[IRQ4] KEY=%d\n", Key_Value);
			Key_Value = 0;
		}

		if(Uart0_Rx_In)
		{
			Uart_Printf("[IRQ28] UART0=%c\n", Uart0_Rx_Data);
			Uart0_Rx_In = 0;
		}
	}
}
