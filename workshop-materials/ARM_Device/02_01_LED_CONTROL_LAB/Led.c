#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

void Led_Init(void)
{
	// LED(GPB10~7)을 출력으로 설정하고 초기에 모두 OFF한다
	Macro_Write_Block(rGPBCON, 0xff, 0x55, 14);
	Macro_Write_Block(rGPBDAT, 0xf , 0xf ,  7);
}

void Led_Display(int disp)
{
	// disp 값에 따라 LED를 제어한다
	Macro_Write_Block(rGPBDAT, 0xf , (~disp & 0xf) ,  7);
}
