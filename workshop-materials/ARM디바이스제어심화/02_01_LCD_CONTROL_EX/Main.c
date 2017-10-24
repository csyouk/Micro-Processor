#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

/* 5:5:5:I Color Definition */

#define BLACK	0x0000
#define WHITE	0xfffe
#define BLUE	0x003e
#define GREEN	0x07c0
#define RED		0xf800

#include <stdlib.h>

#include "./Images/img01.h"
#include "./Images/img02.h"
#include "./Images/img03.h"
#include "./Images/img04.h"
#include "./Images/img05.h"
#include "./Images/shape.h"
#include "./Images/run_er.h"
#include "./Images/run_ep.h"

const unsigned short * Disp[] = {img01, img02, img03, img04};

void Basic_Graphics_Test(void);
void Special_Mode_Test(void);
void Font_Test(void);
void Layer_Test(void);
void Virtual_Test(void);
void Scroll1_Test(void);
void Scroll2_Test(void);
void BMP_File_Test(void);
void Nand_Display_Test(void);
void Double_Buffering_Test(void);
void Draw_Back_Test(void);
void Shape_Back_Image_Test(void);

void (*fa[])(void) =
{
	Basic_Graphics_Test,
	Special_Mode_Test,
	Font_Test,
	Layer_Test,
	Virtual_Test,
	Scroll1_Test,
	Scroll2_Test,
	BMP_File_Test,
	Nand_Display_Test,
	Double_Buffering_Test,
	Draw_Back_Test,
	Shape_Back_Image_Test
};

void Main(void)
{
	int r;

	Nand_Init();
	Timer3_Buzzer_Init();
	Uart_Init(115200);
	Lcd_Graphic_Init();
	Key_Poll_Init();

	Uart_Printf("LCD Graphics Test\n");

	for(;;)
	{
		Lcd_Control_Brightness(5);
		Lcd_Select_Buffer_Mode(LAYER_MODE);
	 	Lcd_Select_Draw_Frame_Buffer(0);
	 	Lcd_Select_Display_Frame_Buffer(0);
		Lcd_Set_Trans_Mode(0);
	    Lcd_Set_Shape_Mode(0,0);
	    Lcd_Set_Shape_Back_Image_Mode(0, (void *)0);
	 	Lcd_Clr_Screen(RED);
		Lcd_Printf(10,10,GREEN,RED,1,1,"Willtek 임베디드 전문가 그룹");

		Uart_Printf("===================================================\n");
		Uart_Printf("GBOX WT2440L API Function Test    (c)Willtek Corp. \n");
		Uart_Printf("===================================================\n");
		Uart_Printf("[1] Basic Graphics Test		                   	\n");
		Uart_Printf("[2] Special Graphics Mode Test                    	\n");
		Uart_Printf("[3] Font Display Test                            	\n");
		Uart_Printf("[4] Layer Mode(320*240) Test                       \n");
		Uart_Printf("[5] Virtual Mode(640*480) Test                     \n");
		Uart_Printf("[6] Scroll-Mode 1 Test                             \n");
		Uart_Printf("[7] Scroll-Mode 2 Test                             \n");
		Uart_Printf("[8] BMP File Display Test                          \n");
		Uart_Printf("[9] NAND BMP Display Test                          \n");
		Uart_Printf("[10] Double Buffering Test                         \n");
		Uart_Printf("[11] LCD Draw Back Test                      	    \n");
		Uart_Printf("[12] Shape Back Image Mode Test               	    \n");
		Uart_Printf("===================================================\n");
		Uart_Printf(">> ");

		r = Uart_GetIntNum()-1;
		Uart_Get_Pressed();

		if((unsigned int)r >= sizeof(fa)/sizeof(fa[0]))
		{
			Uart_Printf("Wrong Number!\n");
			continue;
		}

		fa[r]();

		Uart_Printf("Test Finished\n\n");
	}
}

void Basic_Graphics_Test(void)
{
	int i;
	int xtmp, ytmp;

	Uart_Printf("\n그래픽 기본 함수 실습\n");
	Uart_Printf("키보드 누를때마다 다음단계로 진행\n");

	while(!Uart_Get_Pressed());

	Uart_Printf("Clear Screen\n");
	Lcd_Clr_Screen(BLUE);

	while(!Uart_Get_Pressed());

	Uart_Printf("Draw HLINE, VLINE, LINE\n");
	Lcd_Draw_Hline(100,50,200,RED);
	Lcd_Draw_Vline(80,100,200,RED);
	Lcd_Draw_Line(0,0,319,239,GREEN);

	while(!Uart_Get_Pressed());

	Uart_Printf("Draw Rectangular\n");
	Lcd_Clr_Screen(BLACK);
	Lcd_Draw_Rect(20,20,120,120,RED);
	Lcd_Draw_Rect(70,70,220,220,BLUE);

	while(!Uart_Get_Pressed());

	Uart_Printf("Draw BAR\n");
	Lcd_Clr_Screen(BLACK);
	Lcd_Draw_Bar(0,0,100,100,RED);
	Lcd_Draw_Bar(50,50,200,200,BLUE);

	while(!Uart_Get_Pressed());

	Uart_Printf("Draw BMP\n");
	Uart_Printf("Get BMP Information\n");
	Lcd_Get_Info_BMP(&xtmp, &ytmp, img01);
	Uart_Printf("X=[%d], Y=[%d]\n", xtmp, ytmp);
	Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), img01);

	for(i=0; i<(sizeof(Disp)/sizeof(Disp[0])); i++)
	{
		Lcd_Get_Info_BMP(&xtmp, &ytmp, Disp[i]);
		Uart_Printf("X=[%d], Y=[%d]\n", xtmp, ytmp);
		Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), Disp[i]);
		Timer4_Delay(1000);
	}

	Uart_Printf("키보드 누르면 종료\n");
	while(!Uart_Get_Pressed());
}

void Special_Mode_Test(void)
{
	int xtmp, ytmp;

	Uart_Printf("\n그래픽 기본 함수 실습\n");
	Uart_Printf("키보드 누를때마다 다음단계로 진행\n");

	Uart_Printf("Non Transparency Mode\n");

	Lcd_Clr_Screen(BLACK);
	Lcd_Draw_Bar(0,0,100,100,RED);
	Lcd_Draw_Bar(50,50,200,200,BLUE);

	while(!Uart_Get_Pressed());

	Uart_Printf("Transparency Mode\n");

	Lcd_Clr_Screen(BLACK);
	Lcd_Set_Trans_Mode(1);
	Lcd_Draw_Bar(0,0,100,100,RED);
	Lcd_Draw_Bar(50,50,200,200,BLUE);
	Lcd_Set_Trans_Mode(0);

	while(!Uart_Get_Pressed());

	Uart_Printf("Non Shape Mode\n");

	Lcd_Clr_Screen(GREEN);
    Lcd_Get_Info_BMP(&xtmp, &ytmp, shape);
    Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), shape);

	while(!Uart_Get_Pressed());

	Uart_Printf("Shape Mode(Chroma Key Mode)\n");

    Lcd_Set_Shape_Mode(1,0xFFFE);
	Lcd_Clr_Screen(GREEN);
    Lcd_Get_Info_BMP(&xtmp, &ytmp, shape);
    Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), shape);
    Lcd_Set_Shape_Mode(0,0);

	Uart_Printf("키보드 누르면 종료\n");
	while(!Uart_Get_Pressed());
}

void Font_Test(void)
{
	int i, j;
	char str[20];

	Uart_Printf("영문 Nickname(20글자 이내)");
	Uart_GetString(str);
	Uart_Printf("\nFont Display\n");
	Lcd_Printf(2,2,BLACK,RED,2,2,"윌텍 임베디드 그룹");
	Lcd_Printf(0,150,GREEN,BLACK,1,1,"%s", "(주)윌텍,(주)행복한기술");
	Lcd_Printf(0,170,GREEN,BLACK,1,1,"%s", "(주)기쁜기술,(주)임베딕스,(주)에듀닉스");
	Lcd_Printf(30,100,RED,GREEN,2,1,"윌텍은 %d개 회사", 5);
	Lcd_Printf(0,200,BLUE,GREEN,1,2,"화이팅! %s", str);

	for(i=0; i<10; i++)
	{
		for(j=0; j<10; j++)
		{
			Lcd_Put_Pixel(50+i, 80+j, BLUE);
		}
	}

	Uart_Printf("키보드 누르면 종료\n");
	while(!Uart_Get_Pressed());
}

void Layer_Test(void)
{
	int i;

	Uart_Printf("Layer Mode Test, 320*240 4 Layers\n");
	Lcd_Select_Buffer_Mode(LAYER_MODE);

	for(i=0; i<(sizeof(Disp)/sizeof(Disp[0])); i++)
	{
		Lcd_Select_Draw_Frame_Buffer(i);
		Lcd_Draw_BMP(0,0,Disp[i]);
	}

	for(i=0; i<(sizeof(Disp)/sizeof(Disp[0])); i++)
	{
		Lcd_Select_Display_Frame_Buffer(i);
		Timer4_Delay(1000);
	}
}

void Virtual_Test(void)
{
	Uart_Printf("Virtual Mode Test, 640*480 1 Layer\n");

	Lcd_Select_Buffer_Mode(VIRTUAL_MODE);
	Lcd_Draw_BMP(0,0,img05);

	Lcd_Set_Virtual_Display_Start(0, 0);
	Timer4_Delay(1000);
	Lcd_Set_Virtual_Display_Start(320, 0);
	Timer4_Delay(1000);
	Lcd_Set_Virtual_Display_Start(0, 240);
	Timer4_Delay(1000);
	Lcd_Set_Virtual_Display_Start(320, 240);
	Timer4_Delay(1000);
}

#define X_OFF	5
#define Y_OFF	5
#define MV_DEL	100

void Scroll1_Test(void)
{
	int key;
	int pos[2] = {LCD_XSIZE/3/2, LCD_YSIZE/2};
	int move_offset[][2] = {{0,0},{0,-Y_OFF}, {-X_OFF,0}, {0,+Y_OFF},{+X_OFF,0}};

	Uart_Printf("Scroll Mode 1 Test\n");
	Lcd_Select_Buffer_Mode(VIRTUAL_MODE);
	Lcd_Draw_BMP(0,0,img05);

	Uart_Printf("방향키를 누르면 화면 이동\n");
	Uart_Printf("컴퓨터 키보드를 누르면 종료\n");
	Lcd_Set_Virtual_Display_Start(pos[0], pos[1]);

	for(;;)
	{
		key = Key_Get_Pressed();

		if(((unsigned int)key <= 4) && (key !=0))
		{
			pos[0] += move_offset[key][0];
			pos[1] += move_offset[key][1];

			Lcd_Set_Virtual_Display_Start(pos[0], pos[1]);
			Uart_Printf("POS=(%d, %d)\n", pos[0], pos[1]);
			Timer0_Repeat(MV_DEL);

			while(!Timer0_Check_Timeout());
		}

		if(Uart_Get_Pressed()) return;
	}
}

void Scroll2_Test(void)
{
	int key;
	int back[2], pos[2] = {LCD_XSIZE/3/2, LCD_YSIZE/2};
	int move_offset[][2] = {{0,0},{0,-Y_OFF}, {-X_OFF,0}, {0,+Y_OFF},{+X_OFF,0}};

	Uart_Printf("Scroll Mode 2 Test\n");
	Lcd_Select_Buffer_Mode(VIRTUAL_MODE);
	Lcd_Draw_BMP(0,0,img05);

	Uart_Printf("방향키를 누르면 화면 이동\n");
	Uart_Printf("컴퓨터 키보드를 누르면 종료\n");
	Lcd_Set_Virtual_Display_Start(pos[0], pos[1]);
	back[0] = pos[0];
	back[1] = pos[1];

	for(;;)
	{
		key = Key_Get_Pressed();

		if(((unsigned int)key <= 4) && (key !=0))
		{
			pos[0] += move_offset[key][0];
			pos[1] += move_offset[key][1];

			if((pos[0] < 0) || (pos[0] > LCD_XSIZE/3))
			{
				Uart_Printf("X-Limit Over!\n");
				Timer3_Buzzer_Beep(1,1);
				pos[0] = back[0];
			}

			if((pos[1] < 0) || (pos[1] > LCD_YSIZE))
			{
				Uart_Printf("Y-Limit Over!\n");
				Timer3_Buzzer_Beep(1,1);
				pos[1] = back[1];
			}

			back[0] = pos[0];
			back[1] = pos[1];

			Lcd_Set_Virtual_Display_Start(pos[0], pos[1]);
			Uart_Printf("POS=(%d, %d)\n", pos[0], pos[1]);
			Timer0_Repeat(MV_DEL);

			while(!Timer0_Check_Timeout());
		}

		if(Uart_Get_Pressed()) return;
	}
}

void BMP_File_Test(void)
{
	int add;

	Uart_Printf("24bpp BMP Display\n");
	Uart_Printf("반드시 DRAM 0x31200000 번지 이후에 BMP 파일이 적재되어 있어야 한다\n");
	Uart_Printf("그림은 반드시 24bpp 비압축 BMP 형식이어야 하며 적재 안되어 있으면 오류 발생\n");
	Lcd_Select_Buffer_Mode(LAYER_MODE);
 	Lcd_Select_Draw_Frame_Buffer(0);
 	Lcd_Select_Display_Frame_Buffer(0);

	Uart_Printf("Enter the address of BMP file saved. Default=0x31200000 => ");

	add = Uart_GetIntNum();

	if((add <= 0x31200000) || (add >= 0x31600000))
	{
		add = 0x31200000;
	}

	Uart_Printf("%#x\n", add);
	Lcd_Draw_BMP_File_24bpp(0,0,(void *)add);
	Uart_Printf("컴퓨터 키보드를 누르면 종료\n");
	while(!Uart_Get_Pressed());
}

#define NO_OF_BMP 6

void Nand_Display_Test(void)
{
	int address, offset;
	short width, height;
	int i;
	unsigned short p[240][320];

	Lcd_Select_Buffer_Mode(LAYER_MODE);
 	Lcd_Select_Draw_Frame_Buffer(0);
 	Lcd_Select_Display_Frame_Buffer(0);

	Uart_Printf("NAND BMP Display\n");
	Uart_Printf("NAND 100번 블록 이후에는 반드시 헤더가 포함된 BMP 그룹파일이 Writing 되어 있어야 한다\n");
	Uart_Printf("본 예제는 6장의 그림이 있는 경우의 예제임\n");

	Uart_Printf("\nStart Block Number of Image on NAND? ");
	address = Uart_GetIntNum();
	Uart_Printf("\rBLOCK=%d\n", address);
	address = Nand_Page_2_Addr(address, 0, 0);
	Uart_Printf("address=0x%X\n", address);
	Uart_Get_Pressed();
	Uart_Printf("컴퓨터 키보드를 누르면 종료\n");

	for(;;)
	{
		for(i=0; i<NO_OF_BMP; i++)
		{
			Lcd_Select_Draw_Frame_Buffer(i%2);
			Lcd_Clr_Screen(BLACK);

			Nand_Read(address+i*8, (U8 *)&offset, 4);
			Nand_Read(address+i*8+4, (U8 *)&width, 2);
			Nand_Read(address+i*8+6, (U8 *)&height, 2);

			Uart_Printf("[%d] OFFSET=%u, W=%d, H=%d\n", i, offset, width, height);

			Nand_Read(address+offset, (U8 *)p, width*height*2);
			Lcd_Draw_Image(0,0, (void *)p, width, height);

			Lcd_Wait_Blank();
			Lcd_Select_Display_Frame_Buffer(i%2);

			if(Uart_Get_Pressed()) return;
			Timer4_Delay(1000);
		}
	}
}

void Double_Buffering_Test(void)
{
	int xtmp, ytmp;
	int frame;

	Lcd_Select_Buffer_Mode(LAYER_MODE);
  	Lcd_Select_Draw_Frame_Buffer(1);
	Lcd_Clr_Screen(BLACK);
  	Lcd_Select_Draw_Frame_Buffer(0);
	Lcd_Clr_Screen(BLACK);
	Lcd_Select_Display_Frame_Buffer(0);

	Uart_Printf("Double Buffering Test\n");
	Uart_Printf("컴퓨터 키보드를 누르면 다음 단계로\n");
	Uart_Printf("[1]Single Buffering\n");

	do
	{
	     Lcd_Get_Info_BMP(&xtmp, &ytmp, img01);
	     Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), img01);
	     Timer4_Delay(400);

	     Lcd_Get_Info_BMP(&xtmp, &ytmp, img02);
	     Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), img02);
	     Timer4_Delay(400);
	}while(!Uart_Get_Pressed());

	Uart_Printf("\n[2]Double Buffering\n");

  	Lcd_Select_Draw_Frame_Buffer(1);
	Lcd_Clr_Screen(BLACK);
    Lcd_Get_Info_BMP(&xtmp, &ytmp, img01);
    Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), img01);

	Lcd_Select_Draw_Frame_Buffer(0);
	Lcd_Clr_Screen(BLACK);
    Lcd_Get_Info_BMP(&xtmp, &ytmp, img02);
    Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), img02);
	Lcd_Select_Display_Frame_Buffer(0);
	frame = 0;

	do
	{
  		 Lcd_Wait_Blank();
		 Lcd_Select_Display_Frame_Buffer((frame++)%2);
	     Timer4_Delay(400);
	}while(!Uart_Get_Pressed());
}

void Draw_Back_Test(void)
{
	int pos_x = 0;
	int pos_y = 0;
	int old_x = 0;
	int old_y = 0;
	int back_color = BLUE;
	int object_color = RED;
	int object_width = 30;
	int object_height = 20;
	int step = 10;
	unsigned int *buf, *new_buf;
	int xtmp, ytmp;
	int i;

	Lcd_Select_Buffer_Mode(LAYER_MODE);
  	Lcd_Select_Draw_Frame_Buffer(0);
	Lcd_Clr_Screen(BLACK);
    Lcd_Draw_BMP(0, 0, img01);
	Lcd_Select_Display_Frame_Buffer(0);

	Uart_Printf("Draw Back Test\n");
	Uart_Printf("컴퓨터 키보드를 누르면 다음 단계로\n");
	Uart_Printf("\n[1]Normal Mode\n");
	Uart_Printf("배경 그림이 있는 경우 지나간 자리 처리가 문제가 된다\n");

	for(;;)
	{
		if((old_x != pos_x) || (old_y != pos_y))
		{
			Lcd_Draw_Bar(old_x, old_y, old_x + object_width, old_y + object_height, back_color);
		}

		Lcd_Draw_Bar(pos_x, pos_y, pos_x + object_width, pos_y + object_height, object_color);
		old_x = pos_x;
		old_y = pos_y;
		pos_x += step;
		pos_y += step;

		if((pos_x >= (LCD_XSIZE - object_width)) || (pos_y >= (LCD_YSIZE - object_height)))
		{
			pos_x = 0;
			pos_y = 0;
		}

		if(Uart_Get_Pressed()) break;
		Timer4_Delay(20);
	}

	Uart_Printf("\n[2]Draw Tile Mode Test\n");
	Uart_Printf("Draw Tile 기능으로 해결 (배경 그림의 일부를 오려서 붙이는 방법)\n");

	Lcd_Clr_Screen(BLACK);
    Lcd_Draw_BMP(0, 0, img01);

	pos_x = 0;
	pos_y = 0;
	old_x = 0;
	old_y = 0;

	for(;;)
	{
		if((old_x != pos_x) || (old_y != pos_y))
		{
			Lcd_Draw_BMP_Tile(old_x, old_y, img01, old_x, old_y, object_width, object_height);
		}

		Lcd_Draw_Bar(pos_x, pos_y, pos_x + object_width - 1, pos_y + object_height - 1, object_color);
		old_x = pos_x;
		old_y = pos_y;
		pos_x += step;
		pos_y += step;

		if((pos_x >= (LCD_XSIZE - object_width)) || (pos_y >= (LCD_YSIZE - object_height)))
		{
			pos_x = 0;
			pos_y = 0;
		}

		if(Uart_Get_Pressed()) break;
		Timer4_Delay(50);
	}


	Uart_Printf("\n[3]Draw Back Mode Test 1\n");
	Uart_Printf("Draw Back 기능으로 해결 (현재 LCD에 그려진 그림을 백업한 후 그리는 방법)\n");

	Lcd_Clr_Screen(BLACK);
    Lcd_Draw_BMP(0, 0, img01);
	buf = malloc(object_width * object_height * sizeof(unsigned int));

	Uart_Printf("malloc=%p\n", buf);

	pos_x = 0;
	pos_y = 0;
	old_x = 0;
	old_y = 0;

	for(;;)
	{
		if((old_x != pos_x) || (old_y != pos_y))
		{
			Lcd_Draw_Backup_24bpp(old_x, old_y, object_width, object_height, buf);
		}

		Lcd_Save_Backup_24bpp(pos_x, pos_y, object_width, object_height, buf);

		Lcd_Draw_Bar(pos_x, pos_y, pos_x + object_width - 1, pos_y + object_height - 1, object_color);
		old_x = pos_x;
		old_y = pos_y;
		pos_x += step;
		pos_y += step;

		if((pos_x >= (LCD_XSIZE - object_width)) || (pos_y >= (LCD_YSIZE - object_height)))
		{
			pos_x = 0;
			pos_y = 0;
		}

		if(Uart_Get_Pressed()) break;
		Timer4_Delay(50);
	}

	Uart_Printf("\n[4]Draw Back Mode Test 2\n");
	Uart_Printf("크기가 다른 여러 그림이 배경으로 올라온 상태이면 Draw Back이 유리\n");

	Lcd_Clr_Screen(BLACK);
    Lcd_Draw_BMP(0, 0, img02);
    Lcd_Draw_BMP(100, 100, run_er);

	pos_x = 0;
	pos_y = 0;
	old_x = 0;
	old_y = 0;

	for(;;)
	{
		if((old_x != pos_x) || (old_y != pos_y))
		{
			Lcd_Draw_Backup_24bpp(old_x, old_y, object_width, object_height, buf);
		}

		Lcd_Save_Backup_24bpp(pos_x, pos_y, object_width, object_height, buf);

		Lcd_Draw_Bar(pos_x, pos_y, pos_x + object_width - 1, pos_y + object_height - 1, object_color);
		old_x = pos_x;
		old_y = pos_y;
		pos_x += step;
		pos_y += step;

		if((pos_x >= (LCD_XSIZE - object_width)) || (pos_y >= (LCD_YSIZE - object_height)))
		{
			pos_x = 0;
			pos_y = 0;
		}

		if(Uart_Get_Pressed()) break;
		Timer4_Delay(50);
	}

	Uart_Printf("\n[5]Draw Back Mode Test 3\n");
	Uart_Printf("그림이 변화하는 object가 많을 경우 문제 발생\n");

	Lcd_Clr_Screen(BLACK);
    Lcd_Draw_BMP(0, 0, img02);

    Lcd_Get_Info_BMP(&xtmp, &ytmp, run_er);
    new_buf = malloc(xtmp * ytmp * sizeof(unsigned int));

	pos_x = 0;
	pos_y = 0;
	old_x = 0;
	old_y = 0;
	i = 0;

	Timer0_Repeat(1000);

	for(;;)
	{
		if((old_x != pos_x) || (old_y != pos_y))
		{
			Lcd_Draw_Backup_24bpp(old_x, old_y, object_width, object_height, buf);
		}

		Lcd_Save_Backup_24bpp(pos_x, pos_y, object_width, object_height, buf);

		Lcd_Draw_Bar(pos_x, pos_y, pos_x + object_width - 1, pos_y + object_height - 1, object_color);
		old_x = pos_x;
		old_y = pos_y;
		pos_x += step;
		pos_y += step;

		if((pos_x >= (LCD_XSIZE - object_width)) || (pos_y >= (LCD_YSIZE - object_height)))
		{
			pos_x = 0;
			pos_y = 0;
		}

		if(Timer0_Check_Timeout())
		{
			if(i==0)
			{
				Lcd_Save_Backup_24bpp(100, 100, xtmp, ytmp, new_buf);
				Lcd_Draw_BMP(100, 100, run_er);
				i=1;
			}

			else
			{
				Lcd_Draw_Backup_24bpp(100, 100, xtmp, ytmp, new_buf);
				i = 0;
			}
		}

		if(Uart_Get_Pressed()) break;
		Timer4_Delay(50);
	}
}

void Shape_Back_Image_Test(void)
{
	Lcd_Select_Buffer_Mode(LAYER_MODE);
  	Lcd_Select_Draw_Frame_Buffer(0);
    Lcd_Select_Display_Frame_Buffer(0);

	Uart_Printf("Shape Back Image Mode Test\n");
	Uart_Printf("컴퓨터 키보드를 누르면 다음 단계로\n");

    Uart_Printf("[1]Shape Mode & No Shape Back Image Mode\n");

    Lcd_Set_Shape_Mode(0, 0);
    Lcd_Clr_Screen(GREEN);
    Lcd_Draw_BMP(0, 0, img01);
    Lcd_Set_Shape_Back_Image_Mode(0, (void *)0);
    Lcd_Set_Shape_Mode(1,0xFFFE);
	Lcd_Draw_Bar(20, 20, 200-1, 200-1, BLUE);
    Lcd_Draw_BMP(50, 50, shape);

    Uart_Get_Char();

    Uart_Printf("[2]Shape Mode & Shape Back Image Mode\n");

    Lcd_Set_Shape_Mode(0, 0);
    Lcd_Clr_Screen(GREEN);
    Lcd_Draw_BMP(0, 0, img01);
    Lcd_Set_Shape_Back_Image_Mode(1, (void *)img01);
    Lcd_Set_Shape_Mode(1,0xFFFE);
	Lcd_Draw_Bar(20, 20, 200-1, 200-1, BLUE);
    Lcd_Draw_BMP(50, 50, shape);

    Uart_Get_Char();
}
