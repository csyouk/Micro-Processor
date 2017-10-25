#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"
#include "Init.h"
/* 5:5:5:I Color Definition */

#define BLACK	0x0000
#define WHITE	0xfffe
#define BLUE	0x003e
#define GREEN	0x07c0
#define RED		0xf800

#include <stdlib.h>

void Camera_Polling_Test(void);
void Camera_Interrupt_Test(void);

static void (*Func_Arr[])(void) =
{
		Camera_Interrupt_Test
};

extern char * Cam_Frame_Buf;

extern volatile int Touch_pressed;
extern volatile int Touch_x, Touch_y;




// Drag & Drop
#if 01
#define MAX_Q 10
typedef struct {
	int x,y;
} Touch;
Touch buffer[MAX_Q];
int RP, WP;

void Main(void)
{
	int dis;
	Touch p_event, c_event;
	Touch_Init();
	Camera_Init();

	while(1){
		if(Touch_pressed)
		{
			RP = WP = 0;
			dis = 0;
//			Uart_Printf("Pressed X=%u Y=%u\n", Touch_x, Touch_y);
			c_event.x = Touch_x; c_event.y = Touch_y;
			buffer[WP++] = c_event;

			while(1){
				Touch_Get_Position();
				p_event = buffer[RP++];
				RP %= MAX_Q;
				c_event.x = Touch_x; c_event.y = Touch_y;
				buffer[WP++] = c_event;
				WP %= MAX_Q;

				dis += ABS(p_event.x, c_event.x);

				dis += ABS(p_event.y, c_event.y);

				Touch_Set_Wait_For_Release();
				if(!Touch_pressed){
					Uart_Printf("Total distance : %d\n", dis);
					break;
				}
			}
		}
	}
}

#endif

extern volatile int Key_value;

int Cam_Width  = 320;
int Cam_Height = 240;
int Cam_Exp_Mode = 0;


static int frm = 0;

void Toggle_Frame(void)
{
	if(frm) frm = 0;
	else frm = 1;
}

void Camera_Interrupt_Test(void)
{
	unsigned short *q;

	Uart_Printf("\n\nCamera Test (Interrupt)\n");

	{
		Uart_Printf("Camera test: Press any key for next test\n");
		Uart_Printf("UP: Pause\n");
		Uart_Printf("DOWN: Play\n");

		Cam_Width = 320;
		Cam_Height= 240;

		CAM_Set_Capture_Status(0);
		CAM_Capture_Run(0,0,0);
		CAM_Capture_Run(1, Cam_Width, Cam_Height);
		CAM_XY_Flip(0,1);

		Key_ISR_Enable(1);
		CAM_Interrupt_Enable(1);

		do
		{
			if(CAM_Get_Capture_Status() == 2)
			{
				CAM_Set_Capture_Status(0);
				q = CAM_Get_Image_Address();
				Lcd_Select_Draw_Frame_Buffer(frm);
				Lcd_Draw_Cam_Image(0, 0, (void *)q, Cam_Width, Cam_Height);
				Lcd_Printf(10,10,GREEN,RED,1,1,"(��)���� ���������");
				Lcd_Select_Display_Frame_Buffer(frm);
				Toggle_Frame();
			}

			if(Key_value)
			{
				if(Key_value == 1) CAM_Capture_Pause();
				else if(Key_value == 3) CAM_Capture_Restart();
				Key_value = 0;
			}

		}while(!Uart_Get_Pressed());

		CAM_Capture_Run(0,0,0);
		CAM_Capture_Run(0, Cam_Width, Cam_Height);
		CAM_Interrupt_Enable(0);
		Key_ISR_Enable(0);
	}
}
