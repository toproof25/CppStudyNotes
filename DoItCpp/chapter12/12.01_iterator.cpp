/*
반복자는 컨테이너의 원소를 순회하는 객체로 C++ 컨테이너는 모두 각자만의 iterator를 가지고 있다
*/
#include <iostream>
#include <vector>
using namespace std;

int main()
{
  cout << "iterator 반복자에 대한 이해와 실습" << endl;

  vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  // C++ 컨테이너는 모두 각자의 반복자가 존재한다. "컨테이너<자료형>::iterator 변수명" 으로 정의
  // 반복자는 컨테이너의 원소를 순회하는 객체
  vector<int>::iterator it = v.begin();

  // 컨테이너의 begin(), end()는 각 시작과 마지막에 해당하는 iterator를 반환
  // 포인터와 비슷하게 연산자 오버로딩된 *을 이용하여 값을 역참조한다 
  cout << "iterator로 v의 시작을 구함 : " << *it << endl;  

  // end()의 위치는 컨테이너의 마지막 다음 위치를 반환한다. past-the-end. 그렇기에 -1을 해줘야 진짜 마지막 값이 나타난다
  cout << "iterator로 v의 마지막을 구함 : " << *((vector<int>::iterator)v.end()-1) << endl;

  // 시작 반복자부터 현재 반복자가 마지막 반복자가 아닐 경우에만 (즉 모든 원소를 순회)
  for(vector<int>::iterator iter=v.begin(); iter != v.end(); ++iter)
  {
    cout << "v의 원소 : " << *iter << endl;
  }


  return 0;
}