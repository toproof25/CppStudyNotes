/*
* 부모 클래스 멤버 함수 오버라이드  
*/
#include <iostream>
#include <windows.h>

class Human
{
public:
  void print() { std::cout << "[사람]입니다" << std::endl; }
};


class Man : public Human
{
public:
  // 오버라이드
  void print() { std::cout << "[남자]입니다" << std::endl; } 
};

class Woman : public Human
{
public:
 
  void print() { std::cout << "[여자]입니다" << std::endl; }
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


  // 클래스를 참조할 때 해당 자료형으로 정적 바인딩된 함수가 호출됨
  // ex) 남자와 여자를 하나의 리스트로 관리하고 할 때 이와 같은 경우에는 남자, 여자 자료형이 따로 있어야함
  Human &_human = man;
  Man &_man = man;

  _human.print();
  _man.print();


  std::cout << "\n\n";
  system("pause");
  return 0;
}