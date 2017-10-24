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
	// keyout 1 '0' ���
	// keyout 0 => input
	Macro_Write_Block(rGPCCON, 0xf, 0x4, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	// Key[3:0] 0���� ����
	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);
	while(i++ != DELAY);
}

void Joystick_Btns_Init(void){
	volatile int i=0;
	// Keyout 1 => input
	// keyout 0 '0' ���
	// keyout 0 => output
	Macro_Write_Block(rGPCCON, 0xf, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 9);

	// Key[3:0] 0���� ����
	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);
	while(i++ != DELAY);
}

void Key_Init(void){

	// keyoutput 1 => ���, 0
	// keyoutput 0 => �Է�
	Macro_Write_Block(rGPCCON, 0xf, 0x4, 16);
	Macro_Clear_Bit(rGPCDAT, 9);

	// key 3:0 0���� init
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

	// ���̽�ƽ Ű�� ȸ�θ� Ȱ��ȭ ��Ų��.
	// Keyout 1 => input
	// keyout 0 '0' ���
	// keyout 0 => output
	Macro_Write_Block(rGPCCON, 0xf, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 9);

	// Key[3:0] 0���� ����
	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);
	while(i++ != DELAY);

	// ���̽�ƽ ��ư �� Ȯ��.
	key = Macro_Extract_Area(rGPFDAT, 0xf, 4);
	if(key != 0xf){
		i=0;
		while(i++ != CHATTERING_CNT){
			if(key != Macro_Extract_Area(rGPFDAT, 0xf, 4)) return 0;
		}
		return table[key];
	}

	// ���͹�ư���� Ȯ��.
	if(Macro_Check_Bit_Clear(rGPFDAT,0)) return 9;


	// Ǫ�ù�ư �� �ʱ�ȭ
	// keyout 0 --> output, 0���
	// keyout 1 --> input mode
	//key 3:0 �� �̹� �ʱ�ȭ �����Ƿ� �� �ʿ� ����.
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

