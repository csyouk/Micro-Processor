#include "device_driver.h"

extern void DABT_Gen(void);
extern void PABT_Gen(void);
extern void UND_Gen(void);

void Main(void)
{
	Run_Time_Config();
	MMU_Init();
	Uart_Init(115200);
	Led_Init();

	Uart_Printf("\nWilltek WT2440L - Improved Exception Test\n");

	// ������ ���ʷ� �����Ű�鼭 Exception �߻��� Ȯ��

//	UND_Gen();
	//PABT_Gen();
	DABT_Gen();
	//SWI_Gen();
}
