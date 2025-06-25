/*
단일 연결 리스트 forward list는 list와 달리 다음 노드를 가리키는 포인터만 존재한다
리스트와 차이점은 push_back, pop_back, size 등을 사용할 수 없다
그러나 리스트보다 노드의 포인터가 하나 적기에(pre) 메모리에 유리하다
*/
#include <iostream>
#include <forward_list>
using namespace std;

template<typename T>
void print_list_all(forward_list<T>& li)
{
  for (const T& value : li)
  {
    cout << "li : " << value << endl;
  }
  cout << endl;
}


int main()
{
  // 리스트 변수 선언
  forward_list<int> li;

  // push_front() - 리스트 앞에 값을 추가
  cout << "push_front() - 리스트 앞에 값을 추가" << endl;
  li.push_front(99);
  li.push_front(98);
  li.push_front(97);
  print_list_all(li);

  // 리스트 중간인 두번째에 9999를 삽입 
  cout << "단일 연결 리스트 중간 삽입 -  두번째에 9999를 삽입" << endl;
  forward_list<int>::iterator it = li.begin();
  li.insert_after(it, 9999);
  print_list_all(li);


  // 리스트 중간 요소 제거 - 세번째 요소 제거 
  cout << "리스트 중간 요소 제거 - 세번째 요소 제거" << endl;
  it = li.begin();
  li.erase_after(++it);
  print_list_all(li);


  // pop_front() - 리스트 앞 요소 제거
  cout << "pop_front() - 리스트 앞 요소 제거" << endl;
  li.pop_front();
  li.pop_front();
  print_list_all(li);


  // empty() - 리스트가 비어있는지 확인
  cout << (li.empty() ? "리스트 비어있다" : "리스트 요소있다") << endl;


  cout << "리스트의 첫 원소 : " << li.front() << endl;

  // list 반복자로 요소 출력
  for(forward_list<int>::iterator it = li.begin(); it != li.end(); ++it)
  {
    cout << "list li --> " << *it << endl;
  }




  return 0;
}