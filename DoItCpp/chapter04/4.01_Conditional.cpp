/*
* CPP 조건문, if문과 switch문 학습
*/
#include <iostream>
using namespace std;

int main()
{
  int number = 10;
  char c = 'B';

  // 조건이 참인 경우를 순차적으로 비교하여 실행
  // 단락 평가를 하여 앞선 조건에 따라 결과가 정해지면 결과적으로 참과 거짓을 결정함
  if (number > 0 && c == 'A')
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
  // if문은 참일 때 까지 순차적으로 모든 if~else if를 검사하나, switch는 표현식에 따라 바로 해당 case를 실행
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