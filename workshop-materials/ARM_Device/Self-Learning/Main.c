// 조이스틱 버튼 활성화 해보기
#if 0
#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

void Main(void)
{
	int key;
	Uart_Printf("================================\n");
	Joystick_Btns_Init();

	while(1){
		key = Joystick_Key_Pressed();
		if(!key) continue;
		Uart_Printf("%d", key);
	}
}
#endif


// 푸쉬 버튼 활성화 해보기
#if 0
#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

void Main(void)
{
	int key = 0;
	Uart_Printf("================================\n");
	Push_Btns_Init();

	while(1){
		key = Push_Key_Pressed();
		if(!key) continue;
		Uart_Printf("%d", key);
	}
}
#endif


// 키스캔 알고리즘 해보기
#if 0
#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

void Main(void)
{
	int key = 0;
	Uart_Printf("================================\n");
	Key_Init();

	while(1){
		key = Key_Get_Pressed();
		if(!key) continue;
		Uart_Printf("%d", key);
	}
}
#endif


// LED togging
#if 0
#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"
#define DELAY 0x100000
void Main(void)
{
	volatile int i = 0;
	Uart_Printf("================================\n");
	Led_Init();

	while(1){
		i=0;
		Macro_Invert_Area(rGPBDAT, 0xf, 7);
		while(i++ != DELAY);
	}
}
#endif


// Transmit data using Uart.
#if 0
#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"
#define DELAY 1<<25

char * hello =  "HELLO WORLD! ";

void Main(void)
{
	int i = 0;
	volatile int j = 0;
	Uart_Init(115200);
	Uart_Printf("================================\n");


	while(1){
		if(Macro_Check_Bit_Clear(rUTRSTAT0, 1)) continue;
		if(!hello[i]) i=0;
		rUTXH0 = hello[i++];
		j=0;
		while(j++ != DELAY);
	}
}
#endif


// Receive data using Uart.
#if 0
#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"
#define DELAY 1<<25
#define MAX 30
void Main(void)
{
	int wp = 0, rp = 0;
	volatile int i = 0;
	char buffer[MAX] = {0};
	Uart_Init(115200);
	Uart_Printf("================================\n");


	while(1){
		if(Macro_Check_Bit_Set(rUTRSTAT0, 0)) {
			buffer[wp++] = rURXH0;
			if(wp != MAX-2) continue;
			buffer[wp++] = '\r';
			buffer[wp++] = '\n';
			while(rp < wp){
				Uart_Send_Byte(buffer[rp++]);
			}
			rp = wp = 0;
		}

	}
}
#endif




// Timer 3 setting as stop watch mode
#if 0
#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

#define MAX_TIME 0xffff
#define TIMER3_TICK 20

void Timer3_Stop_Watch_Run(void){
	Macro_Write_Block(rTCFG0, 0xff, (int)(HCLK/50000./16 + 0.5) - 1, 8);
	Macro_Write_Block(rTCFG1, 0xf, 0x3, 0);
	rTCNTB3 = MAX_TIME;
	Macro_Write_Block(rTCON, 0xf, 0x2, 16);
	Macro_Write_Block(rTCON, 0xf, 0x1, 16);
}

unsigned int Timer3_Stop_Watch_Stop(void){
	Macro_Clear_Bit(rTCON,16);
	return (MAX_TIME - rTCNTO3) * TIMER3_TICK;
}

void Main(void)
{
	volatile int i, j;
	Uart_Init(115200);
	Uart_Printf("================================\n");



	for(i=1 ; i<=10; i++){
		Timer3_Stop_Watch_Run();
		for(j=0; j<100000*i; j++);
		Uart_Printf("Time lapse : %d\n", Timer3_Stop_Watch_Stop());
	}

}
#endif



// Timer 3 setting as auto reload
#if 0
#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

#define MAX_TIME 0xffff
#define TIMER3_TICK 20

void Timer3_Repeat(int time){

	Macro_Write_Block(rTCFG0, 0xff, (int)(HCLK/50000./16 + 0.5) - 1, 8);
	Macro_Write_Block(rTCFG1, 0xf, 0x3, 12);
	rTCNTB3 = time * (1000/TIMER3_TICK);
	rSRCPND = (1<<13);
	Macro_Write_Block(rTCON, 0xf, 0x2, 16);
	Macro_Write_Block(rTCON, 0xf, 0x9, 16);
}

int Timer3_Check_Timeout(void){
	if(Macro_Check_Bit_Clear(rSRCPND, 13)) return 0;
	rSRCPND = 1<<13;
	return 1;
}

void Main(void)
{
	Uart_Printf("================================\n");

	Timer3_Repeat(1000);
	Timer0_Repeat(1000);

	while(1){
		if(Timer3_Check_Timeout()){
			Uart_Printf("Timer3\n");
		}
		if(Timer0_Check_Timeout()){
			Uart_Printf("Timer0\n");
		}

	}


}
#endif




// Timer 4 setting as auto reload
#if 0
#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

#define MAX_TIME 0xffff
#define TIMER4_TICK 25

void HW_Led_Init(void){
	Macro_Write_Block(rGPBCON, 0xff, 0x55, 14);
	Macro_Write_Block(rGPBDAT, 0xf, 0xf, 7);
}

void Timer4_Repeat(int time){
	Macro_Write_Block(rTCFG0, 0xff, (int)(PCLK/40000./8 + 0.5) - 1, 8);
	Macro_Write_Block(rTCFG1, 0xf, 0x2,16);

	rTCNTB4 = time * TIMER4_TICK - 1;

	Macro_Write_Block(rTCON, 0x7, 0x2, 20);
	Macro_Write_Block(rTCON, 0x7, 0x5, 20);
}

int Timer4_Check_Timeout(void){
	if(Macro_Check_Bit_Clear(rSRCPND, 14)) return 0;
	rSRCPND = 1<<14;
	return 1;
}

void Main(void)
{
	int i,j,k;
	i=j=k=0;
	Uart_Init(115200);
	Uart_Printf("================================\n");
	HW_Led_Init();
	Timer4_Repeat(1000);

	while(1){
		if(Timer4_Check_Timeout()){
			Macro_Invert_Bit(rGPBDAT, 7);
			i++; j++; k++;
			if(i%2 == 0) {
				Macro_Invert_Bit(rGPBDAT, 8);
				i=0;
			}
			if(j%3 == 0) {
				Macro_Invert_Bit(rGPBDAT, 9);
				j=0;
			}
			if(k%4 == 0) {
				Macro_Invert_Bit(rGPBDAT, 10);
				k=0;
			}
		}
	}


}
#endif




// Timer 4 setting as auto reload
#if 0
#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

#define MAX_TIME 0xffff
#define TIMER4_TICK 25

void HW_Led_Init(void){
	Macro_Write_Block(rGPBCON, 0xff, 0x55, 14);
	Macro_Write_Block(rGPBDAT, 0xf, 0xf, 7);
}

void Main(void)
{
	unsigned char x;
	int i = 0;
	Uart_Init(115200);
	Uart_Printf("================================\n");
	HW_Led_Init();


	while(1){
		if(Macro_Check_Bit_Clear(rUTRSTAT0, 0)) continue;
		x = rURXH0;
		if(x == 'a') i++;
		if(x == 'b') i += 5;
		if(x == 'c') i += 15;
		if(i %  1 == 0) Macro_Invert_Bit(rGPBDAT, 7);
		if(i % 15 == 0) Macro_Invert_Bit(rGPBDAT, 8);
		if(i % 30 == 0) Macro_Invert_Bit(rGPBDAT, 9);
		if(i % 45 == 0) (Macro_Invert_Bit(rGPBDAT, 10), i=0);
		if(i > 45) i=0;
	}


}
#endif

