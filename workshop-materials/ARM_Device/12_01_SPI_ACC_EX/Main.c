// SPI1 & Acc Test

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	int s;
	signed char x, y, z;

	Uart_Init(115200);
	Uart_Printf("SPI1 & ACC Sensor Test\n");

	Uart_Printf("ACC ID = %x\n", SPI1_Acc_Init());
	Timer0_Delay(1000);

	for(;;)
	{
		s = SPI1_ACC_Read_Status();

		if(Macro_Check_Bit_Set(s,3))
		{
			x = SPI1_ACC_Read_X();
			y = SPI1_ACC_Read_Y();
			z = SPI1_ACC_Read_Z();
			Uart_Printf("S=%#x, X=%d, Y=%d, Z=%d\n", s,x,y,z);
			Timer0_Delay(500);
		}
	}
}
