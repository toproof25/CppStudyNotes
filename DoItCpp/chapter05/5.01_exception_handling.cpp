/*
* 코드 설명 작성
*/
#include <iostream>
using namespace std;

int main()
{
  int i = 1;  

  try{
    //실행문
    if (i == 1)
      throw 1; // 예외를 발생시켜 catch로 보낸다
  }
  catch (int e){ // 괄호 내 에러에 대해서 처리한다.
    //정수 예외처리  
    cout << "정수 예외처리 - " << e << endl;
  }
  catch (float e){
    // 부동소수점 예외처리
    cout << "부동소수점 예외처리 - " << e << endl;
  }
  catch (char e){
    // 문자 예외처리
    cout << "문자 예외처리 - " << e << endl;
  }
  catch (...){
    // ...은 기타 예외를 모두 묶어서 처리한다
    cout << "나머지 모든 예외처리" << endl;
  }

  return 0;
}