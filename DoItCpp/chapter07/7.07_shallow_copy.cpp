/*
* 얕은 복사 
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

    copy c_copy = c; // 복체 객체 (원본 객체의 값을 복사하는 얕은 복사 생성자가 호출됨)
    c_copy.print_name(); // 원본 객체의 값이 복사되어 이름이 출력됨 - 동적 변수의 경우 같은 메모리를 참조하게 된다
  }
  // 생성된 객체의 소멸자가 호출된다
  // c 객체 소멸자가 호출되어 number의 동적 할당된 값이 해제가됨
  // c_copy 객체의 소멸자가 호출되면서 number의 값 할당을 해제하는데 오류가 발생 (이미 c 소멸자에서 해제가 됨)


  std::cout << "\n\n";
  system("pause");
  return 0;
}