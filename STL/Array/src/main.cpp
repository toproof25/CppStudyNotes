#include <iostream>
#include <array>
#include <algorithm>
#include "../inc/Array.hpp"

int main()
{
  // std::array<타입, 크기>로 정의
  std::array<int, 10> ar;

  // `[]`연산자로 내부 요소 접근 가능
  for (int i=0; i<10; ++i)
    std::cout << i << ". [ ] 인텍스 값 : " << ar[i] << '\n';
  printf("\n");

  // at(index) 함수로 내부 요소 접근 가능
  for (int i=0; i<10; ++i)
    std::cout << i << " at() 인텍스 값 : " << ar.at(i) << '\n';
  printf("\n");

  // 맨 앞과 맨 뒤 요소 출력
  ar[0] = 111; ar[9] = 999;
  std::cout << "front() 맨 앞 요소의 값: " << ar.front() << '\n';
  std::cout << "back()  맨 뒤 요소의 값: " << ar.back() << '\n';
  printf("\n");
  
  // 크기 출력
  std::cout << "size() 배열의 크기: " << ar.size() << '\n';
  printf("\n");
  
  // 요소가 있는 지 체크
  std::cout << "empty() 요소가 비어있는지 체크: " << (ar.empty() ? "비어있음" : "요소가 있음") << '\n';
  printf("\n");

  // iterator 반복자로 순회
  std::array<int, 10>::iterator it;
  for (it = ar.begin(); it!=ar.end(); ++it)
    std::cout << "iterator로 순회: " << *it << '\n';
  printf("\n");

  // reverse iterator 반복자로 역순회
  std::array<int, 10>::reverse_iterator rit;
  for (rit = ar.rbegin(); rit != ar.rend(); ++rit)
    std::cout << "reverse iterator로 순회: " << *rit << '\n';
  printf("\n");

  // fill 함수로 내부 요소 모두 채우기
  ar.fill(12345);
  for (it = ar.begin(); it!=ar.end(); ++it)
    std::cout << "fill로 모두 채운 값: " << *it << '\n';
  printf("\n");
  
  // data함수는 해당 array 메모리의 시작 주소를 의미 == `&ar`
  std::cout << "&ar 주소 값 : " << &ar << '\n';
  std::cout << "data 함수 값: " << ar.data() << '\n';
  printf("\n");


  printf("---------------------------------------------------------\n\n");
  
  // std::array<타입, 크기>로 정의
  Array<int, 10> okArray {1, 2, 3, 4, 5};

  // `[]`연산자로 내부 요소 접근 가능
  for (int i=0; i<10; ++i)
    std::cout << i << ". [ ] 인텍스 값 : " << okArray[i] << '\n';
  printf("\n");

  // at(index) 함수로 내부 요소 접근 가능
  try
  {
    for (int i=0; i<11; ++i)
      std::cout << i << ". at() 인텍스 값 : " << okArray.at(i) << '\n';
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  printf("\n");

  // fill 함수로 내부 요소 모두 채우기
  okArray.fill(12345);
  for (int i=0; i<10; ++i)
    std::cout << "fill로 모두 채운 값: " << okArray[i] << '\n';
  printf("\n");

  // 맨 앞과 맨 뒤 요소 출력
  okArray[0] = 111; okArray[9] = 999;
  std::cout << "front() 맨 앞 요소의 값: " << okArray.front() << '\n';
  std::cout << "back()  맨 뒤 요소의 값: " << okArray.back() << '\n';
  printf("\n");
  
  // 크기 출력
  std::cout << "size() 배열의 크기: " << okArray.size() << '\n';
  printf("\n");

  
  // iterator 반복자로 순회
  //Array<int, 10>::iterator it;
  for (auto it = okArray.begin(); it!=okArray.end(); ++it)
    std::cout << "iterator로 순회: " << *it << '\n';
  printf("\n");

  std::sort(okArray.begin(), okArray.end());

  for (auto it = okArray.begin(); it!=okArray.end(); ++it)
    std::cout << "srot 함수로 오름차순 정렬 후 순회: " << *it << '\n';
  printf("\n");

  std::reverse(okArray.begin(), okArray.end());

  for (auto it = okArray.begin(); it!=okArray.end(); ++it)
    std::cout << "reverse  함수로 내림차순 정렬 후 순회: " << *it << '\n';
  printf("\n");

  return 0;
}