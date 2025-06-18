/*
* 스택 풀기에 대한 이해
*/
#include <iostream>
using namespace std;

int func(int a)
{
  if (a==0)
    throw 0;
  return a;
}

int main()
{
  int a = 0;

  try
  {
    // throw는 func함수 내에서 발생했으나 스택 풀기가 되어 여기 try구문에서 처리된다
    func(a); // 예외를 발생시켜 catch로 보낸다
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