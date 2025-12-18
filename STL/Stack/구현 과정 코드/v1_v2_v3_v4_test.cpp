#include <iostream>
#include <stack>
#include <string>
#include <chrono>
#include <windows.h>
#include <psapi.h>

#include "v1_v5_stack.hpp"

using namespace std::chrono;

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

const int TEST_COUNT = 20000000; 

int main()
{
    HeavyObject sourceObj(99, "안녕하세요. 구현한 Stack을 테스트하겠습니다.");

    std::cout << "===========================================================\n";
    std::cout << "     스택 성능 테스트 (테스트 수 = " << TEST_COUNT << ")\n";
    std::cout << "===========================================================\n\n";

    std::cout << "--- [STACK] Test ---\n";
    
    
    // v1 Stack
    v1::Stack v1Stack(TEST_COUNT);

    // v2 Stack
    v2::Stack v2Stack(TEST_COUNT);
    
    // v3 Stack
    v3::Stack v3Stack(TEST_COUNT);

    // v4 Stack
    v4::Stack<HeavyObject> v4Stack(TEST_COUNT);

    // STL Stack
    std::stack<HeavyObject> stlStack;

    // 1-1. Push (L-value Copy)
    std::cout << "\n1. Push L-value\n";
    MEASURE("v1 Stack push test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            v1Stack.push(sourceObj);
        }
    );
    MEASURE("v2 Stack push test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            v2Stack.push(sourceObj);
        }
    );
    MEASURE("v3 Stack push test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            v3Stack.push(sourceObj);
        }
    );
    MEASURE("v4 Stack push test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            v4Stack.push(sourceObj);
        }
    );
    MEASURE("STL Stack push test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            stlStack.push(sourceObj);
        }
    );

    // 1-2. Pop
    std::cout << "\n2. Pop\n";
    MEASURE("v1 Stack pop test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            v1Stack.pop();
        }
    );
    MEASURE("v2 Stack pop test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            v2Stack.pop();
        }
    );
    MEASURE("v3 Stack pop test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            v3Stack.pop();
        }
    );
    MEASURE("v4 Stack pop test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            v4Stack.pop();
        }
    );
    MEASURE("STL Stack pop test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            stlStack.pop();
        }
    );

    // v1 Stack
    v1::Stack v1StackR_Value(TEST_COUNT);
    v2::Stack v2StackR_Value(TEST_COUNT);
    v3::Stack v3StackR_Value(TEST_COUNT);
    v4::Stack<HeavyObject> v4StackR_Value(TEST_COUNT);
    std::stack<HeavyObject> stlStackR_Value;

    // 1-3. Push (R-value Copy)
    std::cout << "\n3. Push R-value\n";
    MEASURE("v1 Stack push test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            v1StackR_Value.push(HeavyObject(99, "안녕하세요. 구현한 Stack을 테스트하겠습니다."));
        }
    );
    MEASURE("v2 Stack push test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            v2StackR_Value.push(HeavyObject(99, "안녕하세요. 구현한 Stack을 테스트하겠습니다."));
        }
    );
    MEASURE("v3 Stack push test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            v3StackR_Value.push(HeavyObject(99, "안녕하세요. 구현한 Stack을 테스트하겠습니다."));
        }
    );
    MEASURE("v4 Stack push test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            v4StackR_Value.push(HeavyObject(99, "안녕하세요. 구현한 Stack을 테스트하겠습니다."));
        }
    );
    MEASURE("STL Stack push test", 
        for(int i=0; i<TEST_COUNT; ++i) {
            stlStackR_Value.push(HeavyObject(99, "안녕하세요. 구현한 Stack을 테스트하겠습니다."));
        }
    );


    return 0;
}