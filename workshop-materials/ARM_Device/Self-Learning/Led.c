#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

void Led_Init(void)
{
	Macro_Write_Block(rGPBCON, 0xff, 0x55, 14);
	Macro_Set_Area(rGPBDAT, 0xf, 7);
}

