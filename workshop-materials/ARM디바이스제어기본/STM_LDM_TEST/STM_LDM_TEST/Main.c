#include "device_driver.h"
#include "2440addr.h"

void Sys_Init(char * msg)
{
	Run_Time_Config();
	Uart_Init(115200);
	MMU_Init();
	Led_Init();
	Uart_Printf(msg);
}

extern int* Push_Stack_Int_3(int* p,int d1,int d2,int d3);
extern int* Pop_Stack_Int_3(int *p,int *d1,int *d2,int *d3);

int Main(void)
{
	int i;
	int* sp;
	int stack[10] = {0};
	int data[3];

	Sys_Init("");
	Uart_Printf("===========================\n");
	Uart_Printf("ATPCS using STMDB\n");
	Uart_Printf("push data using single instruction");
	Uart_Printf("Prev Stack Pointer : 0x%08X\n",&stack[5]);
	sp = Push_Stack_Int_3(&stack[5],1,2,3);
	Uart_Printf("Stack Pointer after Use Stack: 0x%08X\n",sp);

	for(i=0;i<10;i++)
	{
		Uart_Printf("[%d] data :%3d [addr] 0x%08X\n",i,stack[i],&stack[i]);
	}



	Uart_Printf("===========================\n");
	Uart_Printf("ATPCS using STMFD\n");
	Uart_Printf("pop data using single instruction");

	Pop_Stack_Int_3(sp,&data[0],&data[1],&data[2]);
	Uart_Printf("\n[Pop Stack]\n",sp);
	Uart_Printf("Stack Pointer after Pop: 0x%08X\n",sp);
	for(i=0;i<3;i++)
	{
		Uart_Printf("[%d] data :%3d\n",i,data[i]);
	}
	return 0;
}

