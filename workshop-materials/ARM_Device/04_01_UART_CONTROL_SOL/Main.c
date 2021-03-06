// UART Echo Back

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	unsigned char x;

	Uart_Init(115200);
	Led_Init();
	Key_Poll_Init();

	Uart_Printf("Uart Echo Back Test\n");

	for(;;)
	{
		if(Macro_Check_Bit_Set(rUTRSTAT0, 0))
		{
			x = rURXH0;
			while(Macro_Check_Bit_Clear(rUTRSTAT0,1));
			rUTXH0 = x;
		}
	}
}

#endif

// UART Input Device Driver Test

#if 0

#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

void Main(void)
{
	volatile int i;

	Led_Init();
	Uart_Init(115200);

	for(;;)
	{
		Uart_Printf("\n키보드로 받는 글자 10개를 echo-back\n");
		Uart_Printf("\n키보드를 누르면 다시 PC로 들자를 전송한다 (10글자)\n");

		for(i=0; i<10; i++)
		{

			Uart_Printf("\n>");
			Uart_Printf("%c", Uart_Get_Char());
		}

		Uart_Printf("\n계속 LED Toggling하고 있으며 키보드의 x를 누르면 종료 함");

		do
		{
			Led_Display(0x5);
			for(i=0; i<0x1000000; i++);
			Led_Display(0xA);
			for(i=0; i<0x1000000; i++);
		}while(Uart_Get_Pressed() != 'x');
	}
}

#endif
