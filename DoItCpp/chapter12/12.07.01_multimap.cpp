/*
multimap은 키와 값이 쌍으로 이루어진 중복을 허용하는 데이터 구조이다
*/
#include <iostream>
#include <map>
using namespace std;


int main()
{
  //multimap<string, double> myMap;
  multimap<string, double> myMap = { make_pair("김철수", 170.5), make_pair("신짱구", 124.1) };

  // multimap.insert(std::make_pair(키, 값)) - 쌍을 만든 후 multimap에 삽입
  myMap.insert(std::make_pair("김유리", 190.2));
  myMap.insert(std::make_pair("김철수", 110.214));
  myMap.insert(std::make_pair("김철수", 86.1268));

  // 또는 중괄호로 묶어서 {키, 값} 삽입이 가능하다
  // multimap의 insert의 반환은 multimap<string, double>::iterator (삽입이 무조건 되기 때문)
  multimap<string, double>::iterator insert_iterator = myMap.insert({"박맹구", 260.515});
  cout << "추가된 원소의 Key : " << (*insert_iterator).first << endl;
  cout << "추가된 원소의 Value : " << insert_iterator->second << endl;

  // multimap은 operator[], at()으로 값 조회가 되지 않는다
  // cout << "김유리의 키는 " << myMap["김유리"] << "입니다" << endl;


  // find를 통해서 찾기가 가능하며, 반환은 해당 multimap의 첫번째 iterator
  multimap<string, double>::iterator find_iterator = myMap.find("김철수");
  cout << "찾기 성공 여부 : " << (find_iterator == myMap.end() ? "multimap에 없습니다" : "찾았습니다") << endl;
  cout << "찾은 사람 : " << find_iterator->first << "의 키는 " << find_iterator->second << endl;

  // Key에 해당하는 모든 원소를 찾기 위해서는 multimap.equal_range(Key) 를 사용한다
  // 반환값은 해당 Key의 시작 iterator와 끝 iterator가 쌍으로 이루어진 std::pair객체가 반환된다
  std::pair<multimap<string, double>::iterator, multimap<string, double>::iterator> equal_range_it = myMap.equal_range("김철수");
  for(multimap<string, double>::iterator it = equal_range_it.first; it != equal_range_it.second; ++it)
  {
    cout << "myMap에 있는 [" << it->first << "]의 키는 " << it->second << endl;
  }

  // 찾는 키가 없다면 end() 를 반환한다
  find_iterator = myMap.find("나미리선생님");
  cout << "나미리선생님 찾기 성공 여부 : " << (find_iterator == myMap.end() ? "multimap에 없습니다" : "찾았습니다") << endl << endl;




  // erase를 이용하여 제거 - 제거된 원소의 수를 반환
  int erase_result = myMap.erase("박맹구");
  cout << (erase_result >= 1 ? "박맹구를 제거했습니다" : "박맹구를 제거하지 못했습니다") << endl;

  // erase로 원소 하나만 제거하기 위해서는 iterator를 넘겨준다 - end()로 확인
  find_iterator = myMap.find("김철수");
  multimap<string, double>::iterator erase_iterator = myMap.erase(find_iterator);
  cout << (erase_iterator != myMap.end() ? "김철수를 제거했습니다" : "김철수를 제거하지 못했습니다") << endl;

  // 없는 키라면 0을 반환
  erase_result = myMap.erase("원장선생님");
  cout << (erase_result == 0 ? "원장선생님를 제거했습니다" : "원장선생님를 제거하지 못했습니다") << endl << endl;


  // multimap의 원소를 출력
  for(multimap<string, double>::iterator it = myMap.begin(); it != myMap.end(); ++it)
  {
    cout << "myMap의 원소 [" << it->first << "] - " << (*it).second << endl;
  }


  return 0;
}