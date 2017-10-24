1. 1번 스위치 토글 -> LED 1번 토글
2. 2번 스위치 토글 -> 레지스터 송신 버퍼에 "A" 넣음.
  - Uart_Init을 먼저 한다.
  - Uart 설정에는 rGPHCON 레지스터를 사용한다. ( rx/tx 모드로 설정한다. )

```cpp
HW_Uart_Init(115200);
void HW_Uart_Init(int baud_rate){
	Macro_Write_Block(rGPHCON, 0xf, 0xa, 4);
	rUFCON0 = 0x0; (FIFO 설정 - disable)
	rUMCON0 = 0x0; flow control - 3
	rULCON0 = 0x3; 8 bit data frame으로 설정.
	rUCON0 = 0x245;
	rUBRDIV0 = ((unsigned int)(PCLK/16./baud_rate + 0.5) - 1);
}
```
3. teraterm에 b를 적으면 uart로 수신 받아서 led2번을 toggle하기
  - uart를 초기화 해야 한다.

4. led3를 1초마다 toggle
  - 타이머를 사용하기 위해서는
  - 먼저 기준펄스 설정이 필요하다.
  - 0번 타이머를 사용한다고 가정하고, 설정해야 하는 내용들은 다음과 같다.
  - rTCFG0 레지스터를 통해서 채널 0을 사용하고, prescaler값을 설정한다.
  - rSRCPND 레지스터는 역할이 매우 특이하다.
  이 레지스터는 하드웨어 event를 관리하는 레지스터인데, 
```cpp
// 목적은 다음과 같다.
50MHz -> 50kHz 로 주파수를 낮춰야 한다. 즉, 1/1000 로 분주해야 한다.
분주 계산식.
분주의 부분은 2가지로 이뤄져 있다.
[prescaler + Clock Divider(고정값)]
prescaler값은 계산을 통해서 값을 설정하는 것이 가능하고,
Clock Divider는 고정값이다.

clock divider를 1/8 짜리로 사용한다고 하면,
prescaler값은 1/125 짜리로 사용해야 한다.
prescaler값은 8bit값(0~255)값을 원래 가지므로, 원래 값으로 shift를 하면, 124값을 설정해야 한다.

5000000 / 8 / (x+1) = 50000
x = PCLK / 50000 / 4 - 1
// PCLK = 50MHz
Macro_Write_Block(rTCFG0, 0xff, (int)(PCLK/50000./4 + 0.5) - 1, 0);


// rTCFG1 레지스터를 통해서 분주기의 분주비를 1/4로 설정
Macro_Write_Block(rTCFG1, 0xf, 0x1, 0);

rTCNTB0 = time * 50; // time은 msec 단위이다.

타이머가 expired되서 이벤트가 발생하면,
rSRCPND = 1<<10;

```
