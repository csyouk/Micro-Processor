// ADC & CDS Sensor Test

#if 1

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	Uart_Init(115200);
	Uart_Printf("ADC & CDS Sensor Test\n");

	for(;;)
	{
		Uart_Printf("ADC=%d\n", ADC_Start(1));
		Timer0_Delay(300);
	}
}

#endif

// ADC Interrupt Test

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

extern volatile int Adc_complete;
extern volatile unsigned int Adc_value;

void Main(void)
{
	int i = 0;

	Uart_Init(115200);
	Uart_Printf("ADC Interrupt Test\n");

	ADC_ISR_Init(1, 1);

	for(;;)
	{
		if(Adc_complete)
		{
			Adc_complete = 0;
			Uart_Printf("[%d]=%d\n", i, Adc_value);
			Timer0_Delay(100);

			if(i++ == 100)ADC_ISR_Init(0, 0);
		}
	}
}

#endif

