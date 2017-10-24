// EINT0 IRQ Test

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	Uart_Init(115200);
	Led_Init();

	Uart_Printf("Push Key IRQ Test\n");

	Key_Push_ISR_Init();
	Key_Push_ISR_Enable(1);

	for(;;)
	{
		Led_Display(0x0);
		Timer0_Delay(100);
		Led_Display(0xf);
		Timer0_Delay(100);
	}
}

#endif

// EINT4_7 IRQ Test

#if 01

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	Uart_Init(115200);
	Led_Init();

	Uart_Printf("Key Matrix IRQ Test\n");

	Key_Push_ISR_Init();
	Key_ISR_Init();
	Key_Push_ISR_Enable(1);
	Key_ISR_Enable(1);

	for(;;)
	{
		Led_Display(0x0);
		Timer0_Delay(100);
		Led_Display(0xf);
		Timer0_Delay(100);
	}
}

#endif
