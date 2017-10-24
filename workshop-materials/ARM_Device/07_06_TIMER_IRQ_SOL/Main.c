// Timer Delay IRQ Test

#if 01

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

extern volatile int Key_push;
extern volatile int Key_value;
extern volatile int Rx_data_in;
extern volatile int Rx_data;
extern volatile int Timer0_time_out;

void Main(void)
{
	int led = 0xf;

	Uart_Init(115200);
	Led_Init();

	Uart_Printf("Timer Delay IRQ Test\n");

	Key_Push_ISR_Init();
	Key_ISR_Init();
	Key_Push_ISR_Enable(1);
	Key_ISR_Enable(1);
	Uart_ISR_Enable(1,0,0);
	Timer0_Delay_ISR_Enable(1, 500);

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

		if(Timer0_time_out)
		{
			Timer0_time_out = 0;
			Led_Display(led = ~led);
			Timer0_Delay_ISR_Enable(1, 500);
			Uart_Printf(".");
		}
	}
}

#endif

// Timer Repeat IRQ Test

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

extern volatile int Key_push;
extern volatile int Key_value;
extern volatile int Rx_data_in;
extern volatile int Rx_data;
extern volatile int Timer0_time_out;

void Main(void)
{
	int led = 0xf;

	Uart_Init(115200);
	Led_Init();

	Uart_Printf("Timer Repeat IRQ Test\n");

	Key_Push_ISR_Init();
	Key_ISR_Init();
	Key_Push_ISR_Enable(1);
	Key_ISR_Enable(1);
	Uart_ISR_Enable(1,0,0);
	Timer0_Repeat_ISR_Enable(1, 500);

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

		if(Timer0_time_out)
		{
			Timer0_time_out = 0;
			Led_Display(led = ~led);
			Uart_Printf(".");
		}
	}
}

#endif
