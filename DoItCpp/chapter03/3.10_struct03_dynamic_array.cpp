/*
* 구조체 타입 동적 배열 선언과 사용
*/
#include <iostream>
#include <windows.h>
#include <string>

// x, y, z값을 가지는 구조체 선언
struct Position
{
    // 좌표
    double x = 0.0;
    double y = 1.0;
    double z = 0.0;

    // 좌표 출력
    void display(const std::string &name)
    {
        std::cout << name << " x, y, z : (" << x << ", " << y << ", " << z << ")" << std::endl;
    }   
};

// 구조체 포인터와 좌표를 받아서 값을 변경 - 원본 구조체의 값을 바꾸기 위해 포인터로 받아옴
void set_position(Position *pos, double x, double y, double z)
{
    (*pos).x = x; // *를 이용하여 값에 접근
    (*pos).y = y;
    (*pos).z = z;
}

// 구조체 포인터를 활용하여 좌표 초기화
void reset_position(Position *pos)
{
    pos->x = 0.0; // ->를 이용하여 값에 접근
    pos->y = 0.0;
    pos->z = 0.0; 
}

int main()
{
  // 동적 배열 생성
  int size = 11; // 배열의 크기
  Position *target = new Position[size];

  // target[i] == *(target+i)
  // 즉 포인터로 접근하기 위해서는 target+i를 통해 포인터 연산을 한 후 ->로 접근한다

  for (int i=0; i<size; ++i)
  {
    (target+i)->x = i;
    (target+i)->y = i * 0.0421;
    (target+i)->z = i * 3120.0;
  };

  std::cout << "초기값 좌표" << std::endl;
  int count = 0;
  for (int i=0; i<size; ++i)
  {
    (target+i)->display("target[" + std::to_string(count++) + "]");
  };
  std::cout << "\n\n";


  std::cout << "set_position 좌표" << std::endl;
  count = 0;
  for (int i=0; i<size; ++i)
  {
      set_position(target+i, 0, i, 0);
  };
  for (int i=0; i<size; ++i)
  {
      (target+i)->display("target[" + std::to_string(count++) + "]");
  };
  std::cout << "\n\n";


  std::cout << "reset_position 좌표" << std::endl;
  count = 0;
  for(int y=0; y<=10; y++)
  {
      reset_position(target+y);
  }
  for (int i=0; i<size; ++i)
  {
      (target+i)->display("target[" + std::to_string(count++) + "]");
  }
  std::cout << "\n\n";

  std::cout << "\n\n";
  system("pause");
  return 0;
}