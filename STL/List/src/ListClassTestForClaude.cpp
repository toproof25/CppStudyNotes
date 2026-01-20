// List Class Test Code

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include <chrono>
#include <type_traits>
#include <numeric>
#include <iterator>
#include <windows.h>
#include <psapi.h>
#include "../inc/List.hpp"

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
        // 테스트 실패 시 예외를 던지지 않고 계속 진행
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

    bool operator==(const Tracker& other) const {
        return data == other.data;
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

    Exploder(int i = 0) : id(i) {
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
    
    Exploder(Exploder&& other) noexcept : id(other.id) {}
    
    Exploder& operator=(const Exploder&) = default;
    Exploder& operator=(Exploder&&) noexcept = default;
    
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
    bool operator==(const NonMovable& other) const { return value == other.value; }
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
    bool operator==(const NonCopyable& other) const { return value == other.value; }
};

// 5. 무거운 객체 (성능 테스트용)
struct HeavyObject {
    std::vector<int> data;
    HeavyObject(int size = 1000) : data(size, 42) {}
    HeavyObject(const HeavyObject&) = default;
    HeavyObject(HeavyObject&&) noexcept = default;
    HeavyObject& operator=(const HeavyObject&) = default;
    HeavyObject& operator=(HeavyObject&&) noexcept = default;
};

// ==========================================
// [테스트 케이스]
// ==========================================

void Test01_BasicConstructor() {
    PrintSection("테스트 1: 기본 생성자 및 초기 상태");
    
    PrintSubSection("1.1 기본 생성자");
    {
        List<int> l;
        AssertTest(l.size() == 0, "초기 size() == 0");
    }
    
    PrintSubSection("1.2 다양한 타입의 리스트 생성");
    {
        List<double> l1;
        List<string> l2;
        List<char> l3;
        
        AssertTest(l1.size() == 0 && l2.size() == 0 && l3.size() == 0, "모든 타입 리스트 생성 성공");
    }
    
    PrintSubSection("1.3 복잡한 타입의 리스트 생성");
    {
        List<std::vector<int>> l1;
        List<Tracker> l2;
        
        AssertTest(l1.size() == 0 && l2.size() == 0, "복잡한 타입 리스트 생성 성공");
    }
}

void Test02_PushFrontOperations() {
    PrintSection("테스트 2: Push_front 연산 (L-value & R-value)");
    
    PrintSubSection("2.1 기본 타입 push_front (int)");
    {
        List<int> l;
        
        l.push_front(10);
        AssertTest(l.size() == 1 && l.front() == 10, "첫 번째 push_front");
        
        l.push_front(20);
        l.push_front(30);
        AssertTest(l.size() == 3 && l.front() == 30, "연속 push_front");
        
        int val = 40;
        l.push_front(val);
        AssertTest(l.front() == 40, "L-value push_front");
    }
    
    PrintSubSection("2.2 복사 vs 이동 추적");
    {
        Tracker::ResetCounts();
        List<Tracker> l;
        
        Tracker t1("L-value");
        l.push_front(t1);
        int copiesAfterLvalue = Tracker::totalCopies;
        
        l.push_front(Tracker("R-value"));
        int movesAfterRvalue = Tracker::totalMoves;
        
        AssertTest(copiesAfterLvalue > 0, "L-value push_front는 복사 발생", 
                   "(복사 횟수: " + to_string(copiesAfterLvalue) + ")");
        AssertTest(movesAfterRvalue > 0, "R-value push_front는 이동 발생", 
                   "(이동 횟수: " + to_string(movesAfterRvalue) + ")");
        
        Tracker::PrintStats();
    }
    
    PrintSubSection("2.3 대량 데이터 push_front");
    {
        List<int> l;
        
        for (int i = 0; i < 100; i++) {
            l.push_front(i);
        }
        
        AssertTest(l.size() == 100, "100개 삽입 성공");
        AssertTest(l.front() == 99, "LIFO 순서 확인");
    }
}

void Test03_PushBackOperations() {
    PrintSection("테스트 3: Push_back 연산 (L-value & R-value)");
    
    PrintSubSection("3.1 기본 타입 push_back (int)");
    {
        List<int> l;
        
        l.push_back(10);
        AssertTest(l.size() == 1 && l.back() == 10, "첫 번째 push_back");
        
        l.push_back(20);
        l.push_back(30);
        AssertTest(l.size() == 3 && l.back() == 30, "연속 push_back");
        
        int val = 40;
        l.push_back(val);
        AssertTest(l.back() == 40, "L-value push_back");
    }
    
    PrintSubSection("3.2 순서 확인");
    {
        List<int> l;
        for (int i = 1; i <= 5; i++) {
            l.push_back(i);
        }
        
        auto it = l.begin();
        bool orderCorrect = true;
        for (int i = 1; i <= 5; i++, ++it) {
            if (*it != i) {
                orderCorrect = false;
                break;
            }
        }
        
        AssertTest(orderCorrect, "push_back 순서 유지 확인");
    }
    
    PrintSubSection("3.3 대용량 문자열 push_back");
    {
        List<string> l;
        string longStr(10000, 'A');
        
        l.push_back(longStr);
        l.push_back(string(10000, 'B'));
        
        AssertTest(l.front().length() == 10000 && l.front()[0] == 'A', "대용량 문자열 push_back 성공");
    }
}

void Test04_PopFrontOperations() {
    PrintSection("테스트 4: Pop_front 연산");
    
    PrintSubSection("4.1 정상 pop_front 동작");
    {
        List<int> l;
        l.push_back(10);
        l.push_back(20);
        l.push_back(30);
        
        AssertTest(l.size() == 3, "pop_front 전 크기 확인");
        
        l.pop_front();
        AssertTest(l.size() == 2 && l.front() == 20, "첫 번째 pop_front 후");
        
        l.pop_front();
        AssertTest(l.size() == 1 && l.front() == 30, "두 번째 pop_front 후");
        
        l.pop_front();
        AssertTest(l.size() == 0, "모든 요소 pop_front 후");
    }
    
    PrintSubSection("4.2 객체 소멸 추적");
    {
        Tracker::ResetCounts();
        {
            List<Tracker> l;
            l.push_back(Tracker("A"));
            l.push_back(Tracker("B"));
            l.push_back(Tracker("C"));
            
            int destructBefore = Tracker::totalDestructs;
            l.pop_front();
            int destructAfter = Tracker::totalDestructs;
            
            AssertTest(destructAfter > destructBefore, "pop_front 시 객체 소멸 확인");
        }
        AssertTest(Tracker::liveCount == 0, "리스트 소멸 시 모든 객체 해제");
    }
}

void Test05_PopBackOperations() {
    PrintSection("테스트 5: Pop_back 연산");
    
    PrintSubSection("5.1 정상 pop_back 동작");
    {
        List<int> l;
        l.push_back(10);
        l.push_back(20);
        l.push_back(30);
        
        l.pop_back();
        AssertTest(l.size() == 2 && l.back() == 20, "첫 번째 pop_back 후");
        
        l.pop_back();
        AssertTest(l.size() == 1 && l.back() == 10, "두 번째 pop_back 후");
        
        l.pop_back();
        AssertTest(l.size() == 0, "모든 요소 pop_back 후");
    }
}

void Test06_AccessOperations() {
    PrintSection("테스트 6: 접근 연산 (front, back)");
    
    PrintSubSection("6.1 front() 함수");
    {
        List<string> l;
        l.push_back("First");
        l.push_back("Second");
        
        AssertTest(l.front() == "First", "front() 반환값 확인");
        
        l.front() = "Modified";
        AssertTest(l.front() == "Modified", "front() 수정 확인");
    }
    
    PrintSubSection("6.2 back() 함수");
    {
        List<string> l;
        l.push_back("First");
        l.push_back("Last");
        
        AssertTest(l.back() == "Last", "back() 반환값 확인");
        
        l.back() = "Changed";
        AssertTest(l.back() == "Changed", "back() 수정 확인");
    }
}

void Test07_CopyConstructor() {
    PrintSection("테스트 7: 복사 생성자 (Deep Copy)");
    
    PrintSubSection("7.1 기본 타입 복사");
    {
        List<int> original;
        original.push_back(1);
        original.push_back(2);
        original.push_back(3);
        
        List<int> copy(original);
        
        AssertTest(copy.size() == original.size(), "복사된 리스트 크기 일치");
        
        auto it1 = original.begin();
        auto it2 = copy.begin();
        bool dataMatch = true;
        for (; it1 != original.end(); ++it1, ++it2) {
            if (*it1 != *it2) {
                dataMatch = false;
                break;
            }
        }
        AssertTest(dataMatch, "복사된 리스트 데이터 일치");
    }
    
    PrintSubSection("7.2 깊은 복사 확인 (독립성)");
    {
        List<int> l1;
        l1.push_back(100);
        l1.push_back(200);
        
        List<int> l2(l1);
        
        l2.front() = 999;
        l2.push_back(300);
        
        AssertTest(l1.front() == 100, "원본 리스트 변경 없음");
        AssertTest(l1.size() == 2, "원본 리스트 크기 불변");
        AssertTest(l2.front() == 999 && l2.size() == 3, "복사본 독립적 수정");
    }
    
    PrintSubSection("7.3 복잡한 타입 복사");
    {
        List<std::vector<int>> l1;
        l1.push_back(std::vector<int>{1, 2, 3});
        l1.push_back(std::vector<int>{4, 5});
        
        List<std::vector<int>> l2(l1);
        
        auto it = l2.begin();
        AssertTest((*it).size() == 3, "복사본 첫 번째 vector 확인");
        ++it;
        AssertTest((*it).size() == 2, "복사본 두 번째 vector 확인");
    }
    
    PrintSubSection("7.4 빈 리스트 복사");
    {
        List<int> empty;
        List<int> copy(empty);
        
        AssertTest(copy.size() == 0, "빈 리스트 복사 성공");
    }
}

void Test08_MoveConstructor() {
    PrintSection("테스트 8: 이동 생성자");
    
    PrintSubSection("8.1 기본 이동 동작");
    {
        List<int> l1;
        l1.push_back(10);
        l1.push_back(20);
        l1.push_back(30);
        
        size_t originalSize = l1.size();
        List<int> l2(std::move(l1));
        
        AssertTest(l2.size() == originalSize && l2.front() == 10, "이동 후 데이터 유지");
        AssertTest(l1.size() == 0, "이동 후 원본은 빈 상태");
    }
    
    PrintSubSection("8.2 이동 효율성 확인");
    {
        Tracker::ResetCounts();
        List<Tracker> l1;
        l1.push_back(Tracker("A"));
        l1.push_back(Tracker("B"));
        
        int copiesBefore = Tracker::totalCopies;
        List<Tracker> l2(std::move(l1));
        int copiesAfter = Tracker::totalCopies;
        
        AssertTest(copiesBefore == copiesAfter, "이동 시 복사 발생 없음 (효율적)");
    }
}

void Test09_CopyAssignment() {
    PrintSection("테스트 9: 복사 대입 연산자");
    
    PrintSubSection("9.1 기본 대입 동작");
    {
        List<int> l1;
        l1.push_back(1);
        l1.push_back(2);
        
        List<int> l2;
        l2.push_back(999);
        
        l2 = l1;
        
        AssertTest(l2.front() == 1 && l2.size() == 2, "대입 후 데이터 복사 확인");
    }
    
    PrintSubSection("9.2 자기 대입");
    {
        List<int> l;
        l.push_back(100);
        l.push_back(200);
        
        l = l;
        
        AssertTest(l.front() == 100 && l.size() == 2, "자기 대입 후 데이터 유지");
    }
    
    PrintSubSection("9.3 대입 후 독립성");
    {
        List<string> l1;
        l1.push_back("Original");
        
        List<string> l2;
        l2 = l1;
        
        l2.push_back("Modified");
        
        AssertTest(l1.size() == 1, "원본 크기 불변"); 
        AssertTest(l2.size() == 2, "대입본 독립적 동작");
    }
}

void Test10_MoveAssignment() {
    PrintSection("테스트 10: 이동 대입 연산자");
    
    PrintSubSection("10.1 기본 이동 대입");
    {
        List<int> l1;
        l1.push_back(10);
        l1.push_back(20);
        
        List<int> l2;
        l2 = std::move(l1);
        
        AssertTest(l2.front() == 10 && l2.size() == 2, "이동 대입 후 데이터 확인");
        AssertTest(l1.size() == 0, "이동 후 원본은 빈 상태");
    }
}

void Test11_EmplaceFrontOperation() {
    PrintSection("테스트 11: Emplace_front 연산 (효율성)");
    
    PrintSubSection("11.1 emplace_front 기본 동작");
    {
        List<string> l;
        l.emplace_front("Direct");
        l.emplace_front("Construction");
        
        AssertTest(l.front() == "Construction", "emplace_front 정상 동작");
    }
    
    PrintSubSection("11.2 emplace_front 효율성 비교");
    {
        Tracker::ResetCounts();
        List<Tracker> l1;
        
        l1.push_front(Tracker("Push"));
        int copiesAfterPush = Tracker::totalCopies + Tracker::totalMoves;
        
        Tracker::ResetCounts();
        List<Tracker> l2;
        
        l2.emplace_front("Emplace");
        int copiesAfterEmplace = Tracker::totalCopies + Tracker::totalMoves;
        
        cout << "    (Push: " << copiesAfterPush << ", Emplace: " << copiesAfterEmplace << ")" << endl;
        AssertTest(copiesAfterEmplace <= copiesAfterPush, "emplace_front이 더 효율적이거나 동등");
    }
    
    PrintSubSection("11.3 다중 인자 emplace_front");
    {
        struct Point {
            int x, y;
            Point(int x = 0, int y = 0) : x(x), y(y) {}
        };
        
        List<Point> l;
        l.emplace_front(10, 20);
        l.emplace_front(30, 40);
        
        AssertTest(l.front().x == 30 && l.front().y == 40, "다중 인자 직접 생성");
    }
}

void Test12_EmplaceBackOperation() {
    PrintSection("테스트 12: Emplace_back 연산 (효율성)");
    
    PrintSubSection("12.1 emplace_back 기본 동작");
    {
        List<string> l;
        l.emplace_back("Direct");
        l.emplace_back("Construction");
        
        AssertTest(l.back() == "Construction", "emplace_back 정상 동작");
    }
    
    PrintSubSection("12.2 다중 인자 emplace_back");
    {
        struct Point {
            int x, y;
            Point(int x = 0, int y = 0) : x(x), y(y) {}
        };
        
        List<Point> l;
        l.emplace_back(10, 20);
        l.emplace_back(30, 40);
        
        AssertTest(l.back().x == 30 && l.back().y == 40, "다중 인자 직접 생성");
    }
}

void Test13_InsertOperation() {
    PrintSection("테스트 13: Insert 연산");
    
    PrintSubSection("13.1 중간 삽입 (L-value)");
    {
        List<int> l;
        l.push_back(1);
        l.push_back(3);
        l.push_back(5);

        int val = 2;
        auto it = l.begin();
        ++it;
        l.insert(it, val);
        
        AssertTest(l.size() == 4, "insert 후 크기 증가");
        
        auto checkIt = l.begin();
        AssertTest(*checkIt == 1, "첫 번째 요소 확인");
        ++checkIt;
        AssertTest(*checkIt == 2, "삽입된 요소 확인");
        ++checkIt;
        AssertTest(*checkIt == 3, "세 번째 요소 확인");
    }
    
    PrintSubSection("13.2 중간 삽입 (R-value)");
    {
        List<string> l;
        l.push_back("A");
        l.push_back("C");
        
        auto it = l.begin();
        ++it;
        l.insert(it, string("B"));
        
        auto checkIt = l.begin();
        AssertTest(*checkIt == "A", "첫 번째");
        ++checkIt;
        AssertTest(*checkIt == "B", "삽입된 요소");
        ++checkIt;
        AssertTest(*checkIt == "C", "마지막");
    }
    
    PrintSubSection("13.3 맨 앞 삽입");
    {
        List<int> l;
        l.push_back(2);
        l.push_back(3);
        
        l.insert(l.begin(), 1);
        
        AssertTest(l.front() == 1 && l.size() == 3, "맨 앞 삽입 성공");
    }
    
    PrintSubSection("13.4 맨 뒤 삽입");
    {
        List<int> l;
        l.push_back(1);
        l.push_back(2);
        
        auto it = l.end();
        l.insert(it, 3);
        
        AssertTest(l.back() == 3 && l.size() == 3, "맨 뒤 삽입 성공");
    }
}

void Test14_EmplaceOperation() {
    PrintSection("테스트 14: Emplace 연산");
    
    PrintSubSection("14.1 중간 emplace");
    {
        struct Point {
            int x, y;
            Point(int x = 0, int y = 0) : x(x), y(y) {}
        };
        
        List<Point> l;
        l.emplace_back(1, 1);
        l.emplace_back(3, 3);
        
        auto it = l.begin();
        ++it;
        l.emplace(it, 2, 2);
        
        AssertTest(l.size() == 3, "emplace 후 크기");
        
        auto checkIt = l.begin();
        AssertTest((*checkIt).x == 1, "첫 번째 요소");
        ++checkIt;
        AssertTest((*checkIt).x == 2, "emplace된 요소");
        ++checkIt;
        AssertTest((*checkIt).x == 3, "세 번째 요소");
    }
}

void Test15_EraseOperation() {
    PrintSection("테스트 15: Erase 연산");
    
    PrintSubSection("15.1 중간 삭제");
    {
        List<int> l;
        for (int i = 1; i <= 5; i++) {
            l.push_back(i);
        }
        
        auto it = l.begin();
        ++it; ++it; // 3번째 요소
        l.erase(it);
        
        AssertTest(l.size() == 4, "erase 후 크기 감소");
        
        auto checkIt = l.begin();
        AssertTest(*checkIt == 1, "1");
        ++checkIt;
        AssertTest(*checkIt == 2, "2");
        ++checkIt;
        AssertTest(*checkIt == 4, "4 (3이 삭제됨)");
        ++checkIt;
        AssertTest(*checkIt == 5, "5");
    }
    
    PrintSubSection("15.2 맨 앞 삭제");
    {
        List<int> l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(3);
        
        l.erase(l.begin());
        
        AssertTest(l.front() == 2 && l.size() == 2, "맨 앞 삭제 성공");
    }
    
    PrintSubSection("15.3 맨 뒤 삭제");
    {
        List<int> l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(3);
        
        auto it = l.begin();
        ++it; ++it; // 마지막 요소
        l.erase(it);
        
        AssertTest(l.size() == 2 && l.back() == 2, "맨 뒤 삭제 성공");
    }
    
    PrintSubSection("15.4 erase 반환 값 확인");
    {
        List<int> l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(3);
        
        auto it = l.begin();
        ++it;
        auto nextIt = l.erase(it);
        
        AssertTest(*nextIt == 3, "erase 후 다음 요소 반환");
    }
}

void Test16_RemoveOperation() {
    PrintSection("테스트 16: Remove 연산 (값으로 삭제)");
  
    PrintSubSection("16.1 front값 제거");
    {
        List<int> l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(3);
        
        l.remove(1);
        
        AssertTest(l.size() == 2, "remove 후 크기 감소");
        
        auto it = l.begin();
        AssertTest(*it == 2, "첫 번째 요소");
        ++it;
        AssertTest(*it == 3, "두 번째 요소");
    }

    PrintSubSection("16.2 단일 값 제거");
    {
        List<int> l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(3);
        
        l.remove(2);
        
        AssertTest(l.size() == 2, "remove 후 크기 감소");
        
        auto it = l.begin();
        AssertTest(*it == 1, "첫 번째 요소");
        ++it;
        AssertTest(*it == 3, "두 번째 요소");
    }
    
    PrintSubSection("16.3 중복 값 모두 제거");
    {
        List<int> l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(2);
        l.push_back(3);
        l.push_back(2);
        
        l.remove(2);
        
        AssertTest(l.size() == 2, "중복 값 모두 제거");
        
        auto it = l.begin();
        AssertTest(*it == 1, "1");
        ++it;
        AssertTest(*it == 3, "3");
    }
    
    PrintSubSection("16.4 존재하지 않는 값 제거");
    {
        List<int> l;
        l.push_back(1);
        l.push_back(2);
        
        l.remove(999);
        
        AssertTest(l.size() == 2, "존재하지 않는 값 제거 시 크기 불변");
    }
    
    PrintSubSection("16.5 모든 요소 제거");
    {
        List<int> l;
        l.push_back(5);
        l.push_back(5);
        l.push_back(5);
        
        l.remove(5);
        
        AssertTest(l.size() == 0, "모든 요소 제거 성공");
    }
}

void Test17_IteratorOperations() {
    PrintSection("테스트 17: 반복자 연산");
    
    PrintSubSection("17.1 begin/end 반복");
    {
        List<int> l;
        for (int i = 0; i < 5; i++) {
            l.push_back(i);
        }
        
        int sum = 0;
        for (auto it = l.begin(); it != l.end(); ++it) {
            sum += *it;
        }
        
        AssertTest(sum == 10, "반복자를 통한 순회");
    }
    
    PrintSubSection("17.2 범위 기반 for 루프");
    {
        List<int> l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(3);
        
        int product = 1;
        for (const auto& val : l) {
            product *= val;
        }
        
        AssertTest(product == 6, "범위 기반 for 루프 동작");
    }
    
    PrintSubSection("17.3 const 반복자");
    {
        List<int> l;
        l.push_back(10);
        l.push_back(20);
        
        int sum = 0;
        for (auto it = l.cbegin(); it != l.cend(); ++it) {
            sum += *it;
        }
        
        AssertTest(sum == 30, "const 반복자 동작");
    }
    
    PrintSubSection("17.4 역방향 반복자");
    {
        List<int> l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(3);
        
        std::vector<int> reversed;
        for (auto it = l.rbegin(); it != l.rend(); ++it) {
            reversed.push_back(*it);
        }
        
        AssertTest(reversed[0] == 3 && reversed[1] == 2 && reversed[2] == 1, 
                   "역방향 반복자 동작");
    }
    
    PrintSubSection("17.5 후위 증가 연산자");
    {
        List<int> l;
        l.push_back(10);
        l.push_back(20);
        l.push_back(30);
        
        auto it = l.begin();
        int first = *it++;
        int second = *it;
        
        AssertTest(first == 10 && second == 20, "후위 증가 연산자 동작");
    }
    
    PrintSubSection("17.6 전위 감소 연산자");
    {
        List<int> l;
        l.push_back(10);
        l.push_back(20);
        
        auto it = l.begin();
        ++it;
        --it;
        
        AssertTest(*it == 10, "전위 감소 연산자 동작");
    }
}

void Test18_MemoryManagement() {
    PrintSection("테스트 18: 메모리 관리 및 누수 검사");
    
    PrintSubSection("18.1 리스트 소멸 시 객체 해제");
    {
        Tracker::ResetCounts();
        
        {
            List<Tracker> l;
            l.push_back(Tracker("A"));
            l.push_back(Tracker("B"));
            l.push_back(Tracker("C"));
        }
        
        AssertTest(Tracker::liveCount == 0, "모든 객체 정상 소멸 (메모리 누수 없음)");
    }
    
    PrintSubSection("18.2 대량 삽입 시 메모리 관리");
    {
        Tracker::ResetCounts();
        
        List<Tracker> l;
        for (int i = 0; i < 1000; i++) {
            l.push_back(Tracker("Item"));
        }
        
        AssertTest(Tracker::liveCount == 1000, "1000개 객체 생존");
    }
    
    PrintSubSection("18.3 복사 후 독립적 메모리");
    {
        List<int> l1;
        l1.push_back(1);
        l1.push_back(2);
        
        List<int> l2(l1);
        
        // 서로 다른 메모리를 사용하는지 확인
        l1.push_back(3);
        
        AssertTest(l1.size() == 3 && l2.size() == 2, "독립적인 메모리 사용");
    }
}

void Test19_ExceptionSafety() {
    PrintSection("테스트 19: 예외 안전성");
    
    PrintSubSection("19.1 복사 생성 중 예외");
    {
        Exploder::Reset();
        List<Exploder> l1;
        l1.push_back(Exploder(1));
        l1.push_back(Exploder(2));
        
        Exploder::throwAtCopy = 2;
        
        bool caught = false;
        try {
            List<Exploder> l2(l1);
        } catch (const std::exception&) {
            caught = true;
        }
        
        AssertTest(caught, "복사 중 예외 발생");
        Exploder::Reset();
    }
}

void Test20_EdgeCases() {
    PrintSection("테스트 20: 경계 조건");
    
    PrintSubSection("20.1 단일 요소 리스트");
    {
        List<int> l;
        l.push_back(42);
        
        AssertTest(l.size() == 1 && l.front() == 42 && l.back() == 42, 
                   "단일 요소 리스트");
        
        l.pop_back();
        AssertTest(l.size() == 0, "단일 요소 제거");
    }
    
    PrintSubSection("20.2 빈 리스트 연산");
    {
        List<int> l;
        
        AssertTest(l.size() == 0, "빈 리스트 크기 확인");
    }
    
    PrintSubSection("20.3 NonMovable 타입");
    {
        List<NonMovable> l;
        NonMovable nm(10);
        l.push_back(nm);
        
        AssertTest(l.front().value == 10, "NonMovable 타입 push_back 성공");
    }
    
    PrintSubSection("20.4 NonCopyable 타입");
    {
        List<NonCopyable> l;
        l.push_back(NonCopyable(20));
        
        AssertTest(l.front().value == 20, "NonCopyable 타입 이동 push_back 성공");
    }
    
    PrintSubSection("20.5 front/back 같은 요소");
    {
        List<int> l;
        l.push_back(100);
        
        l.front() = 200;
        
        AssertTest(l.back() == 200, "단일 요소에서 front와 back은 같은 참조");
    }
}

void Test21_ComplexTypes() {
    PrintSection("테스트 21: 복잡한 타입 지원");
    
    PrintSubSection("21.1 중첩 컨테이너");
    {
        List<std::vector<int>> l;
        
        std::vector<int> inner1 = {1, 2, 3};
        std::vector<int> inner2 = {4, 5};
        
        l.push_back(inner1);
        l.push_back(inner2);
        
        AssertTest(l.front().size() == 3 && l.back().size() == 2, "중첩 vector 지원");
    }
    
    PrintSubSection("21.2 포인터 타입");
    {
        List<int*> l;
        int* ptr = new int(999);
        
        l.push_back(ptr);
        AssertTest(*(l.front()) == 999, "포인터 타입 저장");
        
        delete ptr;
    }
    
    PrintSubSection("21.3 구조체 타입");
    {
        struct Data {
            int id;
            string name;
            Data(int i, string n) : id(i), name(n) {}
        };
        
        List<Data> l;
        l.emplace_back(1, "Alice");
        l.emplace_back(2, "Bob");
        
        AssertTest(l.front().id == 1 && l.front().name == "Alice", "구조체 타입 지원");
    }
}

void Test22_AlgorithmCompatibility() {
    PrintSection("테스트 22: STL 알고리즘 호환성");
    
    PrintSubSection("22.1 std::sort (list는 자체 sort 사용)");
    {
        List<int> l;
        l.push_back(5);
        l.push_back(2);
        l.push_back(8);
        l.push_back(1);
        l.push_back(9);
        
        // List의 iterator를 사용하여 vector로 복사 후 정렬
        std::vector<int> vec;
        for (auto it = l.begin(); it != l.end(); ++it) {
            vec.push_back(*it);
        }
        std::sort(vec.begin(), vec.end());
        
        AssertTest(vec[0] == 1 && vec[1] == 2 && vec[2] == 5, "정렬 결과 확인");
    }
    
    PrintSubSection("22.2 std::find (수동 구현)");
    {
        List<int> l;
        for (int i = 0; i < 10; i++) {
            l.push_back(i);
        }
        
        // 수동 find 구현
        auto it = l.begin();
        bool found = false;
        for (; it != l.end(); ++it) {
            if (*it == 5) {
                found = true;
                break;
            }
        }
        
        AssertTest(found && *it == 5, "find 동작 확인");
    }
    
    PrintSubSection("22.3 std::accumulate (수동 구현)");
    {
        List<int> l;
        for (int i = 1; i <= 5; i++) {
            l.push_back(i);
        }
        
        // 수동 accumulate 구현
        int sum = 0;
        for (auto it = l.begin(); it != l.end(); ++it) {
            sum += *it;
        }
        
        AssertTest(sum == 15, "accumulate 동작 확인");
    }
    
    PrintSubSection("22.4 값 카운트");
    {
        List<int> l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(2);
        l.push_back(3);
        l.push_back(2);
        
        int count = 0;
        for (auto it = l.begin(); it != l.end(); ++it) {
            if (*it == 2) count++;
        }
        
        AssertTest(count == 3, "값 카운트 호환");
    }
}

void Test23_ProblemSolving() {
    PrintSection("테스트 23: 실제 문제 풀이 시나리오");
    
    PrintSubSection("23.1 LRU 캐시 시뮬레이션");
    {
        List<int> cache;
        int capacity = 3;
        
        // 1, 2, 3 추가
        cache.push_back(1);
        cache.push_back(2);
        cache.push_back(3);
        
        // 4 추가 (1 제거됨)
        if (cache.size() >= capacity) {
            cache.pop_front();
        }
        cache.push_back(4);
        
        AssertTest(cache.size() == 3 && cache.front() == 2, "LRU 캐시 동작");
    }
    
    PrintSubSection("23.2 작업 큐 관리");
    {
        struct Task {
            int id;
            string name;
            Task(int i, string n) : id(i), name(n) {}
        };
        
        List<Task> taskQueue;
        taskQueue.emplace_back(1, "Task1");
        taskQueue.emplace_back(2, "Task2");
        taskQueue.emplace_back(3, "Task3");
        
        // 첫 번째 작업 처리
        taskQueue.pop_front();
        
        AssertTest(taskQueue.front().id == 2, "작업 큐 FIFO 동작");
    }
    
    PrintSubSection("23.3 브라우저 히스토리");
    {
        List<string> history;
        history.push_back("google.com");
        history.push_back("github.com");
        history.push_back("stackoverflow.com");
        
        // 뒤로가기
        history.pop_back();
        
        AssertTest(history.back() == "github.com", "브라우저 히스토리 뒤로가기");
        
        // 새 페이지
        history.push_back("reddit.com");
        
        AssertTest(history.back() == "reddit.com" && history.size() == 3, 
                   "새 페이지 추가");
    }
    
    PrintSubSection("23.4 josephus 문제");
    {
        List<int> people;
        for (int i = 1; i <= 7; i++) {
            people.push_back(i);
        }
        
        int k = 3;
        auto it = people.begin();
        
        while (people.size() > 1) {
            for (int i = 1; i < k; i++) {
                ++it;
                if (it == people.end()) {
                    it = people.begin();
                }
            }
            
            auto toRemove = it;
            ++it;
            if (it == people.end()) {
                it = people.begin();
            }
            
            people.erase(toRemove);
        }
        
        AssertTest(people.size() == 1, "Josephus 문제 해결");
    }
}

void Test24_PerformanceComparison() {
    PrintSection("테스트 24: 종합 성능 비교 (List vs std::list)");
    
    const int N = 5000000;
    
    cout << "  데이터 개수: " << N << "개" << endl << endl;
    
    PrintSubSection("24.1 [int] Push_back (삽입) 속도");
    
    MEASURE("List<int> Push_back",
        cout << "List<int> Push_back : ";
        List<int> l;
        for (int i = 0; i < N; i++) {
            l.push_back(i);
        }
    );
    
    MEASURE("std::list<int> Push_back",
        cout << "std::list<int> Push_back : ";
        std::list<int> l;
        for (int i = 0; i < N; i++) {
            l.push_back(i);
        }
    );
    
    PrintSubSection("24.2 [int] Push_front (앞 삽입) 속도");
    
    MEASURE("List<int> Push_front",
        cout << "List<int> Push_front : ";
        List<int> l;
        for (int i = 0; i < N; i++) {
            l.push_front(i);
        }
    );
    
    MEASURE("std::list<int> Push_front",
        cout << "std::list<int> Push_front : ";
        std::list<int> l;
        for (int i = 0; i < N; i++) {
            l.push_front(i);
        }
    );
    
    PrintSubSection("24.3 [int] 순회 (Iteration) 속도");
    
    {
        List<int> myList;
        std::list<int> stlList;
        for(int i = 0; i < N; i++) {
            myList.push_back(i);
            stlList.push_back(i);
        }
        
        MEASURE("List<int> Iteration",
            cout << "List<int> Iteration : ";
            long long sum = 0;
            for(auto it = myList.begin(); it != myList.end(); ++it) {
                sum += *it;
            }
        );
        
        MEASURE("std::list<int> Iteration",
            cout << "std::list<int> Iteration : ";
            long long sum = 0;
            for(auto it = stlList.begin(); it != stlList.end(); ++it) {
                sum += *it;
            }
        );
    }
    
    PrintSubSection("24.4 [HeavyObject] Push_back (이동 시맨틱) 속도");
    
    MEASURE("List<Heavy> Push_back",
        cout << "List<HeavyObject> Push_back : ";
        List<HeavyObject> l;
        for (int i = 0; i < N / 10; i++) {
            l.push_back(HeavyObject());
        }
    );
    
    MEASURE("std::list<Heavy> Push_back",
        cout << "std::list<HeavyObject> Push_back : ";
        std::list<HeavyObject> l;
        for (int i = 0; i < N / 10; i++) {
            l.push_back(HeavyObject());
        }
    );
    
    PrintSubSection("24.5 [Emplace_back] 직접 생성 최적화");
    
    MEASURE("List Emplace_back",
        cout << "List<HeavyObject> Emplace_back : ";
        List<HeavyObject> l;
        for (int i = 0; i < N / 10; i++) {
            l.emplace_back(1000);
        }
    );
    
    MEASURE("std::list Emplace_back",
        cout << "std::list<HeavyObject> Emplace_back : ";
        std::list<HeavyObject> l;
        for (int i = 0; i < N / 10; i++) {
            l.emplace_back(1000);
        }
    );
    
    PrintSubSection("24.6 [Insert] 중간 삽입 성능");
    
    const int InsertCount = 10000;
    
    MEASURE("List Insert (중간)",
        cout << "List<int> Insert (중간) : ";
        List<int> l;
        for(int i = 0; i < InsertCount; i++) {
            l.push_back(i);
        }
        for(int i = 0; i < InsertCount; i++) {
            auto it = l.begin();
            for (int j = 0; j < l.size() / 2 && it != l.end(); j++) {
                ++it;
            }
            if (it != l.end()) {
                l.insert(it, i);
            }
        }
    );
    
    MEASURE("std::list Insert (중간)",
        cout << "std::list<int> Insert (중간) : ";
        std::list<int> l;
        for(int i = 0; i < InsertCount; i++) {
            l.push_back(i);
        }
        for(int i = 0; i < InsertCount; i++) {
            auto it = l.begin();
            std::advance(it, l.size() / 2);
            l.insert(it, i);
        }
    );
    
    PrintSubSection("24.7 [Erase] 중간 삭제 성능");
    
    const int EraseCount = 10000;
    
    MEASURE("List Erase (중간)",
        cout << "List<int> Erase (중간) : ";
        List<int> l;
        for(int i = 0; i < EraseCount; i++) {
            l.push_back(i);
        }
        while(l.size() > 0) {
            auto it = l.begin();
            for (int j = 0; j < l.size() / 2 && it != l.end(); j++) {
                ++it;
            }
            if (it != l.end()) {
                l.erase(it);
            } else if (l.size() > 0) {
                l.erase(l.begin());
            }
        }
    );
    
    MEASURE("std::list Erase (중간)",
        cout << "std::list<int> Erase (중간) : ";
        std::list<int> l;
        for(int i = 0; i < EraseCount; i++) {
            l.push_back(i);
        }
        while(l.size() > 0) {
            auto it = l.begin();
            if (l.size() > 1) {
                std::advance(it, l.size() / 2);
            }
            l.erase(it);
        }
    );
    
    PrintSubSection("24.8 [Remove] 값 제거 성능");
    
    const int RemoveCount = 50000;
    
    MEASURE("List Remove",
        cout << "List<int> Remove : ";
        List<int> l;
        for(int i = 0; i < RemoveCount; i++) {
            l.push_back(i % 100);
        }
        for(int i = 0; i < 50; i++) {
            l.remove(i);
        }
    );
    
    MEASURE("std::list Remove",
        cout << "std::list<int> Remove : ";
        std::list<int> l;
        for(int i = 0; i < RemoveCount; i++) {
            l.push_back(i % 100);
        }
        for(int i = 0; i < 50; i++) {
            l.remove(i);
        }
    );
}

void Test25_StressTest() {
    PrintSection("테스트 25: 스트레스 테스트");
    
    PrintSubSection("25.1 대규모 Push/Pop 반복");
    {
        List<int> l;
        
        for (int i = 0; i < 100000; i++) {
            l.push_back(i);
        }
        
        while (l.size() > 0) {
            l.pop_back();
        }
        
        AssertTest(l.size() == 0, "대규모 작업 안정성");
    }
    
    PrintSubSection("25.2 반복적 삽입/삭제");
    {
        List<int> l;
        
        for (int cycle = 0; cycle < 10; cycle++) {
            for (int i = 0; i < 10000; i++) {
                l.push_back(i);
            }
            
            for (int i = 0; i < 5000; i++) {
                l.pop_front();
            }
            
            for (int i = 0; i < 5000; i++) {
                l.pop_back();
            }
        }
        
        AssertTest(l.size() == 0, "반복적 작업 안정성");
    }
    
    PrintSubSection("25.3 혼합 작업 스트레스");
    {
        List<int> l;
        
        for (int i = 0; i < 1000; i++) {
            l.push_back(i);
            l.push_front(i);
            
            if (i % 3 == 0 && l.size() > 0) {
                l.pop_back();
            }
            if (i % 5 == 0 && l.size() > 0) {
                l.pop_front();
            }
        }
        
        AssertTest(l.size() > 0, "혼합 작업 안정성");
    }
}

void Test26_RealWorldScenarios() {
    PrintSection("테스트 26: 실제 사용 시나리오");
    
    PrintSubSection("26.1 음악 재생 목록");
    {
        struct Song {
            string title;
            int duration;
            Song(string t, int d) : title(t), duration(d) {}
        };
        
        List<Song> playlist;
        playlist.emplace_back("Song1", 180);
        playlist.emplace_back("Song2", 210);
        playlist.emplace_back("Song3", 195);
        
        // 현재 곡 재생 완료
        playlist.pop_front();
        
        AssertTest(playlist.front().title == "Song2", "재생 목록 관리");
        
        // 새 곡 추가
        playlist.emplace_back("Song4", 200);
        
        AssertTest(playlist.size() == 3, "재생 목록 크기 확인");
    }
    
    PrintSubSection("26.2 텍스트 에디터 Undo/Redo");
    {
        List<string> undoStack;
        undoStack.push_back("Initial text");
        undoStack.push_back("Added line 1");
        undoStack.push_back("Added line 2");
        
        // Undo
        string lastAction = undoStack.back();
        undoStack.pop_back();
        
        AssertTest(undoStack.back() == "Added line 1", "Undo 동작");
        AssertTest(lastAction == "Added line 2", "Undo된 액션 저장");
    }
    
    PrintSubSection("26.3 이벤트 로깅 시스템");
    {
        struct LogEntry {
            int timestamp;
            string message;
            LogEntry(int t, string m) : timestamp(t), message(m) {}
        };
        
        List<LogEntry> logs;
        logs.emplace_back(100, "System started");
        logs.emplace_back(200, "User logged in");
        logs.emplace_back(300, "File uploaded");
        
        // 오래된 로그 제거
        if (logs.size() > 2) {
            logs.pop_front();
        }
        
        AssertTest(logs.front().message == "User logged in", "로그 관리");
    }
    
    PrintSubSection("26.4 프로세스 스케줄링");
    {
        struct Process {
            int pid;
            int priority;
            Process(int p, int pr) : pid(p), priority(pr) {}
        };
        
        List<Process> processQueue;
        processQueue.emplace_back(1, 5);
        processQueue.emplace_back(2, 3);
        processQueue.emplace_back(3, 7);
        
        // 첫 번째 프로세스 실행
        processQueue.pop_front();
        
        AssertTest(processQueue.front().pid == 2, "프로세스 스케줄링");
        
        // 새 프로세스 추가
        processQueue.emplace_back(4, 6);
        
        AssertTest(processQueue.size() == 3, "프로세스 큐 크기");
    }
}

void Test27_PrintList() {
    PrintSection("테스트 27: printList 함수 테스트");
    
    PrintSubSection("27.1 printList 기본 동작");
    {
        List<int> l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(3);
        
        cout << "    ";
        l.printList();
        
        AssertTest(true, "printList 함수 실행 완료");
    }
    
    PrintSubSection("27.2 빈 리스트 출력");
    {
        List<int> l;
        
        cout << "    ";
        l.printList();
        
        AssertTest(true, "빈 리스트 출력 완료");
    }
}

void Test28_MixedOperations() {
    PrintSection("테스트 28: 복합 연산 테스트");
    
    PrintSubSection("28.1 삽입과 삭제 혼합");
    {
        List<int> l;
        
        // 앞/뒤 삽입 혼합
        l.push_back(1);
        l.push_front(0);
        l.push_back(2);
        l.push_front(-1);
        
        AssertTest(l.size() == 4, "혼합 삽입 후 크기");
        AssertTest(l.front() == -1 && l.back() == 2, "혼합 삽입 후 값");
        
        // 앞/뒤 삭제 혼합
        l.pop_front();
        l.pop_back();
        
        AssertTest(l.size() == 2, "혼합 삭제 후 크기");
        AssertTest(l.front() == 0 && l.back() == 1, "혼합 삭제 후 값");
    }
    
    PrintSubSection("28.2 emplace와 일반 push 혼합");
    {
        struct Point {
            int x, y;
            Point(int x = 0, int y = 0) : x(x), y(y) {}
        };
        
        List<Point> l;
        
        l.push_back(Point(1, 1));
        l.emplace_back(2, 2);
        l.push_front(Point(0, 0));
        l.emplace_front(-1, -1);
        
        AssertTest(l.size() == 4, "혼합 emplace/push 크기");
        AssertTest(l.front().x == -1, "혼합 연산 front 확인");
        AssertTest(l.back().x == 2, "혼합 연산 back 확인");
    }
    
    PrintSubSection("28.3 insert와 erase 혼합");
    {
        List<int> l;
        for (int i = 0; i < 10; i++) {
            l.push_back(i);
        }
        
        // 중간 삽입
        auto it = l.begin();
        for (int i = 0; i < 5; i++) ++it;
        l.insert(it, 99);
        
        AssertTest(l.size() == 11, "insert 후 크기");
        
        // 중간 삭제
        it = l.begin();
        for (int i = 0; i < 5; i++) ++it;
        l.erase(it);
        
        AssertTest(l.size() == 10, "erase 후 크기");
    }
}

void Test29_IteratorEdgeCases() {
    PrintSection("테스트 29: 반복자 경계 케이스");
    
    PrintSubSection("29.1 단일 요소 반복");
    {
        List<int> l;
        l.push_back(42);
        
        int count = 0;
        for (auto it = l.begin(); it != l.end(); ++it) {
            count++;
        }
        
        AssertTest(count == 1, "단일 요소 반복 횟수");
    }
    
    PrintSubSection("29.2 빈 리스트 반복");
    {
        List<int> l;
        
        int count = 0;
        for (auto it = l.begin(); it != l.end(); ++it) {
            count++;
        }
        
        AssertTest(count == 0, "빈 리스트 반복 횟수");
    }
    
    PrintSubSection("29.3 역방향 반복자 단일 요소");
    {
        List<int> l;
        l.push_back(42);
        
        int count = 0;
        for (auto it = l.rbegin(); it != l.rend(); ++it) {
            count++;
        }
        
        AssertTest(count == 1, "역방향 단일 요소 반복");
    }
    
    PrintSubSection("29.4 반복 중 수정");
    {
        List<int> l;
        for (int i = 0; i < 5; i++) {
            l.push_back(i);
        }
        
        for (auto it = l.begin(); it != l.end(); ++it) {
            *it *= 2;
        }
        
        auto it = l.begin();
        AssertTest(*it == 0, "첫 번째 수정 확인");
        ++it;
        AssertTest(*it == 2, "두 번째 수정 확인");
        ++it;
        AssertTest(*it == 4, "세 번째 수정 확인");
    }
}

void Test30_CopyVsMove() {
    PrintSection("테스트 30: 복사 vs 이동 상세 비교");
    
    PrintSubSection("30.1 push_back: L-value vs R-value");
    {
        Tracker::ResetCounts();
        
        List<Tracker> l1;
        Tracker t("lvalue");
        l1.push_back(t);
        
        int lvalueCopies = Tracker::totalCopies;
        int lvalueMoves = Tracker::totalMoves;
        
        Tracker::ResetCounts();
        
        List<Tracker> l2;
        l2.push_back(Tracker("rvalue"));
        
        int rvalueCopies = Tracker::totalCopies;
        int rvalueMoves = Tracker::totalMoves;
        
        cout << "    L-value: 복사=" << lvalueCopies << ", 이동=" << lvalueMoves << endl;
        cout << "    R-value: 복사=" << rvalueCopies << ", 이동=" << rvalueMoves << endl;
        
        AssertTest(lvalueCopies > 0 || lvalueMoves > 0, "L-value는 복사 또는 이동 발생");
        AssertTest(rvalueMoves > 0, "R-value는 이동 발생");
    }
    
    PrintSubSection("30.2 emplace_back: 직접 생성");
    {
        Tracker::ResetCounts();
        
        List<Tracker> l;
        l.emplace_back("direct");
        
        int constructs = Tracker::totalConstructs;
        int copies = Tracker::totalCopies;
        int moves = Tracker::totalMoves;
        
        cout << "    생성=" << constructs << ", 복사=" << copies << ", 이동=" << moves << endl;
        
        AssertTest(constructs > 0, "emplace_back은 직접 생성");
    }
    
    PrintSubSection("30.3 복사 생성자 vs 이동 생성자");
    {
        Tracker::ResetCounts();
        
        List<Tracker> l1;
        l1.push_back(Tracker("A"));
        l1.push_back(Tracker("B"));
        
        Tracker::ResetCounts();
        List<Tracker> l2(l1); // 복사 생성
        int copyCopies = Tracker::totalCopies;
        
        Tracker::ResetCounts();
        List<Tracker> l3(std::move(l1)); // 이동 생성
        int moveCopies = Tracker::totalCopies;
        
        cout << "    복사 생성: 복사=" << copyCopies << endl;
        cout << "    이동 생성: 복사=" << moveCopies << endl;
        
        AssertTest(copyCopies > 0, "복사 생성자는 복사 발생");
        AssertTest(moveCopies == 0, "이동 생성자는 복사 없음");
    }
}

// ==========================================
// [메인 함수]
// ==========================================
int main() {
    try {
        cout << "\n" << GREEN << "╔═══════════════════════════════════════════════════════════╗" << RESET << endl;
        cout << GREEN << "║         List Class 종합 테스트 시작                         ║" << RESET << endl;
        cout << GREEN << "╚═══════════════════════════════════════════════════════════╝" << RESET << endl;
        
        Test01_BasicConstructor();
        Test02_PushFrontOperations();
        Test03_PushBackOperations();
        Test04_PopFrontOperations();
        Test05_PopBackOperations();
        Test06_AccessOperations();
        Test07_CopyConstructor();
        Test08_MoveConstructor();
        Test09_CopyAssignment();
        Test10_MoveAssignment();
        Test11_EmplaceFrontOperation();
        Test12_EmplaceBackOperation();
        Test13_InsertOperation();
        Test14_EmplaceOperation();
        Test15_EraseOperation();
        Test16_RemoveOperation();
        Test17_IteratorOperations();
        Test18_MemoryManagement();
        Test19_ExceptionSafety();
        Test20_EdgeCases();
        Test21_ComplexTypes();
        Test22_AlgorithmCompatibility();
        Test23_ProblemSolving();
        Test24_PerformanceComparison();
        Test25_StressTest();
        Test26_RealWorldScenarios();
        Test27_PrintList();
        Test28_MixedOperations();
        Test29_IteratorEdgeCases();
        Test30_CopyVsMove();
        
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
            cout << RED << "  실패한 테스트를 확인하고 List.hpp를 수정해주세요." << RESET << endl << endl;
        }
        
    } catch (const exception& e) {
        cerr << "\n" << RED << "╔═══════════════════════════════════════════════════════════╗" << RESET << endl;
        cerr << RED << "║  [치명적 오류] 테스트 중단: " << e.what() << RESET << endl;
        cerr << RED << "╚═══════════════════════════════════════════════════════════╝" << RESET << endl;
        return 1;
    }

    return (failCount > 0) ? 1 : 0;
}