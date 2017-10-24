// Timer0 Stop Watch

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	volatile int i, k;
	unsigned int time = 0x10000;
	unsigned int prev=0, j = 1;

	Uart_Init(115200);
	Led_Init();
	Key_Poll_Init();

	Uart_Printf("Timer Stop Watch Test\n");

	for(k = 0; k < 10; k++)
	{
		Timer0_Stop_Watch_Run();

		for(i=0; i<(time * j); i++);
		Uart_Printf("Loop Time(0x%X) = %u usec / diff = %u\n",
				(time * j), Timer0_Stop_Watch_Stop(), Timer0_Stop_Watch_Stop() - prev);
		prev = Timer0_Stop_Watch_Stop();
		j++;
	}
}

#endif

// Timer0 Stop Watch Unlimited

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	volatile int i, k;
	unsigned int time = 0x10000;
	unsigned int j = 1;

	Uart_Init(115200);
	Led_Init();
	Key_Poll_Init();

	Uart_Printf("Timer Stop Watch Unlimited Test\n");

	for(k = 0; k < 10; k++)
	{
		i = Timer0_Stop_Watch_Run_Unlimited(6000); // 5000(5 sec) works
		if(i == 0) {
			Uart_Printf(">>>>>>>>>> Max Time Configuration Error <<<<<<<<<<\n");
			break;
		}

		for(i=0; i<(time * j); i++);

		Uart_Printf("Loop Time(0x%X) = %u usec\n", (time * j), Timer0_Stop_Watch_Stop_Unlimited());
		j++;
	}
}

#endif

// Delay 함수 검증

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	int i=0;

	Led_Init();
	Uart_Init(115200);

	Uart_Printf("\nLoop Delay Function Test\n\n");

	#if 0

	for(;;)
	{
		Timer0_Delay(1000);
		Uart_Printf("\r[%2u]sec", i++);
		if(i==100) i=0;
	}

	#else

	for(;;)
	{
		Timer0_Delay_Unlimited(5000); // 5초 // 정상적으로 작동하지 않음.
		Uart_Printf("\r[%2u]sec", i++ * 5);
		if(i==100) i=0;
	}

	#endif
}

#endif

// Repeat 함수 검증

#if 0

#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

void Main(void)
{
	int i = 0;
	int disp = 0xf;

	Led_Init();
	Uart_Init(115200);

	Uart_Printf("\nAuto Reload Test\n\n");

	Timer0_Repeat(500);

	for(;;)
	{
		if(Timer0_Check_Timeout())
		{
			Led_Display(disp = ~disp);
			Uart_Printf("[%d]\n", i);
			if(i++ == 10) Timer0_Stop();
		}
	}
}

#endif

// Repeat 동작 중 주기의 변경

#if 0

#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

void Main(void)
{
	int i = 0;
	int j;
	int disp = 0xf;

	Led_Init();
	Uart_Init(115200);

	Uart_Printf("\nVariable Interval Test\n\n");

	Timer0_Repeat(10);

	for(j=1;;j=(j>=20)?(1):(j+1))
	{
		Timer0_Set_Time(5*j);
		Uart_Printf("Delay Time = %dmsec\n", j*5);
		i=0;

		for(;;)
		{
			if(Timer0_Check_Timeout())
			{
				Led_Display(disp = ~disp);
				Uart_Printf("[%d]\n", i);
				if(i++ == 10) break;
			}
		}
	}
}

#endif

// Buzzer Beep 함수 검증

#if 0

#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"

void Main(void)
{
	int i = 0;

	enum key{C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2};
	enum note{N16, N8, N4, N2, N1};

	const int song[][2] = {{G1,N4},{G1,N4},{E1,N8},{F1,N8},{G1,N4},{A1,N4},{A1,N4},{G1,N2},{G1,N4},{C2,N4},{E2,N4},{D2,N8},{C2,N8},{D2,N2},{0xff, 0xff}};

	Led_Init();
	Uart_Init(115200);

	Uart_Printf("Timer3 & Buzzer Test\n");

	Timer3_Buzzer_Init();

	Timer3_Buzzer_Beep(C1,N4);
	Timer3_Buzzer_Beep(D1,N4);
	Timer3_Buzzer_Beep(E1,N4);
	Timer3_Buzzer_Beep(F1,N4);
	Timer3_Buzzer_Beep(G1,N4);
	Timer3_Buzzer_Beep(A1,N4);
	Timer3_Buzzer_Beep(B1,N4);
	Timer3_Buzzer_Beep(C2,N4);

	Timer0_Delay(1000);

	while(song[i][0] != 0xff)
	{
		Timer3_Buzzer_Beep(song[i][0], song[i][1]);
		Uart_Printf(".");
		i++;
	}
}

#endif



// Home work

#if 01

#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"
#define HW_CHATTERINT_CNT 100
enum Key{K1,K2,K3,K4};
enum Led{L1=7, L2=8, L3=9, L4=10};
int table[16] = {
		1,2,1,3,
		1,2,1,4,
		1,2,1,3,
		1,2,1,0
};
void HW_Key_Init(void){
	// Key [3:0] input mode로 설정
	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);

	// keyout 1 은 output mode로 설정
	// keyout 0 은 input mode로 설정
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 18);
	Macro_Clear_Bit(rGPCDAT, 9);

	// key 입력 값을 0으로 모두 초기화 시킨다.
//	Macro_Clear_Area(rGPCDAT, 0xf, 7);

}

void HW_Led_Init(void){
	// led gpb 7 ~ 10 설정
	// output mode로 설정.
	Macro_Write_Block(rGPBCON, 0xff, 0x55, 14);

	// 초기 data를 모두 1로 만들어서 led를 꺼놓는다.
	Macro_Write_Block(rGPBDAT, 0xf, 0xf, 7);
}

void HW_Uart_Init(int baud_rate){
	Macro_Write_Block(rGPHCON, 0xf, 0xa, 4);
	rUFCON0 = 0x0;
	rUMCON0 = 0x0;
	rULCON0 = 0x3;
	rUCON0 = 0x245;
	rUBRDIV0 = ((unsigned int)(PCLK/16./baud_rate + 0.5) - 1);
}

int HW_Key_Get_Pressed(){
	int key, i;
	key = Macro_Extract_Area(rGPFDAT, 0xf, 4);
	if(key == 0xf) return 0;

	for(i=0; i< HW_CHATTERINT_CNT; i++){
		if(key != Macro_Extract_Area(rGPFDAT, 0xf, 4)){
			return 0;
		}
	}
	return table[key];
//	return table[Macro_Extract_Area(rGPFDAT, 0xf, 4)];
}

void Send_A_To_Teraterm(void){
	// 송신버퍼에 데이터가 있으면, 리턴한다.
	if(Macro_Check_Bit_Clear(rUTRSTAT0, 1)) return;
	rUTXH0 = 'A';
}

void Toggle_Led2_If_Rx_Has_B(){
	unsigned char container;
	if(Macro_Check_Bit_Clear(rUTRSTAT0, 0)) return;
	container = rURXH0;
//	Uart_Printf("-%c-", container);
	if(container != 'b') return;
	Macro_Invert_Bit(rGPBDAT, L2);
}

void Main(void)
{
	int lock[4] = {0};
	int key;
	Uart_Printf("Home work start!!\n");
	// HW 1
	HW_Key_Init();
	HW_Led_Init();
	HW_Uart_Init(115200);



	while(1){

		// 1번 과제.
		if(lock[K1] == 0 && Macro_Check_Bit_Clear(rGPFDAT, 4+K1)){
			Macro_Invert_Bit(rGPBDAT, 7+K1);
			lock[K1] = 1;
			Uart_Printf("lock enabled\n");
		}
		else if(lock[K1] == 1 && Macro_Check_Bit_Set(rGPFDAT, 4+K1)){
			lock[K1] = 0;
			Uart_Printf("lock disabled\n");
		}

		// 2번 과제
		if(lock[K2] == 0 && Macro_Check_Bit_Clear(rGPFDAT, 4+K2)){
			Send_A_To_Teraterm();
			lock[K2] = 1;
		}
		else if(lock[K2] == 1 && Macro_Check_Bit_Set(rGPFDAT, 4+K2)){
			lock[K2] = 0;
		}

		// 3번과제
		Toggle_Led2_If_Rx_Has_B();


		// 4번과제
		if(lock[K3] == 0 && Macro_Check_Bit_Clear(rGPFDAT, 4 + K3)){
			lock[K3] = 1;
			Timer0_Repeat(1000);
		}else if(lock[K3] == 1 && Macro_Check_Bit_Set(rGPFDAT, 4 + K3)){
			lock[K3] = 0;
		}


		if(Timer0_Check_Timeout()){
			Macro_Invert_Bit(rGPBDAT, L3);
			rSRCPND = 1<<10;
			Uart_Printf("1sec\n");
		}


		// 5번과제
		key = Key_Get_Pressed();
		if(lock[K4] == 0 && Macro_Check_Bit_Clear(rGPFDAT, 4 + K4)){
			lock[K4] = 1;
			Timer0_Stop();
		} else if(lock[K4] == 1 && Macro_Check_Bit_Set(rGPFDAT, 4 + K4)){
			lock[K4] = 0;
		}

	}
}

#endif

