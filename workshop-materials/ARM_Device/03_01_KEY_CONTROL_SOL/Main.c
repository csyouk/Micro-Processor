// UP Key �ν�

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	Led_Init();

	Macro_Write_Block(rGPFCON, 0x3, 0x0, 8);
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	for(;;)
	{
		if(Macro_Check_Bit_Clear(rGPFDAT, 4))
		{
			Led_Display(1);
		}
		else
		{
			Led_Display(0);
		}
	}
}

#endif

// UP Key �ν��� Ȯ�� LED1 Toggling => Released ���

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	Led_Init();

	Macro_Write_Block(rGPFCON, 0x3, 0x0, 8);
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	for(;;)
	{
		if(Macro_Check_Bit_Clear(rGPFDAT, 4))
		{
			Macro_Invert_Bit(rGPBDAT, 7);
			while(Macro_Check_Bit_Clear(rGPFDAT, 4));
		}
	}
}

#endif

// UP Key �ν��� Ȯ�� LED1 Toggling => Inter-Lock ���

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	int lock = 0;
	volatile int i;

	Led_Init();

	Macro_Write_Block(rGPFCON, 0x3, 0x0, 8);
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	for(;;)
	{
		if((lock == 0) && Macro_Check_Bit_Clear(rGPFDAT, 4))
		{
			Macro_Invert_Bit(rGPBDAT, 7);
			lock = 1;
		}

		else if((lock == 1) && Macro_Check_Bit_Set(rGPFDAT, 4))
		{
			lock = 0;
		}
	}
}

#endif

// UP Key �ν��� Ȯ�� LED1 Toggling => Chattering Rejection

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

#define CHAT_REJECT		100

void Main(void)
{
	int lock = 0;
	volatile int i;

	Led_Init();

	Macro_Write_Block(rGPFCON, 0x3, 0x0, 8);
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	for(;;)
	{
		if((lock == 0) && Macro_Check_Bit_Clear(rGPFDAT, 4))
		{
			for(i=0; i<CHAT_REJECT; i++)
			{
				if(Macro_Check_Bit_Set(rGPFDAT, 4))
				{
					i = -1;
					break;
				}
			}

			if(i != -1)
			{
				Macro_Invert_Bit(rGPBDAT, 7);
				lock = 1;
			}
		}

		else if((lock == 1) && Macro_Check_Bit_Set(rGPFDAT, 4))
		{
			for(i=0; i<CHAT_REJECT; i++)
			{
				if(Macro_Check_Bit_Clear(rGPFDAT, 4))
				{
					i = -1;
					break;
				}
			}

			if(i != -1)
			{
				lock = 0;
			}
		}
	}
}

#endif

// UP Key �ν��� Ȯ�� LED1 Toggling => Chattering Rejection (�ٴ� �б⸦ ���� goto ���)

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

#define CHAT_REJECT		100

void Main(void)
{
	int lock = 0;
	volatile int i;

	Led_Init();

	Macro_Write_Block(rGPFCON, 0x3, 0x0, 8);
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	for(;;)
	{
EXIT:
		if((lock == 0) && Macro_Check_Bit_Clear(rGPFDAT, 4))
		{
			for(i=0; i<CHAT_REJECT; i++)
			{
				if(Macro_Check_Bit_Set(rGPFDAT, 4)) goto EXIT;
			}

			Macro_Invert_Bit(rGPBDAT, 7);
			lock = 1;
		}

		else if((lock == 1) && Macro_Check_Bit_Set(rGPFDAT, 4))
		{
			for(i=0; i<CHAT_REJECT; i++)
			{
				if(Macro_Check_Bit_Clear(rGPFDAT, 4)) goto EXIT;
			}

			lock = 0;
		}
	}
}

#endif

// 4���� ��� Key �ν�

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	Led_Init();

	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 18);
	Macro_Clear_Bit(rGPCDAT, 9);

	for(;;)
	{
		Led_Display(~Macro_Extract_Area(rGPFDAT, 0xF, 4));
	}
}

#endif

// ����Ű�� Push ��ư�� �ν�

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	Led_Init();

	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);
	Macro_Write_Block(rGPFCON, 0x3, 0x0, 0);
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	for(;;)
	{
		if(Macro_Check_Bit_Clear(rGPFDAT, 0))
		{
			Led_Display(0xF);
			continue;
		}

		Led_Display(~Macro_Extract_Area(rGPFDAT, 0xF, 4));
	}
}

#endif

// Key Matrix �ν�, ��ƼŰ�� ����

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	volatile int i;
	int x;
	static int key[] = {0,1,2,0,3,0,0,0,4,0,0,0,0,0,0,0};

	Uart_Init(115200);
	Led_Init();

	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);
	Macro_Write_Block(rGPFCON, 0x3, 0x0, 0);
	Macro_Clear_Area(rGPCDAT, 0x3, 8);

	for(;;)
	{
		Macro_Write_Block(rGPCCON, 0xf, 0x1, 16);
		for(i = 0; i < 0x1000; i++);
		x = ~Macro_Extract_Area(rGPFDAT, 0xF, 4) & 0xF;

		if(x)
		{
			Uart_Printf("%d\n", key[x]);
			continue;
		}

		if(Macro_Check_Bit_Clear(rGPFDAT, 0))
		{
			Uart_Printf("%d\n", 9);
			continue;
		}

		Macro_Write_Block(rGPCCON, 0xf, 0x4, 16);
		for(i = 0; i < 0x1000; i++);
		x = ~Macro_Extract_Area(rGPFDAT, 0xF, 4) & 0xF;

		if(x)
		{
			Uart_Printf("%d\n", key[x]+4);
		}
	}
}

#endif

// Key Matrix �ν�, ��ƼŰ ó��

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	volatile int i;
	int x;

	Uart_Init(115200);
	Led_Init();

	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);
	Macro_Write_Block(rGPFCON, 0x3, 0x0, 0);
	Macro_Clear_Area(rGPCDAT, 0x3, 8);

	for(;;)
	{
		Macro_Write_Block(rGPCCON, 0xf, 0x1, 16);
		for(i = 0; i < 0x1000; i++);
		x = (~rGPFDAT & 0xf0) >> 4;
		x |= (~rGPFDAT & 0x1) << 8;

		Macro_Write_Block(rGPCCON, 0xf, 0x4, 16);
		for(i = 0; i < 0x1000; i++);
		x |= ~rGPFDAT & 0xf0;

		if(x)
		{
			Uart_Printf("0x%X\n", x);
		}
	}
}

#endif

// ��ƼŰ ������ ����̽� ����̹� �Լ� ���� ���α׷�

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	Uart_Init(115200);
	Led_Init();

	Uart_Printf("KEY MATRIX TEST\n");
	Key_Poll_Init();

	for(;;)
	{
		Uart_Printf("KEY=%d\n", Key_Wait_Key_Pressed());
		Key_Wait_Key_Released();
	}
}

#endif

// ��ƼŰ ó�� ����̽� ����̹� �Լ� ���� ���α׷�

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	volatile int i;

	Uart_Init(115200);
	Led_Init();

	Uart_Printf("KEY MATRIX TEST - MULTI\n");
	Key_Poll_Init();

	for(;;)
	{
		Uart_Printf("KEY=0x%X\n", Key_Get_Pressed_Multi());
		for(i = 0; i < 0x1000000; i++);
	}
}

#endif
