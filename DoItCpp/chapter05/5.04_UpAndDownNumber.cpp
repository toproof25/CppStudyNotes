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