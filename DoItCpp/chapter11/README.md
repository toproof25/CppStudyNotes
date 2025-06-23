# Chapter11 C++ 표준 라이브러리
1. 문자열 라이브러리
2. 파일 시스템 라이브러리
3. 기타 유용 함수


# \#include \<string>

- 문자열을 처리하는 표준 라이브러리

### string의 객체 초기화
```cpp
// string 객체 초기화
string str1("생성자 호출 초기화");
string str2 = "추가할 문자열의 길이를 정할 수 있습니다";
```

### string.length() - 문자열 길이 구하기
```cpp
// length - 문자열의 길이 구하기 
cout << str1.length() << endl;
```

### string.size() - 문자열 크기 구하기
- c에서는 크기가 항상 길이보다 +1 크다. 왜냐하면 문자열의 끝을 알리는 `\0`가 존재한다
- cpp에서는 length()와 size()는 동일한 로직으로 값을 반환하기에 편한 방법을 사용하면 된다
```cpp
// size - 문자열의 메모리 크기 구하기
cout << str1.size() << endl;
```

### string.empty() - 문자열 빈값 확인
```cpp
// empty - 문자열이 비어있는 지 확인
cout << (str1.empty() ? "비어있습니다" : "문자열이 존재합니다") << endl;
```

### string.append(추가 문자열) - 문자열 추가하기
- 한글의 경우 한문자가 3byte로 표현되기에 길이를 설정하여 추가할 때 주의해야 한다
- 간단하게 `str += "Test` 혹은 `str = str1 + "Test`로 사용할 수 있으며 직관적이다
```cpp
// append - 문자열을 추가하기
str1.append("\n새로 추가한 첫번째 문자열입니다\n");
cout << str1 << endl;

// 한글은 UTF-8에서 한문자당 3byte를 차지하기에 "추가할"까지만 추가된다
str1.append(str2, 0, 9);
cout << str1 << endl;

// append(추가 문자열, 추가 문자열의 시작 index, 끝 index)
str1.append(str2, 10, str2.size());
cout << str1 << endl;
```

### string.find(찾는 문자열) - 문자열 찾기
- find는 문자/문자열을 찾고 시작 index 번호를 알려준다
- 만일 찾는 문자가 없다면 `sgring::npos`를 반환한다
```cpp
// find - 문자열 찾기 (찾는 문자열이 없으면 string::npos를 반환)
// 실제로 static const size_t npos = -1;
cout << "추가할 시작 위치 : " << str1.find("추가할") << endl; 
cout << "없으면 npos를 반환 " << (str1.find("add") == string::npos) << endl << endl;
```

#### string.find() - 특정 문자열을 모두 바꾸는 코드
```cpp
string text = "C++ is a powerful language. C++ is fast.";
string target = "C++";
string replacement = "Modern C++";

// "C++"를 모두 "Modern C++"로 바꾸기
size_t pos = text.find(target);
while (pos != string::npos) {
    text.replace(pos, target.length(), replacement);
    // 다음 찾을 위치는 방금 바꾼 부분 바로 뒤부터 시작
    pos = text.find(target, pos + replacement.length());
}
cout << text << endl;
```


### string.compare(비교 문자열) - 문자열 비교
- 호출하는 문자열 객체와 매개변수로 들어간 문자열과 비교하는 함수
- 같으면 0, 호출 문자열보다 비교 문자열이 크면 양수, 작으면 음수가 반환
- 세밀한 비교를 위한다면 사용하나 일반적으로 단순 비교일 경우 `str1 == str2`을 사용한다
```cpp
// compare - 문자열 비교 (같으면 0, 대장 문자열이 더 크거나 첫문자가 크면 양수, 아니면 음수)
string str3 = str2;
cout << str2 << " compare " << str1 << " | " << str2.compare(str1) << endl << endl;
cout << str2 << " compare " << str3 << " | " << str2.compare(str3) << endl;
cout << str2 << " == " << str3 << " | " << (str2==str3) << endl << endl;
```

### string.replace(시작 위치, 바꿀 문자열 크기, 바꿀 문자열) - 문자열 교체
- 기존 문자열의 시작 위치에서 바꿀 문자열 크기만큼 바꿀 문자열로 교체한다
- 두번째 매개변수는 시작 위치에서 얼마나 제거할 것인지에 대한 값이다
```cpp
// replace - 문자열 교체 (시작 위치, 바꿀 문자열 크기, 문자열)
str2.replace(0, 32, "리플레이스 된 문자열");
cout << str2 << endl << endl;

str1.replace(str1.find("추가할"), 39, "----replace로 바꾸었습니다----");
cout << str1 << endl << endl;
```



# \#include \<filesystem>
- 파일을 생성, 삭제 등 메타데이터에 대해서 파일 시스템을 조작하기 위한 헤더 파일
- `std::filesystem` 네임 스페이스에 명령어 들이 존재하며, `namespace fs = filesystem`으로 간단하다 **fs**로 줄여 사용할 수 있다

### `fs::create_directories(path)` (디렉토리 생성)
	- 경로에 해당하는 곳에 디렉토리(폴더)를 생성한다

### `fs::directory_entry &entry` (항목에 대한 객체)
- #### entry.is_regular_file() (일반 파일인지 확인)
	- entry 항목이 일반 파일인지 확인하고, 일반 파일이면 true, 폴더이면 false
- #### entry.path().filename() (파일 이름 반환)
	- 해당 entry에 대한 파일 이름을 반환

### `fs::directory_iterator(path)` (path내 반복하는 반복자)
- 반복자로 for문 등에 사용하여 path내 항목들을 확인하는 용도
  ```
  for (const fs::directory_entry &entry : fs::directory_iterator(path))
  ```

### `fs::remove_all(path)`
- path에 대한 파일/폴더를 제거



# \#include \<fstream>
- 파일을 읽고 수정할 수 있는 헤더 파일로 파일의 기본적인 이름, 위치 등 메타데이터는 수정할 수 없다

### 파일 생성 및 수정 (ofstream)
```cpp
  // test.txt 파일 생성
  ofstream outFile(path + "/test.txt");
  outFile << "Hello, File System!" << endl;
  outFile.close();
```

### 파일 읽기 (ifstream)
```cpp
  // 파일 읽기
  cout << "파일 읽기"  << endl;
  ifstream inFile(path+"/test.txt");
  string line;
  while (getline(inFile, line))
  {
    cout << line << endl;
  }
  inFile.close();
```


# \#include \<random\>
- 난수를 생성하는 라이브러리
### 1. `mt19937 mt_rand_32bit`, `mt19937_64 mt_rand_64bit`
- mt는 메르센 소수를 이용하는 난수 생성 알고리즘을 의미
- 19937은 주기를 의미하며 $2^{19937} - 1$ 
- `mt19937 변수명`은 32bit로 난수를 생성하며, 생성자를 통해서 시드값을 설정할 수 있다
- `mt_rand_32bit()`으로 난수를 생성할 수 있다
```cpp
mt19937 mt_rand_32bit;
mt19937_64 mt_rand_64bit;
mt19937_64 mt_rand_seed(millis);
```

### 2. `random_device rng`
- `random_device 변수명` 표준 라이브러리로도 난수를 생성할 수 있으나 `mt19937`보다 느리게 동작하여 많은 난수를 생성할 때는 `mt19937`을 사용하고, 시드값을 결정할 때 `random_device`를 사용하는게 권장된다
```cpp
// 표준 라이브러리 random_device를 이용한 난수 생성
random_device rng;
cout << "random_device 의사 난수" << endl;
for (int i=0; i<10; ++i)
{
cout << i+1 << ": " << rng() << endl;
}
cout << endl;
```

# \#include <chrono\>
- 복잡한 시연 연산을 타입 안정성을 지키며 연산할 수 있게 하는 표준 라이브러리
```cpp
// chrono 시간을 이용하여 시드값 생성
// 시간에 대한 고유값으로 설정할 수 있으나, 추측이 가능함
auto curTime = chrono::system_clock::now();
auto duration = curTime.time_since_epoch();
auto millis = chrono::duration_cast<chrono::milliseconds>(duration).count();
mt19937_64 mt_rand_seed(millis);
```


# deep copy (깊은 복사)
복사 생성자를 오버로딩 하여 사용하는 것
`클래스이름(const 클래스이름 &레퍼런스 변수명)`

즉 오버로딩이 발생하여 기존 생성자와 복사 생성자가 구분되어 호출이 되는 방식
```cpp
copy(std::string name):
name(name)
{ 
std::cout << this->name << " = number 메모리에 정수 10을 할당합니다" << std::endl;
number = new int(10); 
};

// 클래스이름(const 클래스이름 &레퍼런스 변수) - 복사 생성자 오버로딩
copy(const copy &ref)
{
this->name = ref.name;      // 원본 이름을 그대로 복사
this->number = new int(*(ref.number)); // 동적 할당의 경우 새로운 메모리를 할당하여 다른 메모리를 참조하게 됨
}
```
### 컨테이너 변수를 복사 - copy 함수 사용하기
`copy(_Init _First, _InIt _Lask, _OutIn _Dest)` : 기존 컨테이너 변수에 시작과 끝 iterator와 복사할 대상 컨테이너를 통해 복사를 한다
`copy(v.begin(), v.end(), back_inserter(to_v))` : v벡터를 to_v에 복사하는 깊은 복사




# shallow copy (얕은 복사)
기존 객체를 복사하여 만드는 코드 `copy c_copy = c`의 경우 일반 생성자가 아닌 **복사 생성자**가 호출되어 기존의 값을 복사한다.

여기서 동적 할당된 변수의 경우 힙 메모리에 있는 주소를 그대로 복사하기 때문에 같은 값을 가리키게 된다.

원본 객체든 복사 객체든 먼저 동적 할당 변수를 해제하게 되면, 나머지 객체에서 소멸자가 호출될 때 이미 해제된 동적 변수를 해제하게 되기 때문에 오류가 발생한다. 

이를 해결하기 위해서 기존 복사 생성자를 오버라이딩 하여 깊은 복사가 되도록 해야 한다. 
```cpp
// 소멸자 호출을 위해 지역 변수로 만듦
{
copy c("first"); // 원본 객체 (생성자 호출 - 동적 변수 number에 메모리 할당)
c.print_name();

copy c_copy = c; // 복체 객체 (원본 객체의 값을 복사하는 얕은 복사 생성자가 호출됨)
c_copy.print_name(); // 원본 객체의 값이 복사되어 이름이 출력됨 - 동적 변수의 경우 같은 메모리를 참조하게 된다
}
// 생성된 객체의 소멸자가 호출된다
// c 객체 소멸자가 호출되어 number의 동적 할당된 값이 해제가됨
// c_copy 객체의 소멸자가 호출되면서 number의 값 할당을 해제하는데 오류가 발생 (이미 c 소멸자에서 해제가 됨)
```