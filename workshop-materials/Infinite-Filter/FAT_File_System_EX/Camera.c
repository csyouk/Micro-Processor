#include "2440addr.h"
#include "option.h"
#include "macro.h"
#include "device_driver.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#define CAM_IIC_WR_ADDR()		rIICDS=0x60
#define CAM_IIC_WR_DATA(x)		rIICDS=(x)
#define CAM_IIC_WR_START()		rIICSTAT=0xf0
#define CAM_IIC_WR_STOP()		rIICSTAT=0xd0

#define CAM_IIC_RD_DATA()		rIICDS
#define CAM_IIC_RD_START()		rIICSTAT=0xb0
#define CAM_IIC_RD_STOP()		rIICSTAT=0x90

#define CAM_IIC_READ_MODE()	 	Macro_Clear_Bit(rIICCON, 7)

#define CAM_IIC_Pending_Clear()	 Macro_Clear_Bit(rIICCON, 4)
#define CAM_IIC_Wait_INTR()	 	 while(Macro_Check_Bit_Clear(rIICCON, 4))
#define CAM_IIC_Wait_STOP()	 	 while(Macro_Check_Bit_Set(rIICSTAT, 5))

int CAM_IIC_Read(int addr)
{
	int r;

  	Macro_Write_Block(rGPECON, 0xf, 0xa, 28);
	rIICCON = (1<<7)+(1<<6)+(1<<5)+(0<<4)+(3<<0);
			
	CAM_IIC_WR_ADDR();
	CAM_IIC_WR_START();
	CAM_IIC_Wait_INTR();
	
	CAM_IIC_WR_DATA(addr);
	CAM_IIC_Pending_Clear();
	CAM_IIC_Wait_INTR();

	CAM_IIC_Pending_Clear();	
	CAM_IIC_WR_ADDR();
	CAM_IIC_RD_START();
	CAM_IIC_Wait_INTR();

	CAM_IIC_READ_MODE();
	CAM_IIC_Pending_Clear();
	CAM_IIC_Wait_INTR();

	r = CAM_IIC_RD_DATA();

	CAM_IIC_RD_STOP();
	CAM_IIC_Pending_Clear();
	CAM_IIC_Wait_STOP();

	return r;
}

void CAM_IIC_Write(int addr, int data)
{
  	Macro_Write_Block(rGPECON, 0xf, 0xa, 28);
	rIICCON = (1<<7)+(1<<6)+(1<<5)+(0<<4)+(3<<0);

	CAM_IIC_WR_ADDR();
	CAM_IIC_WR_START();
	CAM_IIC_Wait_INTR();
	
	CAM_IIC_WR_DATA(addr);
	CAM_IIC_Pending_Clear();
	CAM_IIC_Wait_INTR();

	CAM_IIC_WR_DATA(data);	
	CAM_IIC_Pending_Clear();
	CAM_IIC_Wait_INTR();
			
	CAM_IIC_WR_STOP();
	CAM_IIC_Pending_Clear();
	CAM_IIC_Wait_STOP();
}

void CAM_Port_Init(void)
{
	// UCLK => 48MHz, CAMCLKOUT => 24MHz
	rLOCKTIME = 0xffffffff;
	rUPLLCON = 0x3c042;
	rLOCKTIME = 0xffffffff;
	rCAMDIVN = (0<<12)+(0<<9)+(0<<8)+(1<<4)+(0<<0);

	// STANDBY0 = GPD4, Ouput, Low(STNADBY), Active-High
	Macro_Clear_Bit(rGPDDAT, 4);
	Macro_Write_Block(rGPDCON, 0x3, 1, 8);

	// CAM_DATA[7-0] = GPJ[7:0], special
	// CAMPCLK = GPJ8, CAMVSYNC = GPJ9, CAMHREF = GPJ10, CAMCLKOUT = GPJ11, CAMRESET = GPJ12, special
	Macro_Write_Block(rGPJCON, 0x03ffffff, 0x02aaaaaaa, 0);
}

void CAM_If_Reset(void)
{
	volatile int i;

	Macro_Set_Bit(rCIGCTRL, 31);
	for(i=0; i<0x100000; i++);
	Macro_Clear_Bit(rCIGCTRL, 31);

	rIICLC = (1<<2)+(3<<0);

	CAM_IIC_WR_STOP();
	CAM_IIC_Wait_STOP();	
	CAM_IIC_Pending_Clear();	
}

void CAM_Sensor_Enable(int en)
{
	volatile int i;

	if(en)
	{
		Macro_Clear_Bit(rGPDDAT, 4);  
		Macro_Clear_Bit(rCIGCTRL, 30);
		for(i=0; i<0x100000; i++);
		Macro_Set_Bit(rGPDDAT, 4);
		for(i=0; i<0x100000; i++);
		Macro_Set_Bit(rCIGCTRL, 30);
	}
	else
	{
		Macro_Clear_Bit(rCIGCTRL, 30);
		for(i=0; i<0x100000; i++);
		Macro_Clear_Bit(rGPDDAT, 4);
	}
}

void CAM_Calculate_Prescaler_Ratio_Shift(U32 SrcSize, U32 DstSize, U32 *ratio,U32 *shift)
{
	if(SrcSize >= (64*DstSize))
	{
		while(1);
	}
	else if(SrcSize >= (32*DstSize))
	{
		*ratio = 32;
		*shift = 5;
	}
	else if(SrcSize >= (16*DstSize))
	{
		*ratio = 16;
		*shift = 4;
	}
	else if(SrcSize >= (8*DstSize))
	{
		*ratio = 8;
		*shift = 3;
	}
	else if(SrcSize >= (4*DstSize))
	{
		*ratio = 4;
		*shift = 2;
	}
	else if(SrcSize >= (2*DstSize))
	{
		*ratio = 2;
		*shift = 1;
	}
	else
	{
		*ratio = 1;
		*shift = 0;
	}    	
}

static int Capture_status = 0;	// 0: Ready, 1: During Capture, 2: Capture Finished 
char * Cam_Frame_Buf;
int Width = 640, Height = 480;
int LineSize, FrameSize;

void CAM_Init(void)
{
	U32 pre_hor_ratio, h_shift, main_hor_ration;
	U32 pre_ver_ratio, v_shitf, main_ver_ration;
	
	CAM_Calculate_Prescaler_Ratio_Shift(Width, Width, &pre_hor_ratio, &h_shift);
	CAM_Calculate_Prescaler_Ratio_Shift(Width, Width, &pre_ver_ratio, &v_shitf);

	main_hor_ration=(Width<<8)/(Width<<h_shift);
	main_ver_ration=(Height<<8)/(Height<<v_shitf);
	
	rCISRCFMT = ((U32)1<<31) + (0<<30) + (Width<<16) + (Height<<0);	
	rCIWDOFST |= (1<<30)|(0xf<<12);
	rCIWDOFST = 0x00;
	rCIGCTRL |= (0<<27);
	rCICOTRGFMT = (Width<<16) + (Height<<0);	
	rCIPRTRGFMT = (Width<<16) + (Height<<0) + (3<<14);
	rCICOCTRL = (4<<19) + (4<<14) + (4<<9) + (4<<4);
	rCIPRCTRL = (4<<19) + (4<<14);
	rCICOSCPRERATIO = (1<<28)|(1<<16)|(1);
	rCIPRSCPRERATIO = (1<<28)|(1<<16)|(1);
	rCICOSCPREDST = ((Width/pre_hor_ratio)<<16) + ((Height/pre_ver_ratio)<<0);
	rCIPRSCPREDST = ((Width/pre_hor_ratio)<<16) + ((Height/pre_ver_ratio)<<0);
	rCICOSCCTRL = ((U32)0<<31) + (1<<29) + (1<<15) + (main_hor_ration<<16) + (main_ver_ration<<0); // ??
	rCIPRSCCTRL = ((U32)1<<31) + (0<<30) + (1<<28) + (main_hor_ration<<16) + (0<<15)+(main_ver_ration<<0); // ??
	rCICOTAREA = Width*Height;
	rCIPRTAREA = Width*Height;
}

void CAM_Camera_Init(void)
{
	CAM_Port_Init();
	CAM_Sensor_Enable(0);	
	CAM_If_Reset();	
	CAM_Init();
}

static const int CAM_MODULE_INIT[ ][2] =
{
	//Page0
	{0x01, 0x01}, 
	{0x01, 0x03}, 
	{0x01, 0x01}, 
	{0x00, 0x00}, 
	{0x02, 0x00}, 
	{0x03, 0x00}, 
	{0x01, 0x01}, 
	{0x03, 0x00}, 
	{0x10, 0x10}, 
	{0x11, 0x80},  // 0x80(Normal) 0x82(Y Flip), 0x83(XY Flip)
	{0x12, 0x0C}, 
	{0x13, 0x00}, 
	{0x14, 0x01}, 
	{0x15, 0x00}, 
	{0x16, 0x05}, 
	{0x17, 0x01}, 
	{0x18, 0xE0}, 
	{0x19, 0x02}, 
	{0x1A, 0x80}, 
	{0x1B, 0x00}, 
	{0x1C, 0x6C}, 
	{0x1D, 0x00}, 
	{0x1E, 0x14}, 
	{0x1F, 0x09}, 
	{0x20, 0xA8}, 
	{0x21, 0x01}, 
	{0x22, 0x08}, 
	{0x23, 0x08}, 
	{0x24, 0x08}, 
	{0x40, 0x33}, 
	{0x41, 0x77}, 
	{0x42, 0x53}, 
	{0x43, 0xB0}, 
	{0x44, 0x10}, 
	{0x45, 0x00}, 
	{0x46, 0x00}, 
	{0x47, 0x8F}, 
	{0x48, 0x4A}, 
	{0x49, 0x00}, 
	{0x4A, 0x03}, 
	{0x85, 0x50}, 
	{0x91, 0x70}, 
	{0x92, 0x72}, 

	//Page1
	{0x03, 0x01}, 
	{0x10, 0x07}, 
	{0x11, 0x03}, 
	{0x12, 0x30}, 
	{0x13, 0x01}, 
	{0x14, 0x80}, 
	{0x19, 0x80}, 
	{0x1a, 0x08}, 
	{0x20, 0x2f}, 
	{0x21, 0x80}, 
	{0x22, 0x80}, 
	{0x30, 0x0f}, 
	{0x33, 0x38}, 
	{0x35, 0x20}, 
	{0x36, 0x2f}, 
	{0x38, 0x6f}, 
	{0x39, 0x2f}, 
	{0x3a, 0x01}, 
	{0x3b, 0x1a}, 
	{0x3c, 0x75}, 
	{0x3d, 0x1a}, 
	{0x3e, 0x0e}, 
	{0x3f, 0x38}, 
	{0x40, 0x87}, 
	{0x41, 0x82}, 
	{0x42, 0x09}, 
	{0x43, 0x85}, 
	{0x44, 0x04}, 
	{0x45, 0x85}, 
	{0x46, 0x01}, 
	{0x47, 0x89}, 
	{0x48, 0x89}, 
	{0x49, 0x12}, 
	{0x60, 0x01}, 
	{0x61, 0x00}, 
	{0x62, 0x0f}, 
	{0x63, 0x1c}, 
	{0x64, 0x30}, 
	{0x65, 0x4f}, 
	{0x66, 0x69}, 
	{0x67, 0x80}, 
	{0x68, 0x93}, 
	{0x69, 0xa5}, 
	{0x6a, 0xb5}, 
	{0x6b, 0xc3}, 
	{0x6c, 0xd9}, 
	{0x6d, 0xea}, 
	{0x6e, 0xf6}, 
	{0x6f, 0xff}, 
	{0x90, 0x80}, 
	{0x91, 0x9d}, 
	{0x93, 0x73}, 
	{0x94, 0x10}, 
	{0x96, 0x08}, 
	{0xa0, 0x1a}, 
	{0xa1, 0x00}, 
	{0xa2, 0x13}, 
	{0xa3, 0x28}, 
	{0xa4, 0x10}, 
	{0xa5, 0x20}, 
	{0xb0, 0x22}, 
	{0xb1, 0x11}, 
	{0xc0, 0xc3}, 
	{0xc2, 0x44}, 
	{0xc4, 0x30}, 
	{0xc5, 0x10}, 
	{0xc6, 0x08}, 
	{0xc7, 0x7f}, 
	{0xd0, 0x07}, 
	{0xd1, 0x80}, 
	{0xd2, 0x80}, 
	{0xd3, 0xa8}, 
	{0xd4, 0x88}, 
	{0xd5, 0x90}, 
	{0xd6, 0x50}, 
	{0xd7, 0x40}, 
	{0xe0, 0x01}, 

	//Page2
	{0x03, 0x02}, 
	{0x10, 0x00}, 
	{0x11, 0x01}, 
	{0x12, 0x40}, 
	{0x13, 0x00}, 
	{0x14, 0xf0}, 
	{0x15, 0x00}, 
	{0x16, 0x00}, 
	{0x17, 0x00}, 
	{0x18, 0x00}, 
	{0x19, 0x01}, 
	{0x1a, 0x40}, 
	{0x1b, 0x00}, 
	{0x1c, 0xf0}, 
	{0x1d, 0x10}, 
	{0x1e, 0x00}, 
	{0x1f, 0x10}, 
	{0x20, 0x00}, 
	{0x21, 0x55}, 
	{0x51, 0x4e}, 
	{0x52, 0x41}, 
	{0x60, 0xef}, 

	//Page3
	{0x03, 0x03}, 
	{0x10, 0x1c}, 
	{0x11, 0x81}, 
	{0x12, 0x30}, 
	{0x13, 0xa5}, 
	{0x14, 0x30}, 
	{0x15, 0x57}, 
	{0x16, 0x0b}, 
	{0x19, 0x30}, 
	{0x1c, 0x43}, 
	{0x1d, 0x23}, 
	{0x1e, 0x20}, 
	{0x26, 0xd9}, 
	{0x33, 0x00}, 
	{0x34, 0xaf}, 
	{0x35, 0xc8}, 
	{0x36, 0x00}, 
	{0x37, 0x60}, 
	{0x38, 0x04}, 
	{0x39, 0x93}, 
	{0x3a, 0xe0}, 
	{0x3b, 0x3a}, 
	{0x3c, 0x98}, 
	{0x3d, 0x30}, 
	{0x3e, 0xd4}, 
	{0x50, 0x16}, 
	{0x51, 0x10}, 
	{0x52, 0x80}, 
	{0x53, 0x13}, 
	{0x54, 0x13}, 
	{0x55, 0x28}, 
	{0x56, 0x30}, 
	{0x57, 0x70}, 
	{0x58, 0x60}, 
	{0x59, 0x23}, 
	{0x5a, 0x1d}, 
	{0x5b, 0x1a}, 
	{0x5c, 0x17}, 
	{0x5d, 0x14}, 
	{0x5e, 0x18}, 
	{0x10, 0x9c}, 

	////Page4
	{0x03, 0x04}, 
	{0x10, 0xe9}, 
	{0x11, 0x2a}, 
	{0x12, 0x08}, 
	{0x19, 0x55}, 
	{0x21, 0x66}, 
	{0x23, 0xc3}, 
	{0x24, 0x88}, 
	{0x25, 0x66}, 
	{0x2c, 0x01}, 
	{0x40, 0x3e}, 
	{0x41, 0x20}, 
	{0x42, 0x35}, 
	{0x43, 0x50}, 
	{0x44, 0x18}, 
	{0x45, 0x50}, 
	{0x46, 0x22}, 
	{0x47, 0x50}, 
	{0x48, 0x40}, 
	{0x49, 0x28}, 
	{0x4a, 0x18}, 
	{0x4b, 0x02}, 
	{0x4c, 0x02}, 
	{0x4f, 0x50}, 
	{0x50, 0x78}, 
	{0x52, 0x44}, 
	{0x53, 0x34}, 
	{0x54, 0x20}, 
	{0x55, 0x18}, 
	{0x5b, 0x88}, 
	{0x60, 0x80}, 
	{0x61, 0x80}, 

	//Page0
	{0x03, 0x00}, 
	{0x01, 0x00}, 
};

void CAM_Module_Init(void)
{
	int i;
	
	for(i=0; i<(sizeof(CAM_MODULE_INIT)/sizeof(CAM_MODULE_INIT[0])); i++)
	{
		CAM_IIC_Write(CAM_MODULE_INIT[i][0], CAM_MODULE_INIT[i][1]);
	}
}

static unsigned int Cam_Buf[5];
const static unsigned int Cam_Frame_Number[4] = {2,3,0,1};

int CAM_Capture_Run(U32 run, int width, int height)
{
	if(run)
	{
		CAM_Sensor_Enable(1);
		CAM_Module_Init();		
		
		Width = width; Height=height;
		
		if((width ==640 && height && 480)||(width ==320 && height && 240)||(width ==160 && height && 120))
		{
			CAM_IIC_Write(0x03, 0x00);
			if(width == 640) CAM_IIC_Write(0x10, 0x00);
			else if(width == 320) CAM_IIC_Write(0x10, 0x10);
			else if(width == 160) CAM_IIC_Write(0x10, 0x20);
			CAM_Init();			
		}

		else
		{
			return 0;
		}
		
		LineSize = width*sizeof(unsigned short);
		FrameSize = LineSize*height;

		rCIPRCLRSA1 = Cam_Buf[0] = (U32)Cam_Frame_Buf;
		rCIPRCLRSA2 = Cam_Buf[1] = (U32)(Cam_Frame_Buf+FrameSize);
		rCIPRCLRSA3 = Cam_Buf[2] = (U32)(Cam_Frame_Buf+FrameSize*2);
		rCIPRCLRSA4 = Cam_Buf[3] = (U32)(Cam_Frame_Buf+FrameSize*3);
		Cam_Buf[4] = (U32)(Cam_Frame_Buf+FrameSize*4);
		
		Macro_Set_Bit(rSUBSRCPND, 12);
		
		rCIPRSCCTRL |= (1<<15);
		rCIIMGCPT |= ((unsigned int)1<<31)+(0<<30)+(1<<29);
		
		rCIWDOFST |= (1<<30)|(0xf<<12);   
		rCIWDOFST &= ~((1<<30)|(0xf<<12));	
		
		Capture_status = 1;
	}
	
	else
	{
		rCIPRSCCTRL &= ~(1<<15);
		rCIIMGCPT = 0;
		CAM_Sensor_Enable(0);
		Capture_status = 0;
	}
	
	return 1;
}

void CAM_XY_Flip(int x, int y)
{
	int val;
	
	CAM_IIC_Write(0x03, 0x00);
	val = CAM_IIC_Read(0x11);
	
	if(x) Macro_Set_Bit(val, 0);
	else Macro_Clear_Bit(val, 0);
	
	if(y)Macro_Set_Bit(val, 1);
	else Macro_Clear_Bit(val, 1);

	CAM_IIC_Write(0x11, val);
}	

void * CAM_Get_Image_Address(void)
{
	int frameCnt;
	unsigned int temp1, temp2;
	
	frameCnt = (rCIPRSTATUS>>26)&0x03;
	
	temp1 = Cam_Buf[Cam_Frame_Number[frameCnt]];
	temp2 = Cam_Buf[4];
	Cam_Buf[4] = temp1;
	Cam_Buf[Cam_Frame_Number[frameCnt]] = temp2;

	switch(frameCnt)
	{
		case 0: rCIPRCLRSA3 = temp2; break;
		case 1: rCIPRCLRSA4 = temp2; break;
		case 2: rCIPRCLRSA1 = temp2; break;
		case 3: rCIPRCLRSA2 = temp2; break;
	}

	return (void *)temp1;
}

void * CAM_Check_Image_Ready(void)
{
	int frameCnt;
	unsigned int temp1, temp2;

	if(Macro_Check_Bit_Set(rSUBSRCPND, SUB_CAM_P))
	{
		Macro_Set_Bit(rSUBSRCPND, SUB_CAM_P);	
	
		rCIWDOFST |= (1<<30)|(0xf<<12);   
		rCIWDOFST &= ~((1<<30)|(0xf<<12));

		frameCnt = (rCIPRSTATUS>>26)&0x03;
		
		temp1 = Cam_Buf[Cam_Frame_Number[frameCnt]];
		temp2 = Cam_Buf[4];
		Cam_Buf[4] = temp1;
		Cam_Buf[Cam_Frame_Number[frameCnt]] = temp2;
	
		switch(frameCnt)
		{
			case 0: rCIPRCLRSA3 = temp2; break;
			case 1: rCIPRCLRSA4 = temp2; break;
			case 2: rCIPRCLRSA1 = temp2; break;
			case 3: rCIPRCLRSA2 = temp2; break;
		}

		return (void *)temp1;
	}
	else
	{
		return (void *)0;
	}
}

void CAM_Interrupt_Enable(int en)
{
	if(en)
	{
		rSUBSRCPND = 1<<12;
		rSRCPND = 1<<6;
		rINTPND = 1<<6;

		Macro_Clear_Bit(rINTSUBMSK, 12);
		Macro_Clear_Bit(rINTMSK, 6);
	}
	else
	{
		rINTSUBMSK |= (BIT_SUB_CAM_P); 
		rINTMSK |= (BIT_CAM);
	}
}

int CAM_Get_Capture_Status(void)
{
	// 0: ready, 1: during capture, 2: capture finished
	// 함수가 호출되어 2가 리턴되면 0상태로 돌아감

	int r = Capture_status;
	
	if(Capture_status == 2)
	{
		Capture_status = 0;
	}

	return r;
}

void CAM_Set_Capture_Status(int state)
{
	Capture_status = state;
}

void CAM_Capture_Pause(void)
{
	rCIPRSTATUS &= ~(1<<21);
	rCIPRSCCTRL &= ~(1<<15);
	rCIIMGCPT = 0;

	rCIWDOFST |= (1<<30)|(0xf<<12);   
	rCIWDOFST &= ~((1<<30)|(0xf<<12));
}

void CAM_Capture_Restart(void)
{
	rCIPRSTATUS |= (1<<21);
	rCIPRSCCTRL |= (1<<15);
	rCIIMGCPT |= ((unsigned int)1<<31)+(0<<30)+(1<<29);
}

void CAM_Set_Auto_White_Balance(int en)
{
	int val = 0;
	
	CAM_IIC_Write(0x03, 0x04);
	val = CAM_IIC_Read(0x10);
	
	if(en) Macro_Set_Bit(val, 7);
	else Macro_Clear_Bit(val, 7);
	
	CAM_IIC_Write(0x10, val);
}

void CAM_Set_Auto_Saturation(int en)
{
	int val = 0;
	
	CAM_IIC_Write(0x03, 0x01);
	val = CAM_IIC_Read(0x20);
	
	if(en) Macro_Set_Bit(val, 1);
	else Macro_Clear_Bit(val, 1);
	
	CAM_IIC_Write(0x20, val);
}

void CAM_Set_Auto_Exposure(int en )
{
	int val = 0;
	
	CAM_IIC_Write(0x03, 0x03);
	val = CAM_IIC_Read(0x10);
	
	if(en) Macro_Set_Bit(val, 7);
	else Macro_Clear_Bit(val, 7);
	
	Macro_Set_Bit(val, 6);
	CAM_IIC_Write(0x10, val);
}

int CAM_Set_Exposure(int ms)
{
	int val = ms*1000000/83.3333/8;
	
	CAM_IIC_Write(0x03, 0x03);
	CAM_IIC_Write(0x33, (val&0xff0000)>>16);
	CAM_IIC_Write(0x34, (val&0x00ff00)>>8);
	CAM_IIC_Write(0x35, 0x0000ff);
	
	return CAM_Get_Exposure();

}

int CAM_Get_Exposure(void)
{
	int val = 0;
	
	CAM_IIC_Write(0x03, 0x03);
	val = CAM_IIC_Read(0x33)<<16;
	val |= CAM_IIC_Read(0x34)<<8;
	val |= CAM_IIC_Read(0x35);
	
	return val*8*83.3333/1000000;
}

int CAM_Set_Gain(int val)
{
	CAM_IIC_Write(0x03, 0x03);
	CAM_IIC_Write(0x50, val&0xff);
	
	return CAM_Get_Gain();
}

int CAM_Get_Gain(void)
{
	CAM_IIC_Write(0x03, 0x03);
	
	return CAM_IIC_Read(0x50);
}

int CAM_Increase_Exposure(void)
{
	int val = CAM_Get_Exposure();
	
	if(val == val*11/10) val++;
	else val = val*11/10;
	
	return CAM_Set_Exposure(val);
}

int CAM_Decrease_Exposure(void)
{
	int val = CAM_Get_Exposure();
	
	if(val == val*9/10)	val--;
	else val = val*9/10;
	
	if(val<0) val = 0;

	return CAM_Set_Exposure(val);
}

int CAM_Increase_Gain(void)
{
	int val = CAM_Get_Gain();
	
	if(val == val*11/10) val++;
	else val = val*11/10;
	
	return CAM_Set_Gain(val);
}

int CAM_Decrease_Gain()
{
	int val = CAM_Get_Gain();

	if(val == val*9/10) val--;
	else val = val*9/10;

	if(val<0) val = 0;

	return CAM_Set_Gain(val);
}

void CAM_Set_Auto_Brightness(int en)
{
	int val = 0;
	
	CAM_IIC_Write(0x03, 0x01);
	val = CAM_IIC_Read(0x12);
	
	if(en) Macro_Set_Bit(val, 5);
	else Macro_Clear_Bit(val, 5);
	CAM_IIC_Write(0x12, val);
}

int CAM_Set_Brightness(int val)
{
	CAM_IIC_Write(0x03, 0x01);
	CAM_IIC_Write(0x19, val&0xff);
	
	return CAM_Get_Brightness();
}

int CAM_Get_Brightness(void)
{
	CAM_IIC_Write(0x03, 0x01);
	
	return CAM_IIC_Read(0x19);
}

int CAM_Increase_Brightness(void)
{	
	int val = CAM_Get_Brightness();
	
	if(val == val*11/10) val++;
	else val = val*11/10;
	
	return CAM_Set_Brightness(val);
}

int CAM_Decrease_Brightness(void)
{
	int val = CAM_Get_Brightness();

	if(val == val*9/10) val--;
	else val = val*9/10;

	if(val<0) val = 0;

	return CAM_Set_Brightness(val);
}

void CAM_Set_Effect(int effect)
{
	int val;
	
	CAM_IIC_Write(0x03, 0x01);

	val = CAM_IIC_Read(0x12);
	Macro_Clear_Bit(val, 0);
	Macro_Clear_Bit(val, 1);
	CAM_IIC_Write(0x12, val);
	CAM_IIC_Write(0x1d, 0x80);
	CAM_IIC_Write(0x1e, 0x80);
	
	val = CAM_IIC_Read(0x11);
	Macro_Clear_Bit(val, 4);
	Macro_Clear_Bit(val, 5);
	CAM_IIC_Write(0x11, val);

	val = CAM_IIC_Read(0x12);
	Macro_Clear_Bit(val, 3);
	CAM_IIC_Write(0x12, val);

	switch(effect)
	{
		case CAM_EFFECT_MONO:
		{
			val = CAM_IIC_Read(0x12);
			Macro_Set_Bit(val, 0);
			Macro_Set_Bit(val, 1);
			CAM_IIC_Write(0x12, val);
			CAM_IIC_Write(0x1d, 0x80);
			CAM_IIC_Write(0x1e, 0x80);
			break;
		}
	
		case CAM_EFFECT_SEPIA:
		{
			val = CAM_IIC_Read(0x12);
			Macro_Set_Bit(val, 0);
			Macro_Set_Bit(val, 1);
			CAM_IIC_Write(0x12, val);
			CAM_IIC_Write(0x1d, 50);
			CAM_IIC_Write(0x1e, 176);
			break;
		}
	
		case CAM_EFFECT_GREEN:
		{
			val = CAM_IIC_Read(0x12);
			Macro_Set_Bit(val, 0);
			Macro_Set_Bit(val, 1);
			CAM_IIC_Write(0x12, val);
			CAM_IIC_Write(0x1d, 53);
			CAM_IIC_Write(0x1e, 103);
			break;
		}
	
		case CAM_EFFECT_AQUA:
		{
			val = CAM_IIC_Read(0x12);
			Macro_Set_Bit(val, 0);
			Macro_Set_Bit(val, 1);
			CAM_IIC_Write(0x12, val);
			CAM_IIC_Write(0x1d, 0xC0);
			CAM_IIC_Write(0x1e, 0x80);
			break;
		}
	
		case CAM_EFFECT_EMBOSSING:
		{
			val = CAM_IIC_Read(0x11);
			Macro_Set_Bit(val, 5);
			CAM_IIC_Write(0x11, val);
			break;					
		}

		case CAM_EFFECT_SKETCH:
		{
			val = CAM_IIC_Read(0x11);
			Macro_Set_Bit(val, 4);
			CAM_IIC_Write(0x11, val);
			break;					
		}

		case CAM_EFFECT_NEGATIVE:
		{
			val = CAM_IIC_Read(0x12);
			Macro_Set_Bit(val, 3);
			CAM_IIC_Write(0x12, val);
			break;					
		}
	}
}

void CAM_Set_Auto_Framerate(int en)
{
	int val;

	CAM_IIC_Write(0x03, 0x00);
	val = CAM_IIC_Read(0x11);

	if(en) Macro_Clear_Bit(val, 2);
	else Macro_Set_Bit(val, 2);
	CAM_IIC_Write(0x11, val);
}

