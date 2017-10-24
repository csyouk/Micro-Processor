# ARM Device Control

## macro list
```cpp
#define	Macro_Set_Bit(dest, position)					((dest) |=  (0x1u<<(position)))
- 특정 위치의 비트를 1로 set.
- ex) Macro_Set_Bit(rGPCCON, 4);

#define	Macro_Clear_Bit(dest, position)					((dest) &= ~(0x1u<<(position)))
- 특정 위치의 비트를 0으로 clear.
- ex) Macro_Clear_Bit(rGPCCON, 4);

#define	Macro_Invert_Bit(dest, position)				((dest) ^=  (0x1u<<(position)))
- 특정 위치의 비트를 toggle.
- ex) Macro_Invert_Bit(rGPCCON, 4);

#define	Macro_Clear_Area(dest, bits, position)			((dest) &= ~(((unsigned)(bits))<<(position)))
- 정한 영역 만큼의 비트를 0으로 clear.
- ex) Macro_Clear_Area(rGPFCON, 0xff, 3)
  - rGPFCON 레지스터의 3번 비트부터 8개 비트를 0으로 clear

#define	Macro_Set_Area(dest, bits, position)			((dest) |=  (((unsigned)(bits))<<(position)))
- 정한 영역 만큼의 비트를 1로 set.
- ex) Macro_Set_Area(rGPFCON, 0xff, 3)
  - rGPFCON 레지스터의 3번 비트부터 8개 비트를 1로 set.

#define	Macro_Invert_Area(dest, bits, position)			((dest) ^=  (((unsigned)(bits))<<(position)))
- 정한 영역 만큼의 비트를 toggle.
- ex) Macro_Invert_Area(rGPFCON, 0xff, 4)
  - rGPFCON 레지스터의 4번 비트부터 상위 8개 비트를 toggle.
  - 1111 0000 1111 0000 1111 0000 1111 0000
  - 토글 후,
  - 1111 0000 1111 0000 1111 1111 0000 0000

#define	Macro_Write_Block(dest, bits, data, position)
  ((dest) = (((unsigned)(dest)) & ~(((unsigned)(bits))<<(position))) | (((unsigned)(data))<<(position)))
- 정한 영역 만큼의 비트를 원하는 값으로 set.
- ex) Macro_Write_Block(rGPFCON, 0xff, 0x55, 4)
  - rGPFCON 레지스터의 4번 비트부터 상위 8개 비트를 0x0으로 clear 후 0x55로 set.
  - 1111 0000 1111 0000 1111 0000 1111 0000
  - 1111 0000 1111 0000 1111 0000 0000 0000
  - 1111 0000 1111 0000 1111 0101 0101 0000

#define Macro_Extract_Area(dest, bits, position)		((((unsigned)(dest))>>(position)) & (bits))
- 정한 영역 만큼의 비트를 추출
- ex) key = Macro_Extract_Area(rGPFCON, 0xff, 4)
  - rGPFCON 레지스터의 4번 비트부터 상위 8개 비트를 추출
  - rGPFCON == 1111 0000 1111 0000 1111 0000 1111 0000
  - key = 0xf0

#define Macro_Check_Bit_Set(dest, position)				((((unsigned)(dest))>>(position)) & 0x1)
- 해당 위치의 비트가 1인지 확인.
- ex) Macro_Check_Bit_Set(rGPFCON, 29);

#define Macro_Check_Bit_Clear(dest, position)			(!((((unsigned)(dest))>>(position)) & 0x1))
- 해당 위치의 비트가 0인지 확인.
- ex) Macro_Check_Bit_Clear(rGPFCON, 29);

```
