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


### 예외 처리 코드
```cpp
#include <iostream>
#include <random> // 랜덤 숫자 
#include <stdexcept> // 예외가 정의된 헤더파일
#include <limits>

using namespace std;

int main(){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(1, 100);

  int answer = dist(gen);
  bool is_correct = false;
  while (!is_correct){
    try{
      // 정답
      int number;
      std::cin >> number;

      // *숫자가 아닌 입력이 들어왔는지 확인하는 부분입니다.
      // *문제와는 상관없지만, 좀 더 어드밴스한 것을 원하는 독자분들을 위해 특별히 추가한 부분입니다. :)
      if (cin.fail()) {
        cin.clear(); // 입력 스트림의 상태를 초기화
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 버퍼 비우기
        throw std::invalid_argument("숫자를 입력해주세요!");
      }

      if (number > 100 || number <= 0){
        throw std::out_of_range("1부터 100 사이의 숫자를 입력해주세요!");
      }
      else if (number > answer){
        std::cout << "너무 크다" << std::endl;
      }
      else if (number < answer){
        std::cout << "너무 small" << std::endl;
      }
      else{
        std::cout << "축하합니다!" << std::endl;
        is_correct = !is_correct;
      }
    }
    
    catch (const std::exception &e){
      // 예외
      std::cerr << e.what() << std::endl;
    }
  }

  return 0;
}
```