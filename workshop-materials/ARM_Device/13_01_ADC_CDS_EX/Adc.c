#include "2440addr.h"
#include "option.h"
#include "macro.h"
#include "device_driver.h"

unsigned int ADC_Start(int ch)
{
	rADCCON = (1<<14)|(39<<6)|(0<<2)|(0<<1)|(0<<0);
	rADCDLY = 0xffff;	
	Macro_Write_Block(rADCCON, 0x7, ch & 0x7, 3);
	Macro_Set_Bit(rADCCON, 0);

	while(Macro_Check_Bit_Set(rADCCON, 0));
	while(Macro_Check_Bit_Clear(rADCCON, 15));

	return Macro_Extract_Area(rADCDAT0, 0x3ff, 0);
}

void ADC_ISR_Init(int en, int ch)
{
	if(en)
	{
		rADCDLY = 0xff;
		rADCCON = (1<<14)|(39<<6)|((ch & 0x7)<<3)|(0<<2)|(1<<1)|(0<<0);
		rSUBSRCPND = 1<<10;
		rSRCPND = 1<<31;
		rINTPND = 1<<31;
		Macro_Clear_Bit(rINTMSK, 31);
		Macro_Clear_Bit(rINTSUBMSK, 10);
		en = rADCDAT0;
	}

	else
	{
		rADCCON = 0;
		Macro_Set_Bit(rINTMSK, 31);
		Macro_Set_Bit(rINTSUBMSK, 10);
	}
}
