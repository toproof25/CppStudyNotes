#include <iostream>
#include <stack>
#include <string>
#include <chrono>
#include <windows.h>
#include <psapi.h>

#include "Stack.hpp"

using namespace std::chrono;

struct HeavyObject
{
    int id;
    double data;
    std::string text; // 힙 할당을 유발하여 복사 비용을 높임
    char buffer[100]; // 객체 크기를 인위적으로 늘림

    // 기본 생성자
    HeavyObject() : id(0), data(0.0), text("") {};

    // 매개변수 생성자
    HeavyObject(int i, std::string t) : id(i), data(3.14), text(std::move(t)) {};
};

// 현재 프로세스의 메모리 사용량을 가져오는 함수
size_t GetMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.PrivateUsage;
}

// 시간 측정 매크로
#define MEASURE(NAME, CODE_BLOCK) \
    { \
        /* 1. 측정 시작 (시간 & 메모리) */ \
        size_t m_start = GetMemoryUsage(); \
        auto t_start = high_resolution_clock::now(); \
        \
        /* 2. 코드 실행 */ \
        CODE_BLOCK \
        \
        /* 3. 측정 종료 (시간 & 메모리) */ \
        auto t_end = high_resolution_clock::now(); \
        size_t m_end = GetMemoryUsage(); \
        \
        /* 4. 결과 계산 */ \
        duration<double> time_span = duration_cast<duration<double>>(t_end - t_start); \
        long long m_delta = (long long)m_end - (long long)m_start; \
        double m_delta_mb = m_delta / 1024.0 / 1024.0; \
        \
        /* 5. 출력 */ \
        std::cout << "[" << NAME << "]\n"; \
        std::cout << "  Time  : " << time_span.count() << " s\n"; \
        if (m_delta >= 0) \
            std::cout << "  Memory: +" << m_delta_mb << " MB (+" << m_delta << " bytes)\n"; \
        else \
            std::cout << "  Memory: " << m_delta_mb << " MB (" << m_delta << " bytes)\n"; \
        std::cout << "\n"; \
    }

const int TEST_COUNT = 10000000; 

int main()
{
    HeavyObject sourceObj(99, "안녕하세요. 구현한 Stack을 테스트하겠습니다.");

    std::cout << "========================================\n";
    std::cout << "     Performance Benchmark (N=" << TEST_COUNT << ")\n";
    std::cout << "========================================\n\n";

    std::cout << "--- [STACK] Test ---\n";
    
    // ok Stack 준비 (고정 크기 할당 - 재할당 비용 없음)
    ok::Stack<HeavyObject> okStack(TEST_COUNT);
    
    // STL Stack 준비 (기본적으로 deque 사용, 동적 재할당 발생)
    std::stack<HeavyObject> stlStack;

    // 1-1. Push (L-value Copy)
    std::cout << "\n1. Push (L-value Copy)\n";
    MEASURE("ok Stack push test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            okStack.push(sourceObj);
        }
    );
    MEASURE("STL Stack push test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            stlStack.push(sourceObj);
        }
    );

    // 스택 비우기 (Pop 테스트를 위해 다시 채워야 하므로 일단 비움 처리 생략하고 Pop 테스트 진행)
    
    // 1-2. Pop
    std::cout << "\n2. Pop\n";
    MEASURE("ok Stack pop test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            okStack.pop();
        }
    );
    MEASURE("STL Stack pop test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            stlStack.pop();
        }
    );

    // 1-3. Push (R-value Move)
    std::cout << "\n3. Push (R-value Move)\n";
    ok::Stack<HeavyObject> okStackMove(TEST_COUNT);
    std::stack<HeavyObject> stlStackMove;
    MEASURE("ok Stack push test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            // 임시 객체 생성 후 Move
            okStackMove.push(HeavyObject(i, "Move"));
        }
    );
    MEASURE("STL Stack push test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            stlStackMove.push(HeavyObject(i, "Move"));
        }
    );


    // 1-4. emplace
    std::cout << "\n4. Emplace\n";
    ok::Stack<HeavyObject> okStackEmplace(TEST_COUNT);
    std::stack<HeavyObject> stlStackEmplace;
    MEASURE("ok Stack Emplace Test" , 
        for(int i=0; i<TEST_COUNT; ++i) {
            okStackEmplace.emplace(i, "Move");
        }
    );
    MEASURE("STL Stack Emplace Test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            stlStackEmplace.emplace(i, "Move");
        }
    );

    return 0;
}