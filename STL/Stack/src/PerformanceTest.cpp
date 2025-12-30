/**
 * @file PerformanceTest.cpp
 * @brief Gemini를 이용하여 제작한 테스트 코드 - ok::Stack, v5::Stack, std::stack의 성능 비교 벤치마크 (한글 출력)
 * @details Windows API를 사용하여 시간 및 메모리 사용량을 측정
 */

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <chrono>
#include <iomanip>

// [OS 의존성] 메모리 측정을 위한 Windows API 헤더
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

// 테스트 대상 헤더 포함
#include "../inc/v1_v5_stack.hpp" 
#include "../inc/Stack.hpp"

using namespace std;
using namespace std::chrono;

// ==========================================
// [성능 측정 도구]
// ==========================================

// 현재 프로세스의 메모리 사용량(Private Bytes)을 가져오는 함수
size_t GetMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.PrivateUsage;
    }
    return 0;
}

// 시간 및 메모리 측정 매크로 (__VA_ARGS__ 적용)
#define MEASURE(NAME, ...) \
    { \
        /* 1. 측정 시작 (시간 & 메모리) */ \
        size_t m_start = GetMemoryUsage(); \
        auto t_start = high_resolution_clock::now(); \
        \
        /* 2. 코드 실행 */ \
        __VA_ARGS__ \
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
        std::cout << "  [" << NAME << "]\n"; \
        std::cout << "   - 소요 시간 : " << fixed << setprecision(5) << time_span.count() << " s\n"; \
        if (m_delta >= 0) \
            std::cout << "   - 메모리    : +" << m_delta_mb << " MB (+" << m_delta << " bytes)\n"; \
        else \
            std::cout << "   - 메모리    : " << m_delta_mb << " MB (" << m_delta << " bytes)\n"; \
        std::cout << "\n"; \
    }

// 테스트 데이터 개수 (천만 건)
const int TEST_COUNT = 10000000; 

int main() {
    // 콘솔 한글 출력을 위한 설정 (필요 시)
    // std::wcout.imbue(std::locale("korean")); 

    // HeavyObject 초기화
    HeavyObject sourceObj(99, "벤치마크 테스트용 무거운 문자열 데이터");

    std::cout << "========================================================\n";
    std::cout << "     스택 성능 벤치마크 테스트 (데이터 개수: " << TEST_COUNT << ")\n";
    std::cout << "========================================================\n";
    std::cout << " * v5::Stack : std::unique_ptr 사용 (모든 요소 기본 생성됨)\n";
    std::cout << " * ok::Stack : std::allocator 사용 (메모리만 할당, 초기화 안 함)\n";
    std::cout << " * std::stack: std::vector 컨테이너 사용 (동적 크기 재할당 발생)\n\n";

    // ---------------------------------------------------------
    // 1. 생성자 및 메모리 할당 속도 테스트 (Construction)
    // ---------------------------------------------------------
    std::cout << "1. 생성 및 메모리 할당 (Construction)\n";
    
    // 포인터 선언 (매크로 스코프 문제 해결)
    v5::Stack<HeavyObject>* v5Ptr = nullptr;
    ok::Stack<HeavyObject>* okPtr = nullptr;
    std::stack<HeavyObject, std::vector<HeavyObject>>* stlPtr = nullptr;

    // v5: unique_ptr<T[]>는 생성 시 모든 요소(1000만개)를 기본 생성함 -> 느림 예상
    MEASURE("v5::Stack 초기화 (unique_ptr)", 
        v5Ptr = new v5::Stack<HeavyObject>(TEST_COUNT);
    );

    // ok: allocator는 메모리 공간만 잡고 생성자는 호출 안 함 -> 매우 빠름 예상
    MEASURE("ok::Stack 초기화 (allocator)", 
        okPtr = new ok::Stack<HeavyObject>(TEST_COUNT);
    );

    // stl: vector 기반
    MEASURE("std::stack 초기화 (vector)", 
        stlPtr = new std::stack<HeavyObject, std::vector<HeavyObject>>();
    );

    // 메모리 해제
    delete v5Ptr; 
    delete okPtr; 
    delete stlPtr;
    
    std::cout << "--------------------------------------------------------\n";


    // ---------------------------------------------------------
    // 2. Push (L-value Copy) 테스트
    // ---------------------------------------------------------
    std::cout << "2. 데이터 삽입 (L-value 복사)\n";
    {
        // 공정한 비교를 위해 미리 할당된 스택 사용
        v5::Stack<HeavyObject> v5S(TEST_COUNT);
        ok::Stack<HeavyObject> okS(TEST_COUNT);
        std::stack<HeavyObject, std::vector<HeavyObject>> stlS;

        MEASURE("v5::Stack 삽입 (복사)", 
            for(int i=0; i<TEST_COUNT; ++i) v5S.push(sourceObj);
        );

        MEASURE("ok::Stack 삽입 (복사)", 
            for(int i=0; i<TEST_COUNT; ++i) okS.push(sourceObj);
        );

        MEASURE("std::stack 삽입 (복사)", 
            for(int i=0; i<TEST_COUNT; ++i) stlS.push(sourceObj);
        );
    } 


    // ---------------------------------------------------------
    // 3. Pop 테스트
    // ---------------------------------------------------------
    std::cout << "3. 데이터 제거 (Pop 연산)\n";
    {
        v5::Stack<HeavyObject> v5S(TEST_COUNT);
        ok::Stack<HeavyObject> okS(TEST_COUNT);
        std::stack<HeavyObject, std::vector<HeavyObject>> stlS;
        
        // 데이터 채우기 (시간 측정 제외)
        for(int i=0; i<TEST_COUNT; ++i) {
            v5S.push(sourceObj);
            okS.push(sourceObj);
            stlS.push(sourceObj);
        }

        MEASURE("v5::Stack 제거", 
            for(int i=0; i<TEST_COUNT; ++i) v5S.pop();
        );

        MEASURE("ok::Stack 제거", 
            for(int i=0; i<TEST_COUNT; ++i) okS.pop();
        );

        MEASURE("std::stack 제거", 
            for(int i=0; i<TEST_COUNT; ++i) stlS.pop();
        );
    }


    // ---------------------------------------------------------
    // 4. Push (R-value Move) 테스트
    // ---------------------------------------------------------
    std::cout << "4. 데이터 삽입 (R-value 이동)\n";
    {
        v5::Stack<HeavyObject> v5S(TEST_COUNT);
        ok::Stack<HeavyObject> okS(TEST_COUNT);
        std::stack<HeavyObject, std::vector<HeavyObject>> stlS;

        MEASURE("v5::Stack 삽입 (이동)", 
            for(int i=0; i<TEST_COUNT; ++i) {
                v5S.push(HeavyObject(i, "이동 테스트 문자열"));
            }
        );

        MEASURE("ok::Stack 삽입 (이동)", 
            for(int i=0; i<TEST_COUNT; ++i) {
                okS.push(HeavyObject(i, "이동 테스트 문자열"));
            }
        );

        MEASURE("std::stack 삽입 (이동)", 
            for(int i=0; i<TEST_COUNT; ++i) {
                stlS.push(HeavyObject(i, "이동 테스트 문자열"));
            }
        );
    }


    // ---------------------------------------------------------
    // 5. Emplace 테스트 (생성 비용 최적화)
    // ---------------------------------------------------------
    std::cout << "5. Emplace (직접 생성 최적화)\n";
    {
        v5::Stack<HeavyObject> v5S(TEST_COUNT);
        ok::Stack<HeavyObject> okS(TEST_COUNT);
        std::stack<HeavyObject, std::vector<HeavyObject>> stlS;

        // v5 Emplace
        MEASURE("v5::Stack Emplace", 
            for(int i=0; i<TEST_COUNT; ++i) {
                v5S.emplace(i, "Emplace 텍스트");
            }
        );

        // ok Emplace
        MEASURE("ok::Stack Emplace", 
            for(int i=0; i<TEST_COUNT; ++i) {
                okS.emplace(i, "Emplace 텍스트");
            }
        );

        // STL Emplace
        MEASURE("std::stack Emplace", 
            for(int i=0; i<TEST_COUNT; ++i) {
                stlS.emplace(i, "Emplace 텍스트");
            }
        );
    }

    std::cout << "========================================================\n";
    std::cout << "모든 벤치마크 테스트가 완료되었습니다.\n";
    
    return 0;
}