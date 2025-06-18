/*
* 리스코프 치환 원칙 적용 시 자식 소멸자를 강제하는 것 
*/
#include <iostream>
#include <windows.h>



class Human
{
public:
  Human() { std::cout << "[Human] 생성자" << std::endl; }

  // 가상 소멸자 지정
  virtual ~Human() { std::cout << "[Human] 소멸자" << std::endl; }
};


class Man : public Human
{
public:
 
  Man() { std::cout << "[Man] 생성자" << std::endl; }
  virtual ~Man() { std::cout << "[Man] 소멸자" << std::endl; }
};


int main()
{

  // 일반 소멸자의 경우 업케스팅이 발생할 시 자식 소멸자가 호출되지 않음
  // 소멸자에 virtual을 작성 시 자식의 소멸자가 호출되도록 할 수 있다
  Human *man = new Man();
  delete man;

  std::cout << "\n\n";
  system("pause");
  return 0;
}