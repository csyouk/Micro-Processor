# I2C-Protocol
- I2C 프로토콜은 IC소자간 저속 통신을 하기위해 발명된 직렬버스이다.
- 1:N 방식의 통신이 가능하다.
- 이 프로토콜을 구현하기 위해서는 SCLK, SDA 라인이 필요하다.
  - 클락(SCLK)은 마스터에서 제공한다.
  - SDA라인으로는 데이터가 전송 되는데, half-duplex방식이다.
    - half-duplex 방식의 의미는, 한 라인을 통해서 마스터가 슬레이브에게
    혹은 슬레이브가 마스터에게 데이터를 전송하는 것이 가능하나, 동시에 보내는 것은
    불가능하다. 만약, 동시에 데이터를 보내려고 했다면, burst가 난다.
    즉, 데이터 유실이 발생한다.

- I2C 프로토콜을 사용하려면 다음 설정을 해야 한다.
### I2C Protocol의 구성
- 1B 방식
- 2B 방식

### I2C Bus Configuration
###


### 레지스터 목록
- IICCON : Bus, Clock 사용 여부 결정.
- IICSTAT : Control Rx/Tx, Bus status check
- IICDS : 송수신 데이터 버퍼의 설정.
- IICADD : slave node address 설정시 필요.
