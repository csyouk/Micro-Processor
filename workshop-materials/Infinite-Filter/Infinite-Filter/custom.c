#include "device_driver.h"
#define BLOCK 100
#define FILE_SIZE (240*(320*3))
#define LCD_FB0	0x31800000 // 4MB

typedef unsigned int (* LAYER_FRAME)[LCD_XSIZE];
extern LAYER_FRAME Fb_Layer;
extern int Cam_Width;
extern int Cam_Height;
extern int frm;
extern unsigned int Display_frame;

void Save_Img_To_Nand(void){
	unsigned int q = Nand_Page_2_Addr(BLOCK, 0, 0);
	Nand_Write(q, Fb_Layer, FILE_SIZE);
	Uart_Printf("Verify : %d\n", Nand_Verify_Block(BLOCK,Fb_Layer));
}

void Disable_Capture(void){
	CAM_Capture_Run(0,0,0);
	CAM_Capture_Run(0, Cam_Width, Cam_Height);
	CAM_Interrupt_Enable(0);
}

void Enable_Capture(void){
	CAM_Capture_Run(0,0,0);
	CAM_Capture_Run(1, Cam_Width, Cam_Height);
	CAM_Interrupt_Enable(1);
}

void Show_Saved_Img(void){
	static unsigned char buff[FILE_SIZE * 4] = {0};
	unsigned int q = Nand_Page_2_Addr(BLOCK, 0, 0);

	Nand_Read(q, buff, FILE_SIZE);
 	Lcd_Select_Draw_Frame_Buffer(frm);
	Lcd_Draw_Cam_Image(0, 0, (void *)buff, Cam_Width, Cam_Height);
	Lcd_Set_Address(buff);
}