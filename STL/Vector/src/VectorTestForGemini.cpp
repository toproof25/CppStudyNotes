// Vector Class Test Code

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include <chrono>
#include <type_traits>
#include <numeric>
#include <windows.h>
#include <psapi.h>
#include "../inc/Vector.hpp"

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
        Vector<int> v;
        AssertTest(v.size() == 0, "초기 size() == 0");
        AssertTest(v.capacity() == 0, "초기 capacity() == 0");
    }
    
    PrintSubSection("1.2 다양한 타입의 벡터 생성");
    {
        Vector<double> v1;
        Vector<string> v2;
        Vector<char> v3;
        
        AssertTest(v1.size() == 0 && v2.size() == 0 && v3.size() == 0, "모든 타입 벡터 생성 성공");
    }
    
    PrintSubSection("1.3 복잡한 타입의 벡터 생성");
    {
        Vector<vector<int>> v1;
        Vector<Tracker> v2;
        
        AssertTest(v1.size() == 0 && v2.size() == 0, "복잡한 타입 벡터 생성 성공");
    }
}

void Test02_PushBackOperations() {
    PrintSection("테스트 2: Push_back 연산 (L-value & R-value)");
    
    PrintSubSection("2.1 기본 타입 push_back (int)");
    {
        Vector<int> v;
        
        v.push_back(10);
        AssertTest(v.size() == 1 && v[0] == 10, "첫 번째 push_back");
        
        v.push_back(20);
        v.push_back(30);
        AssertTest(v.size() == 3 && v[2] == 30, "연속 push_back");
        
        int val = 40;
        v.push_back(val);
        AssertTest(v[3] == 40, "L-value push_back");
    }
    
    PrintSubSection("2.2 복사 vs 이동 추적");
    {
        Tracker::ResetCounts();
        Vector<Tracker> v;
        
        Tracker t1("L-value");
        v.push_back(t1);
        int copiesAfterLvalue = Tracker::totalCopies;
        
        v.push_back(Tracker("R-value"));
        int movesAfterRvalue = Tracker::totalMoves;
        
        AssertTest(copiesAfterLvalue > 0, "L-value push_back는 복사 발생", 
                   "(복사 횟수: " + to_string(copiesAfterLvalue) + ")");
        AssertTest(movesAfterRvalue > 0, "R-value push_back는 이동 발생", 
                   "(이동 횟수: " + to_string(movesAfterRvalue) + ")");
        
        Tracker::PrintStats();
    }
    
    PrintSubSection("2.3 자동 resize 확인");
    {
        Vector<int> v;
        
        for (int i = 0; i < 100; i++) {
            v.push_back(i);
        }
        
        AssertTest(v.size() == 100, "100개 삽입 성공");
        AssertTest(v.capacity() >= 100, "자동 resize 발생");
        
        bool dataIntegrity = true;
        for (int i = 0; i < 100; i++) {
            if (v[i] != i) {
                dataIntegrity = false;
                break;
            }
        }
        AssertTest(dataIntegrity, "데이터 무결성 유지");
    }
    
    PrintSubSection("2.4 대용량 데이터 push_back");
    {
        Vector<string> v;
        string longStr(10000, 'A');
        
        v.push_back(longStr);
        v.push_back(string(10000, 'B'));
        
        AssertTest(v[0].length() == 10000 && v[0][0] == 'A', "대용량 문자열 push_back 성공");
    }
}

void Test03_PopBackOperations() {
    PrintSection("테스트 3: Pop_back 연산 및 예외 처리");
    
    PrintSubSection("3.1 정상 pop_back 동작");
    {
        Vector<int> v;
        v.push_back(10);
        v.push_back(20);
        v.push_back(30);
        
        AssertTest(v.size() == 3, "pop_back 전 크기 확인");
        
        v.pop_back();
        AssertTest(v.size() == 2 && v.back() == 20, "첫 번째 pop_back 후");
        
        v.pop_back();
        AssertTest(v.size() == 1 && v.back() == 10, "두 번째 pop_back 후");
        
        v.pop_back();
        AssertTest(v.size() == 0, "모든 요소 pop_back 후");
    }
    
    PrintSubSection("3.2 빈 벡터 pop_back 예외");
    {
        Vector<int> v;
        bool caught = false;
        
        try {
            v.pop_back();
        } catch (const std::out_of_range& e) {
            caught = true;
        }
        
        AssertTest(caught, "빈 벡터 pop_back 시 out_of_range 예외 발생");
    }
    
    PrintSubSection("3.3 객체 소멸 추적");
    {
        Tracker::ResetCounts();
        {
            Vector<Tracker> v;
            v.push_back(Tracker("A"));
            v.push_back(Tracker("B"));
            v.push_back(Tracker("C"));
            
            int destructBefore = Tracker::totalDestructs;
            v.pop_back();
            int destructAfter = Tracker::totalDestructs;
            
            AssertTest(destructAfter > destructBefore, "pop_back 시 객체 소멸 확인");
        }
        AssertTest(Tracker::liveCount == 0, "벡터 소멸 시 모든 객체 해제");
    }
}

void Test04_AccessOperations() {
    PrintSection("테스트 4: 접근 연산 ([], at, front, back)");
    
    PrintSubSection("4.1 [] 연산자");
    {
        Vector<int> v;
        for (int i = 0; i < 5; i++) {
            v.push_back(i * 10);
        }
        
        AssertTest(v[0] == 0 && v[4] == 40, "[] 연산자 읽기");
        
        v[2] = 999;
        AssertTest(v[2] == 999, "[] 연산자 쓰기");
    }
    
    PrintSubSection("4.2 at() 함수");
    {
        Vector<int> v;
        v.push_back(100);
        v.push_back(200);
        
        AssertTest(v.at(0) == 100, "at() 정상 접근");
        
        bool caught = false;
        try {
            v.at(10);
        } catch (const std::out_of_range&) {
            caught = true;
        }
        AssertTest(caught, "at() 범위 초과 시 예외 발생");
    }
    
    PrintSubSection("4.3 front() 함수");
    {
        Vector<string> v;
        v.push_back("First");
        v.push_back("Second");
        
        AssertTest(v.front() == "First", "front() 반환값 확인");
        
        v.front() = "Modified";
        AssertTest(v.front() == "Modified", "front() 수정 확인");
    }
    
    PrintSubSection("4.4 back() 함수");
    {
        Vector<string> v;
        v.push_back("First");
        v.push_back("Last");
        
        AssertTest(v.back() == "Last", "back() 반환값 확인");
        
        v.back() = "Changed";
        AssertTest(v.back() == "Changed", "back() 수정 확인");
    }
}

void Test05_CopyConstructor() {
    PrintSection("테스트 5: 복사 생성자 (Deep Copy)");
    
    PrintSubSection("5.1 기본 타입 복사");
    {
        Vector<int> original;
        original.push_back(1);
        original.push_back(2);
        original.push_back(3);
        
        Vector<int> copy(original);
        
        AssertTest(copy.size() == original.size(), "복사된 벡터 크기 일치");
        AssertTest(copy[0] == 1 && copy[1] == 2 && copy[2] == 3, "복사된 벡터 데이터 일치");
    }
    
    PrintSubSection("5.2 깊은 복사 확인 (독립성)");
    {
        Vector<int> v1;
        v1.push_back(100);
        v1.push_back(200);
        
        Vector<int> v2(v1);
        
        v2[0] = 999;
        v2.push_back(300);
        
        AssertTest(v1[0] == 100, "원본 벡터 변경 없음");
        AssertTest(v1.size() == 2, "원본 벡터 크기 불변");
        AssertTest(v2[0] == 999 && v2.size() == 3, "복사본 독립적 수정");
    }
    
    PrintSubSection("5.3 복잡한 타입 복사");
    {
        Vector<vector<int>> v1;
        v1.push_back(vector<int>{1, 2, 3});
        v1.push_back(vector<int>{4, 5});
        
        Vector<vector<int>> v2(v1);
        
        AssertTest(v2[0].size() == 3 && v2[1].size() == 2, "복사본 vector 확인");
    }
    
    PrintSubSection("5.4 빈 벡터 복사");
    {
        Vector<int> empty;
        Vector<int> copy(empty);
        
        AssertTest(copy.size() == 0, "빈 벡터 복사 성공");
    }
}

void Test06_MoveConstructor() {
    PrintSection("테스트 6: 이동 생성자");
    
    PrintSubSection("6.1 기본 이동 동작");
    {
        Vector<int> v1;
        v1.push_back(10);
        v1.push_back(20);
        v1.push_back(30);
        
        size_t originalSize = v1.size();
        Vector<int> v2(std::move(v1));
        
        AssertTest(v2.size() == originalSize && v2[0] == 10, "이동 후 데이터 유지");
        AssertTest(v1.size() == 0, "이동 후 원본은 빈 상태");
    }
    
    PrintSubSection("6.2 이동 효율성 확인");
    {
        Tracker::ResetCounts();
        Vector<Tracker> v1;
        v1.push_back(Tracker("A"));
        v1.push_back(Tracker("B"));
        
        int copiesBefore = Tracker::totalCopies;
        Vector<Tracker> v2(std::move(v1));
        int copiesAfter = Tracker::totalCopies;
        
        AssertTest(copiesBefore == copiesAfter, "이동 시 복사 발생 없음 (효율적)");
    }
}

void Test07_CopyAssignment() {
    PrintSection("테스트 7: 복사 대입 연산자 (Copy-and-Swap)");
    
    PrintSubSection("7.1 기본 대입 동작");
    {
        Vector<int> v1;
        v1.push_back(1);
        v1.push_back(2);
        
        Vector<int> v2;
        v2.push_back(999);
        
        v2 = v1;
        
        AssertTest(v2[0] == 1 && v2[1] == 2, "대입 후 데이터 복사 확인");
    }
    
    PrintSubSection("7.2 자기 대입");
    {
        Vector<int> v;
        v.push_back(100);
        v.push_back(200);
        
        v = v;
        
        AssertTest(v[0] == 100 && v[1] == 200, "자기 대입 후 데이터 유지");
    }
    
    PrintSubSection("7.3 대입 후 독립성");
    {
        Vector<string> v1;
        v1.push_back("Original");
        
        Vector<string> v2;
        v2 = v1;
        
        v2.push_back("Modified");
        
        AssertTest(v1.size() == 1, "원본 크기 불변"); 
        AssertTest(v2.size() == 2, "대입본 독립적 동작");
    }
}

void Test08_MoveAssignment() {
    PrintSection("테스트 8: 이동 대입 연산자");
    
    PrintSubSection("8.1 기본 이동 대입");
    {
        Vector<int> v1;
        v1.push_back(10);
        v1.push_back(20);
        
        Vector<int> v2;
        v2 = std::move(v1);
        
        AssertTest(v2[0] == 10 && v2[1] == 20, "이동 대입 후 데이터 확인");
        AssertTest(v1.size() == 0, "이동 후 원본은 빈 상태");
    }
}

void Test09_EmplaceBackOperation() {
    PrintSection("테스트 9: Emplace_back 연산 (효율성)");
    
    PrintSubSection("9.1 emplace_back 기본 동작");
    {
        Vector<string> v;
        v.emplace_back("Direct");
        v.emplace_back("Construction");
        
        AssertTest(v[0] == "Direct" && v[1] == "Construction", "emplace_back 정상 동작");
    }
    
    PrintSubSection("9.2 emplace_back 효율성 비교");
    {
        Tracker::ResetCounts();
        Vector<Tracker> v1;
        
        v1.push_back(Tracker("Push"));
        int copiesAfterPush = Tracker::totalCopies + Tracker::totalMoves;
        
        Tracker::ResetCounts();
        Vector<Tracker> v2;
        
        v2.emplace_back("Emplace");
        int copiesAfterEmplace = Tracker::totalCopies + Tracker::totalMoves;
        
        cout << "    (Push: " << copiesAfterPush << ", Emplace: " << copiesAfterEmplace << ")" << endl;
        AssertTest(copiesAfterEmplace <= copiesAfterPush, "emplace_back이 더 효율적이거나 동등");
    }
    
    PrintSubSection("9.3 다중 인자 emplace_back");
    {
        struct Point {
            int x, y;
            Point(int x = 0, int y = 0) : x(x), y(y) {}
        };
        
        Vector<Point> v;
        v.emplace_back(10, 20);
        v.emplace_back(30, 40);
        
        AssertTest(v[0].x == 10 && v[0].y == 20, "다중 인자 직접 생성");
        AssertTest(v[1].x == 30 && v[1].y == 40, "두 번째 emplace_back");
    }
}

void Test10_ReserveOperation() {
    PrintSection("테스트 10: Reserve 연산");
    
    PrintSubSection("10.1 reserve 기본 동작");
    {
        Vector<int> v;
        v.reserve(100);
        
        AssertTest(v.capacity() >= 100, "capacity 증가 확인");
        AssertTest(v.size() == 0, "size는 변경되지 않음");
    }
    
    PrintSubSection("10.2 reserve 후 push_back");
    {
        Vector<int> v;
        v.reserve(50);
        
        size_t cap_before = v.capacity();
        
        for (int i = 0; i < 50; i++) {
            v.push_back(i);
        }
        
        size_t cap_after = v.capacity();
        
        AssertTest(cap_before == cap_after, "reserve로 재할당 방지");
        AssertTest(v.size() == 50, "모든 요소 삽입 성공");
    }
    
    PrintSubSection("10.3 더 작은 크기로 reserve");
    {
        Vector<int> v;
        v.reserve(100);
        size_t cap1 = v.capacity();
        
        v.reserve(50);
        size_t cap2 = v.capacity();
        
        AssertTest(cap1 == cap2, "더 작은 크기로 reserve는 무시됨");
    }
}

void Test11_ShrinkToFit() {
    PrintSection("테스트 11: Shrink_to_fit 연산");
    
    PrintSubSection("11.1 shrink_to_fit 기본 동작");
    {
        Vector<int> v;
        v.reserve(100);
        
        for (int i = 0; i < 10; i++) {
            v.push_back(i);
        }
        
        AssertTest(v.capacity() >= 100, "reserve 후 큰 capacity");
        
        v.shrink_to_fit();
        
        AssertTest(v.capacity() == v.size(), "shrink_to_fit 후 capacity == size");
        AssertTest(v.size() == 10, "size 유지");
        
        for (int i = 0; i < 10; i++) {
            AssertTest(v[i] == i, "데이터 무결성 유지");
        }
    }
    
    PrintSubSection("11.2 빈 벡터 shrink_to_fit");
    {
        Vector<int> v;
        v.reserve(100);
        v.shrink_to_fit();
        
        AssertTest(v.capacity() == 0 && v.size() == 0, "빈 벡터 shrink_to_fit 성공");
    }
}

void Test12_ClearOperation() {
    PrintSection("테스트 12: Clear 연산");
    
    PrintSubSection("12.1 clear 기본 동작");
    {
        Vector<int> v;
        for (int i = 0; i < 10; i++) {
            v.push_back(i);
        }
        
        size_t cap_before = v.capacity();
        v.clear();
        size_t cap_after = v.capacity();
        
        AssertTest(v.size() == 0, "clear 후 size == 0");
        AssertTest(cap_before == cap_after, "capacity는 유지됨");
    }
    
    PrintSubSection("12.2 clear 후 객체 소멸 확인");
    {
        Tracker::ResetCounts();
        
        Vector<Tracker> v;
        v.push_back(Tracker("A"));
        v.push_back(Tracker("B"));
        v.push_back(Tracker("C"));
        
        int destructBefore = Tracker::totalDestructs;
        v.clear();
        int destructAfter = Tracker::totalDestructs;
        
        AssertTest(destructAfter > destructBefore, "clear 시 모든 객체 소멸");
        AssertTest(v.size() == 0, "clear 후 빈 상태");
    }
}

void Test13_InsertOperation() {
    PrintSection("테스트 13: Insert 연산");
    
    PrintSubSection("13.1 중간 삽입 (L-value)");
    {
        Vector<int> v;
        v.push_back(1);
        v.push_back(3);
        v.push_back(5);

	int val = 2;
        v.insert(v.begin() + 1, val);
        
        AssertTest(v.size() == 4, "insert 후 크기 증가");
        AssertTest(v[0] == 1 && v[1] == 2 && v[2] == 3 && v[3] == 5, "올바른 위치에 삽입");
    }
    
    PrintSubSection("13.2 중간 삽입 (R-value)");
    {
        Vector<string> v;
        v.push_back("A");
        v.push_back("C");
        
        v.insert(v.begin() + 1, string("B"));
        
        AssertTest(v[0] == "A" && v[1] == "B" && v[2] == "C", "R-value insert 성공");
    }
    
    PrintSubSection("13.3 맨 앞 삽입");
    {
        Vector<int> v;
        v.push_back(2);
        v.push_back(3);
        
        v.insert(v.begin(), 1);
        
        AssertTest(v[0] == 1 && v[1] == 2 && v[2] == 3, "맨 앞 삽입 성공");
    }
    
    PrintSubSection("13.4 맨 뒤 삽입");
    {
        Vector<int> v;
        v.push_back(1);
        v.push_back(2);
        
        v.insert(v.end(), 3);
        
        AssertTest(v[0] == 1 && v[1] == 2 && v[2] == 3, "맨 뒤 삽입 성공");
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
        
        Vector<Point> v;
        v.emplace_back(1, 1);
        v.emplace_back(3, 3);
        
        v.emplace(v.begin() + 1, 2, 2);
        
        AssertTest(v.size() == 3, "emplace 후 크기");
        AssertTest(v[0].x == 1 && v[1].x == 2 && v[2].x == 3, "올바른 위치에 emplace");
    }
}

void Test15_EraseOperation() {
    PrintSection("테스트 15: Erase 연산");
    
    PrintSubSection("15.1 중간 삭제");
    {
        Vector<int> v;
        for (int i = 1; i <= 5; i++) {
            v.push_back(i);
        }
        
        v.erase(v.begin() + 2);
        
        AssertTest(v.size() == 4, "erase 후 크기 감소");
        AssertTest(v[0] == 1 && v[1] == 2 && v[2] == 4 && v[3] == 5, "올바른 요소 삭제");
    }
    
    PrintSubSection("15.2 맨 앞 삭제");
    {
        Vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        
        v.erase(v.begin());
        
        AssertTest(v[0] == 2 && v[1] == 3, "맨 앞 삭제 성공");
    }
    
    PrintSubSection("15.3 맨 뒤 삭제");
    {
        Vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        
        v.erase(v.end() - 1);
        
        AssertTest(v.size() == 2 && v[0] == 1 && v[1] == 2, "맨 뒤 삭제 성공");
    }
    
    PrintSubSection("15.4 빈 벡터 erase 예외");
    {
        Vector<int> v;
        bool caught = false;
        
        try {
            v.erase(v.begin());
        } catch (const std::out_of_range&) {
            caught = true;
        }
        
        AssertTest(caught, "빈 벡터 erase 시 예외 발생");
    }
}

void Test16_IteratorOperations() {
    PrintSection("테스트 16: 반복자 연산");
    
    PrintSubSection("16.1 begin/end 반복");
    {
        Vector<int> v;
        for (int i = 0; i < 5; i++) {
            v.push_back(i);
        }
        
        int sum = 0;
        for (auto it = v.begin(); it != v.end(); ++it) {
            sum += *it;
        }
        
        AssertTest(sum == 10, "반복자를 통한 순회");
    }
    
    PrintSubSection("16.2 범위 기반 for 루프");
    {
        Vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        
        int product = 1;
        for (const auto& val : v) {
            product *= val;
        }
        
        AssertTest(product == 6, "범위 기반 for 루프 동작");
    }
    
    PrintSubSection("16.3 const 반복자");
    {
        Vector<int> v;
        v.push_back(10);
        v.push_back(20);
        
        const Vector<int>& cv = v;
        
        int sum = 0;
        for (auto it = cv.cbegin(); it != cv.cend(); ++it) {
            sum += *it;
        }
        
        AssertTest(sum == 30, "const 반복자 동작");
    }
}

void Test17_MemoryManagement() {
    PrintSection("테스트 17: 메모리 관리 및 누수 검사");
    
    PrintSubSection("17.1 벡터 소멸 시 객체 해제");
    {
        Tracker::ResetCounts();
        
        {
            Vector<Tracker> v;
            v.push_back(Tracker("A"));
            v.push_back(Tracker("B"));
            v.push_back(Tracker("C"));
        }
        
        AssertTest(Tracker::liveCount == 0, "모든 객체 정상 소멸 (메모리 누수 없음)");
    }
    
    PrintSubSection("17.2 resize 시 메모리 관리");
    {
        Tracker::ResetCounts();
        
        Vector<Tracker> v;
        for (int i = 0; i < 100; i++) {
            v.push_back(Tracker("Item"));
        }
        
        AssertTest(Tracker::liveCount == 100, "100개 객체 생존");
    }
}

void Test18_ExceptionSafety() {
    PrintSection("테스트 18: 예외 안전성");
    
    PrintSubSection("18.1 복사 생성 중 예외");
    {
        Exploder::Reset();
        Vector<Exploder> v1;
        v1.push_back(Exploder(1));
        v1.push_back(Exploder(2));
        
        Exploder::throwAtCopy = 2;
        
        bool caught = false;
        try {
            Vector<Exploder> v2(v1);
        } catch (const std::exception&) {
            caught = true;
        }
        
        AssertTest(caught, "복사 중 예외 발생");
        Exploder::Reset();
    }
}

void Test19_EdgeCases() {
    PrintSection("테스트 19: 경계 조건");
    
    PrintSubSection("19.1 단일 요소 벡터");
    {
        Vector<int> v;
        v.push_back(42);
        
        AssertTest(v.size() == 1 && v[0] == 42, "단일 요소 벡터");
        
        v.pop_back();
        AssertTest(v.size() == 0, "단일 요소 제거");
    }
    
    PrintSubSection("19.2 빈 벡터 연산");
    {
        Vector<int> v;
        
        bool popException = false;
        try { v.pop_back(); } catch (const std::out_of_range&) { popException = true; }
        
        AssertTest(popException, "빈 벡터 모든 예외 처리");
    }
    
    PrintSubSection("19.3 NonMovable 타입");
    {
        Vector<NonMovable> v;
        NonMovable nm(10);
        v.push_back(nm);
        
        AssertTest(v[0].value == 10, "NonMovable 타입 push_back 성공");
    }
    
    PrintSubSection("19.4 NonCopyable 타입");
    {
        Vector<NonCopyable> v;
        v.push_back(NonCopyable(20));
        
        AssertTest(v[0].value == 20, "NonCopyable 타입 이동 push_back 성공");
    }
}

void Test20_ComplexTypes() {
    PrintSection("테스트 20: 복잡한 타입 지원");
    
    PrintSubSection("20.1 중첩 컨테이너");
    {
        Vector<vector<int>> v;
        
        vector<int> inner1 = {1, 2, 3};
        vector<int> inner2 = {4, 5};
        
        v.push_back(inner1);
        v.push_back(inner2);
        
        AssertTest(v[0].size() == 3 && v[1].size() == 2, "중첩 vector 지원");
    }
    
    PrintSubSection("20.2 포인터 타입");
    {
        Vector<int*> v;
        int* ptr = new int(999);
        
        v.push_back(ptr);
        AssertTest(*(v[0]) == 999, "포인터 타입 저장");
        
        delete ptr;
    }
}

void Test21_AlgorithmCompatibility() {
    PrintSection("테스트 21: STL 알고리즘 호환성");
    
    PrintSubSection("21.1 std::sort");
    {
        Vector<int> v;
        v.push_back(5);
        v.push_back(2);
        v.push_back(8);
        v.push_back(1);
        v.push_back(9);
        
        std::sort(v.begin(), v.end());
        
        AssertTest(v[0] == 1 && v[1] == 2 && v[2] == 5 && v[3] == 8 && v[4] == 9, 
                   "std::sort 호환");
    }
    
    PrintSubSection("21.2 std::find");
    {
        Vector<int> v;
        for (int i = 0; i < 10; i++) {
            v.push_back(i);
        }
        
        auto it = std::find(v.begin(), v.end(), 5);
        
        AssertTest(it != v.end() && *it == 5, "std::find 호환");
    }
    
    PrintSubSection("21.3 std::accumulate");
    {
        Vector<int> v;
        for (int i = 1; i <= 5; i++) {
            v.push_back(i);
        }
        
        int sum = std::accumulate(v.begin(), v.end(), 0);
        
        AssertTest(sum == 15, "std::accumulate 호환");
    }
    
    PrintSubSection("21.4 std::reverse");
    {
        Vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        
        std::reverse(v.begin(), v.end());
        
        AssertTest(v[0] == 3 && v[1] == 2 && v[2] == 1, "std::reverse 호환");
    }
}

void Test22_ProblemSolving() {
    PrintSection("테스트 22: 실제 문제 풀이 시나리오");
    
    PrintSubSection("22.1 동적 배열 문제 (Two Sum)");
    {
        Vector<int> nums;
        nums.push_back(2);
        nums.push_back(7);
        nums.push_back(11);
        nums.push_back(15);
        int target = 9;
        
        Vector<int> result;
        for (size_t i = 0; i < nums.size(); i++) {
            for (size_t j = i + 1; j < nums.size(); j++) {
                if (nums[i] + nums[j] == target) {
                    result.push_back(i);
                    result.push_back(j);
                    break;
                }
            }
            if (result.size() > 0) break;
        }
        
        AssertTest(result.size() == 2 && result[0] == 0 && result[1] == 1, "Two Sum 문제 해결");
    }
    
    PrintSubSection("22.2 동적 프로그래밍 (Fibonacci)");
    {
        int n = 10;
        Vector<int> dp;
        dp.push_back(0);
        dp.push_back(1);
        
        for (int i = 2; i <= n; i++) {
            dp.push_back(dp[i-1] + dp[i-2]);
        }
        
        AssertTest(dp[10] == 55, "피보나치 DP 문제 해결");
    }
    
    PrintSubSection("22.3 배열 정렬 및 중복 제거");
    {
        Vector<int> v;
        v.push_back(1);
        v.push_back(1);
        v.push_back(2);
        v.push_back(2);
        v.push_back(3);
        
        std::sort(v.begin(), v.end());
        auto last = std::unique(v.begin(), v.end());
        
        size_t uniqueCount = last - v.begin();
        
        AssertTest(uniqueCount == 3, "중복 제거 문제 해결");
    }
    
    PrintSubSection("22.4 슬라이딩 윈도우");
    {
        Vector<int> arr;
        for (int i = 1; i <= 6; i++) {
            arr.push_back(i);
        }
        
        int k = 3;
        int maxSum = 0;
        
        int windowSum = 0;
        for (int i = 0; i < k; i++) {
            windowSum += arr[i];
        }
        maxSum = windowSum;
        
        for (size_t i = k; i < arr.size(); i++) {
            windowSum = windowSum - arr[i - k] + arr[i];
            if (windowSum > maxSum) {
                maxSum = windowSum;
            }
        }
        
        AssertTest(maxSum == 15, "슬라이딩 윈도우 문제 해결");
    }
}

void Test23_PerformanceComparison() {
    PrintSection("테스트 23: 종합 성능 비교 (Vector vs std::vector)");
    
    const int N = 1000000;
    
    cout << "  데이터 개수: " << N << "개" << endl << endl;
    
    PrintSubSection("23.1 [int] Push_back (삽입) 속도");
    
    MEASURE("Vector<int> Push_back",
        cout << "Vector<int> Push_back : ";
        Vector<int> v;
        v.reserve(N);
        for (int i = 0; i < N; i++) {
            v.push_back(i);
        }
    );
    
    MEASURE("std::vector<int> Push_back",
        cout << "std::vector<int> Push_back : ";
        std::vector<int> v;
        v.reserve(N);
        for (int i = 0; i < N; i++) {
            v.push_back(i);
        }
    );
    
    PrintSubSection("23.2 [int] Random Access (접근) 속도");
    
    {
        Vector<int> myV;
        std::vector<int> stlV;
        myV.reserve(N);
        stlV.reserve(N);
        for(int i = 0; i < N; i++) {
            myV.push_back(i);
            stlV.push_back(i);
        }
        
        MEASURE("Vector<int> Random Access",
            cout << "Vector<int> Random Access : ";
            long long sum = 0;
            for(int i = 0; i < N; i++) {
                sum += myV[i];
            }
        );
        
        MEASURE("std::vector<int> Random Access",
            cout << "std::vector<int> Random Access : ";
            long long sum = 0;
            for(int i = 0; i < N; i++) {
                sum += stlV[i];
            }
        );
    }
    
    PrintSubSection("23.3 [HeavyObject] Push_back (이동 시맨틱) 속도");
    
    MEASURE("Vector<Heavy> Push_back",
        cout << "Vector<HeavyObject> Push_back : ";
        Vector<HeavyObject> v;
        v.reserve(N);
        for (int i = 0; i < N; i++) {
            v.push_back(HeavyObject());
        }
    );
    
    MEASURE("std::vector<Heavy> Push_back",
        cout << "std::vector<HeavyObject> Push_back : ";
        std::vector<HeavyObject> v;
        v.reserve(N);
        for (int i = 0; i < N; i++) {
            v.push_back(HeavyObject());
        }
    );
    
    PrintSubSection("23.4 [Emplace_back] 직접 생성 최적화");
    
    MEASURE("Vector Emplace_back",
        cout << "Vector<HeavyObject> Emplace_back : ";
        Vector<HeavyObject> v;
        v.reserve(N);
        for (int i = 0; i < N; i++) {
            v.emplace_back(1000);
        }
    );
    
    MEASURE("std::vector Emplace_back",
        cout << "std::vector<HeavyObject> Emplace_back : ";
        std::vector<HeavyObject> v;
        v.reserve(N);
        for (int i = 0; i < N; i++) {
            v.emplace_back(1000);
        }
    );
    
    PrintSubSection("23.5 [Insert] 중간 삽입 성능");
    
    const int InsertCount = 20000;
    
    MEASURE("Vector Insert (중간)",
        cout << "Vector<int> Insert (중간) : ";
        Vector<int> v;
        v.reserve(InsertCount * 2);
        for(int i = 0; i < InsertCount; i++) {
            v.push_back(i);
        }
        for(int i = 0; i < InsertCount; i++) {
            v.insert(v.begin() + v.size() / 2, i);
        }
    );
    
    MEASURE("std::vector Insert (중간)",
        cout << "std::vector<int> Insert (중간) : ";
        std::vector<int> v;
        v.reserve(InsertCount * 2);
        for(int i = 0; i < InsertCount; i++) {
            v.push_back(i);
        }
        for(int i = 0; i < InsertCount; i++) {
            v.insert(v.begin() + v.size() / 2, i);
        }
    );
    
    PrintSubSection("23.6 [Erase] 중간 삭제 성능");
    
    const int EraseCount = 20000;
    
    MEASURE("Vector Erase (중간)",
        cout << "Vector<int> Erase (중간) : ";
        Vector<int> v;
        for(int i = 0; i < EraseCount; i++) {
            v.push_back(i);
        }
        while(v.size() > 0) {
            if(v.size() > 1)
                v.erase(v.begin() + v.size() / 2);
            else
                v.erase(v.begin());
        }
    );
    
    MEASURE("std::vector Erase (중간)",
        cout << "std::vector<int> Erase (중간) : ";
        std::vector<int> v;
        for(int i = 0; i < EraseCount; i++) {
            v.push_back(i);
        }
        while(v.size() > 0) {
            if(v.size() > 1)
                v.erase(v.begin() + v.size() / 2);
            else
                v.erase(v.begin());
        }
    );
}

void Test24_StressTest() {
    PrintSection("테스트 24: 스트레스 테스트");
    
    PrintSubSection("24.1 대규모 Push/Pop 반복");
    {
        Vector<int> v;
        
        for (int i = 0; i < 100000; i++) {
            v.push_back(i);
        }
        
        while (v.size() > 0) {
            v.pop_back();
        }
        
        AssertTest(v.size() == 0, "대규모 작업 안정성");
    }
    
    PrintSubSection("24.2 반복적 resize");
    {
        Vector<int> v;
        
        for (int cycle = 0; cycle < 10; cycle++) {
            for (int i = 0; i < 1000; i++) {
                v.push_back(i);
            }
            v.clear();
        }
        
        AssertTest(v.size() == 0, "반복적 resize 안정성");
    }
}

void Test25_RealWorldScenarios() {
    PrintSection("테스트 25: 실제 사용 시나리오");
    
    PrintSubSection("25.1 학생 성적 관리 시스템");
    {
        struct Student {
            string name;
            int score;
            Student(string n = "", int s = 0) : name(n), score(s) {}
        };
        
        Vector<Student> students;
        students.emplace_back("Alice", 95);
        students.emplace_back("Bob", 87);
        students.emplace_back("Charlie", 92);
        
        std::sort(students.begin(), students.end(), 
                  [](const Student& a, const Student& b) { return a.score > b.score; });
        
        AssertTest(students[0].name == "Alice" && students[0].score == 95, 
                   "성적 순 정렬 성공");
    }
    
    PrintSubSection("25.2 이벤트 큐 시뮬레이션");
    {
        struct Event {
            int timestamp;
            string type;
            Event(int t = 0, string tp = "") : timestamp(t), type(tp) {}
        };
        
        Vector<Event> events;
        events.emplace_back(100, "start");
        events.emplace_back(200, "process");
        events.emplace_back(300, "end");
        
        AssertTest(events.size() == 3, "이벤트 큐 생성 성공");
        
        events.erase(events.begin() + 1);
        
        AssertTest(events.size() == 2 && events[1].type == "end", "이벤트 제거 성공");
    }
}

// ==========================================
// [메인 함수]
// ==========================================
int main() {
    try {
        cout << "\n" << GREEN << "╔═══════════════════════════════════════════════════════════╗" << RESET << endl;
        cout << GREEN << "║         Vector Class 종합 테스트 시작                         ║" << RESET << endl;
        cout << GREEN << "╚═══════════════════════════════════════════════════════════╝" << RESET << endl;
        
        Test01_BasicConstructor();
        Test02_PushBackOperations();
        Test03_PopBackOperations();
        Test04_AccessOperations();
        Test05_CopyConstructor();
        Test06_MoveConstructor();
        Test07_CopyAssignment();
        Test08_MoveAssignment();
        Test09_EmplaceBackOperation();
        Test10_ReserveOperation();
        Test11_ShrinkToFit();
        Test12_ClearOperation();
        Test13_InsertOperation();
        Test14_EmplaceOperation();
        Test15_EraseOperation();
        Test16_IteratorOperations();
        Test17_MemoryManagement();
        Test18_ExceptionSafety();
        Test19_EdgeCases();
        Test20_ComplexTypes();
        Test21_AlgorithmCompatibility();
        Test22_ProblemSolving();
        Test23_PerformanceComparison();
        Test24_StressTest();
        Test25_RealWorldScenarios();
        
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

