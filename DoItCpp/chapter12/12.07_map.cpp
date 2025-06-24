/*
map은 키와 값이 쌍으로 이루어진 중복을 허용하지 않는 데이터 구조이다
*/
#include <iostream>
#include <map>
using namespace std;


int main()
{
  //map<string, double> myMap;
  map<string, double> myMap = { make_pair("김철수", 170.5), make_pair("신짱구", 124.1) };

  // map.insert(std::make_pair(키, 값)) - 쌍을 만든 후 map에 삽입
  myMap.insert(std::make_pair("김유리", 190.2));

  // 또는 중괄호로 묶어서 {키, 값} 삽입이 가능하다
  // insert의 반환은 std::pair<map<string, double>::iterator, bool>
  std::pair<map<string, double>::iterator, bool> insert_iterator = myMap.insert({"박맹구", 260.515});
  cout << "추가된 원소의 Key : " << insert_iterator.first->first << endl;
  cout << "추가된 원소의 Value : " << insert_iterator.first->second << endl;
  cout << "추가 성공 여부 : " << insert_iterator.second << endl;

  // map은 operator[] 으로 값을 찾을 수 있다
  cout << "김유리의 키는 " << myMap["김유리"] << "입니다" << endl;
  cout << "김철수의 키는 " << myMap["김철수"] << "입니다" << endl;

  // 없는 원소를 참조하려는 경우 value가 기본값으로 Key-Value가 되어 추가된 후 참조됨
  cout << "원장선생님의 키는 " << myMap["원장선생님"] << "입니다" << endl << endl;




  // find를 통해서 찾기가 가능하며, 반환은 해당 map의 iterator
  map<string, double>::iterator find_iterator = myMap.find("원장선생님");
  cout << "찾기 성공 여부 : " << (find_iterator == myMap.end() ? "map에 없습니다" : "찾았습니다") << endl;
  cout << "찾은 사람 : " << find_iterator->first << "의 키는 " << find_iterator->second << endl;

  // 찾는 키가 없다면 end() 를 반환한다
  find_iterator = myMap.find("나미리선생님");
  cout << "나미리선생님 찾기 성공 여부 : " << (find_iterator == myMap.end() ? "map에 없습니다" : "찾았습니다") << endl << endl;




  // erase를 이용하여 제거
  int erase_result = myMap.erase("김철수");
  cout << (erase_result == 1 ? "김철수를 제거했습니다" : "김철수를 제거하지 못했습니다") << endl;

  // 없는 키라면 0을 반환
  erase_result = myMap.erase("원장선생님");
  cout << (erase_result == 1 ? "원장선생님를 제거했습니다" : "원장선생님를 제거하지 못했습니다") << endl << endl;


  // map의 원소를 출력
  for(map<string, double>::iterator it = myMap.begin(); it != myMap.end(); ++it)
  {
    cout << "myMap의 원소 [" << it->first << "] - " << (*it).second << endl;
  }


  return 0;
}