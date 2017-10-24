// UART Echo Back

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	unsigned char container;
	volatile int i;
	Uart_Init(115200);
	Led_Init();
	Key_Poll_Init();

	Uart_Printf("Uart Echo Back Test!!!!\n");

	for(;;)
	{

		// if receive buffer status full then wait
  	//	while(Macro_Check_Bit_Clear(rUTRSTAT0, 0));
		if(Macro_Check_Bit_Set(rUTRSTAT0, 0)){
			// read data
			container = rURXH0;

			// is transmit buffer empty?
			while(Macro_Check_Bit_Clear(rUTRSTAT0, 1));

			// send data
			rUTXH0 = container;
		}
	}
}

#endif

// UART Input Device Driver Test

#if 01

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
		Uart_Printf("\n Ű����� �޴� ���� 10���� echo-back \n");
		Uart_Printf("\n Ű���带 ������ �ٽ� PC�� ���ڸ� �����Ѵ� (10����) \n");

		for(i=0; i<10; i++)
		{

			Uart_Printf("\n>");
			Uart_Printf("%c", Uart_Get_Char());
		}

		Uart_Printf("\n��� LED Toggling�ϰ� ������ Ű������ x�� ������ ���� ��");

		do
		{
			Led_Display(0x5); // 1, 3 on
			for(i=0; i<0x1000000; i++);
			Led_Display(0xA); // 2, 4 on
			for(i=0; i<0x1000000; i++);
		}while(Uart_Get_Pressed() != 'x');
	}
}

#endif
