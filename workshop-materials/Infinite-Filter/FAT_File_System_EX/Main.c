#include "2440addr.h"
#include "device_driver.h"
#include "macro.h"

#include <stdlib.h>

#define 	printf		Uart_Printf
#define 	main		User_Main

void User_Main(void);

void Main(void)
{
	Nand_Init();
	Uart_Init(115200);
	Lcd_Graphic_Init();
	Key_ISR_Init();
	Key_ISR_Enable(1);

	printf("\n=================================\n");
	main();
	printf("=================================\n");
}

/*===== << 여기부터 사용자 프로그램을 작성함 >> =====*/

/***********************************************************/
// [9-1] : SD Access
/***********************************************************/

#if 1

void main(void)
{
	int i,j,k;
	int buf[512/4];

	printf("SD Card Access Test\n");
	if(SD_Check_Card() != SD_SUCCESS) printf("SD Card를 삽입해 주십시오\n");

	for(;;)
	{
		if(SD_Check_Card() == SD_SUCCESS) break;
	}

	printf("SD_Check_Card() Passed!\n"); // TG

	i=SD_Init();
	printf("SD_Init() Passed!\n"); // TG
	if(i != SD_SUCCESS) printf("Init Err : [%d]\n", i);

	Key_Poll_Init();

	for(k=0;k<2;k++)
	{
		printf("아무 KEY를 누르면 시작합니다\n");

		Key_Wait_Key_Pressed();
		Key_Wait_Key_Released();

		SD_Read_Sector(k, 1, (U8 *)buf);
		printf("\n[%d]번 Sector Read!\n", k);

		for(i=0; i<(128/8); i++)
		{
			printf("[%2d] ", i);
			for(j=0; j<8; j++)
			{
				printf("%.8X ", buf[i*8+j]);
			}
			printf("\n\r");
		}
	}
}

#endif

/***********************************************************/
// [9-2] : FAT16 분석
/***********************************************************/

#if 0

void main(void)
{
	int i,j,k,r;
	int sector, temp, fat_start, root_start, size = 0, cluster = 0, sector_per_cluster, size_of_root;

	int buf[512/4];
	unsigned char * p = (unsigned char *)buf;

	if(SD_Check_Card() != SD_SUCCESS) printf("Insert SD Card Please!\n");

	for(;;)
	{
		if(SD_Check_Card() == SD_SUCCESS) break;
	}

	i=SD_Init();
	if(i != SD_SUCCESS) printf("Init Err : [%d]\n", i);

	printf("==================================================\n");
	printf("Display MBR(#0 block) => Any Key                  \n");
	printf("==================================================\n");
	printf(">> ");

	r = Uart_Get_Char();
	Uart_Get_Pressed();
	printf("%c\n", r);

	SD_Read_Sector(0, 1, (U8 *)buf);
	printf("[%d]  Sector Read!\n", 0);

	for(i=0; i<(128/8); i++)
	{
		printf("[%3d]", i*32);

		for(j=0; j<8; j++)
		{
			for(k=0; k<4; k++)
			{
				printf("%.2X", ((char *)buf)[i*32+j*4+k]);
			}
			printf(" ");
		}
		printf("\n");
	}

	sector = (p[457] << 24) + (p[456] << 16) + (p[455] << 8) + p[454];

	printf("\n1st Partition = %d Block\n", sector);

	printf("==================================================\n");
	printf("Display 1st partition [#%d] block => Any Key\n", sector);
	printf("==================================================\n");
	printf(">> ");

	r = Uart_Get_Char();
	Uart_Get_Pressed();
	printf("%c\n", r);

	SD_Read_Sector(sector, 1, (U8 *)buf);
	printf("[%d]  Sector Read!\n", sector);

	for(i=0; i<(128/8); i++)
	{
		printf("[%3d]", i*32);

		for(j=0; j<8; j++)
		{
			for(k=0; k<4; k++)
			{
				printf("%.2X", ((char *)buf)[i*32+j*4+k]);
			}
			printf(" ");
		}
		printf("\n");
	}

	// Sector per Cluseter
	sector_per_cluster = p[13];
	// reserved sector count
	sector += (p[15] << 8) + p[14];
	// fat sector start
	fat_start = sector;
	// Number of FATs
	temp = p[16];
	// sectors per FAT
	temp *= (p[23]<<8) + p[22];
	// Root directory sector
	sector += temp;
	// root directory start
	root_start = sector;
	// size of root
	size_of_root = ((p[18] << 8) + p[17])/16;

	printf("\nRoot Directory = %d Block\n", root_start);

	printf("==================================================\n");
	printf("Display ROOT Directory  [#%d] block => Any Key\n", root_start);
	printf("==================================================\n");
	printf(">> ");

	r = Uart_Get_Char();
	Uart_Get_Pressed();
	printf("%c\n", r);


	SD_Read_Sector(root_start, 1, (U8 *)buf);
	printf("[%d]  Sector Read!\n", root_start);

	for(i=0; i<(128/8); i++)
	{
		printf("[%3d]", i*32);

		for(j=0; j<8; j++)
		{
			for(k=0; k<4; k++)
			{
				printf("%.2X", ((char *)buf)[i*32+j*4+k]);
			}
			printf(" ");
		}
		printf("\n");
	}

	// 첫 파일의 분석
	temp = 0;

	for(;;)
	{
		if(p[temp] == 0xe5) temp += 32; // deleted
		else if(p[temp] == 0x00) break; // finish
		// attribute 분석
		else if(p[temp + 11] == 0x0f) temp += 32; // long file name, skip
		else if(p[temp + 11] == 0x10) temp += 32; // directory skip
		else if(0x20)	// archive file
		{
			printf("\nFirst file Name = ");

			size = (p[temp+31]<<24) + (p[temp+30]<<16) + (p[temp+29]<<8) + p[temp+28];
			cluster = (p[temp+27]<<8) + p[temp+26];

			for(i=0; i<8; i++)
			{
				if(p[temp+i] == 0x20) continue;
				printf("%c", p[temp+i]);
			}

			printf(".");

			for(i=8+0; i<(8+3); i++)
			{
				if(p[temp+i] == 0x20) continue;
				printf("%c", p[temp+i]);
			}

			break;
		}
	}

	printf("\n1st cluster number of 1st file => [%d], size => [%d]bytes\n", cluster, size);

	printf("==================================================\n");
	printf("Display FAT0 [#%d] block => Any Key\n", fat_start);
	printf("==================================================\n");
	printf(">> ");

	r = Uart_Get_Char();
	Uart_Get_Pressed();
	printf("%c\n", r);

	SD_Read_Sector(fat_start, 1, (U8 *)buf);
	printf("[%d]  Sector Read!\n", fat_start);

	for(i=0; i<(128/8); i++)
	{
		printf("[%3d]", i*32);

		for(j=0; j<8; j++)
		{
			for(k=0; k<4; k++)
			{
				printf("%.2X", ((char *)buf)[i*32+j*4+k]);
			}
			printf(" ");
		}
		printf("\n");
	}

	// 파일의 첫 클러스터 번호로 해당 파일의 데이터 영역 위치 계산
	sector = root_start + size_of_root + sector_per_cluster * (cluster - 2);

	// 다음 클러스터 위치 분석
	cluster = ((short *)p)[cluster];

	printf("\nNext Cluster Number => [%#4X]\n", (unsigned short)cluster);

	printf("==================================================\n");
	printf("Display 1st cluster of 1st file [#%d] => Any Key\n", sector);
	printf("==================================================\n");
	printf(">> ");

	r = Uart_Get_Char();
	Uart_Get_Pressed();
	printf("%c\n", r);

	SD_Read_Sector(sector, 1, (U8 *)buf);
	printf("\n[%d] Sector Read!\n", sector);

	for(i=0; i<(128/8); i++)
	{
		printf("[%3d]", i*32);

		for(j=0; j<8; j++)
		{
			for(k=0; k<4; k++)
			{
				printf("%.2X", ((char *)buf)[i*32+j*4+k]);
			}
			printf(" ");
		}
		printf("\n");
	}

	printf("\nThank You!!\n");
}

#endif

/***********************************************************/
// [Project_Example] : BMP Display
/***********************************************************/

#if 0

void main(void)
{
	int add;

	Lcd_Graphic_Init();
	Lcd_Clr_Screen(0xf800);

	printf("24bpp BMP Display\n");

	printf("Enter the address of BMP file saved. Default=0x31000000 => ");

	add = Uart_GetIntNum();

	if((add <= 0x30000000) || (add >= 0x32000000))
	{
		add = 0x31000000;
	}

	printf("%#x\n", add);
	Lcd_Draw_BMP_File_24bpp(0,0,(void *)add);
}

#endif
