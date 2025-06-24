/*
multiset은 중복을 허용하는 set

*/
#include <iostream>
#include <set>
using namespace std;

template <typename T>
void print_set_all(const multiset<T>& mySet)
{
  // 상수로 참조하는 mySet을 파라미터로 가져오기에 상수 iterator로 데이터 순회
  // 또한 의존 타입으로 컴파일러에게 타입을 명시
  for(typename multiset<T>::const_iterator it = mySet.cbegin(); it != mySet.cend(); ++it)
  {
    cout << "mySet의 원소 : " << *it << endl;
  }
  cout << endl;
}

int main()
{
  //multiset<string> mySet;
  multiset<string> mySet = {"김철수", "김짱구"}; // 유니폼 초기화

  // multiset.insert(값) - multiset에 데이터를 추가한다
  // multiset은 중복값을 허용하기에 기존 "김철수", "김짱구"가 있어도 원소가 추가된다
  mySet.insert("김철수");
  mySet.insert("김유리");
  mySet.insert("김짱구");
  mySet.insert("신짱구");
  mySet.insert("박명수");
  print_set_all(mySet);

  // multiset.find(찾을값) - 원소를 찾고, 원소가 없으면 end()를 반환
  string find_value = "김철수";
  cout << "mySet.count("+find_value+") : " << mySet.count(find_value) << endl;
  if(mySet.find(find_value) != mySet.end())
    cout << "[" << find_value+"]는 mySet에 있습니다" << endl;
  else
    cout << "[" << find_value+"]는 mySet에 없습니다ㅜㅜㅜㅜ" << endl;

  find_value = "김철수철수수수";
  cout << "mySet.count("+find_value+") : " << mySet.count(find_value) << endl;
  if(mySet.find(find_value) != mySet.end())
    cout << "[" << find_value+"]는 mySet에 있습니다" << endl;
  else
    cout << "[" << find_value+"]는 mySet에 없습니다ㅜㅜㅜㅜ" << endl;
  cout << endl;


  // multiset.size() - multiset 사이즈 구하기
  cout << "mySet 사이즈 : " << mySet.size() << endl << endl;

  // erase(제거할 값)
  mySet.erase("김철수");
  mySet.erase("신짱구");
  cout << "erase 후 mySet 사이즈 : " << mySet.size() << endl;
  print_set_all(mySet); 

  // 모든 원소 제거
  mySet.clear();
  cout << "clear 후 mySet 사이즈 : " << mySet.size() << endl;

  return 0;
}