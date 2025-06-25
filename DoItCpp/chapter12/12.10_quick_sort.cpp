/*
Quick Sort (퀵 정렬)
퀵 정렬은 평균적으로 O(n log n), 최대로는 O(n^2)의 시간복잡도로 정렬을 수행한다

해당 함수는 헤더 파일 <algorithm>에 sort() 함수로 사용할 수 있다.
또한 임의 접근이 가능한 vector, array, deque 등 컨테이너에 사용이 가능하다 
*/

#include <iostream>
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
  return lhs.age < rhs.age;
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
  cout << " 추가된 난수 [";
  for(int i=0; i<20; ++i)
  {
    v.push_back(dist(mt_rand));
    cout << v.back() << ", ";
  }
  cout << ']' << endl << endl;

  // algorithm 라이브러리에서 제공하는 퀵 정렬 함수 sort
  sort(v.begin(), v.end());
  
  cout << "오름차순 정렬된 난수 [";
  for(const int& i : v)
  {
    cout << i << ", ";
  }
  cout << ']' << endl;


  
  // 내림차순은 3번째 매개변수에 greater<type>() 을 추가
  sort(v.begin(), v.end(), greater<int>());
  
  cout << "내림차순 정렬된 난수 [";
  for(const int& i : v)
  {
    cout << i << ", ";
  }
  cout << ']' << endl;
  

  // 사용자 정의 규칙
  Person kim = {"김짱구", 5};
  Person lee = {"이철수", 2};
  Person sin = {"신훈이", 16};
  Person bong = {"봉미선", 35};

  vector<Person> person = {kim, lee, sin, bong};
  cout << "\n기존 Person 정렬" << endl;
  for (const Person& p : person)
  {
    cout << p.name << "의 나이는 " << p.age << endl;
  }
  sort(person.begin(), person.end(), compare);

  cout << "\nPerson 정렬" << endl;
  for (const Person& p : person)
  {
    cout << p.name << "의 나이는 " << p.age << endl;
  }


  return 0;
}

