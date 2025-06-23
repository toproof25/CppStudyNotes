/*
array는 고정 크기의 배열을 담는 컨테이너.
요소를 삽입, 제거를 자주 하지 않으면서 빠르게 접근할 때 사용하는게 유리하다

size()를 이용하여 array의 길이를 찾는다
범위를 벗어나는 원소를 찾을 경우 "out_of_range" 오류가 발생.
*/
#include <iostream>
#include <array>
#include <stdexcept>
using namespace std;

int main()
{
  // array 정의
  std::array<int, 5> arr1 = {0, 1, 2, 3, 4};

  // iterator로 array 순회
  std::array<int, 5>::iterator it = arr1.begin();
  for (it; it != arr1.end(); ++it)
  {
    cout << "arr1의 값 : " << *it << endl;
  }


  // 범위를 벗어나면 "out_of_range" 오류를 던진다
  cout << "arr1의 사이즈 : " << arr1.size() << endl;
  try
  {
    cout << arr1.at(10) << endl;
  }
  catch (const std::out_of_range& e) 
  {
      std::cerr << "오류 발생: 배열 범위를 벗어난 접근입니다." << std::endl;
      std::cerr << "C++ 예외 메시지: " << e.what() << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  

  return 0;
}