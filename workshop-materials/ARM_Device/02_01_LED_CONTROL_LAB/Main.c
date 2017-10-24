// LED ON-OFF ����

#if 01

#define GPBCON *(volatile unsigned int *)0x56000010
#define GPBDAT *(volatile unsigned int *)0x56000014
#define TENTH_OF_ONE_SEC 10000000
#define ONE_SEC 100000000

void Main(void)
{
	volatile int i;
	int delay=1;
	GPBCON |= (0x55<<14);
	GPBDAT &= ~(0xf<<7);

	for(;;){
		for(i=0; i<delay; i++);
		GPBDAT ^= (0xf<<7);
		delay += 500;
	}
}

#endif

// �����翡�� �����ϴ� �������� #define Ȱ��

#if 0

#include "2440addr.h"

void Main(void)
{
	// read - modify - write
	rGPBCON = (rGPBCON &~(0xff<<14)) | (0x55<<14);
	rGPBDAT = (rGPBDAT &~(0xf<<7)) | (~0xf)<<7;

	for(;;);
}

#endif

// ��Ʈ ������ �̿��� LED ����

#if 0

#include "2440addr.h"

void Main(void)
{



	for(;;);
}

#endif

// Macro�� Ȱ���� LED ����

#if 0

#include "2440addr.h"
#include "macro.h"

void Main(void)
{
	volatile int i;

	// LED�� ��� ��Ʈ�� �����ϰ� �ʱ⿡ LED1, LED3 ON

	Macro_Write_Block(rGPBCON, 0xff, 0x55,14);
	Macro_Write_Block(rGPBDAT, 0xf , 0x5,7);
	for(;;)
	{
		// GPB10~7�� ������Ų��
		Macro_Invert_Area(rGPBDAT, 0xf,7);
		for(i=0; i<0xf00000; i++);
	}
}

#endif

// Device Driver ���� �ڵ�

#if 0

#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	int cnt = 0;
	volatile int i;

	Led_Init();

	for(;;)
	{
		Led_Display(cnt++);
		for(i=0; i<0x1000000; i++);
		if(cnt % (0xf+1) == 0) cnt = 0;

	}
}

#endif
