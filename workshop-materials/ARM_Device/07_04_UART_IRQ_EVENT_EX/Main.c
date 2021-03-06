// UART IRQ Echo Back Test

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

extern volatile int Key_push;
extern volatile int Key_value;
extern volatile int Rx_data_in;
extern volatile int Rx_data;

void Main(void)
{
	Uart_Init(115200);
	Led_Init();

	Uart_Printf("UART IRQ Event Test\n");

	Key_Push_ISR_Init();
	Key_ISR_Init();
	Key_Push_ISR_Enable(1);
	Key_ISR_Enable(1);
	Uart_ISR_Enable(1,0,0);

	for(;;)
	{
		if(Key_push)
		{
			Uart_Printf("Push Key!\n");
			Key_push = 0;
		}

		if(Key_value)
		{
			Uart_Printf("Key: %d\n", Key_value);
			Key_value = 0;
		}

		if(Rx_data_in)
		{
			Uart_Printf("Uart: %c\n", Rx_data);
			Rx_data_in = 0;
		}

		Led_Display(0x0);
		Timer0_Delay(100);
		Led_Display(0xf);
		Timer0_Delay(100);
	}
}
