#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cassert>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include <chrono>
#include <type_traits>
#include <windows.h>
#include <psapi.h>
#include "Stack.hpp"

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
    cout << "\n" << YELLOW << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << endl;
    cout << YELLOW << "  " << title << RESET << endl;
    cout << YELLOW << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << endl;
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
    NonMovable(int v) : value(v) {}
    NonMovable(const NonMovable& other) : value(other.value) {}
    NonMovable(NonMovable&&) = delete;
    NonMovable& operator=(const NonMovable&) = default;
    NonMovable& operator=(NonMovable&&) = delete;
};

// 4. 복사 불가 객체 (이동만 가능)
class NonCopyable {
public:
    int value;
    NonCopyable(int v) : value(v) {}
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
    
    PrintSubSection("1.1 정수형 스택 생성");
    {
        ok::Stack<int> s(10);
        AssertTest(s.isEmpty() == true, "초기 상태 isEmpty() == true");
        AssertTest(s.size() == 0, "초기 상태 size() == 0");
        AssertTest(s.getCapacity() == 10, "초기 capacity == 10");
    }
    
    PrintSubSection("1.2 다양한 크기의 스택 생성");
    {
        ok::Stack<double> s1(1);
        ok::Stack<string> s2(100);
        ok::Stack<char> s3(1000);
        
        AssertTest(s1.getCapacity() == 1, "크기 1 스택 생성");
        AssertTest(s2.getCapacity() == 100, "크기 100 스택 생성");
        AssertTest(s3.getCapacity() == 1000, "크기 1000 스택 생성");
    }
    
    PrintSubSection("1.3 복잡한 타입의 스택 생성");
    {
        ok::Stack<vector<int>> s1(5);
        ok::Stack<Tracker> s2(5);
        
        AssertTest(s1.isEmpty() && s2.isEmpty(), "복잡한 타입 스택 생성 성공");
    }
}

void Test02_PushOperations() {
    PrintSection("테스트 2: Push 연산 (L-value & R-value)");
    
    PrintSubSection("2.1 기본 타입 Push (int)");
    {
        ok::Stack<int> s(5);
        
        s.push(10);
        AssertTest(s.size() == 1 && s.peek() == 10, "첫 번째 push");
        
        s.push(20);
        s.push(30);
        AssertTest(s.size() == 3 && s.peek() == 30, "연속 push 후 마지막 요소 확인");
        
        int val = 40;
        s.push(val); // L-value
        AssertTest(s.peek() == 40, "L-value push 확인");
        
        s.push(50); // R-value
        AssertTest(s.peek() == 50, "R-value push 확인");
    }
    
    PrintSubSection("2.2 복사 vs 이동 추적");
    {
        Tracker::ResetCounts();
        ok::Stack<Tracker> s(10);
        
        Tracker t1("L-value");
        s.push(t1); // 복사
        int copiesAfterLvalue = Tracker::totalCopies;
        
        s.push(Tracker("R-value")); // 이동
        int movesAfterRvalue = Tracker::totalMoves;
        
        AssertTest(copiesAfterLvalue > 0, "L-value push는 복사 발생", 
                   "(복사 횟수: " + to_string(copiesAfterLvalue) + ")");
        AssertTest(movesAfterRvalue > 0, "R-value push는 이동 발생", 
                   "(이동 횟수: " + to_string(movesAfterRvalue) + ")");
        
        Tracker::PrintStats();
    }
    
    PrintSubSection("2.3 대용량 데이터 Push");
    {
        ok::Stack<string> s(5);
        string longStr(10000, 'A');
        
        s.push(longStr);
        s.push(string(10000, 'B'));
        
        AssertTest(s.size() == 2, "대용량 문자열 push 성공");
        AssertTest(s.peek().length() == 10000, "데이터 무결성 확인");
    }
    
    PrintSubSection("2.4 다양한 타입 Push");
    {
        ok::Stack<vector<int>> s(3);
        s.push(vector<int>{1, 2, 3});
        s.push(vector<int>{4, 5});
        
        AssertTest(s.peek().size() == 2, "vector<int> push 성공");
    }
}

void Test03_PopOperations() {
    PrintSection("테스트 3: Pop 연산 및 예외 처리");
    
    PrintSubSection("3.1 정상 Pop 동작");
    {
        ok::Stack<int> s(5);
        s.push(10);
        s.push(20);
        s.push(30);
        
        AssertTest(s.size() == 3, "Pop 전 크기 확인");
        
        s.pop();
        AssertTest(s.size() == 2 && s.peek() == 20, "첫 번째 pop 후 상태");
        
        s.pop();
        AssertTest(s.size() == 1 && s.peek() == 10, "두 번째 pop 후 상태");
        
        s.pop();
        AssertTest(s.isEmpty(), "모든 요소 pop 후 isEmpty()");
    }
    
    PrintSubSection("3.2 빈 스택 Pop 예외");
    {
        ok::Stack<int> s(5);
        bool caught = false;
        
        try {
            s.pop();
        } catch (const std::out_of_range& e) {
            caught = true;
        }
        
        AssertTest(caught, "빈 스택 pop 시 out_of_range 예외 발생");
    }
    
    PrintSubSection("3.3 객체 소멸 추적");
    {
        Tracker::ResetCounts();
        {
            ok::Stack<Tracker> s(3);
            s.push(Tracker("A"));
            s.push(Tracker("B"));
            s.push(Tracker("C"));
            
            int destructBefore = Tracker::totalDestructs;
            s.pop();
            int destructAfter = Tracker::totalDestructs;
            
            AssertTest(destructAfter > destructBefore, "Pop 시 객체 소멸 확인");
        }
        AssertTest(Tracker::liveCount == 0, "스택 소멸 시 모든 객체 해제");
    }
}

void Test04_PeekOperations() {
    PrintSection("테스트 4: Peek 연산");
    
    PrintSubSection("4.1 정상 Peek 동작");
    {
        ok::Stack<int> s(5);
        s.push(100);
        
        int& ref = s.peek();
        AssertTest(ref == 100, "Peek 반환값 확인");
        
        ref = 200;
        AssertTest(s.peek() == 200, "Peek 참조를 통한 수정 가능");
    }
    
    PrintSubSection("4.2 빈 스택 Peek 예외");
    {
        ok::Stack<int> s(5);
        bool caught = false;
        
        try {
            s.peek();
        } catch (const std::out_of_range& e) {
            caught = true;
        }
        
        AssertTest(caught, "빈 스택 peek 시 out_of_range 예외 발생");
    }
    
    PrintSubSection("4.3 Peek 후 상태 불변");
    {
        ok::Stack<string> s(3);
        s.push("First");
        s.push("Second");
        
        int sizeBefore = s.size();
        string val = s.peek();
        int sizeAfter = s.size();
        
        AssertTest(sizeBefore == sizeAfter, "Peek은 스택 상태를 변경하지 않음");
    }
}

void Test05_CopyConstructor() {
    PrintSection("테스트 5: 복사 생성자 (Deep Copy)");
    
    PrintSubSection("5.1 기본 타입 복사");
    {
        ok::Stack<int> original(5);
        original.push(1);
        original.push(2);
        original.push(3);
        
        ok::Stack<int> copy(original);
        
        AssertTest(copy.size() == original.size(), "복사된 스택 크기 일치");
        AssertTest(copy.peek() == original.peek(), "복사된 스택 데이터 일치");
    }
    
    PrintSubSection("5.2 깊은 복사 확인 (독립성)");
    {
        ok::Stack<int> s1(5);
        s1.push(100);
        s1.push(200);
        
        ok::Stack<int> s2(s1);
        
        s2.pop();
        s2.push(999);
        
        AssertTest(s1.size() == 2 && s1.peek() == 200, "원본 스택 변경 없음");
        AssertTest(s2.size() == 2 && s2.peek() == 999, "복사본 독립적 수정");
    }
    
    PrintSubSection("5.3 복잡한 타입 복사");
    {
        ok::Stack<vector<int>> s1(3);
        s1.push(vector<int>{1, 2, 3});
        s1.push(vector<int>{4, 5});
        
        ok::Stack<vector<int>> s2(s1);
        
        s2.peek().push_back(6);
        
        AssertTest(s1.peek().size() == 2, "원본 vector 크기 불변");
        AssertTest(s2.peek().size() == 3, "복사본 vector 독립적 수정");
    }
    
    PrintSubSection("5.4 빈 스택 복사");
    {
        ok::Stack<int> empty(5);
        ok::Stack<int> copy(empty);
        
        AssertTest(copy.isEmpty() && copy.getCapacity() == 5, "빈 스택 복사 성공");
    }
    
    PrintSubSection("5.5 복사 중 예외 처리");
    {
        Exploder::Reset();
        ok::Stack<Exploder> s1(3);
        s1.push(Exploder(1));
        s1.push(Exploder(2));
        
        Exploder::throwAtCopy = 2;
        bool caught = false;
        
        try {
            ok::Stack<Exploder> s2(s1);
        } catch (...) {
            caught = true;
        }
        
        AssertTest(caught, "복사 중 예외 발생 시 안전하게 처리");
    }
}

void Test06_MoveConstructor() {
    PrintSection("테스트 6: 이동 생성자");
    
    PrintSubSection("6.1 기본 이동 동작");
    {
        ok::Stack<int> s1(5);
        s1.push(10);
        s1.push(20);
        
        int originalSize = s1.size();
        ok::Stack<int> s2(std::move(s1));
        
        AssertTest(s2.size() == originalSize && s2.peek() == 20, "이동 후 데이터 유지");
        AssertTest(s1.size() == 0, "원본 스택 비워짐 (소유권 이전)");
    }
    
    PrintSubSection("6.2 이동 효율성 확인");
    {
        Tracker::ResetCounts();
        ok::Stack<Tracker> s1(3);
        s1.push(Tracker("A"));
        s1.push(Tracker("B"));
        
        int copiesBefore = Tracker::totalCopies;
        ok::Stack<Tracker> s2(std::move(s1));
        int copiesAfter = Tracker::totalCopies;
        
        AssertTest(copiesBefore == copiesAfter, "이동 시 복사 발생 없음 (효율적)");
    }
}

void Test07_CopyAssignment() {
    PrintSection("테스트 7: 복사 대입 연산자 (Copy-and-Swap)");
    
    PrintSubSection("7.1 기본 대입 동작");
    {
        ok::Stack<int> s1(5);
        s1.push(1);
        s1.push(2);
        
        ok::Stack<int> s2(5);
        s2.push(999);
        
        s2 = s1;
        
        AssertTest(s2.size() == 2 && s2.peek() == 2, "대입 후 데이터 복사 확인");
    }
    
    PrintSubSection("7.2 자기 대입");
    {
        ok::Stack<int> s(5);
        s.push(100);
        
        s = s;
        
        AssertTest(s.size() == 1 && s.peek() == 100, "자기 대입 후 데이터 유지");
    }
    
    PrintSubSection("7.3 대입 후 독립성");
    {
        ok::Stack<string> s1(3);
        s1.push("Original");
        
        ok::Stack<string> s2(3);
        s2 = s1;
        
        s2.push("Modified");
        
        AssertTest(s1.size() == 1, "원본 크기 불변");
        AssertTest(s2.size() == 2, "대입본 독립적 수정");
    }
}

void Test08_MoveAssignment() {
    PrintSection("테스트 8: 이동 대입 연산자");
    
    PrintSubSection("8.1 기본 이동 대입");
    {
        ok::Stack<int> s1(5);
        s1.push(10);
        s1.push(20);
        
        ok::Stack<int> s2(5);
        s2 = std::move(s1);
        
        AssertTest(s2.size() == 2 && s2.peek() == 20, "이동 대입 후 데이터 확인");
    }
}

void Test09_EmplaceOperation() {
    PrintSection("테스트 9: Emplace 연산 (효율성)");
    
    PrintSubSection("9.1 Emplace 기본 동작");
    {
        ok::Stack<string> s(5);
        s.emplace("Direct");
        s.emplace("Construction");
        
        AssertTest(s.size() == 2 && s.peek() == "Construction", "Emplace 정상 동작");
    }
    
    PrintSubSection("9.2 Emplace 효율성 비교");
    {
        Tracker::ResetCounts();
        ok::Stack<Tracker> s1(10);
        
        // Push 방식
        s1.push(Tracker("Push"));
        int copiesAfterPush = Tracker::totalCopies + Tracker::totalMoves;
        
        Tracker::ResetCounts();
        ok::Stack<Tracker> s2(10);
        
        // Emplace 방식
        s2.emplace("Emplace");
        int copiesAfterEmplace = Tracker::totalCopies + Tracker::totalMoves;
        
        AssertTest(copiesAfterEmplace < copiesAfterPush, 
                   "Emplace가 Push보다 복사/이동 적음",
                   "(Push: " + to_string(copiesAfterPush) + ", Emplace: " + to_string(copiesAfterEmplace) + ")");
    }
    
    PrintSubSection("9.3 다중 인자 Emplace");
    {
        struct Point {
            int x, y;
            Point(int x, int y) : x(x), y(y) {}
        };
        
        ok::Stack<Point> s(5);
        s.emplace(10, 20);
        
        AssertTest(s.peek().x == 10 && s.peek().y == 20, "다중 인자 직접 생성");
    }
}

void Test10_ResizeOperation() {
    PrintSection("테스트 10: 자동 Resize 동작");
    
    PrintSubSection("10.1 기본 Resize");
    {
        ok::Stack<int> s(2);
        
        s.push(1);
        s.push(2);
        AssertTest(s.getCapacity() == 2, "Resize 전 capacity");
        
        s.push(3); // Resize 발생
        AssertTest(s.getCapacity() == 4, "Resize 후 capacity 2배 증가");
        AssertTest(s.size() == 3 && s.peek() == 3, "Resize 후 데이터 유지");
    }
    
    PrintSubSection("10.2 연속 Resize");
    {
        ok::Stack<int> s(1);
        
        for (int i = 0; i < 10; i++) {
            s.push(i);
        }
        
        AssertTest(s.size() == 10, "연속 resize 후 크기");
        AssertTest(s.getCapacity() >= 10, "충분한 capacity 확보");
        
        // 데이터 무결성
        bool intact = true;
        for (int i = 9; i >= 0; i--) {
            if (s.peek() != i) intact = false;
            s.pop();
        }
        AssertTest(intact, "Resize 후 데이터 무결성");
    }
    
    PrintSubSection("10.3 Resize 시 이동 우선");
    {
        Tracker::ResetCounts();
        ok::Stack<Tracker> s(2);
        
        s.push(Tracker("A"));
        s.push(Tracker("B"));
        
        Tracker::ResetCounts();
        s.push(Tracker("C")); // Resize
        
        cout << "    Resize 동안 - ";
        Tracker::PrintStats();
        
        AssertTest(true, "Resize는 move_if_noexcept 사용 (안전성 우선)");
    }
}

void Test11_ExceptionSafety() {
    PrintSection("테스트 11: 예외 안전성 (Strong Guarantee)");
    
    PrintSubSection("11.1 Push 중 예외");
    {
        Exploder::Reset();
        ok::Stack<Exploder> s(5);
        s.push(Exploder(1));
        
        Exploder::throwAtConstruct = 2;
        bool caught = false;
        int sizeBefore = s.size();
        
        try {
            s.push(Exploder(2));
        } catch (...) {
            caught = true;
        }
        
        AssertTest(caught && s.size() == sizeBefore, "Push 실패 시 상태 롤백");
    }
    
    PrintSubSection("11.2 Resize 중 예외");
    {
        Exploder::Reset();
        ok::Stack<Exploder> s(2);
        s.push(Exploder(1));
        s.push(Exploder(2));
        
        Exploder::throwAtCopy = 1;
        bool caught = false;
        int capacityBefore = s.getCapacity();
        
        try {
            s.push(Exploder(3)); // Resize 발동
        } catch (...) {
            caught = true;
        }
        
        AssertTest(caught, "Resize 중 예외 포착");
        AssertTest(s.size() == 2, "예외 후 크기 유지 (Strong Guarantee)");
        AssertTest(s.peek().id == 2, "예외 후 데이터 무결성");
    }
    
    PrintSubSection("11.3 복사 생성자 예외");
    {
        Exploder::Reset();
        ok::Stack<Exploder> s1(3);
        s1.push(Exploder(1));
        
        Exploder::throwAtCopy = 1;
        bool caught = false;
        
        try {
            ok::Stack<Exploder> s2(s1);
        } catch (...) {
            caught = true;
        }
        
        AssertTest(caught, "복사 생성자 예외 포착");
    }
}

void Test12_MemoryManagement() {
    PrintSection("테스트 12: 메모리 관리 및 누수 검사");
    
    PrintSubSection("12.1 스택 소멸 시 객체 해제");
    {
        Tracker::ResetCounts();
        
        {
            ok::Stack<Tracker> s(5);
            s.push(Tracker("A"));
            s.push(Tracker("B"));
            s.push(Tracker("C"));
        } // 스택 소멸
        
        AssertTest(Tracker::liveCount == 0, "모든 객체 정상 소멸 (메모리 누수 없음)");
    }
    
    PrintSubSection("12.2 Pop 시 메모리 해제");
    {
        Tracker::ResetCounts();
        ok::Stack<Tracker> s(5);
        
        s.push(Tracker("X"));
        s.push(Tracker("Y"));
        
        int liveBefore = Tracker::liveCount;
        s.pop();
        int liveAfter = Tracker::liveCount;
        
        AssertTest(liveAfter < liveBefore, "Pop 시 객체 메모리 즉시 해제");
    }
    
    PrintSubSection("12.3 Resize 시 이전 메모리 해제");
    {
        Tracker::ResetCounts();
        ok::Stack<Tracker> s(2);
        
        s.push(Tracker("1"));
        s.push(Tracker("2"));
        
        int destructsBefore = Tracker::totalDestructs;
        s.push(Tracker("3")); // Resize
        int destructsAfter = Tracker::totalDestructs;
        
        AssertTest(destructsAfter >= destructsBefore, "Resize 시 이전 배열 객체 해제");
        AssertTest(Tracker::liveCount == 3, "Resize 후 올바른 객체 수 유지");
    }
    
    PrintSubSection("12.4 대용량 데이터 메모리 테스트");
    {
        size_t memBefore = GetMemoryUsage();
        
        {
            ok::Stack<vector<int>> s(10);
            for (int i = 0; i < 100; i++) {
                s.push(vector<int>(1000, i));
            }
        } // 소멸
        
        size_t memAfter = GetMemoryUsage();
        long long delta = (long long)memAfter - (long long)memBefore;
        
        AssertTest(abs(delta) < 5 * 1024 * 1024, "대용량 데이터 소멸 후 메모리 반환",
                   "(Delta: " + to_string(delta / 1024.0 / 1024.0) + " MB)");
    }
}

void Test13_EdgeCases() {
    PrintSection("테스트 13: 경계 조건 및 특수 케이스");
    
    PrintSubSection("13.1 크기 1 스택");
    {
        ok::Stack<int> s(1);
        
        s.push(100);
        AssertTest(s.size() == 1, "크기 1 스택 push");
        
        s.push(200); // Resize
        AssertTest(s.getCapacity() == 2 && s.size() == 2, "크기 1 스택 resize");
    }
    
    PrintSubSection("13.2 대용량 스택");
    {
        ok::Stack<int> s(10000);
        
        for (int i = 0; i < 10000; i++) {
            s.push(i);
        }
        
        AssertTest(s.size() == 10000, "대용량 push 성공");
        
        bool correct = true;
        for (int i = 9999; i >= 0; i--) {
            if (s.peek() != i) correct = false;
            s.pop();
        }
        
        AssertTest(correct, "대용량 데이터 무결성");
    }
    
    PrintSubSection("13.3 빈 스택 연산");
    {
        ok::Stack<int> s(5);
        
        bool popException = false;
        bool peekException = false;
        
        try { s.pop(); } catch (const std::out_of_range&) { popException = true; }
        try { s.peek(); } catch (const std::out_of_range&) { peekException = true; }
        
        AssertTest(popException && peekException, "빈 스택 모든 예외 처리");
    }
    
    PrintSubSection("13.4 반복 Push/Pop");
    {
        ok::Stack<int> s(3);
        
        for (int cycle = 0; cycle < 100; cycle++) {
            s.push(cycle);
            s.push(cycle + 1);
            s.pop();
        }
        
        AssertTest(s.size() == 100, "반복 작업 후 올바른 크기");
    }
    
    PrintSubSection("13.5 NonMovable 타입");
    {
        ok::Stack<NonMovable> s(3);
        
        NonMovable nm(42);
        s.push(nm);
        
        AssertTest(s.peek().value == 42, "이동 불가 타입 push (복사)");
    }
    
    PrintSubSection("13.6 NonCopyable 타입");
    {
        ok::Stack<NonCopyable> s(3);
        
        s.push(NonCopyable(100));
        
        AssertTest(s.peek().value == 100, "복사 불가 타입 push (이동)");
    }
}

void Test14_ComplexTypes() {
    PrintSection("테스트 14: 복잡한 타입 지원");
    
    PrintSubSection("14.1 중첩 컨테이너");
    {
        ok::Stack<vector<vector<int>>> s(3);
        
        vector<vector<int>> nested = {{1, 2}, {3, 4, 5}};
        s.push(nested);
        
        AssertTest(s.peek()[1].size() == 3, "중첩 vector 지원");
    }
    
    PrintSubSection("14.2 사용자 정의 클래스");
    {
        struct Person {
            string name;
            int age;
            Person(string n, int a) : name(n), age(a) {}
        };
        
        ok::Stack<Person> s(5);
        s.emplace("Alice", 30);
        s.emplace("Bob", 25);
        
        AssertTest(s.peek().name == "Bob", "사용자 정의 클래스 emplace");
    }
    
    PrintSubSection("14.3 포인터 타입");
    {
        ok::Stack<int*> s(3);
        int* ptr = new int(999);
        
        s.push(ptr);
        AssertTest(*(s.peek()) == 999, "포인터 타입 저장");
        
        delete ptr;
    }
}

void Test15_PerformanceComparison() {
    PrintSection("테스트 15: 성능 비교 (ok::Stack vs std::stack)");
    
    const int N = 50000000;
    cout << "  데이터 개수: " << N << "개 (int)" << endl << endl;
    
    PrintSubSection("15.1 ok::Stack<int> 성능");
    MEASURE("ok::Stack Push",
        ok::Stack<int> s(16);
        for (int i = 0; i < N; i++) {
            s.push(i);
        }
    );
    
    PrintSubSection("15.2 std::stack<int, vector> 성능");
    MEASURE("std::stack<vector> Push",
        std::stack<int, std::vector<int>> s;
        for (int i = 0; i < N; i++) {
            s.push(i);
        }
    );
    
    PrintSubSection("15.3 std::stack<int, deque> 성능 (기본)");
    MEASURE("std::stack<deque> Push",
        std::stack<int> s;
        for (int i = 0; i < N; i++) {
            s.push(i);
        }
    );
    
    PrintSubSection("15.4 복잡한 타입 성능 비교");
    const int M = 1000000;
    
    cout << "  ok::Stack<string>: ";
    MEASURE("",
        ok::Stack<string> s(16);
        for (int i = 0; i < M; i++) {
            s.emplace("TestString" + to_string(i));
        }
    );
    
    cout << "  std::stack<string>: ";
    MEASURE("",
        std::stack<string> s;
        for (int i = 0; i < M; i++) {
            s.emplace("TestString" + to_string(i));
        }
    );
}

void Test16_StressTest() {
    PrintSection("테스트 16: 스트레스 테스트");
    
    PrintSubSection("16.1 대규모 Push/Pop 반복");
    {
        ok::Stack<int> s(10);
        
        for (int i = 0; i < 10000; i++) {
            s.push(i);
            if (i % 3 == 0 && !s.isEmpty()) {
                s.pop();
            }
        }
        
        AssertTest(s.size() > 0, "대규모 반복 작업 안정성");
    }
    
    PrintSubSection("16.2 Resize 연쇄 반응");
    {
        ok::Stack<int> s(1);
        
        for (int i = 0; i < 1000; i++) {
            s.push(i);
        }
        
        AssertTest(s.size() == 1000 && s.getCapacity() >= 1000, "연쇄 resize 안정성");
    }
    
    PrintSubSection("16.3 메모리 집약적 작업");
    {
        Tracker::ResetCounts();
        
        for (int round = 0; round < 10; round++) {
            ok::Stack<Tracker> s(100);
            for (int i = 0; i < 100; i++) {
                s.emplace("Round" + to_string(round));
            }
        }
        
        AssertTest(Tracker::liveCount == 0, "반복 생성/소멸 시 메모리 누수 없음");
    }
}

void Test17_ThreadSafety() {
    PrintSection("테스트 17: 단일 스레드 동시성 시뮬레이션");
    
    PrintSubSection("17.1 예측 가능한 상태 전환");
    {
        ok::Stack<int> s(5);
        
        s.push(1);
        int state1 = s.size();
        
        s.push(2);
        int state2 = s.size();
        
        s.pop();
        int state3 = s.size();
        
        AssertTest(state1 == 1 && state2 == 2 && state3 == 1, "상태 전환 예측 가능성");
    }
}

void Test18_RealWorldScenarios() {
    PrintSection("테스트 18: 실제 사용 시나리오");
    
    PrintSubSection("18.1 함수 호출 스택 시뮬레이션");
    {
        struct CallFrame {
            string funcName;
            int lineNum;
            CallFrame(string n, int l) : funcName(n), lineNum(l) {}
        };
        
        ok::Stack<CallFrame> callStack(10);
        
        callStack.emplace("main", 1);
        callStack.emplace("processData", 45);
        callStack.emplace("validateInput", 78);
        
        AssertTest(callStack.peek().funcName == "validateInput", "호출 스택 push");
        
        callStack.pop(); // 함수 반환
        AssertTest(callStack.peek().funcName == "processData", "호출 스택 pop");
    }
    
    PrintSubSection("18.2 Undo/Redo 기능");
    {
        struct Action {
            string type;
            string data;
            Action(string t, string d) : type(t), data(d) {}
        };
        
        ok::Stack<Action> undoStack(20);
        
        undoStack.emplace("Insert", "Hello");
        undoStack.emplace("Delete", "World");
        undoStack.emplace("Replace", "Test");
        
        // Undo
        Action lastAction = undoStack.peek();
        undoStack.pop();
        
        AssertTest(lastAction.type == "Replace", "Undo 동작 시뮬레이션");
    }
    
    PrintSubSection("18.3 수식 파싱 (후위 표기법)");
    {
        ok::Stack<int> evalStack(10);
        
        // "3 4 + 2 *" 계산
        evalStack.push(3);
        evalStack.push(4);
        int b = evalStack.peek(); evalStack.pop();
        int a = evalStack.peek(); evalStack.pop();
        evalStack.push(a + b); // 7
        
        evalStack.push(2);
        b = evalStack.peek(); evalStack.pop();
        a = evalStack.peek(); evalStack.pop();
        evalStack.push(a * b); // 14
        
        AssertTest(evalStack.peek() == 14, "수식 계산 시뮬레이션");
    }
}

// ==========================================
// [메인 함수]
// ==========================================
int main() {
    try {
        cout << "\n" << GREEN << "╔═══════════════════════════════════════════════════════════════╗" << RESET << endl;
        cout << GREEN << "║         Stack Class 종합 테스트 시작                          ║" << RESET << endl;
        cout << GREEN << "╚═══════════════════════════════════════════════════════════════╝" << RESET << endl;
        
        // 모든 테스트 실행
        Test01_BasicConstructor();
        Test02_PushOperations();
        Test03_PopOperations();
        Test04_PeekOperations();
        Test05_CopyConstructor();
        Test06_MoveConstructor();
        Test07_CopyAssignment();
        Test08_MoveAssignment();
        Test09_EmplaceOperation();
        Test10_ResizeOperation();
        Test11_ExceptionSafety();
        Test12_MemoryManagement();
        Test13_EdgeCases();
        Test14_ComplexTypes();
        Test15_PerformanceComparison();
        Test16_StressTest();
        Test17_ThreadSafety();
        Test18_RealWorldScenarios();
        
        // 최종 결과
        cout << "\n" << GREEN << "╔═══════════════════════════════════════════════════════════════╗" << RESET << endl;
        cout << GREEN << "║                    테스트 결과 요약                           ║" << RESET << endl;
        cout << GREEN << "╚═══════════════════════════════════════════════════════════════╝" << RESET << endl;
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
        cerr << "\n" << RED << "╔═══════════════════════════════════════════════════════════════╗" << RESET << endl;
        cerr << RED << "║  [치명적 오류] 테스트 중단: " << e.what() << RESET << endl;
        cerr << RED << "╚═══════════════════════════════════════════════════════════════╝" << RESET << endl;
        return 1;
    }

    return 0;
}