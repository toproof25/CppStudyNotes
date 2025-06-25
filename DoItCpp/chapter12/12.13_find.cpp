#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
using namespace std;

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


  int target;
  cout << "찾을 숫자 입력 : ";
  cin >> target;
  
  // find함수는 찾은 데이터의 iterator를 반환하며, 없으면 end를 반환
  vector<int>::iterator it = find(v.begin(), v.end(), target);
  if (it != v.end())
    cout << "target 데이터가 존재합니다 : " << *it << endl;
  else
    cout << "target 데이터가 앖습니다" << endl;

  return 0;
}
