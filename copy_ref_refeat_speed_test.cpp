// 복사와 참조로 반복할 때 실행 시간 차이

#include <iostream>
#include <chrono>
#include <vector>

using namespace std::chrono;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

std::chrono::time_point<std::chrono::high_resolution_clock> t_start; // 시작 시간 저장
std::chrono::time_point<std::chrono::high_resolution_clock> t_end;   // 종료 시간 저장
duration<double> elapsed_seconds;  //  시작 - 종료

std::vector<int> int_test(10000000, 1);
std::vector<double> double_test(10000000, 1.2345);

struct ABCD
{
  int a    = 5;
  double b = 123.4321;
  char c   = 'B';
  bool d   = false;
  std::string e = "안녕하세요 문자열입니다. 이거도 테스트 하겠습니다";

  void add() { int sum = a+b+c+d; }
  void sub() { int sum = a-b-c-d; }
  void mul() { int sum = a*b*c*d; }
  void dev() { int sum = a/b/c/d; }
};
std::vector<ABCD> struct_test(10000000);


template<typename T>
void copyTest(const std::vector<T>& test_vector) {
    T sum = 0;
    for (T v : test_vector) { sum += v; }
}
template<typename T>
void refTest(const std::vector<T>& test_vector) {
    T sum = 0;
    for (const T& v : test_vector) { sum += v; }
}

template<typename T>
void struct_copyTest(const std::vector<T>& test_vector) {
    int sum = 0;
    for (T v : test_vector) { sum += v.a; }
}
template<typename T>
void struct_refTest(const std::vector<T>& test_vector) {
    int sum = 0;
    for (const T& v : test_vector) { sum += v.a; }
}


int main() {
    std::cout << "실행 시간 테스트\n\n";

    
    t_start = high_resolution_clock::now();     
    copyTest(double_test);                                      
    t_end = high_resolution_clock::now();       
    elapsed_seconds = t_end - t_start;
    std::cout << "일반 타입 복사 방식 경과 시간 : " << elapsed_seconds.count() << " seconds\n";

    t_start = high_resolution_clock::now();        
    refTest(double_test);                           
    t_end = high_resolution_clock::now();     
    elapsed_seconds = t_end - t_start;
    std::cout << "일반 타입 const 참조 방식 경과 시간 : " << elapsed_seconds.count() << " seconds\n";

    std::cout << "----------------------------------------------------------------------------------------------\n";

    t_start = high_resolution_clock::now();     
    struct_copyTest(struct_test);                                      
    t_end = high_resolution_clock::now();       
    elapsed_seconds = t_end - t_start;
    std::cout << "구조체 타입 복사 방식 경과 시간 : " << elapsed_seconds.count() << " seconds\n";

    t_start = high_resolution_clock::now();        
    struct_refTest(struct_test);                           
    t_end = high_resolution_clock::now();     
    elapsed_seconds = t_end - t_start;
    std::cout << "구조체 타입 const 참조 방식 경과 시간 : " << elapsed_seconds.count() << " seconds\n";

    return 0;
}
