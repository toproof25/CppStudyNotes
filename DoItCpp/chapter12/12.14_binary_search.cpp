/*
이진탐색
find는 선형 탐색으로 처음부터 끝까지 값을 확인하면서 순차적으로 찾는다
그럴 경우 데이터가 커질 수록 시간이 늘어나기에 이진 탐색을 사용하면 좋다
*/
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
using namespace std;

// == 연산자 오버로딩
struct Person
{
  string name;
  int age;

  // binary_search에서 사용할 때는 '<' or '>' 연산자 오버로딩을 해야 한다
  // 또한 정렬된 상태도 오름차순, 내림차순에 맞게 정렬이 되어있어야 한다
  bool operator<(const Person& other) const {
      return name < other.name;
  }
};

// 이진탐색 오버로딩에 맞게 똑같이 '>'으로 내림차순으로 정렬
bool person_compare(const Person& lhs, const Person& rhs)
{
  return lhs.name < rhs.name;
}


vector<Person> person = {
    {"루피", 35},
    {"김짱구", 35},
    {"신형만", 35}, 
    {"이철수", 35},
    {"신훈이", 35}
};



int main()
{
  // 시드 설정과 랜덤 변수 설정
  random_device rng;
  mt19937_64 mt_rand(rng());

  // 1부터 1000 사이의 정수 분포
  uniform_int_distribution<int> dist(1, 1000);

  // 난수를 벡터에 저장
  vector<int> v;
  cout << "추가된 난수 [";
  for(int i=0; i<20; ++i)
  {
    v.push_back(dist(mt_rand));
    cout << v.back() << ", ";
  }
  cout << ']' << endl << endl;

  // 이진 탐색은 정렬된 상태에서 가능
  sort(v.begin(), v.end());

  int target;
  cout << "찾을 숫자 입력 : ";
  cin >> target;
  
  // binary_search는 데이터를 찾고, bool값을 반환
  bool result = binary_search(v.begin(), v.end(), target);
  if (result)
    cout << "target 데이터가 존재합니다" << endl;
  else
    cout << "target 데이터가 앖습니다" << endl;


  // binary search는 구조체, 클래스에서 '> or <' 연산자 오버로딩
  sort(person.begin(), person.end(), person_compare);
  result = binary_search(person.begin(), person.end(), (Person){"신훈이", 12});
  if (result) 
    cout << "person에 [신훈이] 데이터가 존재합니다." << endl;
  else
    cout << "person에 [신훈이] 데이터가 없습니다" << endl;

  return 0;
}
