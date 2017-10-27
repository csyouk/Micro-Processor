#include <string.h>
#include "device_driver.h"
#define BLOCK 100

extern int Cam_Width;
extern int Cam_Height;

static unsigned short buff[240][320];

void Print_Data(unsigned int * p){
	int i, j;
	for(i=0,j=1;i<12;i++, j++){
		Uart_Printf("%.8p ",p[i]);
		if(j % 4 == 0){j=0;Uart_Printf("\n");}
	}
}

void Save_Img_To_Nand(void){
	int i, j;
	unsigned short * captured_img = CAM_Get_Image_Address();

	for(i=0;i<Cam_Height;i++)
	{
		for(j=0;j<Cam_Width;j++)
		{
			buff[i][j]=captured_img[(i*Cam_Width)+j];
		}
	}

	Nand_Erase_Block(BLOCK);
	Nand_Write_Block(BLOCK,(U8 *)buff);
	Uart_Printf("verify %d \n", Nand_Verify_Block(BLOCK, (U8 *)buff));

	Uart_Printf("=====\n");
//	Print_Data(captured_img);
	Print_Data((unsigned int * )buff);
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
	Lcd_Select_Draw_Frame_Buffer(0);
	Nand_Read_Block(BLOCK, (U8 *)buff);
	Lcd_Draw_Cam_Image(0,0, (void *)buff, Cam_Width, Cam_Height);

	Uart_Printf("=====\n");
	Print_Data((unsigned int * )buff);
}


