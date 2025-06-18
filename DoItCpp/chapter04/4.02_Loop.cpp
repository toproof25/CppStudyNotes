/*
* 반복문 for문, while문, do while문
*/
#include <iostream>
using namespace std;

int main()
{
  
  // for문 - 조건이 참인 경우이며, 특정 범위에 반복하기 유용 (1~100까지)
  for (int i=1; i<101; ++i)
  {
    cout << "for i : " << i << '\n';
  }

  cout << '\n';

  // while문 - 조건이 참인 경우에만 반복 (1~100까지)
  int i=0;
  while (++i < 101)
  {
    cout << "while i : " << i << '\n';
  }

  cout << '\n';

  // do while문 - 1회는 실행이 보장되며, 조건에 따라 반복
  // j는 0으로 while에서 조건이 거짓미나, do에서 한번은 실행을 보장받는다
  int j=0;
  do
  {
    cout << "do while j : " << j << '\n';
  } while (j != 0);
  


  return 0;
}