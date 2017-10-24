# C언어로 작성된 프로그램은 어떻게 작동하는가?
- C언어로 작성된 프로그램.
- preprocessor를 거친다.
- compiler가 c언어로 된 프로그램을 어셈블리어로 바꿔준다.
- Assembly로 작성된 코드가 나옴.
- Assembler로 assembly를 최종적으로 기계어(Machine Language)로 바꿔준다. 기계어로 바뀐 코드를 obj파일로 바꿔준다.
- obj파일들을 Linker가 합친다.
- 실행파일이 만들어진다.(Makefile)
