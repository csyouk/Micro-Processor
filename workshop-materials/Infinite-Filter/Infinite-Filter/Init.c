#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

#define LAYER_MODE		0
#define ELEMENTS(X) (sizeof(X)/sizeof(X[0]))

/* 5:5:5:I Color Definition */
#define BLACK	0x0000
#define WHITE	0xfffe
#define BLUE	0x003e
#define GREEN	0x07c0
#define RED		0xf800

extern int Cam_Width;
extern int Cam_Height;
extern char * Cam_Frame_Buf;

void Touch_Init(void);
void Init_Extras(void);

static void (*Func_Arr[])(void) =
{
		Touch_Init,
		Nand_Init,
		Key_ISR_Init,
		Lcd_Graphic_Init,
		Lcd_Graphic_Init,
		Key_ISR_Init,
		CAM_Camera_Init,
		CAM_Module_Init,
		Init_Extras
};

void Init(void){
	int i;
	for(i = 0; i < ELEMENTS(Func_Arr); i++){
		Func_Arr[i]();
	}
}

void Touch_Init(void){
	Touch_ISR_Enable(1);
}

void Init_Extras(void){
	// Uart
	Uart_Init(115200);


	// Cam
	Cam_Frame_Buf = (char *)Free_Memory_BASE;
	Cam_Width = 320;
	Cam_Height= 240;

	CAM_Set_Capture_Status(0);
	CAM_Capture_Run(0,0,0);
	CAM_Capture_Run(1, Cam_Width, Cam_Height);
	CAM_XY_Flip(0,0); // no flip

	Key_ISR_Enable(1);
	CAM_Interrupt_Enable(1);

	// LCD
	Lcd_Control_Brightness(5);
	Lcd_Select_Buffer_Mode(LAYER_MODE);
 	Lcd_Select_Draw_Frame_Buffer(0);
 	Lcd_Select_Display_Frame_Buffer(0);
	Lcd_Set_Trans_Mode(0);
	Lcd_Set_Shape_Mode(0,0);
	Lcd_Set_Shape_Back_Image_Mode(0, (void *)0);

	Uart_Printf("Init done!!\n");


}
