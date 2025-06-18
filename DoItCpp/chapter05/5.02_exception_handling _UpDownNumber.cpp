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