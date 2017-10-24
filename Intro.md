# 마이크로 프로세서

## 폰 노이만 구조
- 가장 큰 특징은
  - **메모리에 저장된 프로그램을 순차적으로 읽어서 실행시킨다.** 라는 점이다.
  - 메모리는 OP-CODE와 Operand가 조합되어 있다.
  - 폰 노이만 구조에서는 이 메모리에 들어가 있는 명령어를 해독(Decode)해서
  - OP-CODE와 Operand로 분리한 다음.
  - ALU에 넣어서, 연산을 한다.
### ALU
- Shifter, Rotator
- Multiplier, Divider
  - 9 * 18 = 9 << 4 + 9 << 1

### 메모리 인터페이스

### write 동작
- cpu에서
- 주소를 선택해서 레지스터에 넣음.
- 데이터를 준비해서 레지스터에 밀어 넣음.
- 칩 선택
- write신호 rising edge trigger를 인가.
- 레지스터의 데이터가 메모리로 들어감.
- 칩 비활성화.
- setup time + hold time = wait time

### read 동작
- 메모리에서 데이터가 준비됨.
- 칩 선택
- read 신호 rising edge trigger
- access time

- wait generator
- 각 주변장치 별로 access time을 얼마나 걸리는지 관리하고 있다.
-

### CPU의 싸이클
- Fetch / Decode / Execute

### Fetch
- 명령어를 가지고 오는 행위.
- 처음으로 access하는 명령어가 저장된 약속된 주소를 **Reset Vector(0x0)** 라 한다.
- cpu는 명령어를 Fetch하기 위해 명령어의 주소를 program counter(레지스터)에 저장한다.
  - 이 레지스터에는 주소를 저장하고 있다.
- 컴퓨터에 전원을 키는 순간 PC는 Reset vector값을 가지고 있다.
- 컴퓨터는 PC가 가리키고 있는 메모리를 fetch한다.

### CPU가 명령어를 Fetch를 수행하는 경우의 동작
- cpu는 fetch 싸이클과 execution 싸이클을 명확히 구분하여 메모리를 액세스 한다.
- fetch cycle이면 주소 레지스터에 PC의 값이 저장된다.
- fetch가 수행되면 주소 레지스터에 지정된 주소의 데이터가 데이터 레지스터로 들어온다.
- 데이터 레지스터에 들어온 내용은 명령어 이므로 명령어 해독 회로로 연결된다.
- 명령어 해독 회로는 명령 해독 결과에 따른 명령을 수행(execute)한다.
- 이후 CPU는 execution cycle을 수행한다.
- Execution이 종료되면, CPU는 다시 fetch cycle로 들어가게 되고 새로운 명령을 fetch한다.

### CISC VS RISC
- CISC와 RISC는 명령어의 길이의 차이가 있다.
  - CISC는 명령어별로 길이가 다를 수 있다.
  - RISC는 명령어의 길이가 전부 동일하다.
- CPU의 아키텍처는 PC가 값을 증가시킬 때, 얼마만큼의 값을 증가시킬지를 결정한다.


### Operand access
- 읽어온 명령에 의해 메모리에서 데이터를 Load/Store

**PC 값의 변화**  
- pc값은 전원 인가시 reset vector가 된다.
- cpu가 고정길이 명령인 경우 pc는 항상 고정값이 더해진다.(RISC)
- cpu가 가변길이 명령인 경우 pc는 fetch된 명령어의 길이에 따라 가변적으로 더해진다.(CISC)
- 명령이 분기나 함수 호출인 경우 execution 과정에서 pc값이 분기 위치로 변경된다.

### Addressing Mode
- CPU가 연산에 사용할 operand를 액세스하는 방법.
- Register Value
- Immediate Value
- Register Indirect
- Memory Direct

### Pipeline
- cpu가 실행시킬 명령어를 미리 가져다 놓음.
- 파이프라인의 성능을 저하시키는 코드
 - 분기 코드
 - execute단계에서 외부 메모리를 사용하는 경우.



--------------------------------
## 하버드 아키텍처
- 명령어와 데이터가 지나다니는 path를 따로 만들어 놓은 구조.
- 외부 버스는 폰노이만 구조와 동일하다.
- 프로세서 내부에 하버드 아키텍처의 다름이 나타난다.
- 프로세서 내부에 데이터용 cache와 명령어용 cache를 따로 만들어서 따로 각기 처리하게 만든다.


---------------------------


|   | CISC  | RISC  |
|:-:|:-:|:-:|
| 성능  |  더 좋음 |   |
|  프로그램 사이즈 |   | 더 큼  |
|  주 용도 | 거치형  | 핸드헬드  |

---------------------------

## Coprocessor

---------------------

## 프로세서 3단계 구조
- Core
  - ALU + Register + Control Unit
- Coprocessor
  - Core + FPU/Cache/MMU...
- SoC(System On A Chip)
  - Processors + Peripherals
