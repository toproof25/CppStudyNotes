/**
 * @file VectorClassTest.cpp
 * @date 2026-01-13
 * @brief 직접 구현한 Vector Class를 테스트
 * 
 */

#include "../inc/Vector.hpp"
#include <iostream>
#include <string>
#include <algorithm>

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

  // 정렬을 위한 < 연산자 오버라이드
  bool operator<(const test& other) const
  {
    return this->int_number < other.int_number;
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

void Test10(Vector<int>& v)
{
  std::cout << "*************************" << " < 10. iterator 테스트 > " << "*************************" << '\n';

  v.clear();
  for (int i = 0; i < v.capacity(); i++) v.push_back(rand() % 100);

  std::cout << "iterator를 이용한 순회" <<  '\n';
  for (auto it=v.begin(); it!=v.end(); ++it) std::cout << *it << ", ";
  printf("\n\n");

  std::cout << "iterator를 이용한 정렬" <<  '\n';
  std::sort(v.begin(), v.end());
  for (auto it=v.begin(); it!=v.end(); ++it) std::cout << *it << ", ";

  printf("\n\n");
}


void Test11(Vector<int>& v)
{
  std::cout << "*************************" << " < 11. emplace 테스트 > " << "*************************" << '\n';

  Vector<test> v2;
  v2.reserve(20);
  for (int i = 0; i < v2.capacity(); i++) v2.emplace_back(rand() % 100, rand() % 1000 / 10.0, "Test"+std::to_string(i));

  std::cout << "iterator를 이용한 순회" <<  '\n';
  for (auto it=v2.begin(); it!=v2.end(); ++it) std::cout << *it << '\n';
  printf("\n\n");

  std::cout << "iterator를 이용한 정렬(정수를 기준으로 정렬)" <<  '\n';
  std::sort(v2.begin(), v2.end());
  for (auto it=v2.begin(); it!=v2.end(); ++it) std::cout << *it << '\n';

  printf("\n\n");
}

void Test12(Vector<int>& v)
{
  std::cout << "*************************" << " < 12. 이동 생성자 및 이동 할당 종합 테스트 > " << "*************************" << '\n';

  // 1. 이동 생성자 테스트
  std::cout << "--- 1. 이동 생성자 성능 테스트 ---" << '\n';
  Vector<test> source;
  source.reserve(100);
  for (int i = 0; i < 100; i++) {
    source.emplace_back(i, i * 1.5, "Source" + std::to_string(i));
  }
  
  std::cout << "source: size=" << source.size() << ", capacity=" << source.capacity() << '\n';
  Vector<test> moved = std::move(source);
  std::cout << "이동 후 source: size=" << source.size() << ", capacity=" << source.capacity() << '\n';
  std::cout << "이동 후 moved: size=" << moved.size() << ", capacity=" << moved.capacity() << '\n';
  printf("\n");

  // 2. 이동 할당 테스트
  std::cout << "--- 2. 이동 할당 연산자 테스트 ---" << '\n';
  Vector<test> target;
  target.reserve(50);
  for (int i = 0; i < 50; i++) {
    target.emplace_back(i * 2, i * 2.5, "Target" + std::to_string(i));
  }
  std::cout << "target 할당 전: size=" << target.size() << ", capacity=" << target.capacity() << '\n';
  target = std::move(moved);
  std::cout << "target 할당 후: size=" << target.size() << ", capacity=" << target.capacity() << '\n';
  std::cout << "moved 할당 후: size=" << moved.size() << ", capacity=" << moved.capacity() << '\n';
  printf("\n");

  // 3. 안정성 테스트 - 이동 후 원본 접근 안전성
  std::cout << "--- 3. 이동 후 원본 안전성 테스트 ---" << '\n';
  std::cout << "moved가 유효한 상태인가? size=" << moved.size() << '\n';
  moved.push_back(test(999, 999.9, "SafetyTest"));
  std::cout << "moved에 요소 추가 후: size=" << moved.size() << '\n';
  printf("\n");

  // 4. 데이터 무결성 테스트
  std::cout << "--- 4. 데이터 무결성 테스트 ---" << '\n';
  bool data_valid = true;
  for (int i = 0; i < std::min(10, (int)target.size()); i++) {
    if (target[i].int_number != i) {
      data_valid = false;
      break;
    }
  }
  std::cout << "target 데이터 무결성: " << (data_valid ? "PASS" : "FAIL") << '\n';
  printf("\n");

  // 5. 자기 할당 테스트 (안정성)
  std::cout << "--- 5. 자기 할당 테스트 ---" << '\n';
  std::cout << "target 자기할당 전: size=" << target.size() << '\n';
  target = std::move(target);
  std::cout << "target 자기할당 후: size=" << target.size() << '\n';
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
  Test10(v);
  Test11(v);
  Test12(v);

  return 0;
}