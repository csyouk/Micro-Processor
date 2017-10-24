#include "2440addr.h"
#include "macro.h"
#include "device_driver.h"
#include "option.h"

void Timer3_Buzzer_Init(void)
{
	// Timer3 주파수 설정
	
	// 낮은"도" => 130Hz, 높은"시" => 987Hz (3옥타브 지원)
	// 65535 tick 범위에서 이 주파수들이 설정이 가능해야 한다
	// Duty 50%로 이 음을 내려면 987Hz가 최소 2펄스(CNT의 절반이 CMP)로 생성 가능해야 한다
	// 따라서 높은 주파수는 최소 987 * 2 = 1,974Hz 이어야 한다
	// 주파수 범위는 결국 65536펄스~2펄스를 이용하여 130Hz ~ 1,974Hz를 만족해야 한다

	// PCLK/2 => TICK 25MHz로 1974Hz를 만들려면 TCNT3 = 12664펄스
	// PCLK/2 => TICK 25MHz로 130Hz를 만들려면 TCNT3 = 192307펄스 (설정 불가)
	
	// PCLK/4 => TICK 12.5MHz로 1974Hz를 만들려면 TCNT3 = 6332펄스
	// PCLK/4 => TICK 12.5MHz로 130Hz를 만들려면 TCNT3 = 95153펄스 (설정 불가)

	// PCLK/8 => TICK 6.25MHz로 1974Hz를 만들려면 TCNT3 = 3166펄스
	// PCLK/8 => TICK 6.25MHz로 130Hz를 만들려면 TCNT3 = 48076펄스

	// 주파수가 더 낮아지면 미세한 주파수 발생이 어려우므로 3옥타브 음을 위해 1/8분주가 적합하다
	// PRESCALE = 0, MUX = 1/8
	
	// GPB3 => TOUT3로 설정

}

#define BASE 				500 		 // 음길이 base값(단위 msec)
#define TIMER3_TICK		(PCLK/8)  // Timer3의 Tick 주파수

// 음계 주파수(3옥타브)와 음길이(BASE가 1/4음표 길이)
const static unsigned int Tone_value[] = {261,277,293,311,329,349,369,391,415,440,466,493,523,554,587,622,659,698,739,783,830,880,932,987};
const static unsigned int Note_delay[] = {BASE/4, BASE/2, BASE, BASE*2, BASE*4};

void Timer3_Buzzer_Beep(int tone, int duration)
{
	// tone은 주파수 배열의 번호
	// 해당 주파수에 맞는 펄스 값을 TCNTB3에 설정
	// DUTY는 50%가 되도록 TCMPB3 설정



	// Auto Reload, Invert, Maual update, TIMER OFF

	
	// Auto Reload, Invert, Maual update, TIMER ON


	// duration은 음길이 => 배열값 만큼 시간 지연 (msec)
	Timer0_Delay(Note_delay[duration]);

	// Timer OFF 
	Macro_Clear_Bit(rTCON, 16);
}
