/*

포지션 클래스 제작해보기

추상 클래스 (좌표 함수들)
- 좌표 리셋 순수 가상 함수
- 좌표 설정 순수 가상 함수
- 좌표 출력 순수 가상 함수

포지션 class : 추상 클래스를 상속
- 연산자 오버로딩
- x, y, z 부동 소수점 좌표

*/

#include <iostream>
#include <windows.h>
using namespace std;

// position, rotation 추상 클래스
class coordinate
{
  public:
    // 순수 가상 함수를 선언 
    virtual void reset_vector()=0;
    virtual void set_vector(double x, double y, double z)=0;
    virtual void print_vector() const =0;
};

// 추상 클래스 상속을 이용한 position 클래스
class position : public coordinate
{
  public:
    position() : position(0.0, 0.0, 0.0) {};
    position(double x, double y, double z) : x(x), y(y), z(z) {};

    virtual void reset_vector() override;
    virtual void set_vector(double x, double y, double z) override;
    virtual void print_vector() const override;

    position operator+(const position &vector);
    position operator-(const position &vector);

  private:
    double x, y, z;
};

void position::reset_vector()
{
  this->x = 0;
  this->y = 0;
  this->z = 0;
}
void position::set_vector(double x, double y, double z)
{
  this->x = x;
  this->y = y;
  this->z = z;
}
void position::print_vector() const
{
  cout << "포지션 좌표 출력 (" << this->x << ", " << this->y << ", " << this->z << ")" << endl;
}

position position::operator+(const position &pos) 
{
  return position(this->x + pos.x, this->y + pos.y,this->z + pos.z); 
}
position position::operator-(const position &pos) 
{
  return position(this->x - pos.x, this->y - pos.y,this->z - pos.z); 
}

int main()
{
  position p1;
  p1.print_vector();
  p1.set_vector(5.2, 12.1, 4.666);
  p1.print_vector();

  std::cout << "\n\n";

  position p2;
  p2.print_vector();
  p2.set_vector(1115.2, 1112.1, 4111.666);
  p2.print_vector();

  std::cout << "\n\n";

  position p3 = p1 + p2;
  p3.print_vector();

  system("pause");
  return 0;
}