/*
 네임스페이스 이해와 사용 
cpp의 기본 함수가 모인 std 네임스페이스와
같은 함수 이름으로 ok 네임스페이스를 만들어서 사용

각 네임스페이스마다 이름이 같은 함수라도 정의된 내용이 달라 다르게 동작
*/
#include <iostream>
using namespace std;

namespace ok
{
  void cout(string str)
  {
    std::cout << "ok 그룹에서 실행하는 std::cout" << endl;
    std::cout << str << endl;
  }
}

int main()
{
  std::cout << "cout를 쓰기 위해서는 std:: 를 써야하나"<< endl;
  cout << "using namespace std를 쓰면 바로 사용이 가능하다"<< endl;

  ok::cout("ok 그룹으로 실행");
  
  return 0;
}