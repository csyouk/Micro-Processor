rem 입력하는 그림은 X 해상도가 4의 배수일것!
rem BMP2GPR  LIST파일명  IMAGE출력파일명  IMAGE+헤더정보출력파일명
rem HEADER: [offset(4B)][width(2B)][height(2B)]
BMP2GRP list img img_list
pause
