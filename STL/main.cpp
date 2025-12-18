#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <chrono>

#include "./Stack/Stack.hpp"
#include "./Queue/Queue.hpp"

using namespace std::chrono;

// 성능 테스트를 위한 "무거운" 객체
struct HeavyObject
{
    int id;
    double data;
    std::string text; // 힙 할당을 유발하여 복사 비용을 높임
    char buffer[100]; // 객체 크기를 인위적으로 늘림

    // 기본 생성자
    HeavyObject() : id(0), data(0.0), text("") {}

    // 매개변수 생성자
    HeavyObject(int i, std::string t) : id(i), data(3.14), text(std::move(t)) {}
};

// 시간 측정 매크로 (코드 중복 제거)
#define MEASURE(NAME, CODE_BLOCK) \
    { \
        std::cout << "[" << NAME << "] 시작: " << '\n'; \
        auto t_start = high_resolution_clock::now(); \
        CODE_BLOCK \
        auto t_end = high_resolution_clock::now(); \
        duration<double> time_span = duration_cast<duration<double>>(t_end - t_start); \
        std::cout << "[" << NAME << "] 경과 시간: " << time_span.count() << " seconds\n"; \
    }

const int TEST_COUNT = 5000000; 

int main()
{
    // 테스트용 데이터 미리 생성 (복사 테스트용)
    HeavyObject sourceObj(1, "Test String for Copy Overhead");

    std::cout << "========================================\n";
    std::cout << "     Performance Benchmark (N=" << TEST_COUNT << ")\n";
    std::cout << "========================================\n\n";

    // 1. STACK 비교
    std::cout << "--- [STACK] Custom vs STL ---\n";
    
    // Custom Stack 준비 (고정 크기 할당 - 재할당 비용 없음)
    Stack<HeavyObject> myStack(TEST_COUNT);
    
    // STL Stack 준비 (기본적으로 deque 사용, 동적 재할당 발생)
    std::stack<HeavyObject> stlStack;

    // 1-1. Push (L-value Copy)
    std::cout << "\n1. Push (L-value Copy)\n";
    MEASURE("Custom Stack", 
        for(int i=0; i<TEST_COUNT; ++i) {
            myStack.push(sourceObj);
        }
    );
    MEASURE("STL Stack   ", 
        for(int i=0; i<TEST_COUNT; ++i) {
            stlStack.push(sourceObj);
        }
    );

    // 스택 비우기 (Pop 테스트를 위해 다시 채워야 하므로 일단 비움 처리 생략하고 Pop 테스트 진행)
    
    // 1-2. Pop
    std::cout << "\n2. Pop\n";
    MEASURE("Custom Stack", 
        for(int i=0; i<TEST_COUNT; ++i) {
            myStack.pop();
        }
    );
    MEASURE("STL Stack   ", 
        for(int i=0; i<TEST_COUNT; ++i) {
            stlStack.pop();
        }
    );

    // 1-3. Push (R-value Move) & Emplace
    std::cout << "\n3. Push (R-value Move)\n";
    // 다시 빈 상태에서 시작
    Stack<HeavyObject> myStackMove(TEST_COUNT);
    std::stack<HeavyObject> stlStackMove;

    MEASURE("Custom Stack", 
        for(int i=0; i<TEST_COUNT; ++i) {
            // 임시 객체 생성 후 Move
            myStackMove.push(HeavyObject(i, "Move"));
        }
    );
    MEASURE("STL Stack   ", 
        for(int i=0; i<TEST_COUNT; ++i) {
            stlStackMove.push(HeavyObject(i, "Move"));
        }
    );


    std::cout << "\n========================================\n";

    // 2. QUEUE 비교
    std::cout << "--- [QUEUE] Custom vs STL ---\n";

    // Custom Queue 준비
    Queue<HeavyObject> myQueue(TEST_COUNT);
    // STL Queue 준비
    std::queue<HeavyObject> stlQueue;

    // 2-1. Push (L-value Copy)
    std::cout << "\n1. Push (L-value Copy)\n";
    MEASURE("Custom Queue", 
        for(int i=0; i<TEST_COUNT; ++i) {
            myQueue.push(sourceObj);
        }
    );
    MEASURE("STL Queue   ", 
        for(int i=0; i<TEST_COUNT; ++i) {
            stlQueue.push(sourceObj);
        }
    );

    // 2-2. Pop
    // 주의: STL pop은 void반환, Custom pop은 T반환(값 이동 발생)
    // 공정한 비교를 위해 STL도 front()로 값을 꺼낸 후 pop하는 시나리오로 가정하거나
    // 단순 제거 속도만 비교. 여기서는 '값을 꺼내서 사용하는 시나리오'로 가정
    std::cout << "\n2. Pop & Get Value\n";
    
    volatile int dummy = 0; // 최적화 방지용

    MEASURE("Custom Queue", 
        for(int i=0; i<TEST_COUNT; ++i) {
            myQueue.pop(); // 꺼내면서 반환
        }
    );
    MEASURE("STL Queue   ", 
        for(int i=0; i<TEST_COUNT; ++i) {
            stlQueue.pop(); // 제거
        }
    );

    return 0;
}