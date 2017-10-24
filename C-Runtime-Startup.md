# C-Runtime-Startup
- Hosted 환경 : os가 탑재되어서 사용자 프로그램이 구동됨.(최상의 서비스를 제공받는다.)
- Free-standing 환경 : 프로그래머가 임의의 방식으로 사용자 프로그램을 구동한다.


- Free-standing 환경에서 CRT 코드의 설계 방법
  - XIP방식 : eXecute In Place
    - ROM 혹은 NOR flash에 프로그램을 구워놓은 후, 부팅시 버스를 통해서 실행파일의 코드들이 바로 cpu로 흘러들어감.
  - RAM Loading방식
    - 응용프로그램이 RAM으로 옮겨진 후 실행됨.
