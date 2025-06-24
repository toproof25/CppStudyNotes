/*
deque 덱은 배열 기반이며 벡터 컨테이너와 유사하다
벡터는 메모리 하나하나에 원소를 저장하기에 공간이 없다면 메모리를 재할당하는 복사 저장이 발생한다
덱은 여러 메모리 블럭으로 나눠서 저장하여, 공간이 부족하면 새로운 메모리 블럭을 만들어서 연결하여 복사 저장이 발생하지 않는다
*/
#include <iostream>
#include <deque>
using namespace std;

template<typename T>
void print_deque_all(deque<T>& dq)
{
  for (const T& value : dq)
  {
    cout << "dq : " << value << endl;
  }
  cout << endl;
}


int main()
{
  // deque 변수 선언
  deque<int> dq;

  // push_back() - deque 뒤에 값을 추가
  cout << "push_back() - deque 뒤에 값을 추가" << endl;
  dq.push_back(100);
  dq.push_back(101);
  dq.push_back(102);
  print_deque_all(dq);

  // push_front() - deque 앞에 값을 추가
  cout << "push_front() - deque 앞에 값을 추가" << endl;
  dq.push_front(99);
  dq.push_front(98);
  dq.push_front(97);
  print_deque_all(dq);


  // pop_front() - deque 앞 요소 제거
  cout << "pop_front() - deque 앞 요소 제거" << endl;
  dq.pop_front();
  dq.pop_front();
  print_deque_all(dq);

  // pop_back() - deque 뒤 요소 제거
  cout << "pop_back() - deque 뒤 요소 제거" << endl;
  dq.pop_back();
  dq.pop_back();
  print_deque_all(dq);

  
  // empty() - deque가 비어있는지 확인
  cout << (dq.empty() ? "deque 비어있다" : "deque 요소있다") << endl;

  // size() - deque 크기 확인
  cout << "dq 사이즈 : " << dq.size() << endl;

  cout << "dq의 첫 원소 : " << dq.front() << endl;
  cout << "dq의 끝 원소 : " << dq.back() << endl;

  // deque 반복자로 요소 출력
  for(deque<int>::iterator it = dq.begin(); it != dq.end(); ++it)
  {
    cout << "deque dq --> " << *it << endl;
  }




  return 0;
}