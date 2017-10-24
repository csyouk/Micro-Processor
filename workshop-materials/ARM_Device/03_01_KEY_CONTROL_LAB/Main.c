// UP Key 인식

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	Led_Init();

	// KEY_OUT0(GPC8) -> OUT, 0 값 출력
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	// KEY_IN0(GPF4) -> IN
	Macro_Write_Block(rGPFCON, 0x3, 0x0, 8);

	for(;;)
	{
		// UP(GPF4)가 눌리면 LED1 ON 떼면 OFF
		// GPF4번 비트가 0인지 1인지 check
		//if(rGPFDAT & (1<<4)){
		if(Macro_Check_Bit_Clear(rGPFDAT, 4))
		{
			Led_Display(0);
		}
		else
		{
			Led_Display(1);
		}

		// refactoring
		//Led_Display(Macro_Check_Bit_Clear(rGPFDAT, 4));
	}
}

#endif

// UP Key 인식의 확장 LED1 Toggling

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	int lock = 0, flag = 0, i = 0;
	Led_Init();

	// KEY_OUT0(GPC8) -> OUT, 0 값 출력
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	// KEY_IN0(GPF4) -> IN
	Macro_Write_Block(rGPFCON, 0x3, 0x0, 8);



	for (;;)
	{
		// UP(GPF4)가 눌리면 LED1 Toggling
		//if(Macro_Check_Bit_Clear(rGPFDAT, 4))
		//	Led_Display(flag^=1); // led toggle시켜서 넣음.

		// wait key released 방식
//		if (Macro_Check_Bit_Clear(rGPFDAT, 4)) {
//			Macro_Invert_Bit(rGPBDAT, 7);
//			while (Macro_Check_Bit_Clear(rGPFDAT, 4));
//		}

		// inter-lock 방식
		if (lock == 0 && Macro_Check_Bit_Clear(rGPFDAT, 4))
		{
			Macro_Invert_Bit(rGPBDAT, 7);
			lock = 1;
			Uart_Printf("lock 0\n");
		}
		else if (lock == 1 && Macro_Check_Bit_Set(rGPFDAT, 4))
		{
			lock = 0;
			Uart_Printf("lock 1\n");
		}
//		Uart_Printf("%d ", i++);
//		i %= 10;
	}
}

#endif

// 4개의 기능 Key 인식

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void) {
	int led;
	Led_Init();

	// KEY_OUT1 -> OUT mode, 0 값 출력
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 18);
	Macro_Clear_Bit(rGPCDAT, 9);

	// KEY_IN0~3 -> IN mode
	Macro_Write_Block(rGPFCON, 0xff, 0x0, 8);

	for (;;) {
		led = 0;
		// sol 1
//		led = ~Macro_Extract_Area(rGPFDAT, 0xF, 4);
//		Uart_Printf("%d\n", ~led); // 1,3,7,15
		//		Led_Display(~Macro_Extract_Area(rGPFDAT, 0xF, 4));

		// sol 2
//		Led_Display( ~((rGPFDAT >> 4) & 0xf) );

		// sol 3
		Macro_Write_Block(rGPBDAT, 0xf, ((rGPFDAT >> 4) & 0xf), 7);


		// sol 4
		// KEY_IN0~3을 확인하고 눌린 키가 있으면 해당하는 LED ON, 안 눌렸으면 OFF
//		if(Macro_Check_Bit_Clear(rGPFDAT,4)) led |= (1<<0);
//		if(Macro_Check_Bit_Clear(rGPFDAT,5)) led |= (1<<1);
//		if(Macro_Check_Bit_Clear(rGPFDAT,6)) led |= (1<<2);
//		if(Macro_Check_Bit_Clear(rGPFDAT,7)) led |= (1<<3);
//		Led_Display(led);
	}
}

#endif

// 방향키와 Push 버튼의 인식

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	Led_Init();

	// KEY_OUT0 -> OUT, 0 값 출력 GPC 8번 핀 ==> 16번 비트 부터 2개.
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	// KEY_IN0~3 -> IN GPF 4~7 번 핀
	Macro_Write_Block(rGPFCON, 0xff,0x0, 8);

	// KEY_PUSH -> IN
	Macro_Write_Block(rGPFCON, 0x3, 0x0 ,0);

	for(;;)
	{
		// KEY_PUSH가 눌렸으면 4개 LED 모두 ON
		if(Macro_Check_Bit_Clear(rGPFDAT, 0)){ Macro_Write_Block(rGPBDAT, 0xf, 0x0, 7); }
		// KEY_IN0~3을 확인하고 눌린 키가 있으면 해당하는 LED ON,
		// 안 눌렸으면 OFF
		else { Macro_Write_Block(rGPBDAT, 0xf, ((rGPFDAT >> 4) & 0xf), 7); }

	}
}

#endif

// Key Matrix 인식, 멀티키는 무시

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	int ret =0;
	Uart_Init(115200);
	Led_Init();

	Key_Poll_Init();

	while(1){
		ret = Key_Get_Pressed();
		if(ret){
			Uart_Printf("key : = %d\n", ret);
		}
	}
}

#endif

// Key Matrix 인식, 멀티키 처리

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{

	Uart_Init(115200);
	Led_Init();

}

#endif

// 멀티키 미지원 디바이스 드라이버 함수 검증 프로그램

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	Uart_Init(115200);
	Led_Init();

	Uart_Printf("KEY MATRIX TEST\n");
	Key_Poll_Init();

	for(;;)
	{
		Uart_Printf("KEY=%d\n", Key_Wait_Key_Pressed());
		Key_Wait_Key_Released();
	}
}

#endif

// 멀티키 처리 디바이스 드라이버 함수 검증 프로그램

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

void Main(void)
{
	volatile int i;

	Uart_Init(115200);
	Led_Init();

	Uart_Printf("KEY MATRIX TEST - MULTI\n");
	Key_Poll_Init();

	for(;;)
	{
		Uart_Printf("KEY=0x%X\n", Key_Get_Pressed_Multi());
		for(i = 0; i < 0x1000000; i++);
	}
}

#endif

// 조이스틱으로 led toggle시키는 프로그램.

#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

int chk_c(unsigned int dest, int position){
 return (!((((unsigned)(dest))>>(position)) & 0x1));
}

int chk_s(unsigned int dest, int position){
 return ((((unsigned)(dest))>>(position)) & 0x1);
}

void Main(void)
{
	int lock = 0;
	volatile int key;
	Uart_Init(115200);
	Led_Init();

	Uart_Printf("KEY LED TOGGLE TEST\n");
//	Key_Poll_Init();



	for(;;)
	{
		// 안되는 코드. -- 틀린 코드
		key = Key_Get_Pressed();
//		Uart_Printf("%d | %d %d %d %d\n", key,
//				(rGPFDAT>>(4)) & 0x1, (rGPFDAT>>(5)) & 0x1,
//				(rGPFDAT>>(6)) & 0x1, (rGPFDAT>>(7)) & 0x1
//				);
		if(!key) continue;
//		Uart_Printf("key = %d / lock = %d / chk c = %d / chk s = %d\n",
//				key, lock, Macro_Check_Bit_Clear(rGPFDAT, (key + 3)),
//				Macro_Check_Bit_Set(rGPFDAT, (key + 3)));
		if (lock == 0 && Macro_Check_Bit_Clear(rGPFDAT, (key + 3)))
		{
			Macro_Invert_Bit(rGPBDAT, key + 6);
			lock = 1;
			Uart_Printf("lock start\n");
		}
		else if (lock == 1 && Macro_Check_Bit_Set(rGPFDAT, (key + 3)))
		{
			lock = 0;
			Uart_Printf("lock ongoing\n");
		}



		// 되는 코드 -- 틀린 코드
		if (lock == 0 && Macro_Check_Bit_Clear(rGPFDAT, (Key_Get_Pressed() + 3)))
		{
			Macro_Invert_Bit(rGPBDAT, Key_Get_Pressed() + 6);
			lock = 1;
			Uart_Printf("lock start\n");
		}
		else if (lock == 1 && Macro_Check_Bit_Set(rGPFDAT, (Key_Get_Pressed() + 3)))
		{
			lock = 0;
			Uart_Printf("lock ongoing\n");
		}


	}

}

#endif


// solution
#if 0

#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"
enum Key{UP,LEFT,DOWN,RIGHT};
void Main(void)
{
	int i;
	int lock[4] = {0};
	Uart_Init(115200);

	Uart_Printf("KEY LED TOGGLE TEST\n");


	// LED Init
	Macro_Write_Block(rGPBCON, 0xff, 0x55, 14);
	Macro_Set_Area(rGPBDAT,0xf,7);

	// key_output 0 => output
	Macro_Write_Block(rGPCCON, 0x3, 0x1, 16);
	Macro_Clear_Bit(rGPCDAT, 8);

	// key[3:0]
	Macro_Write_Block(rGPFCON,0xff,0x0,8);

	while(1){
		// sol 1 아래 코드 4벌 복사 후, lock을 키별로 만듬.
		// pressed
//		if(lock == 0 && Macro_Check_Bit_Clear(rGPFDAT, 4))
//		{
//			Macro_Invert_Bit(rGPBDAT, 7);
//			lock = 1;
//
//		// released
//		}else if(lock == 1 && Macro_Check_Bit_Set(rGPFDAT, 4)){
//			lock = 0;
//		}


		// sol 2 using for loop
		for(i = 0; i < 4; i++){
			if(lock[i] == 0 && Macro_Check_Bit_Clear(rGPFDAT, 4+i))
			{
				Macro_Invert_Bit(rGPBDAT, 7+i);
				lock[i] = 1;
				Uart_Printf("lock enabled\n");
			// released
			}else if(lock[i] == 1 && Macro_Check_Bit_Set(rGPFDAT, 4+i)){
				lock[i] = 0;
				Uart_Printf("lock disabled\n");
			}
		}
	}
}
#endif
