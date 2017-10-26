#include <stdlib.h>
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

/* Touch distance Calculation  */
#define ELEMENTS(X) (sizeof(X)/sizeof(X[0]))
#define MAX_Q 10
typedef struct {
	int x,y;
} Touch;

extern volatile int Touch_pressed;
extern volatile int Touch_x, Touch_y;
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

static void (*Filters[])(int x, int y, const unsigned short *fp, int width, int height) =
{
		Lcd_Draw_Cam_Image,
		Lcd_Draw_Cam_Image_Sharpen_Filter,
		Lcd_Draw_Cam_Image_Outline_Filter,
		Lcd_Draw_Cam_Image_Blur_Filter,
		Lcd_Draw_Cam_Image_Edge_Detect,
		Lcd_Draw_Cam_Image_Emboss_Filter,
		Lcd_Draw_Cam_Image_Gray_Average_Filter,
		Lcd_Draw_Cam_Image_Gray_Luminosity_Filter,
};

static char * modes[] = {
		"Default",
		"Sharpen_Filter",
		"Outline_Filter",
		"Blur Filter",
		"Edge Detect",
		"Emboss Filter",
		"Gray Average Filter",
		"Gray Luminosity Filter",
};

// Drag & Drop
void Main(void)
{
	int dis, lock = 0, mode = 0;
	unsigned short *q;
	Touch p_event;
	p_event.x = p_event.y = 0;
	dis = 0;

	// Init
	Init();


	while(1){


		// touch event
		if(Touch_pressed)
		{
			lock = 1;
			Touch_Get_Position();
			dis += ABS(p_event.x, Touch_x);
			dis += ABS(p_event.y, Touch_y);
			p_event.x = Touch_x, p_event.y = Touch_y;
			Touch_Set_Wait_For_Release();
		}

		if(!Touch_pressed && lock){
			Uart_Printf("Total distance : %d\n", dis);
			dis = 0; lock = 0;
		}

		// camera
		if(CAM_Get_Capture_Status() == 2)
		{
			CAM_Set_Capture_Status(0);
			q = CAM_Get_Image_Address();
			Lcd_Select_Draw_Frame_Buffer(frm);

			Filters[mode](0, 0, (void *)q, Cam_Width, Cam_Height);

			Lcd_Select_Display_Frame_Buffer(frm);
			Toggle_Frame();

			// switch filter
			if(Key_value == 2){
				mode++;
				if(mode > ELEMENTS(Filters)-1) mode = 0;
				Uart_Printf("%s mode \n", modes[mode]);
				Key_value=0;
			}
			else if(Key_value == 4){
				mode--;
				if(mode < 0) mode = ELEMENTS(Filters)-1;
				Uart_Printf("%s mode \n", modes[mode]);
				Key_value=0;
			}

			// Capture
			else if(Key_value == 5){

			}
		}

	}
}


