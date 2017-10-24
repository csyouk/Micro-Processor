#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

// ISR Functions declaration

typedef void (*VFP)(void);

void Invalid_ISR(void);
void Key_EINT4_7_ISR(void);
void ISR_UART0(void);

const VFP ISR_Vectors[ ] =
{
	Invalid_ISR, 				// ISR_EINT0
	Invalid_ISR, 				// ISR_EINT1
	Invalid_ISR, 				// ISR_EINT2
	Invalid_ISR, 				// ISR_EINT3
	Key_EINT4_7_ISR, 			// ISR_EINT4_7
	Invalid_ISR, 				// ISR_EINT8_23
	Invalid_ISR, 				// ISR_CAM
	Invalid_ISR, 				// ISR_BAT_FLT
	Invalid_ISR, 				// ISR_TICK
	Invalid_ISR, 				// ISR_WDT_AC97
	Invalid_ISR, 				// ISR_TIMER0
	Invalid_ISR, 				// ISR_TIMER1
	Invalid_ISR, 				// ISR_TIMER2
	Invalid_ISR, 				// ISR_TIMER3
	Invalid_ISR, 				// ISR_TIMER4
	Invalid_ISR, 				// ISR_UART2
	Invalid_ISR, 				// ISR_LCD
	Invalid_ISR, 				// ISR_DMA0
	Invalid_ISR, 				// ISR_DMA1
	Invalid_ISR, 				// ISR_DMA2
	Invalid_ISR, 				// ISR_DMA3
	Invalid_ISR, 				// ISR_SDI
	Invalid_ISR, 				// ISR_SPI0
	Invalid_ISR, 				// ISR_UART1
	Invalid_ISR, 				// ISR_NFCON
	Invalid_ISR, 				// ISR_USBD
	Invalid_ISR, 				// ISR_USBH
	Invalid_ISR, 				// ISR_IIC
	ISR_UART0, 					// ISR_UART0
	Invalid_ISR, 				// ISR_SPI1
	Invalid_ISR, 				// ISR_RTC
	Invalid_ISR  				// ISR_ADC
};

void IRQ_ISR(void)
{
	ISR_Vectors[rINTOFFSET]();
}

void Invalid_ISR(void)
{
	Uart_Printf("Invalid IRQ Requested...\n");
	for(;;);
}

volatile int Key_Value = 0;

void Key_EINT4_7_ISR(void)
{
	Key_Poll_Init();
	Key_Value = Key_Get_Pressed();

	rEINTPEND = 0xf<<4;
	rSRCPND = BIT_EINT4_7;
	rINTPND = BIT_EINT4_7;

	Key_ISR_Init();
}

volatile int Uart0_Rx_In = 0;
volatile int Uart0_Rx_Data = 0;

void ISR_UART0(void)
{
	rSUBSRCPND = BIT_SUB_RXD0;
	rSRCPND = BIT_UART0;
	rINTPND = BIT_UART0;

	Uart0_Rx_Data = rURXH0;
	Uart0_Rx_In = 1;
}

//////////// exception Handlers /////////////

void Undef_Handler(int addr, int mode)
{
	Uart_Printf("UND-Exception @[0x%x], Mode=[0x%X]\n", addr, mode);
	for(;;);
}

void Pabort_Handler(int addr, int mode)
{
	Uart_Printf("PABT-Exception @[0x%x], Mode=[0x%X]\n", addr, mode);
	for(;;);
}

void Dabort_Handler(int addr, int mode)
{
	Uart_Printf("DABT-Exception @[0x%x], Mode=[0x%X]\n", addr, mode);
	for(;;);
}

// SWI 개별 서비스 루틴들

void _Print_Hello(void)
{
	Uart_Printf("SW0 Service...\n");
	Uart_Printf("Hello\n");
}

int _Sqr(int a)
{
	Uart_Printf("SW1 Service...\n");
	return a * a;
}

double _Double_Add(double a, double b)
{
	Uart_Printf("SW2 Service...\n");
	return a + b;
}

void * SWI_Handler[] = {(void *)_Print_Hello, (void *)_Sqr, (void *)_Double_Add};




