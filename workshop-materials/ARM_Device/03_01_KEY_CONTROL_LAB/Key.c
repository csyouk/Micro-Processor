#include "2440addr.h"
#include "macro.h"

void Key_Poll_Init(void) {
	// KEY_IN 0~3(rGPF 4~7) -> INPUT
	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);

	// KEY_PUSH(GPF0) -> INPUT
	Macro_Write_Block(rGPFCON, 0x3, 0x0, 0);

	// KEY_OUT 0, 1 -> Input
	Macro_Write_Block(rGPCCON, 0xf, 0x0, 16);
}

#define DELAY_VAL (0x100000)
#define CHATTERINT_CNT (1000)
int lookup_table[16] =
{
		1, 2, 1, 3,
		1, 2, 1, 4,
		1, 2, 1, 3,
		1, 2, 1, 0
};

int Key_Get_Pressed(void) {
	volatile i;
	int key;

	// 조이스틱 확인. --> 1
	// keyout0 -> output,
	// keyout1 -> input
	// keyout0, 1 use GPCDAT 8, 9
	Macro_Write_Block(rGPCCON, 0xf, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	for (i = 0; i < DELAY_VAL; i++);



	// push key --> 2
	if (Macro_Check_Bit_Clear(rGPFDAT, 0)) return 9;

	key = Macro_Extract_Area(rGPFDAT, 0xf, 4);
	// look up table을 만들어서
	// 비트패턴에 맞는 값을 얻어낸다.
	if (key != 0xf) {
		// 채터링 코드 추가.
		for (i = 0; i < CHATTERINT_CNT; i++) {
			if (key != Macro_Extract_Area(rGPFDAT, 0xf, 4))
				return 0;
		}
		return lookup_table[key];
	}


	// 토글 버튼쪽 확인. --> 3
	// keyout0 -> input
 	// keyout1 -> output
	Macro_Write_Block(rGPCCON, 0xf, 0x4, 16);
	Macro_Clear_Bit(rGPCDAT, 9);


	for (i = 0; i < DELAY_VAL; i++);


	key = Macro_Extract_Area(rGPFDAT, 0xf, 4);
	if (key != 0xf) {
		// 채터링 코드 추가.
		for (i = 0; i < CHATTERINT_CNT; i++) {
			if (key != Macro_Extract_Area(rGPFDAT, 0xf, 4))
				return 0;
		}
		return lookup_table[key] + 4; // 5 ~ 8까지 리턴해야 함. 와이?
	}

	return 0;
}

void Key_Wait_Key_Released(void) {
	while (Key_Get_Pressed());
}

int Key_Wait_Key_Pressed(void) {
	int key;
	while (!(key = Key_Get_Pressed()));
	return key;
}

int Key_Get_Pressed_Multi(void) {
	volatile i;
	int key;
	int ret = 0;

	// 조이스틱 확인.
	// keyout0 -> output, keyout1 -> input
	Macro_Write_Block(rGPCCON, 0xf, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	for (i = 0; i < DELAY_VAL; i++);

	// joystick key
	key = Macro_Extract_Area(rGPFDAT, 0xf, 4);
	ret |= key;

	for (i = 0; i < DELAY_VAL; i++);

	// push key
	ret |= (Macro_Check_Bit_Set(rGPFDAT, 0) << 8);


	for (i = 0; i < DELAY_VAL; i++);

	//	// keyout0 -> input
	// 	// keyout1 -> output
	Macro_Write_Block(rGPCCON, 0xf, 0x4, 16);
	Macro_Clear_Bit(rGPCDAT, 9);
	// toggle key
	key = Macro_Extract_Area(rGPFDAT, 0xf, 4);
	ret |= key<<4;


	return (~ret) & 0x1ff;
}

void Key_Wait_Key_Released_Multi(void) {
	while(Key_Get_Pressed_Multi());
}

int Key_Wait_Key_Pressed_Multi(void) {
	int key;
	while(!(key = Key_Get_Pressed_Multi()));
	return key;
}
