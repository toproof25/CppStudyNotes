// Queue Class Test Code

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <cassert>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include <chrono>
#include <type_traits>
#include <windows.h>
#include <psapi.h>
#include "../inc/Queue.hpp"

using namespace std;
using namespace std::chrono;

// ==========================================
// [콘솔 색상 매크로]
// ==========================================
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define BLUE    "\033[34m"

// ==========================================
// [성능 측정 도구]
// ==========================================
size_t GetMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.PrivateUsage;
    }
    return 0;
}

#define MEASURE(NAME, ...) \
    { \
        size_t m_start = GetMemoryUsage(); \
        auto t_start = high_resolution_clock::now(); \
        __VA_ARGS__ \
        auto t_end = high_resolution_clock::now(); \
        size_t m_end = GetMemoryUsage(); \
        duration<double> time_span = duration_cast<duration<double>>(t_end - t_start); \
        long long m_delta = (long long)m_end - (long long)m_start; \
        double m_delta_mb = m_delta / 1024.0 / 1024.0; \
        std::cout << "    시간: " << fixed << setprecision(5) << time_span.count() << " 초"; \
        if (m_delta >= 0) \
            std::cout << " | 메모리: +" << fixed << setprecision(2) << m_delta_mb << " MB\n"; \
        else \
            std::cout << " | 메모리: " << fixed << setprecision(2) << m_delta_mb << " MB\n"; \
    }

// ==========================================
// [테스트 헬퍼]
// ==========================================
int testCount = 0;
int passCount = 0;
int failCount = 0;

void AssertTest(bool condition, const string& testName, const string& details = "") {
    testCount++;
    if (condition) {
        passCount++;
        cout << "  [" << GREEN << "PASS" << RESET << "] " << testName;
        if (!details.empty()) cout << " " << CYAN << details << RESET;
        cout << endl;
    } else {
        failCount++;
        cout << "  [" << RED << "FAIL" << RESET << "] " << testName;
        if (!details.empty()) cout << " " << RED << details << RESET;
        cout << endl;
        throw runtime_error("테스트 실패: " + testName);
    }
}

void PrintSection(const string& title) {
    cout << "\n" << YELLOW << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << endl;
    cout << YELLOW << "  " << title << RESET << endl;
    cout << YELLOW << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << endl;
}

void PrintSubSection(const string& title) {
    cout << "\n  " << CYAN << "▶ " << title << RESET << endl;
}

// ==========================================
// [테스트용 클래스]
// ==========================================

// 1. 추적 가능한 객체 (생성/복사/이동 카운트)
class Tracker {
public:
    static int liveCount;
    static int totalConstructs;
    static int totalCopies;
    static int totalMoves;
    static int totalDestructs;

    string data;

    Tracker(string s = "default") : data(s) { 
        liveCount++; 
        totalConstructs++; 
    }
    
    Tracker(const Tracker& other) : data(other.data + "_copy") { 
        liveCount++; 
        totalCopies++; 
    }
    
    Tracker(Tracker&& other) noexcept : data(std::move(other.data)) { 
        liveCount++; 
        totalMoves++; 
    }

    Tracker& operator=(const Tracker& other) {
        data = other.data;
        totalCopies++;
        return *this;
    }

    Tracker& operator=(Tracker&& other) noexcept {
        data = std::move(other.data);
        totalMoves++;
        return *this;
    }

    ~Tracker() { 
        liveCount--; 
        totalDestructs++; 
    }

    static void ResetCounts() {
        liveCount = 0;
        totalConstructs = 0;
        totalCopies = 0;
        totalMoves = 0;
        totalDestructs = 0;
    }

    static void PrintStats() {
        cout << "    생성: " << totalConstructs 
             << " | 복사: " << totalCopies 
             << " | 이동: " << totalMoves 
             << " | 소멸: " << totalDestructs 
             << " | 현재: " << liveCount << endl;
    }
};

int Tracker::liveCount = 0;
int Tracker::totalConstructs = 0;
int Tracker::totalCopies = 0;
int Tracker::totalMoves = 0;
int Tracker::totalDestructs = 0;

// 2. 예외 발생 시뮬레이터
struct Exploder {
    static int constructCount;
    static int copyCount;
    static int throwAtConstruct;
    static int throwAtCopy;
    int id;

    Exploder(int i) : id(i) {
        constructCount++;
        if (throwAtConstruct > 0 && constructCount == throwAtConstruct) {
            throw runtime_error("Exploder: 생성자 예외!");
        }
    }
    
    Exploder(const Exploder& other) : id(other.id) {
        copyCount++;
        if (throwAtCopy > 0 && copyCount == throwAtCopy) {
            throw runtime_error("Exploder: 복사 생성자 예외!");
        }
    }
    
    Exploder(Exploder&& other) : id(other.id) {}
    
    static void Reset() {
        constructCount = 0;
        copyCount = 0;
        throwAtConstruct = 0;
        throwAtCopy = 0;
    }
};

int Exploder::constructCount = 0;
int Exploder::copyCount = 0;
int Exploder::throwAtConstruct = 0;
int Exploder::throwAtCopy = 0;

// 3. 이동 불가 객체 (복사만 가능)
class NonMovable {
public:
    int value;
    NonMovable(int v = 0) : value(v) {}
    NonMovable(const NonMovable& other) : value(other.value) {}
    NonMovable(NonMovable&&) = delete;
    NonMovable& operator=(const NonMovable&) = default;
    NonMovable& operator=(NonMovable&&) = delete;
};

// 4. 복사 불가 객체 (이동만 가능)
class NonCopyable {
public:
    int value;
    NonCopyable(int v = 0) : value(v) {}
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&& other) noexcept : value(other.value) { other.value = 0; }
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable& operator=(NonCopyable&& other) noexcept { 
        value = other.value; 
        other.value = 0; 
        return *this; 
    }
};

// ==========================================
// [테스트 케이스]
// ==========================================

void Test01_BasicConstructor() {
    PrintSection("테스트 1: 기본 생성자 및 초기 상태");
    
    PrintSubSection("1.1 정수형 큐 생성");
    {
        Queue<int> q(10);
        AssertTest(q.isEmpty() == true, "초기 상태 isEmpty() == true");
        AssertTest(q.getSize() == 0, "초기 getSize() == 0");
    }
    
    PrintSubSection("1.2 다양한 크기의 큐 생성");
    {
        Queue<double> q1(1);
        Queue<string> q2(100);
        Queue<char> q3(1000);
        
        AssertTest(q1.isEmpty(), "크기 1 큐 생성");
        AssertTest(q2.isEmpty(), "크기 100 큐 생성");
        AssertTest(q3.isEmpty(), "크기 1000 큐 생성");
    }
    
    PrintSubSection("1.3 복잡한 타입의 큐 생성");
    {
        Queue<vector<int>> q1(5);
        Queue<Tracker> q2(5);
        
        AssertTest(q1.isEmpty() && q2.isEmpty(), "복잡한 타입 큐 생성 성공");
    }
}

void Test02_PushOperations() {
    PrintSection("테스트 2: Push 연산 (L-value & R-value)");
    
    PrintSubSection("2.1 기본 타입 Push (int)");
    {
        Queue<int> q(5);
        
        q.push(10);
        AssertTest(!q.isEmpty() && q.front() == 10, "첫 번째 push");
        
        q.push(20);
        q.push(30);
        AssertTest(q.front() == 10, "FIFO 구조 확인 (front 불변)");
        
        int val = 40;
        q.push(val);
        
        q.push(50);
        
        AssertTest(q.front() == 10, "연속 push 후 front 확인");
    }
    
    PrintSubSection("2.2 복사 vs 이동 추적");
    {
        Tracker::ResetCounts();
        Queue<Tracker> q(10);
        
        Tracker t1("L-value");
        q.push(t1);
        int copiesAfterLvalue = Tracker::totalCopies;
        
        q.push(Tracker("R-value"));
        int movesAfterRvalue = Tracker::totalMoves;
        
        AssertTest(copiesAfterLvalue > 0, "L-value push는 복사 발생", 
                   "(복사 횟수: " + to_string(copiesAfterLvalue) + ")");
        AssertTest(movesAfterRvalue > 0, "R-value push는 이동 발생", 
                   "(이동 횟수: " + to_string(movesAfterRvalue) + ")");
        
        Tracker::PrintStats();
    }
    
    PrintSubSection("2.3 대용량 데이터 Push");
    {
        Queue<string> q(5);
        string longStr(10000, 'A');
        
        q.push(longStr);
        q.push(string(10000, 'B'));
        
        AssertTest(q.front().length() == 10000 && q.front()[0] == 'A', "대용량 문자열 push 성공");
    }
    
    PrintSubSection("2.4 다양한 타입 Push");
    {
        Queue<vector<int>> q(3);
        q.push(vector<int>{1, 2, 3});
        q.push(vector<int>{4, 5});
        
        AssertTest(q.front().size() == 3, "vector<int> push 성공");
    }
}

void Test03_PopOperations() {
    PrintSection("테스트 3: Pop 연산 및 예외 처리");
    
    PrintSubSection("3.1 정상 Pop 동작");
    {
        Queue<int> q(5);
        q.push(10);
        q.push(20);
        q.push(30);
        
        AssertTest(q.front() == 10, "Pop 전 front 확인");
        
        q.pop();
        AssertTest(q.front() == 20, "첫 번째 pop 후 front (FIFO)");
        
        q.pop();
        AssertTest(q.front() == 30, "두 번째 pop 후 front");
        
        q.pop();
        AssertTest(q.isEmpty(), "모든 요소 pop 후 isEmpty()");
    }
    
    PrintSubSection("3.2 빈 큐 Pop 예외");
    {
        Queue<int> q(5);
        bool caught = false;
        
        try {
            q.pop();
        } catch (const std::out_of_range& e) {
            caught = true;
        }
        
        AssertTest(caught, "빈 큐 pop 시 out_of_range 예외 발생");
    }
    
    PrintSubSection("3.3 객체 소멸 추적");
    {
        Tracker::ResetCounts();
        {
            Queue<Tracker> q(3);
            q.push(Tracker("A"));
            q.push(Tracker("B"));
            q.push(Tracker("C"));
     
            int destructBefore = Tracker::totalDestructs;
            q.pop();
            int destructAfter = Tracker::totalDestructs;
            
            PrintSubSection("3.3 테스트 : unique_ptr 방식으로 pop함수 실행 시 소멸자 동작 테스트는 제외합니다");
            //AssertTest(destructAfter > destructBefore, "Pop 시 객체 소멸 확인");
        }
        AssertTest(Tracker::liveCount == 0, "큐 소멸 시 모든 객체 해제");
    }
}

void Test04_FrontOperations() {
    PrintSection("테스트 4: Front 연산");
    
    PrintSubSection("4.1 정상 Front 동작");
    {
        Queue<int> q(5);
        q.push(100);
        
        const int& ref = q.front();
        AssertTest(ref == 100, "Front 반환값 확인");
    }
    
    PrintSubSection("4.2 빈 큐 Front 예외");
    {
        Queue<int> q(5);
        bool caught = false;
        
        try {
            q.front();
        } catch (const std::out_of_range& e) {
            caught = true;
        }
        
        AssertTest(caught, "빈 큐 front 시 out_of_range 예외 발생");
    }
    
    PrintSubSection("4.3 Front 후 상태 불변");
    {
        Queue<string> q(3);
        q.push("First");
        q.push("Second");
        
        int sizeBefore = q.getSize();
        string val = q.front();
        int sizeAfter = q.getSize();
        
        AssertTest(sizeBefore == sizeAfter, "Front는 큐 상태를 변경하지 않음");
    }
}

void Test05_CopyConstructor() {
    PrintSection("테스트 5: 복사 생성자 (Deep Copy)");
    
    PrintSubSection("5.1 기본 타입 복사");
    {
        Queue<int> original(5);
        original.push(1);
        original.push(2);
        original.push(3);
        
        Queue<int> copy(original);
        
        AssertTest(copy.getSize() == original.getSize(), "복사된 큐 크기 일치");
        AssertTest(copy.front() == original.front(), "복사된 큐 데이터 일치");
    }
    
    PrintSubSection("5.2 깊은 복사 확인 (독립성)");
    {
        Queue<int> q1(5);
        q1.push(100);
        q1.push(200);
        
        Queue<int> q2(q1);
        
        q2.pop();
        q2.push(999);
        
        AssertTest(q1.front() == 100, "원본 큐 변경 없음");
        AssertTest(q2.front() == 200, "복사본 독립적 수정");
    }
    
    PrintSubSection("5.3 복잡한 타입 복사");
    {
        Queue<vector<int>> q1(3);
        q1.push(vector<int>{1, 2, 3});
        q1.push(vector<int>{4, 5});
        AssertTest(q1.front().size() == 3, "복사할 객체 vector 확인");
        
        Queue<vector<int>> q2(q1);
        AssertTest(q2.front().size() == 3, "복사될 객체 vector 확인");
    }
    
    PrintSubSection("5.4 빈 큐 복사");
    {
        Queue<int> empty(5);
        Queue<int> copy(empty);
        
        AssertTest(copy.isEmpty(), "빈 큐 복사 성공");
    }
}

void Test06_MoveConstructor() {
    PrintSection("테스트 6: 이동 생성자");
    
    PrintSubSection("6.1 기본 이동 동작");
    {
        Queue<int> q1(5);
        q1.push(10);
        q1.push(20);
        
        int originalSize = q1.getSize();
        Queue<int> q2(std::move(q1));
        
        AssertTest(q2.getSize() == originalSize && q2.front() == 10, "이동 후 데이터 유지");
    }
    
    PrintSubSection("6.2 이동 효율성 확인");
    {
        Tracker::ResetCounts();
        Queue<Tracker> q1(3);
        q1.push(Tracker("A"));
        q1.push(Tracker("B"));
        
        int copiesBefore = Tracker::totalCopies;
        Queue<Tracker> q2(std::move(q1));
        int copiesAfter = Tracker::totalCopies;
        
        AssertTest(copiesBefore == copiesAfter, "이동 시 복사 발생 없음 (효율적)");
    }
}

void Test07_CopyAssignment() {
    PrintSection("테스트 7: 복사 대입 연산자 (Copy-and-Swap)");
    
    PrintSubSection("7.1 기본 대입 동작");
    {
        Queue<int> q1(5);
        q1.push(1);
        q1.push(2);
        
        Queue<int> q2(5);
        q2.push(999);
        
        q2 = q1;
        
        AssertTest(q2.front() == 1, "대입 후 데이터 복사 확인");
    }
    
    PrintSubSection("7.2 자기 대입");
    {
        Queue<int> q(5);
        q.push(100);
        
        q = q;
        
        AssertTest(q.front() == 100, "자기 대입 후 데이터 유지");
    }
    
    PrintSubSection("7.3 대입 후 독립성");
    {
        Queue<string> q1(3);
        q1.push("Original");
        
        Queue<string> q2(3);
        q2 = q1;
        
        q2.push("Modified");
        
        AssertTest(!q1.isFull(), "원본 크기 불변"); 
        AssertTest(q2.front() == "Original", "대입본 독립적 동작");
    }
}

void Test08_MoveAssignment() {
    PrintSection("테스트 8: 이동 대입 연산자");
    
    PrintSubSection("8.1 기본 이동 대입");
    {
        Queue<int> q1(5);
        q1.push(10);
        q1.push(20);
        
        Queue<int> q2(5);
        q2 = std::move(q1);
        
        AssertTest(q2.front() == 10, "이동 대입 후 데이터 확인");
    }
}

void Test09_EmplaceOperation() {
    PrintSection("테스트 9: Emplace 연산 (효율성)");
    
    PrintSubSection("9.1 Emplace 기본 동작");
    {
        Queue<string> q(5);
        q.emplace("Direct");
        q.emplace("Construction");
        
        AssertTest(q.front() == "Direct", "Emplace 정상 동작");
    }
    
    PrintSubSection("9.2 Emplace 효율성 비교");
    {
        Tracker::ResetCounts();
        Queue<Tracker> q1(10);
        
        q1.push(Tracker("Push"));
        int copiesAfterPush = Tracker::totalCopies + Tracker::totalMoves;
        
        Tracker::ResetCounts();
        Queue<Tracker> q2(10);
        
        q2.emplace("Emplace");
        int copiesAfterEmplace = Tracker::totalCopies + Tracker::totalMoves;
        
        cout << "    (Push: " << copiesAfterPush << ", Emplace: " << copiesAfterEmplace << ")" << endl;
    }
    
    PrintSubSection("9.3 다중 인자 Emplace");
    {
        struct Point {
            int x, y;
            Point(int x = 0, int y = 0) : x(x), y(y) {}
        };
        
        Queue<Point> q(5);
        q.emplace(10, 20);
        
        AssertTest(q.front().x == 10 && q.front().y == 20, "다중 인자 직접 생성");
    }
}

void Test10_ResizeBasic() {
    PrintSection("테스트 10: 자동 Resize 동작 확인");

    PrintSubSection("10.1 용량 증가 확인");
    {
        // 초기 용량 2
        Queue<int> q(2);
        size_t initialCap = q.getCapacity();

        q.push(1);
        q.push(2);
        
        AssertTest(q.isFull(), "초기 용량 도달");

        // 여기서 Resize 발생!
        q.push(3);

        AssertTest(!q.isFull(), "Resize 후 Full 상태 해제됨");
        AssertTest(q.getCapacity() == initialCap * 2, "용량이 2배로 증가함 (2 -> 4)");
        AssertTest(q.getSize() == 3, "데이터 개수 확인");
        
        // 데이터 무결성 확인
        AssertTest(q.front() == 1, "데이터 1 유지"); q.pop();
        AssertTest(q.front() == 2, "데이터 2 유지"); q.pop();
        AssertTest(q.front() == 3, "새 데이터 3 확인"); q.pop();
    }
}

void Test11_ExceptionSafety() {
    PrintSection("테스트 11: 예외 안전성");
    
    PrintSubSection("11.1 Push 중 예외 (Full)");
    {
        Queue<int> q(1);
        q.push(1);
        
        bool caught = false;
        try {
            q.push(2);
        } catch (...) {
            caught = true;
        }
        AssertTest(caught && q.front() == 1, "예외 발생 시 기존 데이터 유지");
    }
}

void Test12_MemoryManagement() {
    PrintSection("테스트 12: 메모리 관리 및 누수 검사");
    
    PrintSubSection("12.1 큐 소멸 시 객체 해제");
    {
        Tracker::ResetCounts();
        
        {
            Queue<Tracker> q(5);
            q.push(Tracker("A"));
            q.push(Tracker("B"));
            q.push(Tracker("C"));
        }
        
        AssertTest(Tracker::liveCount == 0, "모든 객체 정상 소멸 (메모리 누수 없음)");
    }
    
    PrintSubSection("12.2 Pop 시 동작 확인");
    {
        Tracker::ResetCounts();
        Queue<Tracker> q(5);
        
        q.push(Tracker("X"));
        int liveBefore = Tracker::liveCount;
        
        q.pop();
        int liveAfter = Tracker::liveCount;
        
        //AssertTest(liveAfter < liveBefore, "Pop 시 소멸자 호출 확인");
        PrintSubSection("12.2 테스트 : unique_ptr 방식으로 pop함수 실행 시 소멸자 동작 테스트는 제외합니다");
    }
}

void Test13_EdgeCases() {
    PrintSection("테스트 13: 경계 조건");
    
    PrintSubSection("13.1 크기 1 큐");
    {
        Queue<int> q(1);
        
        q.push(100);
        AssertTest(q.isFull(), "크기 1 큐 Full");
        
        q.pop();
        AssertTest(q.isEmpty(), "크기 1 큐 Empty");
        
        q.push(200);
        AssertTest(q.front() == 200, "재사용 성공");
    }
    
    PrintSubSection("13.2 빈 큐 연산");
    {
        Queue<int> q(5);
        
        bool popException = false;
        bool frontException = false;
        
        try { q.pop(); } catch (const std::out_of_range&) { popException = true; }
        try { q.front(); } catch (const std::out_of_range&) { frontException = true; }
        
        AssertTest(popException && frontException, "빈 큐 모든 예외 처리");
    }
    
    PrintSubSection("13.3 반복 Push/Pop (원형 동작)");
    {
        Queue<int> q(3);
        
        for (int cycle = 0; cycle < 100; cycle++) {
            q.push(cycle);
            q.pop();
        }
        AssertTest(q.isEmpty(), "반복 작업 후 상태 확인");
    }
    
    PrintSubSection("13.4 NonMovable 타입");
    {
        Queue<NonMovable> q(3);
        NonMovable nm(10);
        q.push(nm);
        AssertTest(q.front().value == 10, "NonMovable 타입 push 성공");
    }
    
    PrintSubSection("13.5 NonCopyable 타입");
    {
        Queue<NonCopyable> q(3);
        q.push(NonCopyable(20));
        AssertTest(q.front().value == 20, "NonCopyable 타입 이동 push 성공");
    }
}

void Test14_ComplexTypes() {
    PrintSection("테스트 14: 복잡한 타입 지원");
    
    PrintSubSection("14.1 중첩 컨테이너");
    {
        Queue<vector<vector<int>>> q(3);
        
        vector<vector<int>> nested = {{1, 2}, {3, 4, 5}};
        q.push(nested);
        
        AssertTest(q.front()[1].size() == 3, "중첩 vector 지원");
    }
    
    PrintSubSection("14.2 포인터 타입");
    {
        Queue<int*> q(3);
        int* ptr = new int(999);
        
        q.push(ptr);
        AssertTest(*(q.front()) == 999, "포인터 타입 저장");
        
        delete ptr;
    }
}

// 성능 테스트를 위한 무거운 객체 (복사 비용 유발)
struct HeavyObject {
    std::vector<int> data;
    HeavyObject(int size = 100) : data(size, 1) {} // 생성 비용
};

void Test15_PerformanceComparison() {
    PrintSection("테스트 15: 종합 성능 비교 (Queue vs std::queue)");
    
    // 테스트 데이터 개수 설정
    // *주의* std::queue는 덱(deque) 기반이라 재할당이 빠르지만, 
    // 직접 만든 Queue는 resize 비용이 크므로, 
    // 순수 로직 성능 비교를 위해 resize가 발생하지 않도록 넉넉히 잡는 경우와, 
    // 발생시키는 경우를 나누어 볼 수도 있습니다. 여기서는 N을 적절히 조절하세요.
    const int N = 10000000; // 1000만 건

    cout << "  데이터 개수: " << N << "개" << endl << endl;

    // ---------------------------------------------------------
    // Case 1: 기본 타입 (int) Push 성능
    // ---------------------------------------------------------
    PrintSubSection("15.1 [int] Push (삽입) 속도");
    
    MEASURE("ok::Queue<int> Push",
        Queue<int> q(N); // 메모리 미리 확보 (resize 비용 제외)
        for (int i = 0; i < N; i++) {
            q.push(i);
        }
    );

    MEASURE("std::queue<int> Push",
        std::queue<int> q; // deque는 자동 블록 할당
        for (int i = 0; i < N; i++) {
            q.push(i);
        }
    );

    // ---------------------------------------------------------
    // Case 2: 기본 타입 (int) Pop 성능
    // ---------------------------------------------------------
    PrintSubSection("15.2 [int] Pop (제거) 속도");
    
    {
        // 미리 채워두기
        Queue<int> myQ(N);
        std::queue<int> stlQ;
        for(int i=0; i<N; ++i) {
            myQ.push(i);
            stlQ.push(i);
        }

        MEASURE("ok::Queue<int> Pop",
            while(!myQ.isEmpty()) {
                myQ.pop();
            }
        );

        MEASURE("std::queue<int> Pop",
            while(!stlQ.empty()) {
                stlQ.pop();
            }
        );
    }

    // ---------------------------------------------------------
    // Case 3: 무거운 객체 (HeavyObject) Push 성능 (복사/이동 비용)
    // ---------------------------------------------------------
    PrintSubSection("15.3 [HeavyObject] Push (이동 시맨틱) 속도");
    // 벡터를 멤버로 가진 객체를 통해 메모리 이동 효율성 체크

    MEASURE("ok::Queue<Heavy> Push",
        Queue<HeavyObject> q(N);
        for (int i = 0; i < N; i++) {
            q.push(HeavyObject()); // R-value Push (Move)
        }
    );

    MEASURE("std::queue<Heavy> Push",
        std::queue<HeavyObject> q;
        for (int i = 0; i < N; i++) {
            q.push(HeavyObject()); // R-value Push (Move)
        }
    );

    // ---------------------------------------------------------
    // Case 4: Emplace vs Push 비교
    // ---------------------------------------------------------
    PrintSubSection("15.4 [Emplace] 직접 생성 최적화");
    // Emplace는 임시 객체 생성 및 이동 비용을 아예 없애야 함

    MEASURE("ok::Queue Emplace",
        Queue<HeavyObject> q(N);
        for (int i = 0; i < N; i++) {
            // 인자만 전달하여 내부 생성
            q.emplace(100); 
        }
    );

    MEASURE("std::queue Emplace",
        std::queue<HeavyObject> q;
        for (int i = 0; i < N; i++) {
            q.emplace(100);
        }
    );

    // ---------------------------------------------------------
    // Case 5: 순환(Circular) 패턴 성능 (Push & Pop 반복)
    // ---------------------------------------------------------
    PrintSubSection("15.5 [Circular Pattern] Push & Pop 반복");
    // 큐를 가득 채우지 않고, 일정 크기를 유지하며 계속 돌리는 시나리오
    // ok::Queue는 '%' 연산 비용이 발생하고, std::queue는 블록 해제/할당 비용이 발생함
    
    const int LoopCount = 5000000;
    
    MEASURE("ok::Queue Circular",
        Queue<int> q(1000); // 크기 1000 고정
        // 처음에 좀 채워둠
        for(int i=0; i<500; ++i) q.push(i);
        
        for (int i = 0; i < LoopCount; i++) {
            q.push(i);
            q.pop();
        }
    );

    MEASURE("std::queue Circular",
        std::queue<int> q;
        for(int i=0; i<500; ++i) q.push(i);
        
        for (int i = 0; i < LoopCount; i++) {
            q.push(i);
            q.pop();
        }
    );
}
void Test16_StressTest() {
    PrintSection("테스트 16: 스트레스 테스트 (순환)");
    
    PrintSubSection("16.1 대규모 Push/Pop 반복");
    {
        Queue<int> q(100);
        
        for (int i = 0; i < 10000; i++) {
            q.push(i);
            q.pop();
        }
        
        AssertTest(q.isEmpty(), "대규모 순환 작업 안정성");
    }
}

void Test17_StatePredictability() {
    PrintSection("테스트 17: 상태 전환 예측");
    
    PrintSubSection("17.1 예측 가능한 상태 전환");
    {
        Queue<int> q(5);
        
        q.push(1);
        int state1 = q.getSize();
        
        q.push(2);
        int state2 = q.getSize();
        
        q.pop();
        int state3 = q.getSize();
        
        AssertTest(state2 > state1, "상태 전환 확인");
    }
}

void Test18_RealWorldScenarios() {
    PrintSection("테스트 18: 실제 사용 시나리오");
    
    PrintSubSection("18.1 작업 대기열 (Printer Spooler)");
    {
        struct Job {
            int id;
            string docName;
            Job(int i = 0, string n = "") : id(i), docName(n) {}
        };
        
        Queue<Job> spooler(10);
        
        spooler.emplace(1, "Report.pdf");
        spooler.emplace(2, "Photo.jpg");
        
        AssertTest(spooler.front().docName == "Report.pdf", "대기열 처리 (FIFO)");
        spooler.pop();
        AssertTest(spooler.front().docName == "Photo.jpg", "다음 작업 확인");
    }
}

void Test19_ComplexResize() {
    PrintSection("테스트 19: 원형 상태(Wrap Around)에서의 Resize");

    PrintSubSection("19.1 데이터 선형화(Linearization) 검증");
    {
        /*
         * 시나리오:
         * 1. 크기 3인 큐 생성
         * 2. [1, 2, 3] Push (Full)
         * 3. 1, 2 Pop -> [Empty, Empty, 3] (Front: 2, Back: 0)
         * 4. 4, 5 Push -> [4, 5, 3] (Wrap Around 발생 상태!)
         * 5. 6 Push -> Resize 발생!
         * * 기대 결과:
         * 메모리가 [3, 4, 5, 6, Empty, Empty] 순서로 펴져야 함.
         */
        
        Queue<int> q(3);
        
        // 1. 채우기
        q.push(1);
        q.push(2);
        q.push(3);

        // 2. 앞부분 비우기
        q.pop(); // 1 제거
        q.pop(); // 2 제거
        // 현재 상태: [?, ?, 3] (논리적 순서: 3)

        // 3. 뒤에 채워서 앞으로 감기 (Wrap Around)
        q.push(4);
        q.push(5);
        // 현재 상태(물리적): [4, 5, 3] (논리적 순서: 3 -> 4 -> 5)
        
        AssertTest(q.isFull(), "Wrap Around 상태로 가득 참");
        AssertTest(q.front() == 3, "Front값 확인");

        // 4. Resize 트리거
        cout << "    [Info] Resize 트리거 (3 -> 6)" << endl;
        q.push(6);

        // 5. 검증
        size_t newCap = q.getCapacity();
        AssertTest(newCap == 6, "용량 6으로 증가");
        AssertTest(q.getSize() == 4, "데이터 개수 4개");

        // 순서대로 나오는지 확인 (선형화가 잘못되면 여기서 순서가 꼬임)
        int expected[] = {3, 4, 5, 6};
        for(int val : expected) {
            if(q.isEmpty()) {
                AssertTest(false, "데이터 유실 발생!");
                break;
            }
            int frontVal = q.front();
            bool match = (frontVal == val);
            AssertTest(match, "데이터 순서 검증", "기대값: " + to_string(val) + ", 실제값: " + to_string(frontVal));
            q.pop();
        }
    }

    PrintSubSection("19.2 대량 Resize 스트레스 테스트");
    {
        // 10000개까지 늘어나며 데이터가 깨지지 않는지 확인
        Queue<int> q(2);
        for(int i=0; i<10000; ++i) {
            q.push(i);
        }

        AssertTest(q.getSize() == 10000, "10000개 삽입 성공");
        
        bool integrity = true;
        for(int i=0; i<10000; ++i) {
            if(q.front() != i) {
                integrity = false;
                break;
            }
            q.pop();
        }
        AssertTest(integrity, "10000개 데이터 무결성 및 순서 확인");
    }
}


// ==========================================
// [메인 함수]
// ==========================================
int main() {
    try {
        cout << "\n" << GREEN << "╔═══════════════════════════════════════════════════════════╗" << RESET << endl;
        cout << GREEN << "║         Queue Class 종합 테스트 시작                          ║" << RESET << endl;
        cout << GREEN << "╚═══════════════════════════════════════════════════════════╝" << RESET << endl;
        
        Test01_BasicConstructor();
        Test02_PushOperations();
        Test03_PopOperations();
        Test04_FrontOperations();
        Test05_CopyConstructor();
        Test06_MoveConstructor();
        Test07_CopyAssignment();
        Test08_MoveAssignment();
        Test09_EmplaceOperation();
        Test10_ResizeBasic();        
        Test12_MemoryManagement();
        Test13_EdgeCases();
        Test14_ComplexTypes();
        Test15_PerformanceComparison();
        Test16_StressTest();
        Test17_StatePredictability();
        Test18_RealWorldScenarios();
        Test19_ComplexResize();

        // Test11_ExceptionSafety();
        // 해당 테스트는 고정 크기일 경우 push 중 예외가 발생하는 것을 테스트 -> 동적 크기로 수정하여 예외가 발생하지 않아 문제가 된다.
        
        cout << "\n" << GREEN << "╔═══════════════════════════════════════════════════════════╗" << RESET << endl;
        cout << GREEN << "║                    테스트 결과 요약                           ║" << RESET << endl;
        cout << GREEN << "╚═══════════════════════════════════════════════════════════╝" << RESET << endl;
        cout << "  총 테스트: " << testCount << "개" << endl;
        cout << "  " << GREEN << "통과: " << passCount << "개" << RESET << endl;
        cout << "  " << RED << "실패: " << failCount << "개" << RESET << endl;
        
        if (failCount == 0) {
            cout << "\n" << GREEN << "  ★★★ 모든 테스트를 성공적으로 통과했습니다! ★★★" << RESET << endl << endl;
        } else {
            cout << "\n" << RED << "  ✖ 일부 테스트가 실패했습니다." << RESET << endl << endl;
            return 1;
        }
        
    } catch (const exception& e) {
        cerr << "\n" << RED << "╔═══════════════════════════════════════════════════════════╗" << RESET << endl;
        cerr << RED << "║  [치명적 오류] 테스트 중단: " << e.what() << RESET << endl;
        cerr << RED << "╚═══════════════════════════════════════════════════════════╝" << RESET << endl;
        return 1;
    }

    return 0;
}
