/**
 * @file unique_ptr_zero_overhead_test.cpp
 * @date 2026-01-29
 * @author toproof(kmnlmn123@gmail.com)
 * @brief 
 * - new-delete와 unique_ptr을 비교해보며 Zero Overhead Principle을 직접 확인해보기 위한 코드
 * - 어셈블리 .s 파일로 빌드하여 확인해보고자 한다
 */

#include <iostream>
#include <memory>

volatile int* escape; ///< 컴파일 최적화를 막아 함수 그대로의 어셈블리 코드를 보기 위함

void Test_new_delete()
{
  int* number = new int(99999);
  //std::cout << "new-delete 방식 생성 number : " << *number << '\n';
  escape = number;
  *number += 1;
  //std::cout << "new-delete 방식 제거 escape : " << *escape << '\n';
  delete number;
}

void Test_unique_ptr()
{
  std::unique_ptr<int> number = std::make_unique<int>(99999);
  //std::cout << "unique_ptr 방식 생성 number : " << *number << '\n';
  escape = number.get();
  *number += 1;
  //std::cout << "unique_ptr 방식 제거 number : " << *number << '\n';
}


int main()
{
  Test_unique_ptr();
  Test_new_delete();
  return 0;
}

