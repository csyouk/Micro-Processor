#include "2440addr.h"
#include "macro.h"
#define DELAY 0x1000
#define CHATTERING_CNT 1000

int table[16] =
{
		1,2,1,3,
		1,2,1,4,
		1,2,1,3,
		1,2,1,0
};


void Push_Btns_Init(void){
	volatile int i=0;
	// Keyout 1 => output
	// keyout 1 '0' 출력
	// keyout 0 => input
	Macro_Write_Block(rGPCCON, 0xf, 0x4, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	// Key[3:0] 0으로 설정
	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);
	while(i++ != DELAY);
}

void Joystick_Btns_Init(void){
	volatile int i=0;
	// Keyout 1 => input
	// keyout 0 '0' 출력
	// keyout 0 => output
	Macro_Write_Block(rGPCCON, 0xf, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 9);

	// Key[3:0] 0으로 설정
	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);
	while(i++ != DELAY);
}

void Key_Init(void){

	// keyoutput 1 => 출력, 0
	// keyoutput 0 => 입력
	Macro_Write_Block(rGPCCON, 0xf, 0x4, 16);
	Macro_Clear_Bit(rGPCDAT, 9);

	// key 3:0 0으로 init
	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);
}

int Joystick_Key_Pressed(){
	int key, i=0;
	key = Macro_Extract_Area(rGPFDAT, 0xf, 4);
	if(key != 0xf) {
		while(i++ != CHATTERING_CNT){
			if(key != Macro_Extract_Area(rGPFDAT, 0xf, 4)) return 0;
		}
		return table[key];
	}
	return 0;
}

int Push_Key_Pressed(){
	int key, i=0;
	key = Macro_Extract_Area(rGPFDAT, 0xf, 4);
	if(key != 0xf) {
		while(i++ != CHATTERING_CNT){
			if(key != Macro_Extract_Area(rGPFDAT, 0xf, 4)) return 0;
		}
		return table[key]+4;
	}
	return 0;
}

int Key_Get_Pressed(){
	int key;
	volatile int i=0;

	// 조이스틱 키쪽 회로를 활성화 시킨다.
	// Keyout 1 => input
	// keyout 0 '0' 출력
	// keyout 0 => output
	Macro_Write_Block(rGPCCON, 0xf, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 9);

	// Key[3:0] 0으로 설정
	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);
	while(i++ != DELAY);

	// 조이스틱 버튼 쪽 확인.
	key = Macro_Extract_Area(rGPFDAT, 0xf, 4);
	if(key != 0xf){
		i=0;
		while(i++ != CHATTERING_CNT){
			if(key != Macro_Extract_Area(rGPFDAT, 0xf, 4)) return 0;
		}
		return table[key];
	}

	// 엔터버튼인지 확인.
	if(Macro_Check_Bit_Clear(rGPFDAT,0)) return 9;


	// 푸시버튼 쪽 초기화
	// keyout 0 --> output, 0출력
	// keyout 1 --> input mode
	//key 3:0 은 이미 초기화 했으므로 할 필요 없음.
	Macro_Write_Block(rGPCCON, 0xf, 0x4, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	key = Macro_Extract_Area(rGPFDAT, 0xf, 4);
	if(key != 0xf){
		i=0;
		while(i++ != CHATTERING_CNT){
			if(key != Macro_Extract_Area(rGPFDAT, 0xf, 4)) return 0;
		}
		return table[key] + 4;
	}

	return 0;
}

