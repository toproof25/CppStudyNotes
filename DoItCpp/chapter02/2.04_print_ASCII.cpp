/*
 * 아스키 코드 출력 
 */

#include <iostream>
#include <windows.h>

int main() {
  // 윈도우 cmd 출력에 오류가 있어 wcout 사용용
  std::cout << "아스키 코드 출력 [32 ~ 126]:\n";

  // 아스키 코드 문자 32부터 126까지 
  for (char i = 32; i <= 126; i++)    
  {
    std::cout << i << ((i % 16 == 15) ? '\n' : ' ');
  }

  std::cout << "\n\n";
  system("pause");
  return 0;
}