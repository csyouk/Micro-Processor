#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

// ISR Functions declaration

void Invalid_ISR(void) __attribute__ ((interrupt ("IRQ")));
void EINT0_ISR(void) __attribute__ ((interrupt ("IRQ")));
void EINT4_7_ISR(void) __attribute__ ((interrupt ("IRQ")));
void UART0_ISR(void) __attribute__ ((interrupt ("IRQ")));
void TIMER0_ISR(void) __attribute__ ((interrupt ("IRQ")));

typedef void (*VFP)(void);

const VFP ISR_Vectors[ ] =
{
	EINT0_ISR, 					// ISR_EINT0
	Invalid_ISR, 				// ISR_EINT1
	Invalid_ISR, 				// ISR_EINT2
	Invalid_ISR, 				// ISR_EINT3
	EINT4_7_ISR, 				// ISR_EINT4_7
	Invalid_ISR, 				// ISR_EINT8_23
	Invalid_ISR, 				// ISR_CAM
	Invalid_ISR, 				// ISR_BAT_FLT
	Invalid_ISR, 				// ISR_TICK
	Invalid_ISR, 				// ISR_WDT_AC97
	TIMER0_ISR, 				// ISR_TIMER0
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
	UART0_ISR, 					// ISR_UART0
	Invalid_ISR, 				// ISR_SPI1
	Invalid_ISR, 				// ISR_RTC
	Invalid_ISR  				// ISR_ADC
};

volatile int Timer0_time_out = 0;

void TIMER0_ISR(void)
{
	rSRCPND = 1<<10;
	rINTPND = 1<<10;

	Timer0_time_out = 1;
}

volatile int Rx_data_in = 0;
volatile int Rx_data = 0;

void UART0_ISR(void)
{
	rSUBSRCPND = 1<<0;
	rSRCPND = 1<<28;
	rINTPND = 1<<28;

	Rx_data = rURXH0;
	Rx_data_in = 1;
}

volatile int Key_push = 0;

void EINT0_ISR(void)
{
	Key_push = 1;

	rSRCPND = 1<<0;
	rINTPND = 1<<0;
}

volatile int Key_value = 0;

void EINT4_7_ISR(void)
{
	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);
	Key_value = Key_Get_Pressed();
	Key_ISR_Init();

	rEINTPEND = 0xf << 4;
	rSRCPND = 1<<4;
	rINTPND = 1<<4;
}

void Invalid_ISR(void)
{
	Uart_Printf("Invalid IRQ Requested...\n");
	for(;;);
}

void FIQ_Handler(void)
{
	Uart_Printf("FIQ-Exception!\n");
	for(;;);
}

void Undef_Handler(void)
{
	Uart_Printf("UND-Exception!\n");
	for(;;);
}

void Pabort_Handler(void)
{
	Uart_Printf("PABT-Exception!\n");
	for(;;);
}

void Dabort_Handler(void)
{
	Uart_Printf("DABT-Exception!\n");
	for(;;);
}

void SWI_Handler(void)
{
	Uart_Printf("SWI-Exception!\n");
	for(;;);
}


