#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"
#include "option.h"

void Timer3_Buzzer_Init(void)
{
	// Timer3 ���ļ� ����
	
	// ����"��" => 130Hz, ����"��" => 987Hz (3��Ÿ�� ����)
	// 65535 tick �������� �� ���ļ����� ������ �����ؾ� �Ѵ�
	// Duty 50%�� �� ���� ������ 987Hz�� �ּ� 2�޽�(CNT�� ������ CMP)�� ���� �����ؾ� �Ѵ�
	// ���� ���� ���ļ��� �ּ� 987 * 2 = 1,974Hz �̾�� �Ѵ�
	// ���ļ� ������ �ᱹ 65536�޽�~2�޽��� �̿��Ͽ� 130Hz ~ 1,974Hz�� �����ؾ� �Ѵ�

	// PCLK/2 => TICK 25MHz�� 1974Hz�� ������� TCNT3 = 12664�޽�
	// PCLK/2 => TICK 25MHz�� 130Hz�� ������� TCNT3 = 192307�޽� (���� �Ұ�)
	
	// PCLK/4 => TICK 12.5MHz�� 1974Hz�� ������� TCNT3 = 6332�޽�
	// PCLK/4 => TICK 12.5MHz�� 130Hz�� ������� TCNT3 = 95153�޽� (���� �Ұ�)

	// PCLK/8 => TICK 6.25MHz�� 1974Hz�� ������� TCNT3 = 3166�޽�
	// PCLK/8 => TICK 6.25MHz�� 130Hz�� ������� TCNT3 = 48076�޽�

	// ���ļ��� �� �������� �̼��� ���ļ� �߻��� �����Ƿ� 3��Ÿ�� ���� ���� 1/8���ְ� �����ϴ�
	// PRESCALE = 0, MUX = 1/8
	
	// GPB3 => TOUT3�� ����

}

#define BASE 				500 		 // ������ base��(���� msec)
#define TIMER3_TICK		(PCLK/8)  // Timer3�� Tick ���ļ�

// ���� ���ļ�(3��Ÿ��)�� ������(BASE�� 1/4��ǥ ����)
const static unsigned int Tone_value[] = {261,277,293,311,329,349,369,391,415,440,466,493,523,554,587,622,659,698,739,783,830,880,932,987};
const static unsigned int Note_delay[] = {BASE/4, BASE/2, BASE, BASE*2, BASE*4};

void Timer3_Buzzer_Beep(int tone, int duration)
{
	// tone�� ���ļ� �迭�� ��ȣ
	// �ش� ���ļ��� �´� �޽� ���� TCNTB3�� ����
	// DUTY�� 50%�� �ǵ��� TCMPB3 ����



	// Auto Reload, Invert, Maual update, TIMER OFF

	
	// Auto Reload, Invert, Maual update, TIMER ON


	// duration�� ������ => �迭�� ��ŭ �ð� ���� (msec)
	Timer0_Delay(Note_delay[duration]);

	// Timer OFF 
	Macro_Clear_Bit(rTCON, 16);
}
