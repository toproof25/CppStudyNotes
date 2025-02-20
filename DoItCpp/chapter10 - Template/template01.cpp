/*
* template 사용하여 여러 타입의 합을 반환하는 함수 제작
*/
#include <iostream>
#include <windows.h>
using namespace std;

// 템플릿 함수
template<typename T>
T add_value(T T1, T T2)
{
  return T1 + T2;
}

// 특정 타입에 대해서 다르게 적용할 수 있음
template<>
double add_value(double data1, double data2)
{
  return (int)data1 + (int)data2;
}




int main()
{
  int data1 = 10, data2 = 30;
  double data3 = 1.1123, data4 = 5.231325;
  string data5 = "안녕", data6 = "하세요!";

  // 컴파일 단계에서 메개변수에 따라 데이터 타입을 추론
  cout << " >> data1 + data2 = " << add_value(data1, data2) << endl;
  cout << " >> data3 + data4 = " << add_value(data3, data4) << endl;

  // 데이터 타입을 명시적으로 호출
  cout << " >> data5 + data6 = " << add_value<string>(data5, data6) << endl;

  std::cout << "\n\n";
  system("pause");
  return 0;
}