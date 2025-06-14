# C++ 언어에 대한 배경 및 특징


## C++ 언어의 발전
- C언어를 바탕으로 발전한 C++은 C보다 메모리 관리가 편리하며 객체지향 언어의 특징이 나타나면서 발전
- 1998년에 처음으로 **C++98**이란 국게 표준이 제정되었다. 이때 Template를 이용한 meta-programming과 STL 도입이 되었다
  
## C++ 프로그램 빌드
- 빌드 과정은 다음과 같다. **전처리 -> 컴파일 -> 링크 -> 실행 파일**
- 작성한 소스 코드는 위 과정을 통해 실행 파일로 만들어지고, 프로그램을 동작할 수 있다
  
### pre-procssing
>CPP에서 전처리는 소스 코드에서 상단에 작성한 `#...`으로 시작되는 문구이다.
>대표적으로 `#include <iostream>`을 예로 들 수 있다.
>
>전처리 **앞으로 빌드 과정을 더 효율적으로 처리하기 위해 사전 작업을 하는 과정이다**
- `#include` : 헤더파일을 소스코드에 사용하기 위한 전처리 문구
- `#define 지정문자 대체문자` : 메크로 처리 문구로 특정 단어를 하나로 치환하느 것
	- PI = 3.14로 하면 해당 코드 내 모든 PI변수는 3.14로 치환된다
- `#ifdef` : 조건무 전처리
- 기타 주석 처리 등...
>소스 코드에 작성된 일부를 전처리를 통해 다음 컴파일 과정을 효율적으로 진행할 수 있다.
>전처리가 완료되면 결과적으로 `main.i`와 같은 파일을 생성한다

### compile
>**인간이 이해하는 프로로그래밍 언어를 기계가 이해할 수 있는 기계어로 번역하는 것을 Compile이라 부른다.**
>전처리된 파일들을 **어셈블리 코드 `main.s`** 로 변환한다. 
>컴파일 과정에는 어휘 분석, 구분 분석, 의미 분석, ... 등을 수행하여 자연어로 작성된 프로그래밍 언어를 어셈블리 코드로 변환하다.
>이후 어셈블리로 컴파일된 파일을 목적 파일은 오브젝트 파일 `main.o(bj)` 로 만든다.
>생성된 각 오브젝트 파일 이후 링크 과정에서 하나로 연결이 된다.


### link
>여러 오브젝트 파일을 하나로 합치는 과정을 의미한다. 
>컴파일에서 생성된 오브젝트 파일들과 각종 라이브러리를 하나로 연결하는 것이 링크 과정이다
>두가지로 정적 링크과 동적 링크로 나누어지게 된다.

## C++의 특징
- 낮은 수준의 엑섹스와 추상화
- 객체지향 프로그래밍
- 저수준 접근과 메모리 직접 관리
- 데이터 추상화와 범용성

## C++ 버전
>흔히 C++11부터 모던 C++이라고 부른다
- C++11
  - 범위 기반 for문
  - auto, nullptr
  - 람다 함수
- C++14
  - 이진수 리터럴
  - 숫자 구분 기호
- C++17
  - 파일 시스템, 병렬 처리 라이브러리
- C++20
  - 모듈, 컨셉, 코루틴
  - 3방향 비교 연산자
- C++23
  - 연역 this 포인터
  - 가변 인자 print

# Visual Studio Code에서 C++ 개발 환경 설정

1. #### MinGW 설치
	1. https://www.mingw-w64.org/
	2. Download - Sources - 첫번째 줄 **SourceForge** 링크 접속
	3. 자동 설치 install 실행 **(MinGW-W64 Online Installer)**, 오류가 발생하면 수동 설치 4번 실행
	4.  **(MinGW-W64 GCC-8.1.0)** 에서 **(x86_64-win32-seh)** 파일 다운로드 후 압축 해제
	5. 압축 해제한 파일 **(mingw64)** 을 C드라이브 바로 아래로 이동 
	6. 이후 Win+R - sysdm.cpl - 고급 - 환경 변수 - 시스템 변수에 변수이름이 Path을 더블클릭
	7. 새로 만들기를 누른 후 **"C:\\mingw64\\bin"** 을 입력 후 확인
2. #### VS Code 설치
	1. VS Code를 설치 후 패키지 **C/C++, C/C++ Extension Pack** 설치
	2. 새로운 폴더에 test.cpp 파일을 생성 후 F1키를 눌러 **C/C++ 구성편집(UI)** 로 이동
	3. 컴파일러 경로를 **C:/mingw64/bin/g++.exe** 로 설정
	4. IntelliSense 모드를 **windows-gcc-x64** 로 설정
	5. test.cpp폴더에 .vscode가 생성된 것을 확인
	6. 이후 test.cpp파일에서 상단 톱니바퀴 클릭 후 해당 옵션 클릭
	7. launch.json, tasks.json 파일이 생성된 것을 확인
	8. 두 json파일에 "\${fileDirname}\\\\${fileBasenameNoExtension}.exe"부분을 9번으로 수정
	9. build를 추가 | "\${fileDirname}**\\\\build**\\\\${fileBasenameNoExtension}.exe"
	10. 이후 test.cpp파일에 간단한 프로그램 작성 후 환경 설정이 잘 마무리 됐는지 확인