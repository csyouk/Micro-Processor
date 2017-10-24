#include "device_driver.h"

extern void Print_Hello(void);
extern int Sqr(int a);
extern double Double_Add(double a, double b);

void Main(void)
{
	Run_Time_Config();
	MMU_Init();
	Uart_Init(115200);
	Led_Init();

	Uart_Printf("\nWilltek WT2440L - System Call Test\n");

	{
		Print_Hello();
		Uart_Printf("%d\n", Sqr(5));
		Uart_Printf("%f\n", Double_Add(3.14, 1.2));
		Uart_Printf("Returned from SWI\n");
	}
}
