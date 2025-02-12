/*
* 트랜스폼 클래스 제작해보기

Transform class : 컴포지션 포지션, 크기, 회전

추상 클래스 (좌표 함수들)
- 좌표 리셋 순수 가상 함수
- 좌표 설정 순수 가상 함수
- 좌표 출력 순수 가상 함수
- 연산자 오버로딩 vector3 반환 +, -

포지션 class : 추상 클래스를 상속


회전 class : 추상 클래스를 컴포지션으로 사용

vector3 클래스
- x, y, z값

*/

#include <iostream>
#include <windows.h>
using namespace std;

// position, rotation, scale 클래스의 바탕이되는 추상 클래스
class coordinate
{
  public:
    virtual void reset_vector()=0;
    virtual void set_vector(double x, double y, double z)=0;
    virtual void print_vector()=0;
};

// 추상 클래스 상속을 이용한 position 클래스
class position : public coordinate
{
  public:
    position() : position(0.0, 0.0, 0.0) {};
    position(double x, double y, double z) : x(x), y(y), z(z) {};

    virtual void reset_vector() override;
    virtual void set_vector(double x, double y, double z) override;
    virtual void print_vector() override;

    position operator+(const position &vector);
    position operator-(const position &vector);

  private:
    double x, y, z;
};

void position::reset_vector()
{
  this->x = 0.0;
  this->y = 0.0;
  this->z = 0.0;
}
void position::set_vector(double x, double y, double z)
{
  this->x = x;
  this->y = y;
  this->z = z;
}
void position::print_vector()
{
  cout << "좌표 ("<< this->x << ", " << this->y << ", " << this->z << ")" << endl;
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
  position p;
  p.print_vector();
  p.set_vector(5.2, 12.1, 4.666);
  p.print_vector();
  p.reset_vector();
  p.print_vector();



  std::cout << "\n\n";
  system("pause");
  return 0;
}