#include "2440addr.h"
#include "macro.h"
#include "option.h"
#include "device_driver.h"

#define TIMER0_MAX	0xffff
#define TIMER0_TICK	20
#define TIMER0_PULSE_FOR_1MS (1000/TIMER0_TICK)

void Timer0_Stop_Watch_Run(void)
{
	// Timer0 Prescaler와 MUX를 20usec tick (50KHz)가 되도록 설정한다
  // Macro_Write_Block(rTCFG0, 0xff,0xf9, 0);
	// Refactored
	// 0xf9 => (int)(PCLK/50000./4 + 0.5) 이 값이 8비트 이내의 값인지는 확인해야..
	Macro_Write_Block(rTCFG0, 0xff,(int)(PCLK/50000./4 + 0.5) - 1, 0);

	Macro_Write_Block(rTCFG1, 0xf,0x1, 0);

	// TCNTB0에 최대값을 저장한다
	rTCNTB0 = TIMER0_MAX;

	// Timer0 Manual Update On & Timer Off
	Macro_Write_Block(rTCON,0xf,0x2,0);

	// Timer0 Manual Update Off & Timer ON
	Macro_Write_Block(rTCON,0xf,0x1,0);
}

unsigned int Timer0_Stop_Watch_Stop(void)
{
	// Timer0 Off
	Macro_Clear_Bit(rTCON,0);
	// 경과 펄스수에 20usec를 곱한 값을 리턴
	return (TIMER0_MAX - rTCNTO0) * TIMER0_TICK;
}

int Timer0_Stop_Watch_Run_Unlimited(unsigned int max_msec)
{
	int i;
	int n;
	int prescaler;

	for(i = 0, n = 2; n <= 16; n*=2, i++)
	{
		prescaler = \
				(int)(((double)PCLK * (double)max_msec)/(1000.0 * 65535.0 * n));

		if(prescaler <= 256)
		{
			Macro_Write_Block(rTCFG0, 0xff, prescaler - 1, 0);
			Macro_Write_Block(rTCFG1, 0xf, i, 0);
			rTCNTB0 = 0xffff;
			Macro_Write_Block(rTCON,0x3, 2, 0);
			Macro_Write_Block(rTCON,0x3, 1, 0);
			return 1;
		}
	}

	return 0;
}

unsigned int Timer0_Stop_Watch_Stop_Unlimited(void)
{
	int mux[] = {2,4,8,16};

	Macro_Clear_Bit(rTCON, 0);
	return (int)((1000000.0 * (0xffff - rTCNTO0) * mux[Macro_Extract_Area(rTCFG1, 0xf, 0)] * (Macro_Extract_Area(rTCFG0, 0xff, 0) + 1) / (double)PCLK) + 0.5);
}


/* 시간 제약이 약 1.25초 정도인 함수 */

#if 1

void Timer0_Delay(int time)
{
	// 20usec tick (50KHz)로 Prescaler와 MUX 설정
	Macro_Write_Block(rTCFG0, 0xff, (int)(PCLK/50000./4. + 0.5) - 1, 0);
	Macro_Write_Block(rTCFG1, 0xf,0x1, 0);


	// TCNTB0에 주어진 time(msec 단위)를 지연하기 위한 시간 설정
//	rTCNTB0 = time * 50;
	rTCNTB0 = time * TIMER0_PULSE_FOR_1MS;

	// SRCPND 레지스터 해당 비트 PENDING Clear
	rSRCPND = 1<<10;

	// Timer0 Manual Update On & Timer Off
	Macro_Write_Block(rTCON,0xf,0x2,0);

	// Timer0 Manual Update Off & Timer ON
	Macro_Write_Block(rTCON,0xf,0x1,0);

	// Timer0 종료 대기
	while(Macro_Check_Bit_Clear(rSRCPND, 10));

	// SRCPND 레지스터 해당 비트 PENDING Clear
	rSRCPND = 1<<10;
}



/* 시간 제약이 없는 함수 */

void Timer0_Delay_Unlimited(int time)
{
	int i;
	unsigned int temp = time * 50;

	Macro_Write_Block(rTCFG0, 0xff, PCLK/(50000 * 8)-1, 0);
	Macro_Write_Block(rTCFG1, 0xf, 2, 0);

	for(i=0; i<(temp/0x10000); i++)
	{
		rTCNTB0 = 0xFFFF;
		rSRCPND = BIT_TIMER0;
		Macro_Write_Block(rTCON,0x3, 2, 0);
		Macro_Write_Block(rTCON,0x3, 1, 0);

		while(Macro_Check_Bit_Clear(rSRCPND, TIMER0));
		rSRCPND = 1<<10;;
	}

	if(temp % 0x10000)
	{
		rTCNTB0 = (temp % 0x10000) - 1;
		rSRCPND = BIT_TIMER0;
		Macro_Write_Block(rTCON,0x3, 2, 0);
		Macro_Write_Block(rTCON,0x3, 1, 0);

		while(Macro_Check_Bit_Clear(rSRCPND, TIMER0));
		rSRCPND = 1<<10;;
	}
}

#endif

void Timer0_Repeat(int time)
{
	// 20usec tick (50KHz)로 Prescaler와 MUX 설정
	Macro_Write_Block(rTCFG0, 0xff, (int)(PCLK/50000./4 + 0.5) - 1, 0);
	Macro_Write_Block(rTCFG1, 0xf, 0x1, 0);

	// TCNTB0에 주어진 time(msec 단위) 지연 시간 설정
	rTCNTB0 = time * TIMER0_PULSE_FOR_1MS;

	// SRCPND 레지스터 해당 비트 PENDING Clear
	rSRCPND = 1<<10;

	// Manual Update ON & TImer OFF
	Macro_Write_Block(rTCON, 0xf, 0x2, 0);

	// Manual Update OFF & TImer ON, Auto Reload ON
	Macro_Write_Block(rTCON, 0xf, 0x9, 0);
}

int Timer0_Check_Timeout(void)
{
	// Timer가 Expired 되었으면 Pending Clear하고 1 리턴
	// Timer가 Expired 되지 않았으면 0 리턴
	if(Macro_Check_Bit_Set(rSRCPND, 10)) {
		rSRCPND = 1<<10;
		return 1;
	}
	return 0;
}

void Timer0_Stop(void)
{
	// Timer0 Stop
	Macro_Write_Block(rTCON, 0x1, 0x0, 0);
//	Macro_Clear_Bit(rTCON, 0);
}

void Timer0_Set_Time(int time)
{
	rTCNTB0 = (time * TIMER0_PULSE_FOR_1MS) - 1;
}

