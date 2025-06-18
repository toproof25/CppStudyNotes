/*
* 간단한 클래스 정의로 클래스, 객체 이해
*/
#include <iostream>
#include <windows.h>

class Position
{
  // 외부에서 참조할 수 있는 접근 제어자
  public: 
    // 생성자 : 메모리 상(힙, 스택)에 할당될 때 호출됨
    Position()
    {
      std::cout << "Position 생성자 호출\n" << std::endl;
      x = 0.0;
      y = 0.0;
      z = 0.0;
    }

    // 소멸자 : 객체가 메모리에서 사라질 때(지역 변수, 동적 메모리 해제) 호출됨
    ~Position()
    {
      std::cout << "Position 소멸자 호출" << std::endl;
    }

    // 멤버 함수 : 좌표 리셋
    void reset_position()
    {
      std::cout << "reset_position 메서드 호출" << std::endl;
      x = 0.0;
      y = 0.0;
      z = 0.0;
    }

    // 멤버 함수 : 좌표 설정
    void set_position(double x, double y, double z)
    {
      std::cout << "set_position 메서드 호출" << std::endl;

      // this는 객체 자신을 가리키는 포인터
      this->x = x;
      this->y = y;
      this->z = z;
    } 

    void print_position()
    {
      std::cout << "print_position 메서드 호출" << std::endl;
      std::cout << "x : " << x << std::endl;
      std::cout << "y : " << y << std::endl;
      std::cout << "z : " << z << std::endl;
      std::cout << "\n";
    }

  // 외부에서 참조할 수 없는 접근 제어자
  private:
    // 멤버 변수
    double x;
    double y;
    double z;
};


int main()
{ 
    // 객체 생성 - 소멸자 호출을 위해 블록으로 묶음
    {
      std::cout << "일반 객체 생성\n";
      Position pos1;

      pos1.print_position(); 

      pos1.set_position(10.0, 20.0, 30.0);
      pos1.print_position();

      pos1.reset_position();
      pos1.print_position();

      // 지역 변수로 선언된 pos1은 스택에서 사라지면서 소멸자 호출
    }

    std::cout << "\n-------------------------------------------------------\n";

    // 동적 할당이 되면서 생성자 호출
    std::cout << "동적 객체 생성\n";
    Position *pos2 = new Position();

    // 포인터 변수로 사용하기에 ->을 이용하여 접근
    // (*pos2) 은 pos2-> 와 같다
    pos2->print_position();

    pos2->set_position(10.0, 20.0, 30.0);
    pos2->print_position();

    pos2->reset_position();
    pos2->print_position();

    // 동적 할당을 해제하면서 소멸자 호출
    delete pos2;

    std::cout << "\n\n";
    system("pause");
    return 0;
}

