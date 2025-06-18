/*
* noexcept를 이용하여 예외를 던지는 함수 구분과 사용법 학습 
*/
#include <iostream>
using namespace std;

int func(int a)
{
  if (a==0)
    throw 0;
  return a;
}

int func_noexcept(int a) noexcept
{
  if (a==0)
    return -1;
  return a;
}

int main()
{
  int a = 0;

  try
  {
    int result;
    bool check;

    // noexcept로 예외가 발생하지 않음을 명시한 함수
    // noexcept(함수명)은 해당 함수가 예외가 있으면 false
    check = noexcept(func_noexcept(a));
    cout << (check ? "func_noexcept함수는 예외가 없습니다" : "func_noexcept함수는 예외를 던집니다") << endl;
    result = func_noexcept(a);

    // 예외를 발생하는 함수
    check = noexcept(func(a));
    cout << (check ? "func함수는 예외가 없습니다" : "func함수는 예외를 던집니다") << endl;
    result = func(a);
  }
  catch (int e)
  { // 괄호 내 에러에 대해서 처리한다.
    cout << "a가 0입니다" << endl; 
  }
  catch (...){
    cout << "...에러..." << endl; 
  }

  return 0;
}