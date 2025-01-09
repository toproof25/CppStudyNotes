#include <iostream> // input, output 기능을 가진 iostream 헤더파일
#include <windows.h>

int main() 
{
  int number; 

  // std::cout : 화면에 출력하는 객체
  std::cout << "number에 들어갈 값을 입력하세요 : ";

  // std::cin : 키보드로부터 입력을 받는 객체
  std::cin >> number; 
  
  std::cout << "number : " << number << std::endl; 

  std::cout << "\n\n";
  system("pause");
  return 0;
}

