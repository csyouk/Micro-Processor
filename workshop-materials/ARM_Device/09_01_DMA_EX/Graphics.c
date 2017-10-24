#include "2440addr.h"
#include "option.h"
#include "macro.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "device_driver.h"

//////////////////// GBOX LCD SPI Control ////////////////////////

#define LCD_SPI0_CLK		(20000000)
#define LCD_SPI0_ENABLE() 	Macro_Clear_Bit(rGPGDAT, 2)
#define LCD_SPI0_DISABLE()	Macro_Set_Bit(rGPGDAT, 2)
#define LCD_WRITE			(0<<0)
#define LCD_REG				(0<<1)
#define LCD_DATA			(1<<1)
#define LCD_ID				(0x1c<<2)
#define SPI0_WAIT_SEND()	while(!(rSPSTA0 & 1))

void SPI0_LCD_Init(void);
void SPI0_LCD_Write(int reg, int data);

void SPI0_LCD_Init(void)
{
	double d;

	Macro_Write_Block(rGPECON, 0xf, 0xa, 24);
	Macro_Write_Block(rGPGCON, 0x3, 0x1, 4);
	LCD_SPI0_DISABLE();

	rSPCON0 = (0<<5) + (1<<4) + (1<<3) + (0<<2) + (0<<1) + (0<<0);
	d = PCLK/(2 * LCD_SPI0_CLK);
	rSPPRE0 = (d == 0)? ((int)d -1) : ((int)d);
	rSPPIN0 = (0<<2) + (0<<0);
}

void SPI0_LCD_Write(int reg, int data)
{
	LCD_SPI0_ENABLE();
	rSPTDAT0 = LCD_ID + LCD_REG + LCD_WRITE;
	SPI0_WAIT_SEND();
	rSPTDAT0 = (reg>>8) & 0xff;
	SPI0_WAIT_SEND();
	rSPTDAT0 = reg & 0xff;
	SPI0_WAIT_SEND();
	LCD_SPI0_DISABLE();

	LCD_SPI0_ENABLE();
	rSPTDAT0 = LCD_ID + LCD_DATA + LCD_WRITE;
	SPI0_WAIT_SEND();
	rSPTDAT0 = (data>>8) & 0xff;
	SPI0_WAIT_SEND();
	rSPTDAT0 = data & 0xff;
	SPI0_WAIT_SEND();
	LCD_SPI0_DISABLE();
}

//////////////////// GBOX LCD Configuration ////////////////////////

#define rLPCSEL (*(volatile unsigned int *)0x4d000060)

#define LCD_XSIZE 		(960)
#define LCD_YSIZE 		(240)
#define HOZVAL			(LCD_XSIZE-1)
#define LINEVAL			(LCD_YSIZE-1)

#define VBPD_030561		((15-1)&0xff)
#define VFPD_030561		((4-1)&0xff)
#define VSPW_030561		((3-1)&0x3f)
#define HBPD_030561		((114-1)&0x7f)
#define HFPD_030561		((60-1)&0xff)
#define HSPW_030561 	((90-1)&0xff)
#define CLKVAL			1

#define M5D(n) 		((n) & 0x1fffff)

void Lcd_Init(void);
void Lcd_Port_Init(void);
void Lcd_Power_Enable(int en);
void Lcd_Reset(int reset);
void Lcd_Envid_On_Off(int onoff);
void Lcd_Set_Address(unsigned int fp);

void Lcd_Reset(int reset)
{
   if(reset) Macro_Clear_Bit(rGPCDAT, 0);
   else Macro_Set_Bit(rGPCDAT, 0);
}

void Lcd_Port_Init(void)
{
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 0);
	Macro_Clear_Bit(rGPCDAT, 0);
	Macro_Write_Block(rGPGCON, 0x3, 0x3, 8);
	rGPCUP=0xffffffff;
	Macro_Write_Block(rGPCCON, 0xff, 0xaa, 2);
	rGPDUP=0xffffffff;
	Macro_Write_Block(rGPDCON, 0xfff, 0xaaa, 20);
}

void Lcd_Envid_On_Off(int onoff)
{
	(onoff) ? (rLCDCON1 |= 1) : (rLCDCON1 &= ~0x1);
}

void Lcd_Power_Enable(int en)
{
    rLCDCON5= (rLCDCON5&(~(1<<3)))|(en<<3);
}

void Lcd_Init(void)
{
	rLCDCON1=(CLKVAL<<8)|(3<<5)|(13<<1)|0;
	rLCDCON2=(VBPD_030561<<24)|((LINEVAL)<<14)|(VFPD_030561<<6)|(VSPW_030561);
	rLCDCON3=(HBPD_030561<<19)|(HOZVAL<<8)|(HFPD_030561);
	rLCDCON4=(HSPW_030561);
	rLCDCON5=(0<<12)|(1<<10)|(1<<9)|(1<<8);
	rLCDINTMSK|=(3);
	rLPCSEL &=(~7);
}

void Lcd_Set_Address(unsigned int fp)
{
	rLCDSADDR1=((fp>>22)<<21)+M5D(fp>>1);
	rLCDSADDR2=M5D(fp+(LCD_XSIZE*LCD_YSIZE*2));
	rLCDSADDR3=LCD_XSIZE;
}

void Lcd_Graphic_Init(void)
{
	Lcd_Port_Init();void Lcd_Set_Address(unsigned int fp)
	{
		rLCDSADDR1=((fp>>22)<<21)+M5D(fp>>1);
		rLCDSADDR2=M5D(fp+(LCD_XSIZE*LCD_YSIZE*2));
		rLCDSADDR3=LCD_XSIZE;
	}
	Lcd_Init();
	Lcd_Reset(0);
	Lcd_Envid_On_Off(0);

   	Lcd_Set_Address(LCD_FB0);

	SPI0_LCD_Init();
	SPI0_LCD_Write(0x0001,0x7300);
	SPI0_LCD_Write(0x0002,0x0200);
	SPI0_LCD_Write(0x0003,0x6364);
	SPI0_LCD_Write(0x0004,0x04cf);
	SPI0_LCD_Write(0x0005,0xbcd4);
	SPI0_LCD_Write(0x000a,0x4008);
	SPI0_LCD_Write(0x000b,0xd400);
	SPI0_LCD_Write(0x000d,0x3229);
	SPI0_LCD_Write(0x000e,0x3200);
	SPI0_LCD_Write(0x000f,0x0000);
	SPI0_LCD_Write(0x0016,0x9f80);
	SPI0_LCD_Write(0x0017,0x3fff);
	SPI0_LCD_Write(0x001e,0x0052);

	Lcd_Clr_Screen(0);
	Lcd_Envid_On_Off(1);
	Lcd_Power_Enable(1);
}

typedef unsigned int (* Fptr)[960];

volatile Fptr Fb_ptr = (Fptr)LCD_FB0;

void Lcd_Clr_Screen(int color)
{
	int i,j;

	for(j=0;j<LCD_YSIZE;j++)
	{
		for(i=0;i<(LCD_XSIZE);i+=3)
		{
			Fb_ptr[j][i] = ((color>>8)&(0xf8))<<16;
			Fb_ptr[j][i+1]= ((color>>3)&(0xf8))<<16;
			Fb_ptr[j][i+2]= ((color<<2)&(0xf8))<<16;;		        		        
		}
	}
}

void Lcd_Put_Pixel(int x,int y,int color)
{
	Fb_ptr[y][3*x] 	 = ((color>>8)&(0xf8))<<16;
	Fb_ptr[y][3*x+1] = ((color>>3)&(0xf8))<<16;
	Fb_ptr[y][3*x+2] = ((color<<2)&(0xf8))<<16;
}

void Lcd_Draw_BMP(int x, int y, const unsigned short int *fp)
{
	register int width = fp[0], height = fp[1];
	register int xx, yy;
	
	for(yy=0;yy<height;yy++)
	{
		for(xx=0;xx<width;xx++)
		{
			Lcd_Put_Pixel(xx+x,yy+y,(int)fp[yy*width+xx+2]);
		} 
	}
}

