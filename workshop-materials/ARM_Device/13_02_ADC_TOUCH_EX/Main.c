// Touch Data Conversion Test

#if 01

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	unsigned int r;

	Uart_Init(115200);
	Uart_Printf("Touch X-Y Input Test\n");
	Uart_Printf("Press the touch panel and drag\n");

	do
	{
		r = Touch_Get_Data();
		Uart_Printf("X=%#u, Y=%u\r", (r>>16)&0x3ff, r&0x3ff);
		Timer0_Delay(500);
	}while(!Uart_Get_Pressed());
}

#endif

// Touch Calibration

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

extern volatile int Touch_pressed;

void Main(void)
{
	unsigned int r;

	Uart_Init(115200);
	Uart_Printf("Touch Calibration\n");
	Lcd_Graphic_Init();
	Lcd_Clr_Screen(0);

	Touch_ISR_Enable(1);
	Uart_Printf("Press Left-Top Box\n");
	Lcd_Draw_Bar(3,3,7,7,0xF800);
	while(!Touch_pressed);
	r = Touch_Get_Data();
	Uart_Printf("XM=%u, YM=%u\n", (r>>16)&0x3ff, r&0x3ff);
	rADCTSC=(1<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3);
	rADCDLY=100;
	while(Touch_pressed);

	Touch_ISR_Enable(1);
	Uart_Printf("Press Right-Bottom Box\n");
	Lcd_Clr_Screen(0);
	Lcd_Draw_Bar(319-3,239-3,319-7,239-7,0xF800);
	while(!Touch_pressed);
	r = Touch_Get_Data();
	Uart_Printf("XP=%u, YP=%u\n", (r>>16)&0x3ff, r&0x3ff);
	Lcd_Clr_Screen(0);
}

#endif

// Touch Interrupt Test

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

extern volatile int Touch_pressed;
extern volatile int Touch_x, Touch_y;

void Main(void)
{
	Uart_Init(115200);
	Uart_Printf("Touch Interrupt Test\n");

	Touch_ISR_Enable(1);

	for(;;)
	{
		if(Touch_pressed)
		{
			Uart_Printf("Pressed: X=%u,Y=%u\n", Touch_x, Touch_y);
			while(Touch_pressed);
			Uart_Printf("Released\n");
		}
	}
}

#endif

// Drag & Drop Test

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

extern volatile int Touch_pressed;
extern volatile int Touch_x, Touch_y;

void Main(void)
{
	Uart_Init(115200);
	Uart_Printf("Drag & Drop Test\n");

	Touch_ISR_Enable(1);

	for(;;)
	{
		if(Touch_pressed)
		{
			Uart_Printf("Pressed: X=%u,Y=%u\n", Touch_x, Touch_y);

			for(;;)
			{
				Touch_Get_Position();
				Uart_Printf("Drag: X=%u,Y=%u\n", Touch_x, Touch_y);
				Touch_Set_Wait_For_Release();

				if(!Touch_pressed)
				{
					Uart_Printf("Drop\n");
					break;
				}
			}
		}
	}
}

#endif
