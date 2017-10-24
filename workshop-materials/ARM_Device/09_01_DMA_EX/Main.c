// DMA 특성 테스트

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

extern volatile unsigned int * DSTAT[];
extern volatile unsigned int * DMASKTRIG[];

extern volatile int DMA_complete[];

#define BUF_MAX 	10

int sbuf[BUF_MAX] = {10,20,30,40,50,60,70,80,90,100};

// Caching 영역의 불일치 방지를 위하여 Non-Cache 영역으로 Destination을 확보함
volatile int *dbuf = (int *)0x31C80000;

void Main(void)
{
	int i;
	int ch = 1;

	UNI_SRCC srcc;
	UNI_DSTC dstc;
	UNI_DCON dcon;

	Uart_Init(115200);
	Led_Init();

	Uart_Printf("DMA1 test\n");

	// S/W Trigger
	srcc.udata = 0;
	srcc.st.INC = DMA_ADDR_INC;
	srcc.st.LOC = DMA_LOC_AHB;

	dstc.udata = 0;
	dstc.st.INC = DMA_ADDR_INC;
	dstc.st.LOC = DMA_LOC_AHB;
	dstc.st.CHK_INT = DMA_INT_TC;

	dcon.udata = 0;
	dcon.st.SYNC = DMA_SYNC_HCLK;
	dcon.st.INT = DMA_INT_EN;
	dcon.st.SWHW_SEL = DMA_TRIGGER_SW;
	dcon.st.RELOAD = DMA_RELOAD_OFF;

	dcon.st.DMD_HS = DMA_DEMAND;
	dcon.st.SERVMODE = DMA_SVC_WHOLE;

	dcon.st.TSZ = DMA_TSZ_1UNIT;
	dcon.st.DSZ = DMA_DSZ_4B;
	dcon.st.TC = BUF_MAX;

	DMA_Start(ch, (void *)sbuf, (void *)dbuf, srcc, dstc, dcon);

	while(!DMA_complete[ch]);
	Uart_Printf("[1]DMA_complete!\n", ch);
	DMA_complete[ch] = 0;

	for(i=0; i < BUF_MAX; i++)
	{
		Uart_Printf("[%d] SRC = %d, DST = %d\n", i, sbuf[i], dbuf[i]);
		dbuf[i] = 0;
	}

	// TSZ, DSZ, TC의 관계
	dcon.st.TSZ = DMA_TSZ_4UNIT;
	dcon.st.DSZ = DMA_DSZ_4B;
	dcon.st.TC = BUF_MAX/4;

	DMA_Start(ch, (void *)sbuf, (void *)dbuf, srcc, dstc, dcon);

	while(!DMA_complete[ch]);
	Uart_Printf("[2]DMA_complete!\n", ch);
	DMA_complete[ch] = 0;

	for(i=0; i < BUF_MAX; i++)
	{
		Uart_Printf("[%d] SRC = %d, DST = %d\n", i, sbuf[i], dbuf[i]);
		dbuf[i] = 0;
	}

	// Single Service & Handshake Mode
	dcon.st.TSZ = DMA_TSZ_1UNIT;
	dcon.st.DSZ = DMA_DSZ_4B;
	dcon.st.TC = BUF_MAX;
	dcon.st.DMD_HS = DMA_HANDSHAKE;
	dcon.st.SERVMODE = DMA_SVC_SINGLE;

	DMA_Start(ch, (void *)sbuf, (void *)dbuf, srcc, dstc, dcon);

	for(;;)
	{
		Uart_Printf("CURR_TC = 0x%x\n", *DSTAT[ch]);
		Macro_Set_Bit(*DMASKTRIG[ch], 0);
		Timer0_Delay(300);
		if(DMA_complete[ch]) break;
	}

	Uart_Printf("[3]DMA_complete!\n", ch);
	DMA_complete[ch] = 0;

	for(i=0; i < BUF_MAX; i++)
	{
		Uart_Printf("[%d] SRC = %d, DST = %d\n", i, sbuf[i], dbuf[i]);
		dbuf[i] = 0;
	}

	// Single Service & Demand Mode
	// Internal은 무관, External이면 Single일 때는 Handshake로 설정하는 것이 안전하다
	dcon.st.DMD_HS = DMA_DEMAND;
	dcon.st.SERVMODE = DMA_SVC_SINGLE;

	DMA_Start(1, (void *)sbuf, (void *)dbuf, srcc, dstc, dcon);

	for(;;)
	{
		Uart_Printf("CURR_TC = 0x%x\n", *DSTAT[ch]);
		Macro_Set_Bit(*DMASKTRIG[ch], 0);
		Timer0_Delay(300);
		if(DMA_complete[ch]) break;
	}

	Uart_Printf("[4]DMA_complete!\n", ch);
	DMA_complete[ch] = 0;

	for(i=0; i < BUF_MAX; i++)
	{
		Uart_Printf("[%d] SRC = %d, DST = %d\n", i, sbuf[i], dbuf[i]);
		dbuf[i] = 0;
	}
}

#endif

// S/W에 의한 IMAGE 전송

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

#include ".\Image\img01.h"
#include ".\Image\img02.h"
#include ".\Image\img03.h"
#include ".\Image\img04.h"

extern void Lcd_Graphic_Init(void);
extern void Lcd_Clr_Screen(int color);
extern void Lcd_Draw_BMP(int x, int y, const unsigned short int *fp);

/* 1초마다 그림 한장을 그리기 위하여 LCD에 그림을 그리기 위하여 그림을 LCD_FB1 영역으로 복사한다 */
/* LCD_FB1 => 0x31C00000 번지 (Non-Caching 영역)*/

const unsigned short * img[] = {img01, img02, img03, img04};

void Main(void)
{
	int j, i = 0;

	Uart_Init(115200);
	Led_Init();

	Uart_Printf("LCD Display (SW)\n");

	Lcd_Graphic_Init();
	Lcd_Clr_Screen(0xf800);

	for(;;)
	{
		for(i=0; i<(sizeof(img)/sizeof(img[0])); i++)
		{
			for(j=0;j<(320*240+2); j++)
			{
				((unsigned short *)(LCD_FB1))[j] = img[i][j];
			}

			Lcd_Draw_BMP(0,0,(void *)LCD_FB1);
			Timer0_Delay(1000);
		}
	}
}

#endif

// DMA에 의한 IMAGE 전송

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

#include ".\Image\img01.h"
#include ".\Image\img02.h"
#include ".\Image\img03.h"
#include ".\Image\img04.h"

const unsigned short * img[] = {img01, img02, img03, img04};
extern volatile int DMA_complete[];

void Main(void)
{
	int i = 0;
	int ch = 1;

	UNI_SRCC srcc;
	UNI_DSTC dstc;
	UNI_DCON dcon;

	Uart_Init(115200);
	Led_Init();

	Uart_Printf("LCD Display (DMA)\n");

	Lcd_Graphic_Init();
	Lcd_Clr_Screen(0xf800);

	srcc.udata = 0;
	srcc.st.INC = DMA_ADDR_INC;
	srcc.st.LOC = DMA_LOC_AHB;

	dstc.udata = 0;
	dstc.st.INC = DMA_ADDR_INC;
	dstc.st.LOC = DMA_LOC_AHB;
	dstc.st.CHK_INT = DMA_INT_TC;

	dcon.udata = 0;
	dcon.st.SYNC = DMA_SYNC_HCLK;
	dcon.st.INT = DMA_INT_EN;
	dcon.st.SWHW_SEL = DMA_TRIGGER_SW;
	dcon.st.RELOAD = DMA_RELOAD_OFF;
	dcon.st.DMD_HS = DMA_DEMAND;
	dcon.st.SERVMODE = DMA_SVC_WHOLE;

	dcon.st.TSZ = DMA_TSZ_1UNIT;
	dcon.st.DSZ = DMA_DSZ_2B;
	dcon.st.TC = 320*240+2;

	for(;;)
	{
		for(i=0; i<(sizeof(img)/sizeof(img[0])); i++)
		{
			DMA_Start(ch, (void *)img[i], (void *)LCD_FB1, srcc, dstc, dcon);

			while(!DMA_complete[ch])
			{
				Uart_Printf(".");
				Timer0_Delay(20);
			}

			Uart_Printf("\n[%d] DMA_complete!\n", i, ch);
			DMA_complete[ch] = 0;
			Lcd_Draw_BMP(0,0,(void *)LCD_FB1);
		}
	}
}

#endif

// DMA에 의한 IMAGE 전송 = HW Trigger (Timer0), Whole Service

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

#include ".\Image\img01.h"
#include ".\Image\img02.h"
#include ".\Image\img03.h"
#include ".\Image\img04.h"

extern void Lcd_Graphic_Init(void);
extern void Lcd_Clr_Screen(int color);
extern void Lcd_Draw_BMP(int x, int y, const unsigned short int *fp);

extern volatile int DMA_complete[];

const unsigned short * img[] = {img01, img02, img03, img04};

void Main(void)
{
	int i = 0;
	int ch = 2;

	UNI_SRCC srcc;
	UNI_DSTC dstc;
	UNI_DCON dcon;

	Uart_Init(115200);
	Led_Init();

	Uart_Printf("LCD Display (DMA, HW Trigger - Timer0)\n");

	Lcd_Graphic_Init();
	Lcd_Clr_Screen(0xf800);

	srcc.udata = 0;
	srcc.st.INC = DMA_ADDR_INC;
	srcc.st.LOC = DMA_LOC_AHB;

	dstc.udata = 0;
	dstc.st.INC = DMA_ADDR_INC;
	dstc.st.LOC = DMA_LOC_AHB;
	dstc.st.CHK_INT = DMA_INT_TC;

	dcon.udata = 0;
	dcon.st.SYNC = DMA_SYNC_HCLK;
	dcon.st.INT = DMA_INT_EN;
	dcon.st.RELOAD = DMA_RELOAD_OFF;
	dcon.st.DMD_HS = DMA_DEMAND;

	dcon.st.SERVMODE = DMA_SVC_WHOLE;
	dcon.st.SWHW_SEL = DMA_TRIGGER_HW;
	dcon.st.HWSRCSEL = DMA_HWSRC_CH2_TIMER;

	dcon.st.TSZ = DMA_TSZ_1UNIT;
	dcon.st.DSZ = DMA_DSZ_2B;
	dcon.st.TC = 320*240+2;

	DMA_Timer_Select(1, 0);

	for(i=0; i<(sizeof(img)/sizeof(img[0])); i++)
	{
		DMA_Start(ch, (void *)img[i], (void *)LCD_FB1, srcc, dstc, dcon);
		Timer0_Repeat_ISR_Enable(1, 200);

		while(!DMA_complete[ch])
		{
			Uart_Printf(".");
		}

		Uart_Printf("\n[%d] DMA_complete!\n", i, ch);
		DMA_complete[ch] = 0;
		Lcd_Draw_BMP(0,0,(void *)LCD_FB1);
	}

	Timer0_Stop();
	DMA_Timer_Select(0, 0);
}

#endif

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

char * str[] = {"[1]The S3C2440A supports four-channel DMA controller located between the system bus and the peripheral bus.\n", \
 				"[2]Each channel of DMA controller can perform data movements between devices in the system bus and/or\n",\
 				"[3]peripheral bus with no restrictions. In other words, each channel can handle the following four cases:\n", \
				"[4]	1. Both source and destination are in the system bus\n", \
				"[5]	2. The source is in the system bus while the destination is in the peripheral bus\n", \
				"[6]	3. The source is in the peripheral bus while the destination is in the system bus\n", \
				"[7]	4. Both source and destination are in the peripheral bus\n", \
				"[8]The main advantage of the DMA is that it can transfer the data without CPU intervention. The operation of DMA can\n", \
				"[9]be initiated by software, or requests from internal peripherals or external request pins.\n", };

void Main(void)
{
	int i = 0;

	Uart_Init(115200);
	Led_Init();

	Uart_Printf("Uart Printf Test\n");

	for(i=0; i<(sizeof(str)/sizeof(str[0])); i++)
	{
		Uart_Printf(str[i]);

		Led_Display(0x5);
		Timer0_Delay(500);
		Led_Display(0xa);
		Timer0_Delay(500);
	}
}

#endif

#if 0

#include <string.h>

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

char * str[] = {"[1]The S3C2440A supports four-channel DMA controller located between the system bus and the peripheral bus.\n", \
 				"[2]Each channel of DMA controller can perform data movements between devices in the system bus and/or\n",\
 				"[3]peripheral bus with no restrictions. In other words, each channel can handle the following four cases:\n", \
				"[4]	1. Both source and destination are in the system bus\n", \
				"[5]	2. The source is in the system bus while the destination is in the peripheral bus\n", \
				"[6]	3. The source is in the peripheral bus while the destination is in the system bus\n", \
				"[7]	4. Both source and destination are in the peripheral bus\n", \
				"[8]The main advantage of the DMA is that it can transfer the data without CPU intervention. The operation of DMA can\n", \
				"[9]be initiated by software, or requests from internal peripherals or external request pins.\n", };

extern volatile int Key_push;
extern volatile int Key_value;
extern volatile int Rx_data_in;
extern volatile int Rx_data;
extern volatile int Timer0_time_out;
extern volatile int DMA_complete[];

void Main(void)
{
	int i = 0;
	int ch = 0;
	int led = 0xf;

	UNI_SRCC srcc;
	UNI_DSTC dstc;
	UNI_DCON dcon;

	Uart_Init(115200);
	Led_Init();
	Key_Push_ISR_Init();
	Key_ISR_Init();
	Key_Push_ISR_Enable(1);
	Key_ISR_Enable(1);
	Timer0_Delay_ISR_Enable(1, 500);

	Uart_Printf("Uart TX (DMA, HW Trigger - UART0-TX, Single Service)\n");

	srcc.udata = 0;
	srcc.st.INC = DMA_ADDR_INC;
	srcc.st.LOC = DMA_LOC_AHB;

	dstc.udata = 0;
	dstc.st.INC = DMA_ADDR_FIX;
	dstc.st.LOC = DMA_LOC_APB;
	dstc.st.CHK_INT = DMA_INT_TC;

	dcon.udata = 0;
	dcon.st.SYNC = DMA_SYNC_PCLK;
	dcon.st.INT = DMA_INT_EN;
	dcon.st.RELOAD = DMA_RELOAD_OFF;
	dcon.st.DMD_HS = DMA_DEMAND;

	dcon.st.SERVMODE = DMA_SVC_SINGLE;
	dcon.st.SWHW_SEL = DMA_TRIGGER_HW;
	dcon.st.HWSRCSEL = DMA_HWSRC_CH0_UART0;

	dcon.st.TSZ = DMA_TSZ_1UNIT;
	dcon.st.DSZ = DMA_DSZ_1B;

	DMA_Uart0_Ch0_Select(1, 1);
	Uart_ISR_Enable(1,0,0);
	i = 0;

	for(;;)
	{
		if((DMA_complete[ch] || (i == 0)) && (i < (sizeof(str)/sizeof(str[0]))))
		{
			DMA_complete[ch] = 0;
			dcon.st.TC = strlen(str[i]);
			DMA_Start(ch, (void *)str[i], (void *)&rUTXH0, srcc, dstc, dcon);
			i++;
		}

		if(Key_push)
		{
			Uart_Printf("Push Key!\n");
			Key_push = 0;
		}

		if(Key_value)
		{
			Uart_Printf("Key: %d\n", Key_value);
			Key_value = 0;
		}

		if(Rx_data_in)
		{
			Uart_Printf("Uart: %c\n", Rx_data);
			Rx_data_in = 0;
		}

		if(Timer0_time_out)
		{
			Timer0_time_out = 0;
			Led_Display(led = ~led);
			Timer0_Delay_ISR_Enable(1, 500);
			Uart_Printf(".");
		}
	}
}

#endif

