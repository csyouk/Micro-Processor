// RTC Test

#if 1

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	int saved_sec = 0xff;

	enum key{C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2};
	enum note{N16, N8, N4, N2, N1};
	enum DAY{SUN=1, MON, TUE, WED, THU, FRI, SAT};
	static char day[][4]={"\n","SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
	RTC_TIME a = {SUN,9,3,1,19,59,57};

	Timer3_Buzzer_Init();
	Uart_Init(115200);
	Uart_Printf("RTC Test\n");

	RTC_Set_Time(&a);

	for(;;)
	{
		do
		{
			RTC_Get_Time(&a);
		}while(saved_sec == a.sec);

		Uart_Printf("[%2d/%2d/%2d]%2d:%2d:%2d(%3s)\r", a.year, a.mon, a.date, a.hour, a.min, saved_sec = a.sec, day[a.day]);
		Timer3_Buzzer_Beep(E1,N16);
	}
}

#endif

// RTC Alarm Test

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

extern volatile int RTC_alarm;

void Main(void)
{
	int saved_sec = 0xff;

	enum key{C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2};
	enum note{N16, N8, N4, N2, N1};

	enum DAY{SUN=1, MON, TUE, WED, THU, FRI, SAT};
	static char day[][4]={"\n","SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

	RTC_TIME     a = {SUN,9,3,1,19,59,57};
	RTC_TIME alarm = {SUN,9,3,1,19, 0, 3};
	RTC_TIME match = {SUN,0,0,0,0,1,1};

	RTC_Set_Time(&a);
	RTC_Set_Alarm(&alarm);
	RTC_Set_Alarm_Enable(1, &match);

	Timer3_Buzzer_Init();
	Uart_Init(115200);
	Uart_Printf("RTC Alarm Test\n");

	for(;;)
	{
		RTC_Get_Time(&a);

		if(saved_sec != a.sec)
		{
			RTC_Get_Time(&a);
			Uart_Printf("[%2d/%2d/%2d]%2d:%2d:%2d(%3s)\r", a.year, a.mon, a.date, a.hour, a.min, saved_sec = a.sec, day[a.day]);
		}

		if(RTC_alarm)
		{
			RTC_alarm = 0;
			Timer3_Buzzer_Beep(E1,N16);
			Uart_Printf("\n\nWake Up!!!\n");
			match.min = 0;
			RTC_Set_Alarm_Enable(1, &match);
		}
	}
}

#endif

// RTC Tick Interrupt Test

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

extern volatile int RTC_tick;

void Main(void)
{
	RTC_Tick_ISR_Enable(1, 500);
	Uart_Init(115200);
	Uart_Printf("RTC Tick Interrupt Test\n");

	for(;;)
	{
		if(RTC_tick)
		{
			Uart_Printf(".");
			RTC_tick = 0;
		}
	}
}

#endif
