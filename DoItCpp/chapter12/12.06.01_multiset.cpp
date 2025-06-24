/*
multiset은 중복을 허용하면서 정렬된 집합으로 데이터를 관리하는 컨테이너이다
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
  // 중복 데이터를 허용
  mySet.insert("김짱구");
  mySet.insert("신짱구");
  mySet.insert("박명수");
  mySet.insert("김유리");
  mySet.insert("김유리");
  mySet.insert("김유리");
  mySet.insert("김유리");
  mySet.insert("김유리");

  // multiset의 insert의 반환은 set<string>::iterator 으로 interator는 삽인된 원소 반복자
  multiset<string>::iterator result = mySet.insert("김철수");
  cout << "["+(*result)+"]를 추가하였습니다" << endl;

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




  // erase(제거할 값) - 부호 없는 정수가 반환되어 확인 가능
  // set과 차이점은 multiset의 경우 제거할 원소를 모두 제거한다 
  // (즉 "김유리"가 중복으로 5개가 있으면 5개가 모두 제거된다)
  cout << "mySet.count(""김유리"") : " << mySet.count("김유리") << endl;

  // iterator로 erase를 하면 원소 하나만 제거된다
  multiset<string>::iterator find_result = mySet.find("김유리");
  mySet.erase(find_result);
  cout << "erase에 iterator로 제거 (원소 하나만 제거).... ";
  cout << "mySet.count(""김유리"") : " << mySet.count("김유리") << endl;
  
  
  cout << "김유리 데이터로 모두 제거.....";
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