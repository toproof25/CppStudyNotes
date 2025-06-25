/*
Quick Sort (퀵 정렬)
퀵 정렬은 평균적으로 O(n log n), 최대로는 O(n^2)의 시간복잡도로 정렬을 수행한다

해당 함수는 헤더 파일 <algorithm>에 sort() 함수로 사용할 수 있다.
또한 임의 접근이 가능한 vector, array, deque 등 컨테이너에 사용이 가능하다 
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <random>
using namespace std;

// 사용자 정의 정렬 규칙 (나이순)
struct Person
{
  string name;
  int age;
};

// 나이순으로 정렬
bool compare(const Person& lhs, const Person& rhs)
{
  return lhs.age > rhs.age;
};


int main()
{
  string unstable_name="";
  string stable_name="";

  // 불안정 정렬
  vector<Person> person = {
      {"루피", 35},
      {"김짱구", 35},
      {"신형만", 35}, 
      {"이철수", 35},
      {"신훈이", 35},
      {"봉미선", 35}, 
      {"조로", 35},
      {"뷟철수", 35}, 
      {"방맹구", 1},
      {"흰둥이", 23},
      {"우솝", 35}
  };

  cout << "\n기존 Person 정렬" << endl;
  for (const Person& p : person)
  {
    cout << p.name << "의 나이는 " << p.age << endl;
  }
  cout << "--------------------------------" << endl;
  
  // 불안정 정렬
  sort(person.begin(), person.end(), compare);

  cout << "\nPerson 불안정 정렬" << endl;
  for (const Person& p : person)
  {
    cout << "[" << p.name << "]의 나이는 " << p.age << endl;
    unstable_name += p.name.substr(0, 3) + " -> ";
  }
  person.clear();


  // 안정 정렬 - stable_sort()
  person = {
      {"루피", 35},
      {"김짱구", 5},
      {"신형만", 35}, 
      {"이철수", 2},
      {"신훈이", 16},
      {"봉미선", 35}, 
      {"조로", 35},
      {"뷟철수", 2}, 
      {"방맹구", 1},
      {"흰둥이", 23},
      {"우솝", 35}
  };
  
  // 안정 정렬
  stable_sort(person.begin(), person.end(), compare);

  cout << "\nPerson 안정 정렬" << endl;
  for (const Person& p : person)
  {
    cout << "[" << p.name << "]의 나이는 " << p.age << endl;
    stable_name += p.name.substr(0, 3) + " -> ";
  }

  // 불안정 정렬와 안정 정렬 비교
  cout << "\n불안정 정렬된 성 출력 : " << unstable_name << endl;
  cout << "  안정 정렬된 성 출력 : " << stable_name << endl;

  return 0;
}

