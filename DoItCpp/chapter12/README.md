# Chapter12 STL 컨테이너와 알고리즘
1. [STL](#stlstandard-template-library)
   1. [컨테이너](#container-컨테이너)
2. [iterator 반복자](#iterator)
3. 순차 컨테이너
   1. [\#include \<array>](#stl-array-배열-컨테이너)
   2. [\#include \<vector>](#stl-vector-벡터-컨테이너)
   3. [\#include \<list>](#stl-list-리스트-컨테이너)
      - [forward_list](#stl-forward-list-단일-연결-리스트-컨테이너)
   4. [\#include \<deque>](#stl-deque-덱-컨테이너)
4. 연관 컨테이너
   1. [\#include \<set>](#stl-set-세트-컨테이너)
      - [multiset](#stl-multiset)
   2. [\#include \<map>](#stl-map-맵-컨테이너)
      - [multimap](#stl-multimap-멀티맵-컨테이너)
5. 컨테이너 어댑터
   1. [\#include \<stack>](#stl-stack)
   2. [\#include \<queue>](#stl-queue)
6. 알고리즘
   1. [Quick Sort (퀵 정렬)](#quick-sort-퀵-정렬)
   2. [안정 정렬](#안정-정렬)
   3. [부분 정렬](#partial-sort-부분-정렬)
   4. [find](#stdfind)
   5. [이진 탐색](#stl-binary-search)


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




# STL Vector (벡터 컨테이너)
- 동적 크기의 삽입과 제거를 할 수 있는 컨테이너
- 원소에 빠른 접근과 동적 배열 관리를 안전하게 하여 많이 사용된다

### vector 정의
- vector에서는 크기를 설정하지 않고 정의, 크기 설정, 크기와 초기값, 유니폼 초기화로 vector를 선언한다
```cpp
// vector 정의
std::vector<int> v0;                    // 단순 정의
std::vector<int> v1(10);                // 크기를 설정
std::vector<int> v2(10, 111);           // 크기와 초기값을 설정
std::vector<int> v4 = {0, 1, 2, 3, 4};  // 유니폼 초기화
```

### vector - 모든 요소 출력
- `vector::iterator`로 첫 요소부터 마지막 요소까지 반복하여 값을 출력한다
```cpp
// iterator로 vector 순회
std::vector<int>::iterator it = v2.begin();
for (it; it != v2.end(); ++it)
{
	cout << "v2의 값 : " << *it << endl;
}
cout << endl;
```
- 아래는 template를 이용하여 어떤 자료형 vector라도 모두 값을 출력할 수 있도록 구현한 함수
```cpp
// template으로 어떤 자료형의 vector라도 출력할 수 있도록 함수 구현
template<typename T>
void print_vector_all(const vector<T>& v)
{
  for(const T& i : v)
  {
    cout << "v : " << i << endl;
  }
}
```


### vector.push_back(추가할 값), vector.pop_back - 삽입과 삭제
- vector에서 가장 많이 사용되는 함수로 마지막 요소 뒤에 값을 추가하거나 마지막 요소를 제거한다
```cpp
// push_back로 맨 뒤에 원소를 추가
for (int i=0; i<5; ++i)
	v2.push_back(i);

// pop로 맨 뒤에 원소를 제거
for (int i=0; i<10; ++i)
	v2.pop_back();
```

### vector.insert(삽입 위치 반복자, 추가할 값)
```cpp
// insert(반복자, 값) - 반복자 앞으로 값을 추가
v2.insert(v2.begin()+2, 25); // v2[2] 앞에 25 추가

// 먼저 v2.begin()+2 가 추가되었기 때문에 반복자가 밀려서 의도하지 않은 위치에 값이 추가될 수 있다
v2.insert(v2.begin()+4, 25);
v2.insert(v2.begin()+6, 25);
```

### vector.erase(제거할 위치 반복자)
```cpp
// erase(반복자) 해당 위치의 요소를 제거
v2.erase(v2.begin()+2); // v2[2] 제거

// 먼저 v2.erase()+2 가 제어되었기 때문에 반복자가 앞당겨져서 의도하지 않은 값이 사라질 수 있다
v2.erase(v2.begin()+4);
v2.erase(v2.begin()+6);
```

### vector::const_iterator 상수 반복자
```cpp
// 상수 반복자 - 반복자가 가리키는 원소의 값을 변경할 수 없다
// 증감 연산자로 역참조를 하는 등의 연산은 가능
vector<int>::const_iterator cit = v2.cbegin();
cout << "++cit : " << *(++cit) << endl;
cout << endl;
```

### vector:reverse_iterator 리버스 반복자
```cpp
// 리버스 반복자 - rbegin은 원소의 마지막, rend는 첫원소의 이전 주소
for (vector<int>::reverse_iterator rit = v2.rbegin(); rit != v2.rend(); rit++)
{
	cout << "리버스 반복자 : " << *rit << endl;
}
cout << endl;
```

### vector의 메모리 관리
- vector는 항상 가진 크기보다 여유있게 메모리를 관리한다.
- 또한 push_back, pop_back의 동작은 빠르나 배열의 특성 상 중간에 삽입하고 제거하는 것은 매우 오래걸린다

### 메모리 재할당
- 기존 메모리 공간이 부족한 경우 보다 더 큰 메모리 공간을 만들고 기존 요소들을 복사하어 **메모리 재할당**을 한다
- 이 경우 요소의 개수 n개만큼 시간복잡도가 $O(n)$만큼 발생하게 된다

### vector.reserve(n)
- 메모리 공간을 n개만큼 미리 할당하여 메모리 재할당이 발생하지 않도록 미리 공간을 확보할 수 있다




# STL List (리스트 컨테이너)
- 이중 연결 리스트
>리스트와 벡터는 유사하나 내부적으로 벡터는 연속된 메모리에 데이터를 저장
>리스트는 요소가 다음 요소를 가리키는 포인터를 가지고 있다.
>그렇기에 벡터는 임의의 요소 접근이 매우 빠르나 리스트는 임의 접근이 불가능하며, 찾아가야한다.
>
>벡터는 원소 접근이 자주 발생하고 수정할 경우에 유리하며, 리스트는 삽입과 삭제가 빈번할 때 유리하다
>왜냐하면 벡터의 경우 중간에 값을 추가하면, 중간 이후 값들을 모두 뒤로 미루고 추가해야 한다.
>하지만 이중 연결 리스트는 next, pre의 포인터만 변경하면 되기에 삽입에 유리하다
>
>리스트에서는 iterator에서 +, - 연산을 하지 못하는데 이유는 vector, deque등은 메모리에 순차적으로 데이터를 저장하고 관리하기에 바로바로 이동이 가능하나. 즉 임의 접근이 $O(1)$로 가능하나 리스트의 경우 노드로 다음과 이전 노드에 대한 포인터로 연결되기에 바로 접근이 불가능하다.

### list.push_front(값) / list.push_back(값)
- 리스트의 앞과 뒤에 값을 추가한다
```cpp
// push_back() - 리스트 뒤에 값을 추가
cout << "push_back() - 리스트 뒤에 값을 추가" << endl;
li.push_back(100);
li.push_back(101);
li.push_back(102);

// push_front() - 리스트 앞에 값을 추가
cout << "push_front() - 리스트 앞에 값을 추가" << endl;
li.push_front(99);
li.push_front(98);
li.push_front(97);
```

### list.pop_front() / list.pop_back()
- 리스트 앞과 뒤 요소를 제거한다
```cpp
// pop_front() - 리스트 앞 요소 제거
cout << "pop_front() - 리스트 앞 요소 제거" << endl;
li.pop_front();
li.pop_front();
print_list_all(li);

// pop_back() - 리스트 뒤 요소 제거
cout << "pop_back() - 리스트 뒤 요소 제거" << endl;
li.pop_back();
li.pop_back();
print_list_all(li);
```

### list.empty() / list.size()
- 리스트가 비어있는 지 확인하는 함수와 사이즈를 확인 함수
```cpp
// empty() - 리스트가 비어있는지 확인
cout << (li.empty() ? "리스트 비어있다" : "리스트 요소있다") << endl;

// size() - 리스트 크기 확인
cout << "li 사이즈 : " << li.size() << endl;
```

### list::iterator
- 리스트도 컨테이너도 반복자를 이용하여 요소 접근이 가능하다
```cpp
// list 반복자로 요소 출력
for(list<int>::iterator it = li.begin(); it != li.end(); ++it)
{
	cout << "list li --> " << *it << endl;
}
```


# STL forward list (단일 연결 리스트 컨테이너)
> 단일 연결 리스트 forward list는 list와 달리 다음 노드를 가리키는 포인터만 존재한다
> 리스트와 차이점은 push_back, pop_back, size 등을 사용할 수 없다
> 그러나 리스트보다 노드의 포인터가 하나 적기에(pre) 메모리에 유리하다

### forward_list.push_front(추가할 값)
```cpp
// push_front() - 리스트 앞에 값을 추가
cout << "push_front() - 리스트 앞에 값을 추가" << endl;
li.push_front(99);
li.push_front(98);
li.push_front(97);
print_list_all(li);
```

### forward_list.insert_afer(iterator, 추가할 값)
- insert_after는 매개변수로 넣은 반복자의 다음 위치에 값을 추가한다
```cpp
// 리스트 중간인 두번째에 9999를 삽입 
cout << "단일 연결 리스트 중간 삽입 -  두번째에 9999를 삽입" << endl;
forward_list<int>::iterator it = li.begin();
li.insert_after(it, 9999);
print_list_all(li);
```


### forward_list.erase_after(iterater)
- erase_after는 iterator의 다음 요소를 제거한다
```cpp
// 리스트 중간 요소 제거 - 세번째 요소 제거 
cout << "리스트 중간 요소 제거 - 세번째 요소 제거" << endl;
it = li.begin();
li.erase_after(++it);
print_list_all(li);
```


### forward_list.pop_front()
```cpp
cout << "pop_front() - 리스트 앞 요소 제거" << endl;
li.pop_front();
li.pop_front();
print_list_all(li);
```


### 기타 함수들
- empty() : 비어있는지 확인
- front() : 앞 요소 반환




# STL deque (덱 컨테이너)
- deque은 순차 컨테이너로 vector 컨테이너와 유사하나 vector는 큰 메모리 블럭 공간에 원소를 순서대로 저장하지만, deque는 작은 메모리 블럭으로 나눈 후 작은 블럭들을 모아서 저장하기에 메모리 재할당이 발생하지 않아 더 빠르다
- 여러 블럭으로 나누었기에 캐시 효율이 vector보다 떨어지며, 내부적으로 찾아가는 포인터 변수가 있어 vector보다는 미세하게 느리다
- vector와 마찬가지로 중간삽입/제거는 $O(n)$이 발생하나 양방향에서 삽입/제거와 임의 접근은 $O(1)$의 시간복잡도로 유리하기에 그러한 상황에서 사용하기 유리하다

### deque 선언
```cpp
// deque 변수 선언
deque<int> dq;
```

### deque 반복자로 원소 순회
```cpp
// deque 반복자로 요소 출력
for(deque<int>::iterator it = dq.begin(); it != dq.end(); ++it)
{
  cout << "deque dq --> " << *it << endl;
}

```

### deque Template를 이용한 원소 출력
```cpp
template<typename T>
void print_deque_all(deque<T>& dq)
{
  for (const T& value : dq)
    cout << "dq : " << value << endl;
  cout << endl;
}
```

### deque.push_back(v) / deque.push_front(v)
- 양방향 큐인만큼 앞뒤로 데이터 추가가 가능
```cpp
// push_back() - deque 뒤에 값을 추가
cout << "push_back() - deque 뒤에 값을 추가" << endl;
dq.push_back(100);
dq.push_back(101);
dq.push_back(102);
print_deque_all(dq);

// push_front() - deque 앞에 값을 추가
cout << "push_front() - deque 앞에 값을 추가" << endl;
dq.push_front(99);
dq.push_front(98);
dq.push_front(97);
print_deque_all(dq);
```

### deque.pop_back() / deque.pop_front()
```cpp
// pop_front() - deque 앞 요소 제거
cout << "pop_front() - deque 앞 요소 제거" << endl;
dq.pop_front();
dq.pop_front();
print_deque_all(dq);

// pop_back() - deque 뒤 요소 제거
cout << "pop_back() - deque 뒤 요소 제거" << endl;
dq.pop_back();
dq.pop_back();
print_deque_all(dq);
```

### deque.empty() / deque.size()
```cpp
// empty() - deque가 비어있는지 확인
cout << (dq.empty() ? "deque 비어있다" : "deque 요소있다") << endl;

// size() - deque 크기 확인
cout << "dq 사이즈 : " << dq.size() << endl;

cout << "dq의 첫 원소 : " << dq.front() << endl;
cout << "dq의 끝 원소 : " << dq.back() << endl;
```


# STL Set (세트 컨테이너)
- set은 중복을 허용하지 않으며, 정렬을 하여 데이터를 저장하고 관리하는 컨테이너이다
- 내부적으로는 이진탐색을 이용하여 데이터가 추가될 때 바로 정렬하기에 검색, 삽입 등 작업에서 $O(log n)$에 가까운 시간복잡도가 생긴다

### set 선언
```cpp
//set<string> mySet;
set<string> mySet = {"김철수", "김짱구"}; // 유니폼 초기화
```

### set.insert(추가할 값)
- inset는 값을 추가하는 함수로 `std::pair<set<string>::iterator, bool>`을 반환하여 상태를 확인할 수 있다
```cpp
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
```

### set.erase(제거할 값)
- erase는 반환하여 부호 없는 정수로 1 또는 0을 반환하여 제거 상태를 확인할 수 있다
```cpp
// erase(제거할 값) - 부호 없는 정수가 반환되어 확인 가능
int erase_result = mySet.erase("김유리");
if (erase_result)
	cout << "김유리가 제거되었습니다" << endl;


erase_result = mySet.erase("김철수수수수수수");
if (!erase_result)
	cout << "김철수수수수수수는 원소에 존재하지 않아 제거 실패" << endl;


cout << "erase 후 mySet 사이즈 : " << mySet.size() << endl;
print_set_all(mySet); 
```

### set.clear()
```cpp
// 모든 원소 제거
mySet.clear();
cout << "clear 후 mySet 사이즈 : " << mySet.size() << endl;
```

### set.find(찾을값) / set.count(찾을값)
- count는 해당 원소의 개수를 알려주며, 중복이 없는 set은 0 or 1만 반환한다
- find는 `set<string>::iterator`로 해당 원소의 `iterator`를 반환하며, 원소가 없다면 end()를 반환하는 함수이다
```cpp
// set.count(찾을값) - 해당 원소의 개수 반환
string find_value = "김철수";
cout << "mySet.count("+find_value+") : " << mySet.count(find_value) << endl;


// set.find(찾을값)는 해당 iterator를 반환, 원소가 없으면 end()를 반환
set<string>::iterator find_result = mySet.find(find_value);
if(find_result != mySet.end())
	cout << "[" << *find_result+"]는 mySet에 있습니다" << endl;
else
	cout << "[" << find_value+"]는 mySet에 없습니다ㅜㅜㅜㅜ" << endl;


// 없는 원소의 경우
find_value = "김철수철수수수";
cout << "mySet.count("+find_value+") : " << mySet.count(find_value) << endl;
find_result = mySet.find(find_value);
if(find_result != mySet.end())
	cout << "[" << *find_result+"]는 mySet에 있습니다" << endl;
else
	cout << "[" << find_value+"]는 mySet에 없습니다ㅜㅜㅜㅜ" << endl;

```

### set.size()
- 해당 LIst의 size를 반환하는 함수



# STL multiset
- multiset은 set의 일부로 중복을 허용하면서 정렬이 된 집합 데이터이다
- 중복을 허용하기에 insert와 erase에서 일부 차이점이 존재한다
### multiset 선언
```cpp
//set<string> mySet;
multiset<string> mySet = {"김철수", "김짱구"}; // 유니폼 초기화
```

### multiset.insert(추가할 값)
- 중복을 허용하기에 insert의 반환은 `multiset<string>::iterator`로 삽입한 원소 반복자를 반환한다
- 실습하다가 `set<string>::iterator`으로 했는데 동작하기도 하였다
```cpp
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
set<string>::iterator result = mySet.insert("김철수");
cout << "["+(*result)+"]를 추가하였습니다" << endl;
```

### multiset.erase(제거할 값)
- erase는 반환하여 부호 없는 정수로 1 또는 0을 반환하여 제거 상태를 확인할 수 있다
- 추가로 multiset에서 erase는 해당하는 모든 원소를 제거한다.
- 하나만 제거하고자 한다면 제거하고자 하는 원소의 `iterator`를 구한 후 erase 매개변수에 넣으면 하나만 제거된다
```cpp
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
```

### multiset.clear()
```cpp
// 모든 원소 제거
mySet.clear();
cout << "clear 후 mySet 사이즈 : " << mySet.size() << endl;
```

### multiset.find(찾을값) / multiset.count(찾을값)
- count는 해당 원소의 개수를 알려주며, 중복이 없는 set은 0 or 1만 반환한다
- find는 `set<string>::iterator`로 해당 원소의 `iterator`를 반환하며, 원소가 없다면 end()를 반환하는 함수이다
```cpp
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
```

### multiset.size()
>multiset의 size를 반환



# STL Map (맵 컨테이너)
> Map은 키-값이 쌍으로 이루어져 정렬된 데이터 구조이다
>키-쌍은 `std::pair`객체로 이루어져 있으며, map의 반복자로 키와 값에 접근할 때는 `->`을 사용해야 한다
### map 선언
```cpp
//map<string, double> myMap;
map<string, double> myMap = { make_pair("김철수", 170.5), make_pair("신짱구", 124.1) };
```

### map.insert(std::pair(Key, Value)) / map.insert( {Key, Value} )
- insert의 반환은 `std::pair<map<string, double>::iterator, bool>`
```cpp
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
```


### map.find(찾을 Key)
- find의 반환은 해당하는 `iterator`
```cpp
// find를 통해서 찾기가 가능하며, 반환은 해당 map의 iterator
map<string, double>::iterator find_iterator = myMap.find("원장선생님");
cout << "찾기 성공 여부 : " << (find_iterator == myMap.end() ? "map에 없습니다" : "찾았습니다") << endl;
cout << "찾은 사람 : " << find_iterator->first << "의 키는 " << find_iterator->second << endl;

// 찾는 키가 없다면 end() 를 반환한다
find_iterator = myMap.find("나미리선생님");
cout << "나미리선생님 찾기 성공 여부 : " << (find_iterator == myMap.end() ? "map에 없습니다" : "찾았습니다") << endl << endl;
```

### map.erase(제거할 Key)
- erase는 해당하는 Key를 제거하며, 제거 여부를 부호 없는 정수로 알려준다
```cpp
// erase를 이용하여 제거
int erase_result = myMap.erase("김철수");
cout << (erase_result == 1 ? "김철수를 제거했습니다" : "김철수를 제거하지 못했습니다") << endl;

// 없는 키라면 0을 반환
erase_result = myMap.erase("원장선생님");
cout << (erase_result == 1 ? "원장선생님를 제거했습니다" : "원장선생님를 제거하지 못했습니다") << endl << endl;
```

### map 원소 출력
```cpp
// map의 원소를 출력
for(map<string, double>::iterator it = myMap.begin(); it != myMap.end(); ++it)
{
	cout << "myMap의 원소 [" << it->first << "] - " << (*it).second << endl;
}
```



# STL Multimap (멀티맵 컨테이너)
- Map은 키-값이 쌍으로 이루어져 정렬된 데이터 구조이다
- 키-쌍은 `std::pair`객체로 이루어져 있으며, map의 반복자로 키와 값에 접근할 때는 `->`을 사용해야 한다
- multimap은 at(), 혹은 operator\[\] 으로 값을 찾을 수 없다

### multimap 선언
```cpp
//map<string, double> myMap;
multimap<string, double> myMap = { make_pair("김철수", 170.5), make_pair("신짱구", 124.1) };
```

### multimap.insert(std::pair(Key, Value)) / map.insert( {Key, Value} )
- insert의 반환은 `std::pair<map<string, double>::iterator, bool>`
```cpp
// multimap.insert(std::make_pair(키, 값)) - 쌍을 만든 후 multimap에 삽입
myMap.insert(std::make_pair("김유리", 190.2));
myMap.insert(std::make_pair("김철수", 110.214));
myMap.insert(std::make_pair("김철수", 86.1268));

// 또는 중괄호로 묶어서 {키, 값} 삽입이 가능하다
// multimap의 insert의 반환은 multimap<string, double>::iterator (삽입이 무조건 되기 때문)
multimap<string, double>::iterator insert_iterator = myMap.insert({"박맹구", 260.515});
cout << "추가된 원소의 Key : " << (*insert_iterator).first << endl;
cout << "추가된 원소의 Value : " << insert_iterator->second << endl;
```


### multimap.find(찾을 Key)
- find의 반환은 해당하는 `iterator`
```cpp
// find를 통해서 찾기가 가능하며, 반환은 해당 multimap의 첫번째 iterator
multimap<string, double>::iterator find_iterator = myMap.find("김철수");
cout << "찾기 성공 여부 : " << (find_iterator == myMap.end() ? "multimap에 없습니다" : "찾았습니다") << endl;
cout << "찾은 사람 : " << find_iterator->first << "의 키는 " << find_iterator->second << endl;
```

### multimap.equal_range(찾을 Key)
```cpp
// Key에 해당하는 모든 원소를 찾기 위해서는 multimap.equal_range(Key) 를 사용한다
// 반환값은 해당 Key의 시작 iterator와 끝 iterator가 쌍으로 이루어진 std::pair객체가 반환된다
std::pair<multimap<string, double>::iterator, multimap<string, double>::iterator> equal_range_it = myMap.equal_range("김철수");
for(multimap<string, double>::iterator it = equal_range_it.first; it != equal_range_it.second; ++it)
{
	cout << "myMap에 있는 [" << it->first << "]의 키는 " << it->second << endl;
}
```

### multimap.erase(제거할 Key)
- erase는 해당하는 Key를 제거하며, 제거한 원소의 수를 알려준다
- set과 비슷하게 하나 원소만 제거하고자 한다면 `iterator`를 이용하여 원소 하나를 제거한다
```cpp
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
```

### multimap 원소 출력
```cpp
// multimap의 원소를 출력
for(multimap<string, double>::iterator it = myMap.begin(); it != myMap.end(); ++it)
{
	cout << "myMap의 원소 [" << it->first << "] - " << (*it).second << endl;
}
```


# STL Stack
- Stack 컨테이너 어댑터는 기존 컨테이너를 바탕으로 Stack의 구조와 기능을 사용할 수 있도록 제공하는 헤더 파일이다
  - LIFO (Lask In First Out) 구조로 사용하는 자료구조
  - 추가 : Push
  - 제거 : Pop
  - 맨 위 데이터 : top
  - 비어있는지 확인 : empty
- **Stack은 top데이터 이외에 다른 요소를 접근하여 조회할 수는 없다**

### Stack 선언
```cpp
stack<int> myStack;
```

### Stack.push()
```cpp
// stack push
for (int i=1; i<102; ++i)
{
  myStack.push(i);
}
```

### Stack.top()
```cpp
// top은 stack의 제일 위 데이터
cout << "top value : " << myStack.top() << endl; 
```

### Stack.pop()
```cpp
// stack pop
cout << "myStack.pop() 실행 " << endl; 
myStack.pop();
cout << "pop 이후 top value : " << myStack.top() << endl; 

cout << (myStack.empty() ? "스택이 비었습니다" : "스택에 데이터가 있습니다") << endl;
```

### Stack.empty() (비울때까지 pop)
```cpp
// stack이 모두 비워질 때 까지 pop
while(!myStack.empty())
{
  cout << "반복중... top value : " << myStack.top() << endl; 
  myStack.pop();
}
cout << "myStack이 모두 비워졌습니다" << endl;
```


# STL Queue
- STL의 컨테이너 어댑터 Queue는 기존 컨테이너를 바탕으로 Queue구조를 사용할 수 있도록 만든 헤더 파일
	- FIFO(First In First Out) 선입선출의 구조로 사용한다
	- 추가 : push
	- 제거 : pop
	- 맨 앞 데이터 : front
	- 맨 뒤 데이터 : back
	- 비어있는지 확인 : empty

### queue 선언
```cpp
queue<int> myQueue;
```

### queue.push(v)
```cpp
for(int i=1; i<102; i++)
  myQueue.push(i);
```

### queue.front()
```cpp
// front은 queue의 제일 앞 데이터
cout << "front value : " << myQueue.front() << endl; 
```

### queue.back()
```cpp
// back은 queue의 제일 뒤 데이터
cout << "back value : " << myQueue.back() << endl; 
```

### queue.pop()
```cpp
// queue pop
cout << "myQueue.pop() 실행 " << endl; 
myQueue.pop();
cout << "pop 이후 front value : " << myQueue.front() << endl; 
cout << "pop 이후 back value : " << myQueue.back() << endl; 
```

### queue.empty()
```cpp
cout << (myQueue.empty() ? "queue이 비었습니다" : "queue에 데이터가 있습니다") << endl;

// queue이 모두 비워질 때 까지 pop
while(!myQueue.empty())
{
  cout << "반복중... front value : " << myQueue.front() << endl; 
  myQueue.pop();
}
cout << "myQueue이 모두 비워졌습니다" << endl;
```




# Quick Sort (퀵 정렬)
- C++ 표준 라이브러리 `<algorithm>`에 `sort()`함수로 퀵 정렬을 수행할 수 있다
- 또한 임의 접근이 가능한 `vector`, `array`, `deque` 컨테이너에서 정렬을 수행할 수 있다
- 
- 평균 시간 복잡도는 $O(n log n)$, 최대 시간복잡도는 $O(n^2)$가 된다

### 오름차순 정렬
- sort함수에 첫번째 매개변수로 컨테이너의 시작 반복자, 두번째는 end 반복자를 설정한다
```cpp
// algorithm 라이브러리에서 제공하는 퀵 정렬 함수 sort
sort(v.begin(), v.end());
```

### 내림차순 정렬
- 내림차순은 3번째 매개변수에  `greater<type>()`을 주어 사용한다
```cpp
// 내림차순은 3번째 매개변수에 greater<type>() 을 추가
sort(v.begin(), v.end(), greater<int>());
```

### 사용자 정의 규칙
- 사용자 정의 규칙 함수는 2개의 파라미터로 비교하고자 하는 타입의 참조 변수로 상수화 하여 받아야 한다. `const Type& lhs`
- 반드시 bool타입으로 리턴해야 한다
```cpp
// 간단한 구조체
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

// 사용자 정의 규칙으로 정렬
sort(person.begin(), person.end(), compare);
```


# 안정 정렬
- 안정 정렬은 같은 원소들이 정렬이 된 후에도 원소들의 위치가 일치하는 것을 의미
- cpp에서는 \<algorithm>에서 `stable_sotr`로 사용할 수 있다

- 이와 같이 데이터가 있을 경우 안정 정렬을 수행한다면
  
|이름|나이|
|-|-|
|**철수**|5|
|유리|7|
|**짱구**|5|

- 아래와 같이 기존에 정렬된 \[철수, 짱구]의 순서가 일치한다 
  
|이름|나이|
|-|-|
|**철수**|5|
|**짱구**|5|
|유리|7|

### stable_sort() - 안정 정렬
```cpp
// 안정 정렬
stable_sort(person.begin(), person.end(), compare);
```

```출력 결과
불안정 정렬된 성 출력 : 루 -> 김 -> 신 -> 이 -> 신 -> 봉 -> 조 -> 뷟 -> 우 -> 흰 -> 방 ->
  안정 정렬된 성 출력 : 루 -> 신 -> 봉 -> 조 -> 우 -> 흰 -> 신 -> 김 -> 이 -> 뷟 -> 방 ->
```



# std::find
- find는 순차 컨테이너에서 원하는 값을 찾은 후 해당 반복자를 반환하는 함수

### 순차 컨테이너에서 find
```cpp
// find함수는 찾은 데이터의 iterator를 반환하며, 없으면 end를 반환
vector<int>::iterator it = find(v.begin(), v.end(), target);
if (it != v.end())
	cout << "target 데이터가 존재합니다 : " << *it << endl;
else
	cout << "target 데이터가 앖습니다" << endl;
```

### == 연산자 오버로딩을 이용한 구조체, 클래스에서 find
```cpp
// find를 위한 == 연산자 오버로딩
struct Person
{
	string name;
	int age;

	bool operator==(const Person& other) const {
		return name == other.name;
	}
};


// == 연산자 오버로딩을 통해 find함수를 구조체, 클래스에서 사용할 수 있다
vector<Person>::iterator pit = find(person.begin(), person.end(), (Person){"신훈이", 12});
if (pit != person.end())
	cout << "target 데이터가 존재합니다. 이름: " << pit->name << " 나이: " << pit->age << " 위치: " << distance(person.begin(), pit) << endl;
else
	cout << "target 데이터가 없습니다" << endl;
```


# partial sort (부분 정렬)
- 부분 정렬은 말 그대로 전체에서 일부 범위만 정렬을 수행하는 것을 의미한다
- 부분만 정렬하기에 기존 sort보다 빠르며, `<algorithm>`에서 `partial_sort(시작 반복자, 부분 범위 반복자, 끝 반복자)`로 사용한다
- 부분 정렬은 오름차순을 한다고 했을 때 전체 범위를 기준으로 정렬될 값을 고르고, middle범위만큼만 수행한다.

### partial_sort(first, middle, last)
- 컨테이너 반복자의 begin(), 범위, end() 반복자를 넣어 부분 정렬을 수행한다
```cpp
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
```

#### 출력 결과
```
추가된 난수 [429, 691, 835, 61, 26, 450, 255, 109, 586, 795, 300, 378, 422, 401, 113, 412, 652, 875, 45, 517, ]
부분정렬된 난수 [26, 45, 61, 109, 113, 255, 300, 378, 401, 412,  <-- 부분 정렬 | 정렬 안된 부분 --> 835, 795, 691, 586, 450, 429, 652, 875, 422, 517, ]
```



# STL binary search
- 이진 탐색 알고리즘을 이용하여 정렬된 컨테이너의 값을 탐색하는 algorithm의 함수
- find의 경우 순차적으로 탐색하기에 데이터아 많을 경우 시간복잡도에서는 이진 탐색이 유리하다
- 이진탐색은 먼저 컨테이너가 정렬된 상태여야 하기에 정렬을 먼저 수행해야 한다

### binary_search(begin, end, targer)
- 이진 탐색의 반환값은 해당 bool로 데이터의 유무를 확인할 수 있다
```cpp
int target;
cout << "찾을 숫자 입력 : ";
cin >> target;

// binary_search는 데이터를 찾고, bool값을 반환
bool result = binary_search(v.begin(), v.end(), target);
if (result)
  cout << "target 데이터가 존재합니다" << endl;
else
  cout << "target 데이터가 앖습니다" << endl;
```

### >, < 연산자 오버로딩을 이용한 이진 탐색

##### 구조체에서 연산자 오버로딩과 이진 탐색을 위한 함수를 구현
```cpp
// 연산자 오버로딩을 이용하여 그조체 이진 탐색
struct Person
{
	string name;
	int age;
	
	// binary_search에서 사용할 때는 '<' or '>' 연산자 오버로딩을 해야 한다
	// 또한 정렬된 상태도 오름차순, 내림차순에 맞게 정렬이 되어있어야 한다
	bool operator<(const Person& other) const {
	    return name < other.name;
	}
};

// 이진탐색 오버로딩에 맞게 똑같이 '<'으로 오름차순으로 정렬
bool person_compare(const Person& lhs, const Person& rhs)
{
	return lhs.name < rhs.name;
}
```

##### binary_search(begin, end, 찾을 타입의 데이터)
- 먼저 구조체/클래스를 정렬하기 위한 sort를 수행(사용자 정의 규칙으로 정렬)
- 정렬된 상태에서 이진 탐색으로 데이터를 찾는다
```cpp
vector<Person> person = {
  {"루피", 35},
  {"김짱구", 35},
  {"신형만", 35}, 
  {"이철수", 35},
  {"신훈이", 35}
};

// binary search는 구조체, 클래스에서 '> or <' 연산자 오버로딩
sort(person.begin(), person.end(), person_compare);
result = binary_search(person.begin(), person.end(), (Person){"신훈이", 12});
if (result) 
  cout << "person에 [신훈이] 데이터가 존재합니다." << endl;
else
  cout << "person에 [신훈이] 데이터가 없습니다" << endl;
```
