typedef unsigned int 		U32;
typedef signed int			S32;
typedef unsigned short		U16;
typedef short int			S16;
typedef unsigned char		U8;
typedef signed char 		S8;
typedef unsigned long long 	ULL;

extern void Run_Time_Config(void);

// Heap.c
extern void * Get_Heap_Limit(void);
extern void * Get_Heap_Base(void);
extern unsigned int Get_Heap_Size(void);

// Led.c
extern void Led_Init(void);
extern void Led_Display(int disp);

// Uart.c
extern void Uart_Init(int baud);
extern void Uart_Printf(char *fmt,...);
extern char Uart_Get_Char(void);
extern char Uart_Get_Pressed(void);
extern int Uart_GetIntNum(void);
extern void Uart_GetString(char *string);

// Key.c
extern void Key_Poll_Init(void);
extern int Key_Get_Pressed(void);
extern int Key_Get_Pressed_Multi(void);
extern void Key_Wait_Key_Released(void);
extern int Key_Wait_Key_Pressed(void);
extern void Key_Wait_Key_Released_Multi(void);
extern int Key_Wait_Key_Pressed_Multi(void);

// Timer.c
extern void Timer0_Stop_Watch_Run(void);
extern unsigned int Timer0_Stop_Watch_Stop(void);
extern int Timer0_Stop_Watch_Run_Unlimited(unsigned int max);
extern unsigned int Timer0_Stop_Watch_Stop_Unlimited(void);
extern void Timer0_Delay(int time);
extern void Timer0_Repeat(int time);
extern int Timer0_Check_Timeout(void);
extern void Timer0_Stop(void);
extern void Timer0_Set_Time(int time);

// Buzzer.c
extern void Timer3_Buzzer_Beep(int tone, int duration);
extern void Timer3_Buzzer_Init(void);

