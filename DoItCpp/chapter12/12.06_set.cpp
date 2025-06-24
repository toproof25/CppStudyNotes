/*
set은 중복된 값을 허용하지 않는 데이터 구조로 정렬하여 데이터를 관리하는 컨테이너
내부적으로는 이진 탐색으로 정렬이 되어 데이터를 저장하기에 검색 속도가 O(log n)에 가깝다

insert로 데이터를 삽입하고, erase로 데이터 제거 및 clear로 모든 데이터를 제거한다
find로 데이터를 찾을 수 있다
*/
#include <iostream>
#include <set>
using namespace std;

template <typename T>
void print_set_all(const set<T>& mySet)
{
  // 상수로 참조하는 mySet을 파라미터로 가져오기에 상수 iterator로 데이터 순회
  // 또한 의존 타입으로 컴파일러에게 타입을 명시
  for(typename set<T>::const_iterator it = mySet.cbegin(); it != mySet.cend(); ++it)
  {
    cout << "mySet의 원소 : " << *it << endl;
  }
  cout << endl;
}

int main()
{
  //set<string> mySet;
  set<string> mySet = {"김철수", "김짱구"}; // 유니폼 초기화

  // set.insert(값) - set에 데이터를 추가한다
  // 중복 데이터를 허용하지 않기에 중복된 값을 추가해도 변하지 않음
  mySet.insert("김짱구");
  mySet.insert("신짱구");
  mySet.insert("박명수");

  // insert의 반환은 std::pair<iterator, bool> 으로 interator는 삽인된 원소 반복자, bool인 삽입 여부를 의미
  std::pair<set<string>::iterator, bool> result = mySet.insert("김철수");
  if (!result.second)
    cout << "[김철수]는 이미 존재하는 데이터입니다" << endl;

  result = mySet.insert("김유리");
  if (!result.second)
    cout << "[김유리]는 이미 존재하는 데이터입니다" << endl;

  print_set_all(mySet);




  // set.find(찾을값) - 원소를 찾고, 원소가 없으면 end()를 반환
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




  // set.size() - set 사이즈 구하기
  cout << "mySet 사이즈 : " << mySet.size() << endl << endl;




  // erase(제거할 값) - 부호 없는 정수가 반환되어 확인 가능
  int erase_result = mySet.erase("김유리");
  if (erase_result)
    cout << "김유리가 제거되었습니다" << endl;

  erase_result = mySet.erase("김철수수수수수수");
  if (!erase_result)
    cout << "김철수수수수수수는 원소에 존재하지 않아 제거 실패" << endl;

  cout << "erase 후 mySet 사이즈 : " << mySet.size() << endl;
  print_set_all(mySet); 

  // 모든 원소 제거
  mySet.clear();
  cout << "clear 후 mySet 사이즈 : " << mySet.size() << endl;

  return 0;
}