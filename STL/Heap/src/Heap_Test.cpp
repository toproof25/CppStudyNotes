#include <iostream>
#include <string>
#include "../inc/Heap.hpp"

struct test
{
  int age;
  double height;
  double weight;

  test(int age, double height, double weight) : age(age), height(height), weight(weight) {}

  bool operator>=(const test& other)
  {
    // 체중 / (키 * 키)
    double bmi = weight / (height * height);
    double other_bmi = other.weight / (other.height * other.height);

    return bmi >= other_bmi;
  }
  bool operator<=(const test& other)
  {
    // 체중 / (키 * 키)
    double bmi = weight / (height * height);
    double other_bmi = other.weight / (other.height * other.height);

    return bmi <= other_bmi;
  }

  // << 연산자 오버라이드
  friend std::ostream& operator<<(std::ostream& os, const test& other) 
  {
    double other_bmi = (other.weight / (other.height * other.height))*10000;
    os << std::to_string(other_bmi);
    return os; 
  }
};

Heap<int> h;
Heap<int, 1> h2;
Heap<test> struct_h;

void Test_push()
{
  std::cout << "************************" << "< push Test >" << "************************" << '\n';
  h.printHeap();

  h.push(10);
  h.printHeap();

  h.push(6);
  h.printHeap();

  h.push(5);
  h.printHeap();

  h.push(2);
  h.printHeap();

  h.push(1);
  h.printHeap();

  h.push(7);
  h.printHeap();

  h.push(8);
  h.printHeap();

  h.push(9);
  h.printHeap();

  h.push(3);
  h.printHeap();

  h.push(4);
  h.printHeap();
  std::cout << '\n' << '\n';
}

void Test_pop()
{
  std::cout << "************************" << "< pop Test >" << "************************" << '\n';
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();
  std::cout << '\n' << '\n';
}

void Test_push_h2()
{
  std::cout << "************************" << "< push Test >" << "************************" << '\n';
  h2.printHeap();

  h2.push(1);
  h2.printHeap();

  h2.push(6);
  h2.printHeap();

  h2.push(5);
  h2.printHeap();

  h2.push(2);
  h2.printHeap();

  h2.push(10);
  h2.printHeap();

  h2.push(7);
  h2.printHeap();

  h2.push(8);
  h2.printHeap();

  h2.push(9);
  h2.printHeap();

  h2.push(3);
  h2.printHeap();

  h2.push(4);
  h2.printHeap();
  std::cout << '\n' << '\n';
}

void Test_pop_h2()
{
  std::cout << "************************" << "< pop Test >" << "************************" << '\n';
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();
  std::cout << '\n' << '\n';
}

void Test_push_struct_h()
{
  std::cout << "************************" << "< push Test >" << "************************" << '\n';
  struct_h.printHeap();

  // 첫 번째 데이터 (기존)
  struct_h.push({1, 171.6, 65.35});
  struct_h.printHeap();

  // 추가 데이터: 나이(정수), 키(실수), 몸무게(실수)
  struct_h.push({25, 182.4, 78.2});  // 20대 남성 평균 이상
  struct_h.printHeap();

  struct_h.push({32, 164.2, 52.8});  // 30대 여성 평균
  struct_h.printHeap();

  struct_h.push({12, 155.0, 45.5});  // 청소년기 학생
  struct_h.printHeap();

  struct_h.push({45, 175.8, 82.1});  // 40대 중년 남성
  struct_h.printHeap();

  struct_h.push({28, 160.5, 49.3});  // 20대 여성
  struct_h.printHeap();

  struct_h.push({58, 168.2, 70.5});  // 50대 남성
  struct_h.printHeap();

  struct_h.push({7, 122.4, 24.8});   // 초등학교 저학년 어린이
  struct_h.printHeap();

  struct_h.push({36, 173.1, 68.4});  // 30대 남성
  struct_h.printHeap();

  struct_h.push({52, 158.7, 58.2});  // 50대 여성
  struct_h.printHeap();

  struct_h.push({19, 178.6, 72.9});  // 10대 후반 남성
  struct_h.printHeap();

  std::cout << '\n' << '\n';
}

void Test_pop_struct_h()
{
  std::cout << "************************" << "< pop Test >" << "************************" << '\n';
  struct_h.printHeap();

  struct_h.pop();
  struct_h.printHeap();

  struct_h.pop();
  struct_h.printHeap();

  struct_h.pop();
  struct_h.printHeap();

  struct_h.pop();
  struct_h.printHeap();

  struct_h.pop();
  struct_h.printHeap();

  struct_h.pop();
  struct_h.printHeap();

  struct_h.pop();
  struct_h.printHeap();

  struct_h.pop();
  struct_h.printHeap();

  struct_h.pop();
  struct_h.printHeap();

  struct_h.pop();
  struct_h.printHeap();
  std::cout << '\n' << '\n';
}


int main()
{
  Test_push();
  Test_pop();

  Test_push_h2();
  Test_pop_h2();

  Test_push_struct_h();
  Test_pop_struct_h();



  return 0;
}