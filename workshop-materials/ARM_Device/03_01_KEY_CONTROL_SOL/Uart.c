#include "2440addr.h"
#include "macro.h"
#include "option.h"
#include "device_driver.h"

#include <stdio.h>
#include <stdarg.h>

// Uart0 (Debug Port) 

void Uart_Init(int baud)
{
	// PORT GPIO initial
	Macro_Write_Block(rGPHCON, 0xf, 0xa, 4);

	rUFCON0 = 0x0;
	rUMCON0 = 0x0;
	rULCON0 = 0x3;
	rUCON0  = 0x245;		
	rUBRDIV0= ((unsigned int)(PCLK/16./baud+0.5)-1 );
}

void Uart_Send_Byte(char data)
{
	if(data == '\n')
	{
		while(Macro_Check_Bit_Clear(rUTRSTAT0,1));
		WrUTXH0('\r');
	}
	while(Macro_Check_Bit_Clear(rUTRSTAT0,1));
	WrUTXH0(data);
}               

void Uart_Send_String(char *pt)
{
	while(*pt) Uart_Send_Byte(*pt++);
}

void Uart_Printf(char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_Send_String(string);
    va_end(ap);
}

