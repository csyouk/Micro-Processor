typedef unsigned int 		U32;
typedef signed int			S32;
typedef unsigned short		U16;
typedef short int			S16;
typedef unsigned char		U8;
typedef signed char 		S8;
typedef unsigned long long 	ULL;

typedef	struct _ST_SRCC
{
	unsigned int INC: 1;
	unsigned int LOC: 1;
}ST_SRCC;

typedef	struct _ST_DSTC
{
	unsigned int INC: 1;
	unsigned int LOC: 1;
	unsigned int CHK_INT: 1;
}ST_DSTC;

typedef	struct _ST_DCON
{
	unsigned int TC: 20;
	unsigned int DSZ: 2;
	unsigned int RELOAD: 1;
	unsigned int SWHW_SEL: 1;
	unsigned int HWSRCSEL: 3;
	unsigned int SERVMODE: 1;
	unsigned int TSZ: 1;
	unsigned int INT: 1;
	unsigned int SYNC: 1;
	unsigned int DMD_HS: 1;
}ST_DCON;

typedef union _SRCC
{
	unsigned int udata;
	ST_SRCC	st;
}UNI_SRCC;

typedef union _DSTC
{
	unsigned int udata;
	ST_DSTC	st;
}UNI_DSTC;

typedef union _DCON
{
	unsigned int udata;
	ST_DCON	st;
}UNI_DCON;

typedef struct
{
	int day;
	int year;
	int mon;
	int date;
	int hour;
	int min;
	int sec;
}RTC_TIME;

// Heap.c
extern void * Get_Heap_Limit(void);
extern void * Get_Heap_Base(void);
extern unsigned int Get_Heap_Size(void);

// Led.c
extern void Led_Init(void);
extern void Led_Display(int disp);

// Uart.c
extern void Uart_Init(int baud);
extern void Uart_Printf(char *fmt,...);
extern char Uart_Get_Char(void);
extern char Uart_Get_Pressed(void);
extern int Uart_GetIntNum(void);
extern void Uart_GetString(char *string);
extern void Uart_ISR_Enable(int rx, int tx, int err);

// Key.c
extern void Key_Poll_Init(void);
extern int Key_Get_Pressed(void);
extern int Key_Get_Pressed_Multi(void);
extern void Key_Wait_Key_Released(void);
extern int Key_Wait_Key_Pressed(void);
extern void Key_Wait_Key_Released_Multi(void);
extern int Key_Wait_Key_Pressed_Multi(void);
extern void Key_Push_ISR_Init(void);
extern void Key_Push_ISR_Enable(int en);
extern void Key_ISR_Init(void);
extern void Key_ISR_Enable(int en);

// Timer.c
extern void Timer0_Stop_Watch_Run(void);
extern unsigned int Timer0_Stop_Watch_Stop(void);
extern int Timer0_Stop_Watch_Run_Unlimited(unsigned int max);
extern unsigned int Timer0_Stop_Watch_Stop_Unlimited(void);
extern void Timer0_Delay(int time);
extern void Timer0_Repeat(int time);
extern int Timer0_Check_Timeout(void);
extern void Timer0_Stop(void);
extern void Timer0_Set_Time(int time);
extern void Timer0_Delay_ISR_Enable(int en, int time);
extern void Timer0_Repeat_ISR_Enable(int en, int time);

// Buzzer.c
extern void Timer3_Buzzer_Beep(int tone, int duration);
extern void Timer3_Buzzer_Init(void);

// DMA.c
extern void DMA_Start(int ch, void * sa, void * da, UNI_SRCC srcc, UNI_DSTC dstc, UNI_DCON dcon);
extern void DMA_Stop(int ch);
extern void DMA_Timer_Select(int dma, int ch);
extern void DMA_Uart0_Ch0_Select(int dma, int rx_tx);
extern void DMA_Uart1_Ch1_Select(int dma, int rx_tx);
extern void DMA_Uart2_Ch3_Select(int dma, int rx_tx);

#define DMA_LOC_AHB			0
#define DMA_LOC_APB			1
#define DMA_ADDR_INC		0
#define DMA_ADDR_FIX		1
#define DMA_INT_TC			0
#define DMA_INT_RELOAD		1
#define DMA_DEMAND			0
#define DMA_HANDSHAKE		1
#define DMA_SYNC_PCLK		0
#define DMA_SYNC_HCLK		1
#define DMA_INT_DIS			0
#define DMA_INT_EN			1
#define DMA_TSZ_1UNIT		0
#define DMA_TSZ_4UNIT		1
#define DMA_SVC_SINGLE		0
#define DMA_SVC_WHOLE		1
#define DMA_TRIGGER_SW		0
#define DMA_TRIGGER_HW		1
#define DMA_RELOAD_ON		0
#define DMA_RELOAD_OFF		1
#define DMA_DSZ_1B			0
#define DMA_DSZ_2B			1
#define DMA_DSZ_4B			2
#define DMA_HWSRC_CH0_XDREQ0	0
#define DMA_HWSRC_CH1_XDREQ1	0
#define DMA_HWSRC_CH2_I2SSDO	0
#define DMA_HWSRC_CH3_UART2		0
#define DMA_HWSRC_CH0_UART0		1
#define DMA_HWSRC_CH1_UART1		1
#define DMA_HWSRC_CH2_I2SSDI	1
#define DMA_HWSRC_CH3_SDI		1
#define DMA_HWSRC_CH0_SDI		2
#define DMA_HWSRC_CH1_I2SSDI	2
#define DMA_HWSRC_CH2_SDI		2
#define DMA_HWSRC_CH3_SPI1		2
#define DMA_HWSRC_CH0_TIMER		3
#define DMA_HWSRC_CH1_SPI0		3
#define DMA_HWSRC_CH2_TIMER		3
#define DMA_HWSRC_CH3_TIMER		3
#define DMA_HWSRC_CH0_EP1		4
#define DMA_HWSRC_CH1_EP2		4
#define DMA_HWSRC_CH2_EP3		4
#define DMA_HWSRC_CH3_EP4		4
#define DMA_HWSRC_CH0_I2SSDO	5
#define DMA_HWSRC_CH1_PCMOUT	5
#define DMA_HWSRC_CH2_PCMIN		5
#define DMA_HWSRC_CH3_MICIN		5
#define DMA_HWSRC_CH0_PCMIN		6
#define DMA_HWSRC_CH1_SDI		6
#define DMA_HWSRC_CH2_MICIN		6
#define DMA_HWSRC_CH3_PCMOUT	6

// RTC.c
extern void RTC_Set_Time(RTC_TIME *time);
extern void RTC_Get_Time(RTC_TIME *time);
extern void RTC_Set_Alarm(RTC_TIME *time);
extern void RTC_Get_Alarm(RTC_TIME *time);
extern void RTC_Set_Alarm_Enable(int en, RTC_TIME *time);
extern void RTC_Tick_ISR_Enable(int en, int msec);

// WDT.c
extern void WDT_ISR_Enable(int enable, int msec);
extern void WDT_Start(int start, int msec);
extern void WDT_Clear(unsigned int msec);
extern void WDT_Reset_Enable(int enable);

