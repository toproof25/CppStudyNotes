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

  // 가상 함수를 사용하여 문제를 해결
  Human &_human = man;
  Man &_man = man;

  _human.print();
  _man.print();


  std::cout << "\n\n";
  system("pause");
  return 0;
}