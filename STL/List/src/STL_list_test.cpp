#include <iostream>
#include <list>
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


void Test01(std::list<int>& l)
{
  std::cout << "*************************" << " < 1. 객체 생성 및 크기 조절 테스트 > " << "*************************" << '\n';
  
  std::cout << "최초 정의 후 list 객체 size : " << std::to_string(l.size()) << '\n';
  l.push_back(999);
  std::cout << "테스트 데이터 삽입 후 list 객체 size : " << std::to_string(l.size()) << '\n';
  printf("\n\n");
}
void Test02(std::list<int>& l)
{
  std::cout << "*************************" << " < 2. push_front(), push_back() 객체 데이터 삽입 테스트 > " << "*************************" << '\n';
  
  for (int i=0; i<5; ++i) l.push_back(1);
  for (int i=100; i<105; ++i) l.push_front(i);
  std::cout << "push_front(), push_back() 후 list 객체 size : " << std::to_string(l.size()) << '\n';

  int i=1;
  for (auto it = l.begin(); it!=l.end(); ++it) std::cout << i++ << ". l : " << *it << '\n';

  printf("\n\n");
}
void Test03(std::list<int>& l)
{
  
  std::cout << "*************************" << " < 3. insert() 객체 데이터 중간 삽입 테스트 > " << "*************************" << '\n';
  
  l.insert(++l.begin(), 123456);
  std::cout << "insert() 후 list 객체 size : " << std::to_string(l.size()) << '\n';

  int i=0;
  for (auto it = l.begin(); it!=l.end(); ++it) 
  {
    std::cout << ++i << ". l : " << *it << (i==2 ? " <-- 삽입한 부분" : "") << '\n';
  }
  printf("\n\n");
}
void Test04(std::list<int>& l)
{
  std::cout << "*************************" << " < 4. pop_front(), pop_back() 객체 데이터 제거 테스트 > " << "*************************" << '\n';
  
  for (int i=0; i<3; ++i) l.pop_front();
  for (int i=0; i<3; ++i) l.pop_back();
  std::cout << "pop_front(), pop_back() 후 list 객체 size : " << std::to_string(l.size()) << '\n';

  int i=1;
  for (auto it = l.begin(); it!=l.end(); ++it) std::cout << i++ << ". l : " << *it << '\n';

  printf("\n\n");
}
void Test05(std::list<int>& l)
{
  std::cout << "*************************" << " < 5. erase() 객체 데이터 중간 제거 테스트 > " << "*************************" << '\n';
  

  std::cout << "제거 전 상태 size : " << std::to_string(l.size()) << '\n';
  int i=0;
  for (auto it = l.begin(); it!=l.end(); ++it) std::cout << ++i << ". l : " << *it << (*it == 1 ? " <- 제거할 부분" : "") << '\n';

  for (auto eit = l.begin(); eit != l.end();)
  {
    if (*eit == 1) eit = l.erase(eit);
    else ++eit;
  }
  std::cout << "erase() 후 list 객체 size : " << std::to_string(l.size()) << '\n' << '\n';

  i=0;
  for (auto it = l.begin(); it!=l.end(); ++it) std::cout << ++i << ". l : " << *it << '\n';

  printf("\n\n");
}
void Test06(std::list<int>& l)
{
  std::cout << "*************************" << " < 6. remove() 특정 객체 요소 모두 제거 > " << "*************************" << '\n';

  for (int i=0; i<20; ++i) 
  {
    if((i&1)==1) l.push_back(1); 
    else l.push_front(1);
  }

  std::cout << "제거 전 상태 size : " << std::to_string(l.size()) << '\n';
  int i=0;
  for (auto it = l.begin(); it!=l.end(); ++it) std::cout << ++i << ". l : " << *it << (*it == 1 ? " <- 제거할 부분" : "") << '\n';
  printf("\n\n");


  l.remove(1);
  std::cout << "remove() 후 list 객체 size : " << std::to_string(l.size()) << '\n' << '\n';

  i=0;
  for (auto it = l.begin(); it!=l.end(); ++it) std::cout << ++i << ". l : " << *it << '\n';

  printf("\n\n");
}

int main()
{ 
  std::list<int> l;

  Test01(l);
  Test02(l);
  Test03(l);
  Test04(l);
  Test05(l);
  Test06(l);


  return 0;
}