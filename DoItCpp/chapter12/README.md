# Chapter12 STL 컨테이너와 알고리즘
1. [STL](#stlstandard-template-library)
   1. [컨테이너](#container-컨테이너)
2. [iterator 반복자](#iterator)
3. 순차 컨테이너
   1. [\#include \<array>](#stl-array-배열 컨테이너)
   2. \#include \<vector>
   3. \#include \<list>
   4. \#include \<deque>
4. 연관 컨테이너
   1. \#include \<set>
   2. \#include \<map>
5. 컨테이너 어댑터
   1. \#include \<stack>
   2. \#include \<queue>
6. 알고리즘
   1. 퀵 정렬
   2. 안정 정렬
   3. find
   4. 이진 탐색


# STL(Standard Template Library)
>STL은 C++에서 자료구조, 알고리즘 등을 모아놓은 표준 라이브러리이다

## Container (컨테이너)
> 컨테이너는 한가지 타입에 대해서 데이터를 관리하는 탬플릿 클래스 형태의 단위

- ### 순차 컨테이너
  >데이터를 연속적으로 저장하고 관리하는 컨테이너
	- **\# include \<string>
	- **\# include \<array>
	- **\# include \<vector>
	- **\# include \<list>
	- **\# include \<deque>
- ### 연관 컨테이너
  > Key-Value로 쌍을 이루어 데이터를 저장 및 관리하는 컨테이너
	- **\# include \<set>
	- **\# include \<map>
- ### 컨테이너 어탭터
  > 기존 컨테이너를 바탕으로 Stack, Queue 등 특정 기능을 사용하는 컨테이너 어댑터
	- **\# include \<stack>
	- **\# include \<queue>





# iterator
> iterator는 반복자로 STL 컨테이너에 원소를 순회하는 객체로 컨테이너마다 각자의 iterator를 가지고 이를 통해 정렬, 탐색, 수정 등이 가능하다

### 간단한 반복자 변수 선언
```cpp
// C++ 컨테이너는 모두 각자의 반복자가 존재한다. "컨테이너<자료형>::iterator 변수명" 으로 정의
// 반복자는 컨테이너의 원소를 순회하는 객체
vector<int>::iterator it = v.begin();
```


### iterator를 이용한 값 참조
```cpp
// 컨테이너의 begin(), end()는 각 시작과 마지막에 해당하는 iterator를 반환
// 포인터와 비슷하게 연산자 오버로딩된 *을 이용하여 값을 역참조한다
cout << "iterator로 v의 시작을 구함 : " << *it << endl;
```

### iterator를 이용한 컨테이너 원소 출력
```cpp
// 시작 반복자부터 현재 반복자가 마지막 반복자가 아닐 경우에만 (즉 모든 원소를 순회)
for(vector<int>::iterator iter=v.begin(); iter != v.end(); ++iter)
{
	cout << "v의 원소 : " << *iter << endl;
}
```




# STL array (배열 컨테이너)
> array 컨테이너는 고정된 크기를 이용하여 원소를 담는 데이터 구조이다. 빠르게 접근하면서 원소의 수정, 제거 등이 필요하지 않을 때 사용하기에 유리하다

### array 선언
```cpp
// array 정의
std::array<int, 5> arr1 = {0, 1, 2, 3, 4};
```

### array iterator로 순회
```cpp
// iterator로 array 순회
std::array<int, 5>::iterator it = arr1.begin();
for (it; it != arr1.end(); ++it)
{
	cout << "arr1의 값 : " << *it << endl;
}
```

### array - out_of_range 에러
- 고정 크기 array는 범위를 벗어나는 참조를 할 경우 `out_of_range`에러가 발생한다
```cpp
// 범위를 벗어나면 "out_of_range" 오류를 던진다
cout << "arr1의 사이즈 : " << arr1.size() << endl;
try
{
  cout << arr1.at(10) << endl;
}
catch (const std::out_of_range& e) 
{
    std::cerr << "오류 발생: 배열 범위를 벗어난 접근입니다." << std::endl;
    std::cerr << "C++ 예외 메시지: " << e.what() << std::endl;
}
catch(const std::exception& e)
{
  std::cerr << e.what() << '\n';
}
```
