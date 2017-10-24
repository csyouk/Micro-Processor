// UART IRQ Echo Back Test

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"
#include "txt.h"

extern volatile int Key_push;
extern volatile int Key_value;

extern volatile int Uart_tx_complete;
extern volatile char * Uart_tx_string;
extern char * Str[];

void Main(void)
{
	int line = 0;

	Uart_Init(115200);
	Led_Init();

	Uart_tx_string = Str[line++];

	Uart_Printf("UART IRQ Event Test\n");

	Key_Push_ISR_Init();
	Key_ISR_Init();
	Key_Push_ISR_Enable(1);
	Key_ISR_Enable(1);
	Uart_ISR_Enable(0,1,0);



	for(;;)
	{
		if(Uart_tx_complete)
		{
			Uart_tx_complete = 0;

			if(line < (sizeof(Str)/sizeof(Str[0])))
			{
				Uart_tx_string = Str[line++];
				Uart_ISR_Enable(0,1,0);
			}
		}

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

		Led_Display(0x0);
		Timer0_Delay(100);
		Led_Display(0xf);
		Timer0_Delay(100);
	}
}
