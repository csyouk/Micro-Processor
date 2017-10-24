// WDT Test

#if 1

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

#define TIME	1200
//#define TIME	800

void Main(void)
{
	Uart_Init(115200);
	Uart_Printf("WDT Test\n");

	if(rGSTATUS2 & 0x4)
	{
		rGSTATUS2 |= 0x4;
		Uart_Printf("WDT Reset!\n");
	}

	WDT_Reset_Enable(1);
	WDT_Start(1, 1000);

	for(;;)
	{
		Timer0_Delay(TIME);
		WDT_Clear(1000);
		Uart_Printf("*");
	}
}

#endif

// WDT Interrupt Test

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

extern volatile int Wdt_timeout;

void Main(void)
{
	Uart_Init(115200);
	Uart_Printf("WDT Interrupt Test\n");

	WDT_Reset_Enable(0);
	WDT_ISR_Enable(1, 1000);

	for(;;)
	{
		if(Wdt_timeout)
		{
			Uart_Printf("#");
			Wdt_timeout = 0;
		}
	}
}

#endif
