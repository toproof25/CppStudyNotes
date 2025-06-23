/*
random 라이브러리로 난수 생성하기
chrono 라이브러리는 시간 연산에 대한 헤더 파일로, 시간을 이용한 시드값을 생성할 수 있다

컴퓨터에서 난수는 일정한 패턴을 가진 숫자의 나열로 의사 난수이다.
즉 완전한 랜덤이 아니기에 시드를 설정하지 않으면 같은 패턴의 숫자가 나타난다.
그렇다고 시드를 설정한다고 해도 완전한 랜덤은 아니지만 매번 패턴을 달리하기에 
난수를 사용할 때는 고려해야 하는 부분이다

1. random_device를 통해 랜덤 시드 생성
2. mt19937 생성자로 시드를 넣어 난수를 생성

*/
#include <iostream>
#include <random>     // 난수를 생성하는 헤더 파일  
#include <chrono>     // 난수 시드값을 생성

using namespace std;

int main()
{
  mt19937 mt_rand_32bit;
  mt19937_64 mt_rand_64bit;

  cout << "mt19937 의사 난수" << endl;
  for (int i=0; i<10; ++i)
  {
    cout << i+1 << ": " << mt_rand_32bit() << endl;
  }
  cout << endl;
  
  cout << "mt19937_64 의사 난수" << endl;
  for (int i=0; i<10; ++i)
  {
    cout << i+1 << ": " << mt_rand_64bit() << endl;
  }
  cout << endl;


  // 표준 라이브러리 random_device를 이용한 난수 생성
  // 하드웨어 노이즈를 이용한 난수 생성으로 랜덤 시드를 생성한다
  random_device rng;
  cout << "random_device 의사 난수" << endl;
  for (int i=0; i<10; ++i)
  {
    cout << i+1 << ": " << rng() << endl;
  }
  cout << endl;


  // chrono 시간을 이용하여 시드값 생성
  // 시간에 대한 고유값으로 설정할 수 있으나, 추측이 가능함
  auto curTime = chrono::system_clock::now();
  auto duration = curTime.time_since_epoch();
  auto millis = chrono::duration_cast<chrono::milliseconds>(duration).count();

  mt19937_64 mt_rand_seed(millis);
  cout << "mt_rand_seed 시드값을 설정한 의사 난수" << endl;
  for (int i=0; i<10; ++i)
  {
    cout << i+1 << ": " << mt_rand_seed() << endl;
  }
  cout << endl;




  return 0;
}