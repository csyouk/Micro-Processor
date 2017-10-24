#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

void Led_Init(void)
{
	// LED(GPB10~7)�� ������� �����ϰ� �ʱ⿡ ��� OFF�Ѵ�
	Macro_Write_Block(rGPBCON, 0xff, 0x55, 14);
	Macro_Write_Block(rGPBDAT, 0xf , 0xf ,  7);
}

void Led_Display(int disp)
{
	// disp ���� ���� LED�� �����Ѵ�
	Macro_Write_Block(rGPBDAT, 0xf , (~disp & 0xf) ,  7);
}
