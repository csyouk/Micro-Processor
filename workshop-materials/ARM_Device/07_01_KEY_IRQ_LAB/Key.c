#include "2440addr.h"
#include "macro.h"

void Key_Poll_Init(void) {
	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);
	Macro_Write_Block(rGPFCON, 0x3, 0x0, 0);
	Macro_Write_Block(rGPCCON, 0xf, 0x0, 16);
	Macro_Clear_Area(rGPCDAT, 0x3, 8);
}

#define OUTPUT_DELAY		0x1000
#define COUNT_FOR_CHAT 		500
#define SCAN_ALL()			Macro_Write_Block(rGPCCON, 0xf, 0x5, 16)
#define SCAN_LINE(line) 	Macro_Write_Block(rGPCCON, 0xf, 0x1<<(line*2), 16)
#define DELAY_FOR_OUTPUT()	{volatile int i = OUTPUT_DELAY; while(i--);}

int Key_Get_Pressed(void) {
	// key scan algorithm
	int i;
	int x;
	static unsigned char key[] =
			{ 0, 1, 2, 0, 3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0 };

	SCAN_ALL();
	DELAY_FOR_OUTPUT();
	x = rGPFDAT & ((0xF << 4) | 1);

	for (i = 0; i < COUNT_FOR_CHAT; i++) {
		if ((rGPFDAT & ((0xF << 4) | 1)) != x)
			return 0;
	}

	SCAN_LINE(1);
	DELAY_FOR_OUTPUT();
	x = rGPFDAT & (0xf << 4);
	x = key[(~x >> 4) & 0xF];
	if (x)
		return x + 4;

	SCAN_LINE(0);
	DELAY_FOR_OUTPUT();
	x = rGPFDAT & ((0xf << 4) | 1);
	if (!(x & 0x1))
		return 9;
	x = key[(~x >> 4) & 0xF];
	return x;
}

void Key_Wait_Key_Released(void) {
	while (Key_Get_Pressed())
		;
}

int Key_Wait_Key_Pressed(void) {
	int k;

	while ((k = Key_Get_Pressed()) == 0)
		;
	return k;
}

int Key_Get_Pressed_Multi(void) {
	int x;
	int i;

	SCAN_ALL();
	DELAY_FOR_OUTPUT();
	x = rGPFDAT & ((0xF << 4) | 1);

	for (i = 0; i < COUNT_FOR_CHAT; i++) {
		if ((rGPFDAT & ((0xF << 4) | 1)) != x)
			return 0;
	}

	SCAN_LINE(0);
	DELAY_FOR_OUTPUT();
	x = (~rGPFDAT & 0xf0) >> 4;
	x |= (~rGPFDAT & 0x1) << 8;

	SCAN_LINE(1);
	DELAY_FOR_OUTPUT();
	x |= ~rGPFDAT & 0xf0;

	return x;
}

void Key_Wait_Key_Released_Multi(void) {
	while (Key_Get_Pressed_Multi())
		;
}

int Key_Wait_Key_Pressed_Multi(void) {
	int k;

	while ((k = Key_Get_Pressed_Multi()) == 0)
		;
	return k;
}

void Key_Push_ISR_Init(void) {

	// Key_Push => GPF0 (EINT0)
	Macro_Write_Block(rGPFCON, 0x3, 0x2, 0);

	// KEY_OUT0 => GPC8 (Output)
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 16);

	// KEY_OUT0 => Out 0
	Macro_Clear_Bit(rGPCDAT, 8);

	// EINT0 Trigger ==> Falling Edge
	Macro_Write_Block(rEXTINT0, 0xf, 0x2, 0);

}

void Key_Push_ISR_Enable(int en) {
	if (en) {
		// 기존 인터럽트 Pending Clear
		rSRCPND = BIT_EINT0; // (1<<0)
		rINTPND = BIT_EINT0; // (1<<0)

		// EINT0 Enable
		// mask 해제
		Macro_Clear_Bit(rINTMSK, 0);
	} else {
		// EINT0 Disable
		// mask 활성화
		Macro_Set_Bit(rINTMSK, 0);
	}
}

void Key_ISR_Init(void) {
	// KEY_IN[3:0] => GPF4,5,6,7 (EINT4~7)
	Macro_Write_Block(rGPFCON, 0xff, 0xaa, 8);

	// KEY_OUT[1:0] => GPC9,8 (Output)
	Macro_Write_Block(rGPCCON, 0xf, 0x5, 16);

	// KEY_OUT[1:0] => All Clear
	Macro_Clear_Area(rGPCDAT, 0xf, 8);

	// EINT[7:4] Trigger ==> Falling Edge
	Macro_Write_Block(rEXTINT0, 0xffff, 0x2222, 16);

}

void Key_ISR_Enable(int en) {
	if (en) {
		// 기존 인터럽트 PEnding Clear
		// EINTPEND, SRCPND, INTPND
		// clear 방식은 1을 write하는 방식으로..
		rEINTPEND = (0xf<<4);
		rSRCPND = (0x1<<4);
		rINTPND = (0x1<<4);

		// EINT4~7 개별 인터럽트 Enable
		Macro_Clear_Area(rEINTMASK, 0xf, 4);

		// EINT4_7 IRQ Enable
		Macro_Clear_Bit(rINTMSK, 4);
	} else {
		// EINT4~7 개별 인터럽트 Disable
		// mask설정(EINTMSK, INTMSK) - mask disable
		Macro_Set_Area(rEINTMASK, 0xf, 4);

		// EINT4_7 IRQ Disable
		Macro_Set_Bit(rINTMSK, 4);
	}
}

