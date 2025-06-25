/*
부분 정렬 실습
*/

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

  // 부분 정렬 함수 (시작 반복자, 부분 정렬 범위 반복자, 끝 반복자)
  partial_sort(v.begin(), v.begin()+10, v.end());
  
  cout << "부분정렬된 난수 [";
  int index = 1;
  for(const int& i : v)
  {
    cout << i << ", ";
    if (index++ == 10) cout << " <-- 부분 정렬 | 정렬 안된 부분 --> ";
  }
  cout << ']' << endl;


  return 0;
}

