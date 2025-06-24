/*
vector는 동적 크기의 배열을 담는 컨테이너.

size() - 벡터의 길이
at(번호) - 번호에 해당하는 원소
push_back(원소), pop_back() - 맨 뒤에 값을 추가 및 제거
insert(), erase() - 반복자로 중간에 값을 삽입 및 제거

범위를 벗어나는 원소를 찾을 경우 "out_of_range" 오류가 발생.
*/
#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

// template으로 어떤 자료형의 vector라도 출력할 수 있도록 함수 구현
template<typename T>
void print_vector_all(const vector<T>& v)
{
  for(const T& i : v)
  {
    cout << "v : " << i << endl;
  }
}


int main()
{
  // vector 정의
  std::vector<int> v0;                    // 단순 정의
  std::vector<int> v1(10);                // 크기를 설정
  std::vector<int> v2(10, 111);           // 크기와 초기값을 설정
  std::vector<int> v4 = {0, 1, 2, 3, 4};  // 유니폼 초기화

  // iterator로 vector 순회
  std::vector<int>::iterator it = v2.begin();
  for (it; it != v2.end(); ++it)
  {
    cout << "v2의 값 : " << *it << endl;
  }
  cout << endl;


  // push_back로 맨 뒤에 원소를 추가
  for (int i=0; i<5; ++i)
    v2.push_back(i);


  print_vector_all(v2);
  cout << endl;


  // pop로 맨 뒤에 원소를 제거
  for (int i=0; i<10; ++i)
    v2.pop_back();


  // 원소 제거 후 출력
  for (it = v2.begin(); it != v2.end(); ++it)
  {
    cout << "원소가 [제거된] v2의 값 : " << *it << endl;
  }
  cout << endl;


  // insert(반복자, 값) - 반복자 앞으로 값을 추가
  v2.insert(v2.begin()+2, 25); // v2[2] 앞에 25 추가

  // 먼저 v2.begin()+2 가 추가되었기 때문에 반복자가 밀려서 의도하지 않은 위치에 값이 추가될 수 있다
  v2.insert(v2.begin()+4, 25);
  v2.insert(v2.begin()+6, 25);
  // insert 후 출력
  for (it = v2.begin(); it != v2.end(); ++it)
  {
    cout << "원소 [insert] v2의 값 : " << *it << endl;
  }
  cout << endl;

  // erase(반복자) 해당 위치의 요소를 제거
  v2.erase(v2.begin()+2); // v2[2] 앞에 25 추가

  // 먼저 v2.begin()+2 가 제어되었기 때문에 반복자가 앞당겨져서 의도하지 않은 값이 사라질 수 있다
  v2.erase(v2.begin()+4);
  v2.erase(v2.begin()+6);
  // erase 후 출력
  for (it = v2.begin(); it != v2.end(); ++it)
  {
    cout << "원소 [erase] v2의 값 : " << *it << endl;
  }
  cout << endl;


  // 상수 반복자 - 반복자가 가리키는 원소의 값을 변경할 수 없다
  // 증감 연산자로 역참조를 하는 등의 연산은 가능
  vector<int>::const_iterator cit = v2.cbegin();
  cout << "++cit : " << *(++cit) << endl;
  cout << endl;
  


  // 리버스 반복자 - rbegin은 원소의 마지막, rend는 첫원소의 이전 주소
  for (vector<int>::reverse_iterator rit = v2.rbegin(); rit != v2.rend(); rit++)
  {
    cout << "리버스 반복자 : " << *rit << endl;
  }
  cout << endl;



  // 범위를 벗어나면 "out_of_range" 오류를 던진다
  cout << "v1의 사이즈 : " << v1.size() << endl;
  try
  {
    cout << v1.at(1111) << endl;
  }
  catch (const std::out_of_range& e) 
  {
      std::cerr << "오류 발생: vector 범위를 벗어난 접근입니다." << std::endl;
      std::cerr << "C++ 예외 메시지: " << e.what() << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  

  return 0;
}