/*
* 각 멤버 변수 초기화를 연습

1. 사이즈 입력 받기
2. Man클래스를 동적 배열로 사이즈만큼 할당
3. Man객체가 생성될 때 생성되는 수를 계산하기 위한 정적 변수를 선언 및 초기화
4. 정적 변수의 +1 한 후, 상수 변수로 직접 초기화
5. 초기화된 상수 변수(생성된 넘버) 문자열 "남자 + number"로 이름을 생성
6. 위치 배열을 초기화(유니콘 초기화) - 동절 배열로 할당하여 생성자에 직접 값을 할당할 수 없음
7. 생성자 내부에서 초기화된 이름을 출력
8. print함수로 동적 할당된 객체 정보를 모두 출력
9. 동적 할당된 객체를 할당 해제

*/
#include <iostream>
#include <string>
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
  number(++static_number),                 // 멤버 상수 변수
  name("남자"+std::to_string(number)),     // 이름을 생성되는 문서대로 넘버를 붙임
  location{x, y}                           // 위치 저장
  {
    std::cout << name <<" 설정 완료" << std::endl; 
  }

  ~Man()
  {
    std::cout << name <<" 제거" << std::endl; 
  }

  void print() override 
  { 
    std::cout << "이름 : [" << name <<"]" << std::endl; 
    std::cout << "위치 : [" << location[0] << ", " << location[1] <<"]" << std::endl << std::endl;
  } 

private:
  int location[2];
  const int number;
  std::string name;
  static int static_number;
};

// Man 클래스에 선언된 정적 변수를 외부 전역 변수로 초기화
int Man::static_number = 0;

int main()
{
  Human human;

  std::cout <<"size >>> "; 
  int size;
  std::cin >> size;
  Man *mans = new Man[size];

  for(int i=0; i<size; ++i)
  {
    mans[i].print();
  };

  delete[] mans;

  std::cout << "\n\n";
  system("pause");
  return 0;
}