/*
리스트 사용법과 이해 

리스트와 벡터는 유사하나 내부적으로 벡터는 연속된 메모리에 데이터를 저장
리스트는 요소가 다음 요소를 가리키는 포인터를 가지고 있다.
그렇기에 벡터는 임의의 요소 접근이 매우 빠르나 리스트는 임의 접근이 불가능하며, 찾아가야한다.

벡터는 원소 접근이 자주 발생하고 수정할 경우에 유리하며, 리스트는 삽입과 삭제가 빈번할 때 유리하다
하지만 이중 연결 리스트는 next, pre의 포인터만 변경하면 되기에 삽입에 유리하다
*/
#include <iostream>
#include <list>
using namespace std;

template<typename T>
void print_list_all(list<T>& li)
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
  list<int> li;

  // push_back() - 리스트 뒤에 값을 추가
  cout << "push_back() - 리스트 뒤에 값을 추가" << endl;
  li.push_back(100);
  li.push_back(101);
  li.push_back(102);
  print_list_all(li);

  // 리스트 중간인 두번째에 9999를 삽입 
  cout << "리스트 중간인 두번째에 9999를 삽입" << endl;
  list<int>::iterator it = li.begin();
  li.insert(++it, 9999);
  print_list_all(li);
  

  // push_front() - 리스트 앞에 값을 추가
  cout << "push_front() - 리스트 앞에 값을 추가" << endl;
  li.push_front(99);
  li.push_front(98);
  li.push_front(97);
  print_list_all(li);

  // 리스트 중간 제거 - 마지막에서 두번째 노드 제거 
  cout << "리스트 중간 제거 - 마지막에서 두번째 노드 제거" << endl;
  it = li.end();
  li.erase(--it);
  print_list_all(li);


  // pop_front() - 리스트 앞 요소 제거
  cout << "pop_front() - 리스트 앞 요소 제거" << endl;
  li.pop_front();
  li.pop_front();
  print_list_all(li);

  // pop_back() - 리스트 뒤 요소 제거
  cout << "pop_back() - 리스트 뒤 요소 제거" << endl;
  li.pop_back();
  li.pop_back();
  print_list_all(li);

  
  // empty() - 리스트가 비어있는지 확인
  cout << (li.empty() ? "리스트 비어있다" : "리스트 요소있다") << endl;

  // size() - 리스트 크기 확인
  cout << "li 사이즈 : " << li.size() << endl;

  cout << "리스트의 첫 원소 : " << li.front() << endl;
  cout << "리스트의 끝 원소 : " << li.back() << endl;

  // list 반복자로 요소 출력
  for(list<int>::iterator it = li.begin(); it != li.end(); ++it)
  {
    cout << "list li --> " << *it << endl;
  }




  return 0;
}