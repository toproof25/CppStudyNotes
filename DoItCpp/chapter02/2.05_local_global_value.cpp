/*
함수를 이용하여 지역 변수와 전역 변수의 생명 주기를 이해
함수 내 생성된 변수는 해당 함수에서만 사용할 수 있다
*/
#include <iostream>
using namespace std;

int value = 100;

// add_local 내 생성된 변수는 해당 함수 내에서만 생존
void add_local(int number)
{
  int value = number + 99;
}

// add_global 내 사용된 value는 전역변수 value를 의미 
void add_global(int number)
{
  value += number;
}

int main()
{
  int value = 1;


  // add_local에 있는 value는 해당 함수에만 존재하는 value
  add_local(99);
  // 해당 value또한 main함수에 있는 지역 변수
  cout << "local value : " << value << endl;

  // add_global에 있는 value는 전역변수 value를 의미
  add_global(99);
  cout << "global value : " << ::value << endl;

  // 전역 변수와 지역 변수 이름이 같다면, 전역 변수는 ::을 이용하여 호출
  cout << "global ::value : " << ::value << endl;

  return 0;
}