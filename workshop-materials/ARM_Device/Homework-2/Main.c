// Home work

#if 0

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



// sol
#if 01
#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"
#define CHATTERING_CNT 5000

enum Key{Key1,Key2,Key3,Key4};

void Timer3_Repeat(int time){
	// prescaler config
	Macro_Write_Block(rTCFG0, 0xff, (int)(PCLK/50000./4 + 0.5) - 1, 8);

	//mux configuration
	Macro_Write_Block(rTCFG1, 0xf, 0x1,12);

	// clear srcpnd register
	rSRCPND = 1<<13;

	// tcntb3 config
	rTCNTB3 = time * 50 - 1;

	// manual update on, timer stop
	Macro_Write_Block(rTCON, 0xf, 0x2, 16);

	// manual update off, timer on, auto reload on
	Macro_Write_Block(rTCON, 0xf, 0x9, 16);

	Uart_Printf("Timer3 Start!\n");
}

void Timer3_Stop(void){
	Macro_Clear_Bit(rTCON, 16);
	Uart_Printf("Timer3 Stop!\n");


//	Macro_Write_Block(rTCON, 0x1, 0x0, 0);
}

int Timer3_Check_Expired(void){
	if(Macro_Check_Bit_Set(rSRCPND, 13)){
		rSRCPND = 1<<13;
		return 1;
	}
	return 0;
}

void Main(void){
	int lock[4] = {0};
	int i;

	Uart_Init(115200);

	// led init
	Macro_Write_Block(rGPBCON, 0xff, 0x55, 14);
	Macro_Write_Block(rGPBDAT, 0xf, 0xf, 7);


	// key init
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 18);
	Macro_Clear_Bit(rGPCDAT, 9);
	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);

	for(;;){
		// key1, led1 toggle
		if(lock[Key1] == 0 && Macro_Check_Bit_Clear(rGPFDAT, 4)){
			for(i=0;i<CHATTERING_CNT;i++){
				if(Macro_Check_Bit_Set(rGPFDAT, 4)) break;
			}
			if(i == CHATTERING_CNT){
				lock[Key1] = 1;
				Macro_Invert_Bit(rGPBDAT, 7);
			}
		}else if(lock[Key1] == 1 && Macro_Check_Bit_Set(rGPFDAT, 4)){
			lock[Key1] = 0;
		}


		// key2, led1 toggle
		if(lock[Key2] == 0 && Macro_Check_Bit_Clear(rGPFDAT, 5)){
			lock[Key2] = 1;
			while(Macro_Check_Bit_Clear(rUTRSTAT0, 1));
			rUTXH0 = 'A';
		}else if(lock[Key2] == 1 && Macro_Check_Bit_Set(rGPFDAT, 5)){
			lock[Key2] = 0;
		}


		if(Macro_Check_Bit_Set(rUTRSTAT0, 0)){
			char x = rURXH0;
			if(x == 'b'){
				Macro_Invert_Bit(rGPBDAT, 8);
			}
		}


		// key3, led 3 toggle 1sec
		if(lock[Key3] == 0 && Macro_Check_Bit_Clear(rGPFDAT, 6)){
			lock[Key3] = 1;
			Timer3_Repeat(100);
		}else if(lock[Key3] == 1 && Macro_Check_Bit_Set(rGPFDAT, 6)){
			lock[Key3] = 0;
		}
		if(Timer3_Check_Expired()){
			Macro_Invert_Bit(rGPBDAT, 9);
		}


		// Key 4
		if(lock[Key4] == 0 && Macro_Check_Bit_Clear(rGPFDAT, 7)){
			lock[Key4] = 1;
			Timer3_Stop();
		}else if(lock[Key4] == 1 && Macro_Check_Bit_Set(rGPFDAT, 7)){
			lock[Key4] = 0;
		}

	}

}


#endif
