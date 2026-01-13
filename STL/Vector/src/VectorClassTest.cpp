/**
 * @file VectorClassTest.cpp
 * @date 2026-01-13
 * @brief 직접 구현한 Vector Class를 테스트
 * 
 */

#include "../inc/Vector.hpp"
#include <iostream>
#include <string>

struct test
{
  int int_number;
  double double_number;
  std::string string_text;

  test(int i, double d, std::string s): int_number(i), double_number(d), string_text(s) {};

  // << 연산자 오버라이드
  friend std::ostream& operator<<(std::ostream& os, const test& other) 
  {
    os << "{" << std::to_string(other.int_number) << ", " << std::to_string(other.double_number) << ", " << other.string_text << "}";
    return os; 
  }

};


void Test01(Vector<int>& v)
{
  std::cout << "*************************" << " < 1. 객체 생성 및 크기 조절 테스트 > " << "*************************" << '\n';
  std::cout << "크기 변경 전 vector 객체 size, capacity : " << std::to_string(v.size()) << ", " << std::to_string(v.capacity()) << '\n';
  std::cout << "v.reserve(10)" << '\n';
  v.reserve(10);
  std::cout << "크기 변경 후 vector 객체 size, capacity : " << std::to_string(v.size()) << ", " << std::to_string(v.capacity()) << '\n';
  printf("\n\n");
}

void Test02(Vector<int>& v)
{
  std::cout << "*************************" << " < 2. push_back 함수 테스트 > " << "*************************" << '\n';
  for (int i=0; i<20; ++i)
  {
    v.push_back(i);
    std::cout << i+1 << ". v.push_back("<< i << ")" << '\n';
  }
  std::cout << "push 후 v 객체의 size, capacity: " << v.size() << ", " << v.capacity() << '\n';
  printf("\n\n");
}

void Test03(Vector<int>& v)
{
  std::cout << "*************************" << " < 3. []연산자 오버라이드 함수 테스트 > " << "*************************" << '\n';
  for (int i=0; i<v.size(); ++i)
  {
    std::cout << i+1 << ". v["<< i << "]: " << v[i] << '\n';
  }
  printf("\n\n");
}

void Test04(Vector<int>& v)
{
  std::cout << "*************************" << " < 4. at 함수 테스트 > " << "*************************" << '\n';
  try
  {
    for (int i=0; i<v.size()+1; ++i)
    {
      std::cout << i+1 << ". v.at("<< i << ") : " << v.at(i) << '\n';
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  printf("\n\n");

}

void Test05(Vector<int>& v)
{
  std::cout << "*************************" << " < 5. pop_back 함수 테스트 > " << "*************************" << '\n';
  std::cout << "pop() 전 v 객체의 size, capacity: " << v.size() << ", " << v.capacity() << '\n';
  try
  {
    for (int i=v.size()-1; i>=-10; --i)
    {
      int back = v.back();
      v.pop_back();
      std::cout << i+1 << ". v.pop_back(): " << back << '\n';
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

  std::cout << "pop() 후 v 객체의 size, capacity: " << v.size() << ", " << v.capacity() << '\n';
  printf("\n\n");
}

void Test06(Vector<int>& v)
{
  for (int i = 0; i < 10; i++) v.push_back(i * 10);
  std::cout << "*************************" << " < 6. shrink_to_fit() 함수 테스트 > " << "*************************" << '\n';
  std::cout << "shrink_to_fit() 전 v 객체의 size, capacity: " << v.size() << ", " << v.capacity() << '\n';
  v.shrink_to_fit();
  std::cout << "shrink_to_fit() 후 v 객체의 size, capacity: " << v.size() << ", " << v.capacity() << '\n';
  printf("\n\n");
}

void Test07(Vector<int>& v)
{
  std::cout << "*************************" << " < 7. clear() 함수 테스트 > " << "*************************" << '\n';
  std::cout << "clear() 전 v 객체의 size, capacity: " << v.size() << ", " << v.capacity() << '\n';
  v.clear();
  std::cout << "clear() 후 v 객체의 size, capacity: " << v.size() << ", " << v.capacity() << '\n';
  printf("\n\n");
}

void Test08(Vector<int>& v)
{
  std::cout << "*************************" << " < 8. insert() 함수 테스트 > " << "*************************" << '\n';
  for (int i=0; i<10; ++i) v.push_back(i);
  std::cout << "insert() 전 v 객체 상태와 크기 size, capacity: " << v.size() << ", " << v.capacity() << '\n';
  for (int i=0; i<v.size(); ++i) std::cout << i+1 << ". v["<< i << "]: " << v[i] << '\n';
  printf("\n");

  v.insert(v.begin()+3, 999);
  v.insert(v.begin()+6, 9999);

  std::cout << "insert() 후 v 객체 상태와 크기 size, capacity: " << v.size() << ", " << v.capacity() << '\n';
  for (int i=0; i<v.size(); ++i) std::cout << i+1 << ". v["<< i << "]: " << v[i] << '\n';

  printf("\n\n");
}


void Test09(Vector<int>& v)
{
  std::cout << "*************************" << " < 9. erase() 함수 테스트 > " << "*************************" << '\n';

  std::cout << "erase() 전 v 객체 상태와 크기 size, capacity: " << v.size() << ", " << v.capacity() << '\n';
  for (int i=0; i<v.size(); ++i) std::cout << i+1 << ". v["<< i << "]: " << v[i] << '\n';
  printf("\n");

  v.erase(v.begin()+3);
  v.erase(v.begin()+5);
  v.erase(v.begin()+7);

  std::cout << "erase() 후 v 객체 상태와 크기 size, capacity: " << v.size() << ", " << v.capacity() << '\n';
  for (int i=0; i<v.size(); ++i) std::cout << i+1 << ". v["<< i << "]: " << v[i] << '\n';

  printf("\n\n");
}



int main()
{ 

  // 정의
  Vector<int> v;

  Test01(v);
  Test02(v);
  Test03(v);
  Test04(v);
  Test05(v);
  Test06(v);
  Test07(v);
  Test08(v);
  Test09(v);



  return 0;
}