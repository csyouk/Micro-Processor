// LED ON-OFF ����

#if 0

#define GPBCON	(*(volatile unsigned int *)0x56000010)
#define GPBDAT	(*(volatile unsigned int *)0x56000014)

void Main(void)
{
	GPBCON = 0x00154000;
	GPBDAT = 0x00000300;

	for(;;);
}

#endif

// �����翡�� �����ϴ� �������� #define Ȱ��

#if 0

#include "2440addr.h"

void Main(void)
{
	rGPBCON = 0x00154000;
	rGPBDAT = 0x00000300;

	for(;;);
}

#endif

// ��Ʈ ������ �̿��� LED ����

#if 0

#include "2440addr.h"

void Main(void)
{
	rGPBCON = (rGPBCON & ~(0xFF << 14)) | (0x55 << 14);
	rGPBDAT = (rGPBDAT & ~(0xF << 7)) | (0x9 << 7);

	for(;;);
}

#endif

// Macro�� Ȱ���� LED ����

#if 01

#include "2440addr.h"
#include "macro.h"

void Main(void)
{
	volatile int i;

	Macro_Write_Block(rGPBCON, 0xFF, 0x55, 14);
	Macro_Write_Block(rGPBDAT, 0xF, 0xA, 7);

	for(;;)
	{
		Macro_Invert_Area(rGPBDAT, 0xF, 7);
		for(i=0; i<0x1000000; i++);
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
		for(i=0; i<0x2000000; i++);
	}
}

#endif
