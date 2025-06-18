### 예외 처리
- CPP에서 예외를 처리하는 방법은 try, throw, catch를 사용한다.
- try에서는 예외가 발생할 만한 코드를
- throw는 다양한 조건에 의해 의도적으로 예외를 발생시키는 것
- catch는 발생한 예외에 맞는 처리를 수행한다.
- throw는 return처럼 실행되면 아래 코드는 실행되지 않는다.
```cpp
try{
	// 실행문
	if (i == 1)
		throw 1; // 예외를 발생시켜 catch로 보낸다
}
catch (int e){ // 괄호 내 에러에 대해서 처리한다.
	//정수 예외처리  
}
catch (float e){
	// 부동소수점 예외처리
}
>catch (char e){
	// 문자 예외처리
}
catch (...){
	// ...은 기타 예외를 모두 묶어서 처리한다
}
```

### 스택 풀기(Stack Unwinding)
- 스택 풀기는 함수에서 throw가 호출되었을 때 해당 함수에서 예외를 처리할 수 없는 경우 호출한 곳에 넘져 예외가 처리되는 것을 의미한다.
- 이는 스택의 구조를 생각해보면 각 호출되는 곳이 stack에 push되고, 예외는 이를 다시 역순으로 찾아가는 과정이기에 스택 풀기라 부른다.
```cpp
int func(int a){
	if (a==0)
		throw 0;
	return a;
}

try{
	// 실행문
	func(int a) // 예외를 발생시켜 catch로 보낸다
}
catch (int e){ // 괄호 내 에러에 대해서 처리한다.
	//정수 예외처리  
}
catch (...){
	// ...은 기타 예외를 모두 묶어서 처리한다
}
```
- 이처럼 함수에서 throw가 발생했을 때 해당 함수에 catch가 없다면 예외를 처리하기 위해 호출된 곳에서 예외가 처리된다.


###  noexcept
- noexcept는 이 함수는 예외 처리 문구가 throw를 발생시키지 않는 것을 명시하는 키워드이다.
- 이를 통해 컴파일이 더 빠르고 명확하게 진행될 수는 있다.
- 그러나 키워드를 사용했다고 하여 컴파일 과정에서 오류가 발생되지는 않으나, 런타임중에 오류가 발생할 수 있다.
```cpp
void func() noexcept
{
	cout << "Hello";
	throw 1;
}

int main()
{
	try{
		func();
	}
	catch (int e){
		cout << "error : " << e;
	}
	return 0;
}
```


### Up&Down에서 사용하는 예외 처리 코드 
```cpp
#include <iostream>
#include <random> // 랜덤 숫자 
#include <stdexcept> // 예외가 정의된 헤더파일
#include <limits>

using namespace std;

int main(){

 // 난수 생성 부분
 random_device rd;
 mt19937 gen(rd());
 uniform_int_distribution<> dist(1, 100);

 int answer = dist(gen);
 bool is_correct = false;

 // 정답이 아닌 상태이면 계속 반복
 while (!is_correct){

   // 게임 진행을 실행문으로 감싼다
   try{
     // 사용자 입력
     int number;
     cin >> number;

     // 숫자가 아닌 입력이 들어왔는지 확인하는 부분
     if (cin.fail()) {
       cin.clear(); // 입력 스트림의 상태 초기화
       cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 버퍼 비우기
       throw invalid_argument("숫자를 입력해주세요!"); // 오류를 던진다
     }

     // 숫자 범위가 아니라면
     if (number > 100 || number <= 0){
       throw out_of_range("1부터 100 사이의 숫자를 입력해주세요!");
     }
     // 정답보다 큰 경우
     else if (number > answer){
       cout << "너무 크다" << endl;
     }
     // 정답보다 작은 경우
     else if (number < answer){
       cout << "너무 small" << endl;
     }
     // 정답~
     else{
       cout << "축하합니다!" << endl;
       is_correct = !is_correct;
     }
   }
   
   // 발생한 오류를 출력
   catch (const exception &e){
     cerr << e.what() << endl;
   }
 }

 return 0;
}
```