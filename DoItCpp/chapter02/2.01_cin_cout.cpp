/*
 * 기본 C++의 입력과 출력 
 */

#include <iostream>  // input, output 기능을 가진 iostream 헤더파일
#include <windows.h> // system 함수를 사용하기 위한 헤더파일 - cmd창이 바로 종료되지 않도록

int main() 
{

  int number; 

  // std -> 표준 라이브러리의 namespace로 standard의 약자

  // std::cout -> 화면에 출력
  std::cout << "number에 들어갈 값을 입력하세요\nstd::cin >> ";

  // std::cin -> 키보드로부터 입력받음
  std::cin >> number; 
  
  std::cout << "std::cout << " << number << std::endl; 

  std::cout << "\n\n";
  system("pause");
  return 0;
}