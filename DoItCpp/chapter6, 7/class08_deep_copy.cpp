/*
* 동적 할당 시 깊은 복사를 사용
  기존 복사 생성자를 오버라이딩하여 사용한다
*/
#include <iostream>
#include <windows.h>

class copy
{

public:

  copy(std::string name):
  name(name)
  { 
    std::cout << this->name << " = number 메모리에 정수 10을 할당합니다" << std::endl;
    number = new int(10); 
  };

  // 클래스이름(const 클래스이름 &레퍼런스 변수) - 복사 생성자 오버라이딩
  copy(const copy &ref)
  {
    this->name = ref.name;      // 원본 이름을 그대로 복사
    this->number = new int(10); // 동적 할당의 경우 새로운 메모리를 할당하여 다른 메모리를 참조하게 됨
  }

  ~copy()
  { 
    std::cout << this->name << " = number 메모리 할당을 해제합니다" << std::endl;
    delete number; 
  };
  
  void print_name() 
  {
    std::cout << "객체 이름 : " << this->name << std::endl;
  }
  
private:
  std::string name;
  int *number;
};


int main()
{

  // 소멸자 호출을 위해 지역 변수로 만듦
  {
    copy c("first"); // 원본 객체 (생성자 호출 - 동적 변수 number에 메모리 할당)
    c.print_name();

    copy c_copy = c; // 복체 객체 (복사되는 객체는 오버라이딩된 복사 생성자가 호출됨)
    c_copy.print_name(); // 원본 객체의 값이 복사되어 이름이 출력됨
  }
  // 생성된 객체의 소멸자가 호출된다
  // 이전에 오류를 깊은 복사로 해결


  std::cout << "\n\n";
  system("pause");
  return 0;
}