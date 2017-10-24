// Key IRQ Event Driven Test

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

extern volatile int Key_push;
extern volatile int Key_value;

void Main(void)
{
	Uart_Init(115200);
	Led_Init();

	Uart_Printf("Key Matrix IRQ Event Test\n");

	Key_Push_ISR_Init();
	Key_ISR_Init();
	Key_Push_ISR_Enable(1);
	Key_ISR_Enable(1);

	for(;;)
	{
		if(Key_push)
		{
			Uart_Printf("Push Key!\r");
			Key_push = 0;
		}

		if(Key_value)
		{
			Uart_Printf("Key: %d\r", Key_value);
			Key_value = 0;
		}

		Led_Display(0x0);
		Timer0_Delay(100);
		Led_Display(0xf);
		Timer0_Delay(100);
	}
}
