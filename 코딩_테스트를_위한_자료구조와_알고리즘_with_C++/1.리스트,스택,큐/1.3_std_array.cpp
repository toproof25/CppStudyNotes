/*
* array 사용하기
*/
#include <iostream>
#include <array>

int main()
{
  // 정수형 크기 10인 배열을 선언
  std::array<int, 10> arr1;

  // 배열 첫번째 공간에 정수 [1]을 정의
  arr1[0] = 1;
  std::cout << "arr1 배열의 첫 번째 원소" << arr1[0] << std::endl;

  // 정수형 크기 4인 배열을 선언과 동시에 정의
  std::array<int, 4> arr2 = {1, 22, 333, 4444};
  std::cout << "arr2의 모든 원소";

  for (int i=0; i<4; ++i)
  {
    std::cout << arr2[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}