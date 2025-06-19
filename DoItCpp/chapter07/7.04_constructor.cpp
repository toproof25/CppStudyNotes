/*
  생성자 - 멤버 변수 초기화 방법  

  직접 초기화
  복사 초기화

  멤버 래퍼런스 변수 초기화
  상수 멤버 변수 초기화
  정적 멤버 변수 초기화
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
  Man(int x=0, int y=0) : 
  x(x),             // 일반 멤버 변수 
  y(y),
  p_y(&(this->y)),  // 멤버 포인터 변수
  location{x, y},   // 멤버 고정 배열
  number(9999),     // 멤버 상수 변수
  ref_x(x)          // 멤버 레퍼런스 변수
  {
    std::cout << "직접 초기화 방식 - 생성자(매개변수) : 변수(초기값), 변수2(초기값2) {}" << std::endl;
    std::cout << "x : " << x << std::endl;
    std::cout << "y : " << y << std::endl;
    std::cout << "p_y : " << *p_y << std::endl;
    std::cout << "location(x, y) : " << location[0] << ", " << location[1] << std::endl;
    std::cout << "number : " << number << std::endl;
    std::cout << "ref_x : " << ref_x << std::endl;
    std::cout << "static_number : " << static_number << std::endl;
  }

  void print() override { std::cout << "[남자]입니다" << std::endl; } 

private:
  int x, y;
  int location[2];
  
  const int number;
  int &ref_x;
  int *p_y;
  static int static_number;
  static const int const_static_number;
};

// Man 클래스에 선언된 정적 변수를 외부 전역 변수로 초기화
int Man::static_number = 50;

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
  Man man(15, 30); std::cout << "\n";
  Woman woman(111, 222);

  human.print();
  man.print();
  woman.print();

  std::cout << "\n\n";
  system("pause");
  return 0;
}