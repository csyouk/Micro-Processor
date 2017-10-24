// Timer0 Stop Watch

#if 01

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	volatile int i, k;
	unsigned int time = 0x10000;
	unsigned int j = 1;

	Uart_Init(115200);
	Led_Init();
	Key_Poll_Init();

	Uart_Printf("Timer Stop Watch Test\n");

	for(k = 0; k < 10; k++)
	{
		Timer0_Stop_Watch_Run();

		for(i=0; i<(time * j); i++);

		Uart_Printf("Loop Time(0x%X) = %u usec\n", (time * j), Timer0_Stop_Watch_Stop());
		j++;
	}
}

#endif

// Timer0 Stop Watch Unlimited

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	volatile int i, k;
	unsigned int time = 0x10000;
	unsigned int j = 1;

	Uart_Init(115200);
	Led_Init();
	Key_Poll_Init();

	Uart_Printf("Timer Stop Watch Unlimited Test\n");

	for(k = 0; k < 10; k++)
	{
		i = Timer0_Stop_Watch_Run_Unlimited(5000);
		if(i == 0) Uart_Printf("Max Time Configuration Error\n");

		for(i=0; i<(time * j); i++);

		Uart_Printf("Loop Time(0x%X) = %u usec\n", (time * j), Timer0_Stop_Watch_Stop_Unlimited());
		j++;
	}
}

#endif

// Delay 함수 검증

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	int i=0;

	Led_Init();
	Uart_Init(115200);

	Uart_Printf("\nLoop Delay Function Test\n\n");

	#if 1

	for(;;)
	{
		Timer0_Delay(1000);
		Uart_Printf("\r[%2u]sec", i++);
		if(i==100) i=0;
	}

	#else

	for(;;)
	{
		Timer0_Delay(5000); // 5초
		Uart_Printf("\r[%2u]sec", i++ * 5);
		if(i==100) i=0;
	}

	#endif
}

#endif

// Repeat 함수 검증

#if 0

#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

void Main(void)
{
	int i = 0;
	int disp = 0xf;

	Led_Init();
	Uart_Init(115200);

	Uart_Printf("\nAuto Reload Test\n\n");

	Timer0_Repeat(1000);

	for(;;)
	{
		if(Timer0_Check_Timeout())
		{
			Led_Display(disp = ~disp);
			Uart_Printf("[%d]\n", i);
			if(i++ == 10) Timer0_Stop();
		}
	}
}

#endif

// Repeat 동작 중 주기의 변경

#if 0

#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

void Main(void)
{
	int i = 0;
	int j;
	int disp = 0xf;

	Led_Init();
	Uart_Init(115200);

	Uart_Printf("\nVariable Interval Test\n\n");

	Timer0_Repeat(100);

	for(j=1;;j=(j>=10)?(1):(j+1))
	{
		Timer0_Set_Time(100*j);
		Uart_Printf("Delay Time = %dmsec\n", j*100);
		i=0;

		for(;;)
		{
			if(Timer0_Check_Timeout())
			{
				Led_Display(disp = ~disp);
				Uart_Printf("[%d]\n", i);
				if(i++ == 4) break;
			}
		}
	}
}

#endif

// Buzzer Beep 함수 검증

#if 0

#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

void Main(void)
{
	int i = 0;

	enum key{C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2};
	enum note{N16, N8, N4, N2, N1};

	const int song[][2] = {{G1,N4},{G1,N4},{E1,N8},{F1,N8},{G1,N4},{A1,N4},{A1,N4},{G1,N2},{G1,N4},{C2,N4},{E2,N4},{D2,N8},{C2,N8},{D2,N2},{0xff, 0xff}};

	Led_Init();
	Uart_Init(115200);

	Uart_Printf("Timer3 & Buzzer Test\n");

	Timer3_Buzzer_Init();

	Timer3_Buzzer_Beep(C1,N4);
	Timer3_Buzzer_Beep(D1,N4);
	Timer3_Buzzer_Beep(E1,N4);
	Timer3_Buzzer_Beep(F1,N4);
	Timer3_Buzzer_Beep(G1,N4);
	Timer3_Buzzer_Beep(A1,N4);
	Timer3_Buzzer_Beep(B1,N4);
	Timer3_Buzzer_Beep(C2,N4);

	Timer0_Delay(1000);

	while(song[i][0] != 0xff)
	{
		Timer3_Buzzer_Beep(song[i][0], song[i][1]);
		Uart_Printf(".");
		i++;
	}
}

#endif


