/*
* 멤버 변수 초기화 방법
직접 초기화
복사 초기화
*/
#include <iostream>
#include <windows.h>

class Human
{
public:
  virtual void print() { std::cout << "[사람]입니다" << std::endl; }
};


class Man : public Human
{
public:
  // 직접 초기화
  Man(int x=0, int y=0) : x(x), y(y) 
  {
    std::cout << "직접 초기화 방식 - 생성자(매개변수) : 변수(초기값), 변수2(초기값2) {}" << std::endl;
    std::cout << "x : " << x << std::endl;
    std::cout << "y : " << y << std::endl;
  }

  void print() override { std::cout << "[남자]입니다" << std::endl; } 

private:
  int x, y;

};

class Woman : public Human
{
public:

  // 복사 초기화
  Woman(int x=0, int y=0)
  {
    // this는 현재 자신의 객체를 가리키는 포인터
    // 포인터이기에 (*this).변수 or this->변수 로 사용
    this->x = x;
    this->y = y;

    std::cout << "복사 초기화 방식 - 일반적인 변수에 대입하듯이 작성" << std::endl;
    std::cout << "x : " << x << std::endl;
    std::cout << "y : " << y << std::endl;
  }

  void print() override { std::cout << "[여자]입니다" << std::endl; }

private:
  int x, y;
};


int main()
{
  Human human;
  Man man(15, 30);
  Woman woman(111, 222);

  human.print();
  man.print();
  woman.print();

  std::cout << "\n\n";
  system("pause");
  return 0;
}