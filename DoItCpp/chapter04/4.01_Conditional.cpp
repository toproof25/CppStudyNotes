/*
* CPP 조건문
*/
#include <iostream>
using namespace std;

int main()
{
  int number = 10;

  // 조건이 참인 경우를 순차적으로 비교하여 실행
  if (number > 0)
  {
    cout << "number는 양수입니다" << endl; 
  }
  else if (number < 0)
  {
    cout << "number는 음수입니다" << endl; 
  }
  else
  {
    cout << "number는 0 입니다" << endl; 
  }

  // 어떤 값에 따라 점프 테이블 형태로 실행
  char c = 'B';
  switch (c)
  {
    case 'A':
      cout << "c는 [" << c << "]입니다" << endl;
      break;
    case 'B':
      cout << "c는 [" << c << "]입니다" << endl;
    case 'C':
      cout << "c는 [" << c << "]입니다" << endl;
      break;
    default :
      cout << "일치하는 값이 없습니다" << endl;
  }



  return 0;
}