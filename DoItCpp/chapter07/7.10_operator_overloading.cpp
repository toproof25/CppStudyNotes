/*
* 연산자 오버로딩 
*/

#include <iostream>
#include <windows.h>

class position
{
  public:
    position(int x=0, int y=0, int z=0): x(x), y(y), z(z) {};
    position operator+(position &operand);
    position operator-(position &operand);
    position operator*(position &operand);
    position operator/(position &operand);
    void print_position();

  private:
    int x, y, z;
};

void position::print_position()
{
  std::cout << "x -> " << this->x << std::endl;
  std::cout << "y -> " << this->y << std::endl;
  std::cout << "z -> " << this->z << std::endl << std::endl;
}

// 포지션 클래스 연산자 오버로딩
position position::operator+(position &operand)
{
  // 같은 클래스의 멤버 함수이기에 operand 객체 private 변수에 접근할 수 있다
  int x = this->x + operand.x;
  int y = this->y + operand.y;
  int z = this->z + operand.z;
  
  position p(x, y, z);
  return p;
}

position position::operator-(position &operand)
{
  int x = this->x - operand.x;
  int y = this->y - operand.y;
  int z = this->z - operand.z;
  
  position p(x, y, z);
  return p;
}

position position::operator*(position &operand)
{
  int x = this->x * operand.x;
  int y = this->y * operand.y;
  int z = this->z * operand.z;
  
  position p(x, y, z);
  return p;
}
position position::operator/(position &operand)
{
  int x = this->x / operand.x;
  int y = this->y / operand.y;
  int z = this->z / operand.z;
  
  position p(x, y, z);
  return p;
}



int main()
{
  position p1(1, 2, 3);
  std::cout << "p1의 좌표" << std::endl;
  p1.print_position();

  position p2(10, 20, 30);
  std::cout << "p2의 좌표" << std::endl;
  p2.print_position();

  // 연산자 오버로딩을 이용하여 좌표를 더하는 연산을 할 수 있다
  position p3 = p1 + p2;
  std::cout << "p1+p2의 좌표" << std::endl;
  p3.print_position();

  // 함수 이름으로도 사용이 가능하다
  position p4 = p2.operator+(p3);
  std::cout << "p2.operator+(p3)의 좌표" << std::endl;
  p4.print_position();


  // 빼기
  position p5 = p4 - p2;
  std::cout << "p4 - p2의 좌표" << std::endl;
  p5.print_position();

  // 곱하기
  position p6 = p1 * p3;
  std::cout << "p1 * p3의 좌표" << std::endl;
  p6.print_position();

  // 나누기
  position p7 = p6 / p1;
  std::cout << "p6 / p1의 좌표" << std::endl;
  p7.print_position();

  position p8 = p1 + p2 + p3 + p4 + p5 + p6 + p7;
  std::cout << "p1 + p2 + p3 + p4 + p5 + p6 + p7의 좌표" << std::endl;
  p8.print_position();



  std::cout << "\n\n";
  system("pause");
  return 0;
}