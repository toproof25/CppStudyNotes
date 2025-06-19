/*
 소멸자란 클래스에서 `~클래스명() {}`으로 정의하며, 객체가 소멸할 시 실행된다.
 동적 할당한 데이터나 객체 소멸 시점에 정리하고 발생해야 하는 코드를 작성하게 된다.
 생성자는 부모 -> 자식 순서로 생성자가 호출되며, 소멸자는 반대로 자식 -> 부모 순으로 소멸자가 호출된다 
*/

#include <iostream>

class Human
{
public:
  virtual void print() { std::cout << "[사람]입니다" << std::endl; }

  Human() { std::cout << "Human 생성자 실행" << std::endl; }
  ~Human() { std::cout << "Human 소멸자 실행" << std::endl; }
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
  ref_x(x),          // 멤버 레퍼런스 변수
  int_arr(new int[10])
  {
    std::cout << "직접 초기화 방식 - 생성자(매개변수) : 변수(초기값), 변수2(초기값2) {}" << std::endl;
    std::cout << "x : " << x << std::endl;
    std::cout << "y : " << y << std::endl;
    std::cout << "p_y : " << *p_y << std::endl;
    std::cout << "location(x, y) : " << location[0] << ", " << location[1] << std::endl;
    std::cout << "number : " << number << std::endl;
    std::cout << "ref_x : " << ref_x << std::endl;
  }

  ~Man()
  {
    std::cout << "Man 소멸자 실행" << std::endl;
    delete[] int_arr;
  }

  void print() override { std::cout << "[남자]입니다" << std::endl; } 

private:
  int x, y;
  int location[2];
  int *int_arr;
  
  const int number;
  int &ref_x;
  int *p_y;
};



int main()
{
  {
  Man man(15, 30); 
  std::cout << "\n";
  man.print();
  }

  return 0;
}