/**
 * @file vector_test.cpp
 * @date 2026-01-11
 * @brief vector 클래스 구현을 위한 기존 STL vector를 테스트
 * 
 */

#include <vector>
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


int main()
{ 
  // 정의
  std::vector<int> v;

  // [] 연산자, at를 이용하여 요소 접근

  // push_back(): 맨 뒤에 요소 삽입
  // front(): 맨 앞 요소 확인
  // capasity가 `0->1->2->4->8->6->16->32->64->128...` 으로 2의 거듭제곱으로 증가 (비트 연산을 이용하여 크기를 늘리는 듯)
  std::cout << "*************************" << " push_back() 함수 테스트 " << "*************************" << '\n';
  for (int i=0; i<10; ++i)
  {
    v.push_back(i);
    std::cout << i << ". vector push_back("<< i << "): " << v.front() <<  "| v.capacity = " << v.capacity() << '\n';
  }
  printf("\n\n");

  // pop_back(): 맨 뒤 요소 제거
  // back(): 맨 뒤 요소 확인
  // 크기를 줄여도 늘어난 capacity는 줄어들지 않음
  std::cout << "*************************" << " pop_back() 함수 테스트 " << "*************************" << '\n';
  for (int i=9; i>=0; --i)
  {
    std::cout << 49-i << ". vector pop_back(): " << v.back() << " | v.capacity = " << v.capacity() << '\n';
    v.pop_back();
  }
  printf("\n\n");

  // size(): 요소의 개수
  // clear():  요소를 모두 제거, 메모리 크기는 변하지 않음
  std::cout << "*************************" << " clear() 함수 테스트 " << "*************************" << '\n';
  v.clear();
  for (int i=0; i<=v.size(); ++i)
  {
    std::cout << i << ". vector 요소: " << v[i] << " | v.capacity = " << v.capacity() << '\n';
  }
  printf("\n\n");

  // insert(iterator, value): iterator위치에 value를 삽입 (중간 삽입)
  std::cout << "*************************" << " insert() 함수 테스트 " << "*************************" << '\n';
  for (int i=0; i<10; ++i)
  {
    std::cout << i << ". v.insert(v.begin(), "<< i << ") | v.capacity = " << v.capacity() << '\n';
    v.insert(v.begin(), i);
  }

  printf("\n");
  std::cout << "추가된 요소들 (순서)" << '\n';
  for (int i=0; i<10; ++i)
  {
    std::cout << i << ". v["<< i << "]: "<< v[i] << '\n';
  }
  printf("\n\n");

  std::cout << "*************************" << " erase() 함수 테스트 " << "*************************" << '\n';
  for (int i=0; i<7; ++i)
  {
    std::cout << i << ". v.erase(v.begin()+1): " << '\n';
    v.erase(v.begin()+1);
  }

  printf("\n");
  std::cout << "1번째만 제거된 요소들 (순서)" << '\n';
  for (int i=0; i<v.size(); ++i)
  {
    std::cout << i << ". v["<< i << "]: "<< v[i] << '\n';
  }
  printf("\n\n");


  std::vector<test> v2;
  std::cout << "*************************" << " emplace_back() 함수 테스트 " << "*************************" << '\n';
  for (int i=0; i<10; ++i)
  {
    v2.emplace_back(i+10, i+123.123, "hello");
    std::cout << i << ". v.emplace_back(int, double, back): " << v2.back() << '\n';
  }
  printf("\n\n");

  std::cout << "*************************" << " emplace() 함수 테스트 " << "*************************" << '\n';
  for (int i=0; i<10; ++i)
  {
    v2.emplace(v2.begin()+3, i+99, i+99.5421, "emplace");
    //std::cout << i << ". v.emplace(iterator, int, double, back): " << v2.back() << '\n';
  }

  printf("\n");
  std::cout << "중간에 삽입한 emplace" << '\n';
  for (int i=0; i<v2.size(); ++i)
  {
    std::cout << i << ". v2[" << i <<"]: " << v2[i] << '\n';
  }
  printf("\n\n");


  // reserve(i): i만큼 공간을 할당(메모리 공간만 할당)
  // shrink_to_fit(): size만큼 메모리 공간을 줄임 (빈 공간 제거) 
  std::cout << "*************************" << " reserve() 함수 테스트 " << "*************************" << '\n';
  std::cout << "v2.size: " << v2.size() << '\n';
  std::cout << "v2.capacity: " << v2.capacity() << '\n';
  v2.reserve(999);
  std::cout << "reserve v2.capacity: " << v2.capacity() << '\n';
  v2.shrink_to_fit();
  std::cout << "shrink_to_fit v2.capacity: " << v2.capacity() << '\n';

  printf("\n\n");


  

  return 0;
}