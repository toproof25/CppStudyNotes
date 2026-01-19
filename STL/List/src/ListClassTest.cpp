#include <iostream>
#include "../inc/List.hpp"

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


List<int> l;
List<test> l2;

void Test01_push_front()
{
  std::cout  << "******************" << " < push_front() 테스트 > " << "******************" << '\n';
  l.push_front(100); 
  l.printList();

  l.push_front(200); 
  l.printList();

  l.push_front(300); 
  l.printList();

  printf("\n\n");
}

void Test02_push_back()
{
  std::cout  << "******************" << " < push_back() 테스트 > " << "******************" << '\n';
  l.push_back(900); 
  l.printList();

  l.push_back(800); 
  l.printList();

  l.push_back(700); 
  l.printList();

  printf("\n\n");
}

void Test03_pop_front()
{
  std::cout  << "******************" << " < pop_front() 테스트 > " << "******************" << '\n';
  l.pop_front(); 
  l.printList();

  l.pop_front(); 
  l.printList();

  l.pop_front(); 
  l.printList();

  printf("\n\n");
}

void Test04_pop_back()
{
  std::cout  << "******************" << " < pop_back() 테스트 > " << "******************" << '\n';
  l.pop_back(); 
  l.printList();

  l.pop_back(); 
  l.printList();

  l.pop_back(); 
  l.printList();

  printf("\n\n");
}


void Test05_emplace_front()
{
  std::cout  << "******************" << " < emplace_front() 테스트 > " << "******************" << '\n';
  l2.emplace_front(1, 1.234, "emplace_front"); 
  l2.printList();

  l2.emplace_front(100, 10.2304, "맨 앞에 삽입합니다"); 
  l2.printList();

  l2.emplace_front(100*5, 1.234 * 123, "인자로 보내서 객체를 생성합니다"); 
  l2.printList();

  printf("\n\n");
}

void Test06_emplace_back()
{
  std::cout  << "******************" << " < emplace_back() 테스트 > " << "******************" << '\n';
  l2.emplace_back(99, 99.99, "emplace_back"); 
  l2.printList();

  l2.emplace_back(999, 999.999, "맨 뒤에 삽입합니다"); 
  l2.printList();

  l2.emplace_back(9999, 9999.99999, "인자를 보내서 객체를 생성합니다"); 
  l2.printList();

  printf("\n\n");
}

void Test07_iterator()
{
  std::cout  << "******************" << " < iterator() 테스트 > " << "******************" << '\n';
  l.push_front(999);
  l.push_front(888);
  l.push_front(777);
  l.push_front(666);
  l.push_front(555);
  l.push_front(444);
  l.push_front(333);

  std::cout << "\n***** l iterator\n";
  List<int>::iterator it = l.begin();
  int i=0;
  for (; it != l.end(); ++it)
  {
    std::cout << ++i << ". iterator : " << *it << '\n';   
  }

  std::cout << "\n\n***** l2 iterator\n";
  List<test>::iterator it2 = l2.begin();
  i=0;
  for (; it2 != l2.end(); ++it2)
  {
    std::cout << ++i << ". l2 : " << *it2 << '\n';   
  }

  printf("\n\n");
}

void Test08_ReverseIterator()
{
  std::cout  << "******************" << " < reverse iterator() 테스트 > " << "******************" << '\n';

  std::cout << "\n***** l reverse iterator\n";
  List<int>::reverse_iterator it = l.rbegin();
  int i=0;
  for (; it != l.rend(); ++it)
  {
    std::cout << ++i << ". reverse iterator : " << *it << '\n';   
  }

  std::cout << "\n\n***** reverse l2 iterator\n";
  List<test>::reverse_iterator it2 = l2.rbegin();
  i=0;
  for (; it2 != l2.rend(); ++it2)
  {
    std::cout << ++i << ". reverse l2 : " << *it2 << '\n';   
  }

  printf("\n\n");
}

void Test09_insert()
{
  std::cout  << "******************" << " < insert() 테스트 > " << "******************" << '\n';
  List<int>::iterator it = l.begin();

  l.insert(it, 0); 
  l.printList();

  ++it;
  it++;
  
  l.insert(it, 0); 
  l.printList();



  printf("\n\n");
}

void Test10_emplace()
{
  std::cout  << "******************" << " < emplace() 테스트 > " << "******************" << '\n';
  List<test>::iterator it = l2.begin();

  // 첫번재에 삽입
  l2.insert(it, {0, 0.0, "중간 삽입 emplace 1"}); 
  l2.printList();


  ++it;
  it++;
  
  l2.insert(it, {2, 22.22, "중간 삽입 emplace 222"}); 
  l2.printList();

  printf("\n\n");
}


void Test11_erase()
{
  std::cout  << "******************" << " < erase() 테스트 > " << "******************" << '\n';
  List<int>::iterator it = l.begin();

  l.printList();

  l.erase(++it); 
  l.printList();

  ++it;
  it++;
  
  l.erase(it); 
  l.printList();



  printf("\n\n");
}

void Test12_remove()
{
  std::cout  << "******************" << " < remove() 테스트 > " << "******************" << '\n';
  List<int>::iterator it = l.begin();

  l.insert(++it, 0);
  l.insert(++it, 0);
  l.insert(++it, 0);

  l.printList();

  l.remove(0);
  
  l.printList();

  printf("\n\n");
}




int main()
{
  Test01_push_front();
  Test02_push_back();

  Test03_pop_front();
  Test04_pop_back();

  Test05_emplace_front();
  Test06_emplace_back();

  Test07_iterator();
  Test08_ReverseIterator();

  Test09_insert();
  Test10_emplace();
  Test11_erase();

  Test12_remove();
  
  
  
  

  return 0;
}