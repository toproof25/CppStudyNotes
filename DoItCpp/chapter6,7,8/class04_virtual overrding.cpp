/*
* 가상함수 오버라이드 
*/
#include <iostream>
#include <windows.h>

class Human
{
public:
  // 가상 함수
  virtual void print() { std::cout << "[사람]입니다" << std::endl; }
};


class Man : public Human
{
public:
  // 오버라이드 명시
  void print() override { std::cout << "[남자]입니다" << std::endl; } 
};

class Woman : public Human
{
public:
 
  void print() override { std::cout << "[여자]입니다" << std::endl; }
};


int main()
{
  Human human;
  Man man;
  Woman woman;

  human.print();
  man.print();
  woman.print();

  std::cout << "\n\n";

  /* 
  가상 함수를 사용하여 문제를 해결

  가상함수를 사용하면 객체의 가장 마지막에 오버라이드된 함수를 실행한다. 
  부모의 타입으로 자식을 참조하더라도 객체의 오버라이드된 함수를 동작하여 **다형성**을 유지할 수 있다.
  `virtual 함수이름(매개변수) {}`
  */
  Human &_human = man;
  Man &_man = man;

  _human.print(); // 가상함수로 인해 man의 함수가 호출됨
  _man.print();

  // 가상함수를 사용해도 상위 클래스의 함수를 호출할 수 있음
  _human.Human::print();


  std::cout << "\n\n";
  system("pause");
  return 0;
}