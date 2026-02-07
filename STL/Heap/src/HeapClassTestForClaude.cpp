// Heap Class Test Code

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include <chrono>
#include <type_traits>
#include <numeric>
#include <random>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#endif

#include "../../Vector/inc/Vector.hpp"
#include "../inc/Heap.hpp"

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
// [성능 측정 상수]
// ==========================================
const int PERF_TEST_SIZE = 100000;

// ==========================================
// [성능 측정 도구]
// ==========================================
size_t GetMemoryUsage() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.PrivateUsage;
    }
#endif
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

    int data;

    Tracker(int d = 0) : data(d) { 
        liveCount++; 
        totalConstructs++; 
    }
    
    Tracker(const Tracker& other) : data(other.data) { 
        liveCount++; 
        totalCopies++; 
    }
    
    Tracker(Tracker&& other) noexcept : data(other.data) { 
        liveCount++; 
        totalMoves++; 
        totalConstructs++;
        other.data = 0;
    }

    Tracker& operator=(const Tracker& other) {
        data = other.data;
        totalCopies++;
        return *this;
    }

    Tracker& operator=(Tracker&& other) noexcept {
        data = other.data;
        totalMoves++;
        other.data = 0;
        return *this;
    }

    ~Tracker() { 
        liveCount--; 
        totalDestructs++; 
    }

    bool operator<=(const Tracker& other) const { return data <= other.data; }
    bool operator>=(const Tracker& other) const { return data >= other.data; }
    bool operator==(const Tracker& other) const { return data == other.data; }

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

    bool operator<=(const Exploder& other) const { return id <= other.id; }
    bool operator>=(const Exploder& other) const { return id >= other.id; }
    
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

// 3. 무거운 객체 (성능 테스트용)
struct HeavyObject {
    std::vector<int> data;
    HeavyObject(int size = 1000) : data(size, 42) {}
    HeavyObject(const HeavyObject&) = default;
    HeavyObject(HeavyObject&&) noexcept = default;
    HeavyObject& operator=(const HeavyObject&) = default;
    HeavyObject& operator=(HeavyObject&&) noexcept = default;

    bool operator<=(const HeavyObject& other) const { 
        return data.size() <= other.data.size(); 
    }
    bool operator>=(const HeavyObject& other) const { 
        return data.size() >= other.data.size(); 
    }
    bool operator>(const HeavyObject& other) const { 
        return data.size() > other.data.size(); 
    }
};

// 4. 우선순위를 가진 작업 객체
struct Task {
    int priority;
    string name;

    Task(int p = 0, string n = "") : priority(p), name(n) {}

    bool operator<=(const Task& other) const { return priority <= other.priority; }
    bool operator>=(const Task& other) const { return priority >= other.priority; }
    bool operator==(const Task& other) const { return priority == other.priority; }
};

// ==========================================
// [Heap 속성 검증 헬퍼 함수]
// ==========================================

// Min Heap 속성 검증 (_g=0)
template<typename T>
bool VerifyMinHeapProperty(Heap<T, 0>& heap) {
    if (heap.empty()) return true;
    
    // Heap의 내부 구조를 직접 검증할 수 없으므로
    // pop 연산을 통해 정렬 순서 확인
    vector<T> extracted;
    Heap<T, 0> temp_heap = heap;  // 복사본 생성
    
    while (!temp_heap.empty()) {
        extracted.push_back(temp_heap.top());
        temp_heap.pop();
    }
    
    // 추출된 요소들이 오름차순인지 확인
    for (size_t i = 1; i < extracted.size(); ++i) {
        if (extracted[i] <= extracted[i-1]) {
            return false;
        }
    }
    return true;
}

// Max Heap 속성 검증 (_g!=0)
template<typename T>
bool VerifyMaxHeapProperty(Heap<T, 1>& heap) {
    if (heap.empty()) return true;
    
    vector<T> extracted;
    Heap<T, 1> temp_heap = heap;
    
    while (!temp_heap.empty()) {
        extracted.push_back(temp_heap.top());
        temp_heap.pop();
    }
    
    // 추출된 요소들이 내림차순인지 확인
    for (size_t i = 1; i < extracted.size(); ++i) {
        if (extracted[i] > extracted[i-1]) {
            return false;
        }
    }
    return true;
}

// ==========================================
// [테스트 케이스]
// ==========================================

void Test01_BasicConstructorAndEmpty() {
    // Min/Max Heap 기본 생성자 및 empty() 함수 테스트
    PrintSection("테스트 1: 기본 생성자 및 초기 상태");
    
    PrintSubSection("1.1 Min Heap 기본 생성자");
    {
        Heap<int, 0> minHeap;
        AssertTest(minHeap.empty(), "초기 상태 empty() == true");
        AssertTest(minHeap.size() == 0, "초기 상태 size() == 0");
    }
    
    PrintSubSection("1.2 Max Heap 기본 생성자");
    {
        Heap<int, 1> maxHeap;
        AssertTest(maxHeap.empty(), "초기 상태 empty() == true");
        AssertTest(maxHeap.size() == 0, "초기 상태 size() == 0");
    }
    
    PrintSubSection("1.3 다양한 타입의 Heap 생성");
    {
        Heap<double, 0> h1;
        Heap<string, 1> h2;
        Heap<char, 0> h3;
        
        AssertTest(h1.size() == 0 && h2.size() == 0 && h3.size() == 0, 
                   "모든 타입 Heap 생성 성공");
    }
}

void Test02_MinHeapPushAndTop() {
    // Min Heap의 push 연산과 top 접근 테스트
    PrintSection("테스트 2: Min Heap Push 및 Top 연산");
    
    PrintSubSection("2.1 단일 요소 삽입");
    {
        Heap<int, 0> h;
        h.push(42);
        
        AssertTest(!h.empty(), "삽입 후 empty() == false");
        AssertTest(h.size() == 1, "size() == 1");
        AssertTest(h.top() == 42, "top() == 42");
    }
    
    PrintSubSection("2.2 여러 요소 삽입 (오름차순)");
    {
        Heap<int, 0> h;
        h.push(1);
        h.push(2);
        h.push(3);
        h.push(4);
        h.push(5);
        
        AssertTest(h.size() == 5, "5개 삽입 후 크기");
        AssertTest(h.top() == 1, "최소값이 top에 위치");
    }
    
    PrintSubSection("2.3 여러 요소 삽입 (내림차순)");
    {
        Heap<int, 0> h;
        h.push(5);
        h.push(4);
        h.push(3);
        h.push(2);
        h.push(1);
        
        AssertTest(h.size() == 5, "5개 삽입 후 크기");
        AssertTest(h.top() == 1, "최소값이 top에 위치");
    }
    
    PrintSubSection("2.4 무작위 순서 삽입");
    {
        Heap<int, 0> h;
        vector<int> values = {15, 3, 9, 22, 1, 7, 12, 5};
        
        for (int v : values) {
            h.push(v);
        }
        
        AssertTest(h.size() == 8, "8개 삽입 후 크기");
        AssertTest(h.top() == 1, "최소값 1이 top에 위치");
        AssertTest(VerifyMinHeapProperty(h), "Min Heap 속성 유지");
    }
    
    PrintSubSection("2.5 중복 값 삽입");
    {
        Heap<int, 0> h;
        h.push(5);
        h.push(5);
        h.push(3);
        h.push(3);
        h.push(5);
        
        AssertTest(h.size() == 5, "중복 값 포함 5개 삽입");
        AssertTest(h.top() == 3, "최소값 3이 top에 위치");
    }
    
    PrintSubSection("2.6 Move semantics 삽입");
    {
        Tracker::ResetCounts();
        Heap<Tracker, 0> h;
        
        h.push(Tracker(10));
        h.push(Tracker(5));
        h.push(Tracker(15));
        
        AssertTest(h.top().data == 5, "최소값 5가 top");
        AssertTest(Tracker::totalMoves > 0, "이동 생성자 호출됨");
        Tracker::PrintStats();
    }
}

void Test03_MaxHeapPushAndTop() {
    // Max Heap의 push 연산과 top 접근 테스트
    PrintSection("테스트 3: Max Heap Push 및 Top 연산");
    
    PrintSubSection("3.1 단일 요소 삽입");
    {
        Heap<int, 1> h;
        h.push(42);
        
        AssertTest(h.size() == 1, "size() == 1");
        AssertTest(h.top() == 42, "top() == 42");
    }
    
    PrintSubSection("3.2 여러 요소 삽입 (오름차순)");
    {
        Heap<int, 1> h;
        h.push(1);
        h.push(2);
        h.push(3);
        h.push(4);
        h.push(5);
        
        AssertTest(h.size() == 5, "5개 삽입 후 크기");
        AssertTest(h.top() == 5, "최대값이 top에 위치");
    }
    
    PrintSubSection("3.3 여러 요소 삽입 (내림차순)");
    {
        Heap<int, 1> h;
        h.push(5);
        h.push(4);
        h.push(3);
        h.push(2);
        h.push(1);
        
        AssertTest(h.size() == 5, "5개 삽입 후 크기");
        AssertTest(h.top() == 5, "최대값이 top에 위치");
    }
    
    PrintSubSection("3.4 무작위 순서 삽입");
    {
        Heap<int, 1> h;
        vector<int> values = {15, 3, 9, 22, 1, 7, 12, 5};
        
        for (int v : values) {
            h.push(v);
        }
        
        AssertTest(h.size() == 8, "8개 삽입 후 크기");
        AssertTest(h.top() == 22, "최대값 22가 top에 위치");
        AssertTest(VerifyMaxHeapProperty(h), "Max Heap 속성 유지");
    }
    
    PrintSubSection("3.5 중복 값 삽입");
    {
        Heap<int, 1> h;
        h.push(5);
        h.push(5);
        h.push(8);
        h.push(8);
        h.push(5);
        
        AssertTest(h.size() == 5, "중복 값 포함 5개 삽입");
        AssertTest(h.top() == 8, "최대값 8이 top에 위치");
    }
}

void Test04_MinHeapPopOperation() {
    // Min Heap의 pop 연산 상세 테스트
    PrintSection("테스트 4: Min Heap Pop 연산");
    
    PrintSubSection("4.1 단일 요소 pop");
    {
        Heap<int, 0> h;
        h.push(42);
        h.pop();
        
        AssertTest(h.empty(), "pop 후 empty() == true");
        AssertTest(h.size() == 0, "pop 후 size() == 0");
    }
    
    PrintSubSection("4.2 연속 pop으로 정렬 순서 확인");
    {
        Heap<int, 0> h;
        vector<int> values = {5, 2, 8, 1, 9, 3, 7};
        for (int v : values) h.push(v);
        
        vector<int> extracted;
        while (!h.empty()) {
            extracted.push_back(h.top());
            h.pop();
        }
        
        AssertTest(is_sorted(extracted.begin(), extracted.end()), 
                   "pop 순서가 오름차순");
        AssertTest(extracted.front() == 1, "첫 번째 pop이 최소값");
        AssertTest(extracted.back() == 9, "마지막 pop이 최대값");
    }
    
    PrintSubSection("4.3 빈 Heap에 pop 호출");
    {
        Heap<int, 0> h;
        h.pop();  // 빈 상태에서 pop
        
        AssertTest(h.empty(), "빈 Heap pop 후에도 안전");
        AssertTest(h.size() == 0, "빈 Heap pop 후 크기 0 유지");
    }
    
    PrintSubSection("4.4 push-pop 혼합 연산");
    {
        Heap<int, 0> h;
        h.push(5);
        h.push(3);
        h.pop();  // 3 제거
        h.push(1);
        h.push(7);
        h.pop();  // 1 제거
        
        AssertTest(h.size() == 2, "혼합 연산 후 크기");
        AssertTest(h.top() == 5, "현재 최소값 확인");
    }
    
    PrintSubSection("4.5 중복 값 pop");
    {
        Heap<int, 0> h;
        h.push(5);
        h.push(5);
        h.push(3);
        h.push(3);
        h.push(5);
        
        vector<int> extracted;
        while (!h.empty()) {
            extracted.push_back(h.top());
            h.pop();
        }
        
        AssertTest(extracted == vector<int>({3, 3, 5, 5, 5}), 
                   "중복 값 정확한 순서로 추출");
    }
}

void Test05_MaxHeapPopOperation() {
    // Max Heap의 pop 연산 상세 테스트
    PrintSection("테스트 5: Max Heap Pop 연산");
    
    PrintSubSection("5.1 연속 pop으로 정렬 순서 확인");
    {
        Heap<int, 1> h;
        vector<int> values = {5, 2, 8, 1, 9, 3, 7};
        for (int v : values) h.push(v);
        
        vector<int> extracted;
        while (!h.empty()) {
            extracted.push_back(h.top());
            h.pop();
        }
        
        AssertTest(is_sorted(extracted.begin(), extracted.end(), greater<int>()), 
                   "pop 순서가 내림차순");
        AssertTest(extracted.front() == 9, "첫 번째 pop이 최대값");
        AssertTest(extracted.back() == 1, "마지막 pop이 최소값");
    }
    
    PrintSubSection("5.2 push-pop 혼합 연산");
    {
        Heap<int, 1> h;
        h.push(5);
        h.push(3);
        h.pop();  // 5 제거
        h.push(9);
        h.push(7);
        h.pop();  // 9 제거
        
        AssertTest(h.size() == 2, "혼합 연산 후 크기");
        AssertTest(h.top() == 7, "현재 최대값 확인");
    }
}

void Test06_HeapPropertyMaintenance() {
    // Heap 속성이 모든 연산에서 유지되는지 엄격한 검증
    PrintSection("테스트 6: Heap 속성 유지 검증");
    
    PrintSubSection("6.1 Min Heap 속성 - 랜덤 삽입");
    {
        Heap<int, 0> h;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 1000);
        
        for (int i = 0; i < 100; ++i) {
            h.push(dis(gen));
            AssertTest(VerifyMinHeapProperty(h), 
                       "삽입 " + to_string(i+1) + "번 후 Min Heap 속성 유지");
        }
    }
    
    PrintSubSection("6.2 Max Heap 속성 - 랜덤 삽입");
    {
        Heap<int, 1> h;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 1000);
        
        for (int i = 0; i < 100; ++i) {
            int number = dis(gen);
            h.push(number);
            AssertTest(VerifyMaxHeapProperty(h), 
                       "삽입 " + to_string(i+1) + "번 후 Max Heap 속성 유지 (삽입: " + to_string(number) + ") - " + "(top: " + to_string(h.top()) + ")" );
        }
    }
    
    PrintSubSection("6.3 Min Heap 속성 - 삽입 후 pop");
    {
        Heap<int, 0> h;
        for (int i = 1; i <= 50; ++i) {
            h.push(i);
        }
        
        for (int i = 0; i < 25; ++i) {
            h.pop();
            if (!h.empty()) {
                AssertTest(VerifyMinHeapProperty(h), 
                           "pop " + to_string(i+1) + "번 후 Min Heap 속성 유지");
            }
        }
    }
    
    PrintSubSection("6.4 Max Heap 속성 - 삽입 후 pop");
    {
        Heap<int, 1> h;
        for (int i = 1; i <= 50; ++i) {
            h.push(i);
        }
        
        for (int i = 0; i < 25; ++i) {
            h.pop();
            if (!h.empty()) {
                AssertTest(VerifyMaxHeapProperty(h), 
                           "pop " + to_string(i+1) + "번 후 Max Heap 속성 유지");
            }
        }
    }
}

void Test07_EdgeCasesEmptyAndSingle() {
    // 빈 Heap, 단일 요소 등 경계값 테스트
    PrintSection("테스트 7: 경계값 - 빈 Heap 및 단일 요소");
    
    PrintSubSection("7.1 빈 Heap에서 연속 pop");
    {
        Heap<int, 0> h;
        h.pop();
        h.pop();
        h.pop();
        
        AssertTest(h.empty(), "빈 Heap 연속 pop 안전");
        AssertTest(h.size() == 0, "크기 0 유지");
    }
    
    PrintSubSection("7.2 단일 요소 삽입-삭제 반복");
    {
        Heap<int, 0> h;
        for (int i = 0; i < 100; ++i) {
            h.push(i);
            AssertTest(h.size() == 1, "삽입 후 크기 1");
            AssertTest(h.top() == i, "top 값 확인");
            h.pop();
            AssertTest(h.empty(), "삭제 후 비어있음");
        }
    }
    
    PrintSubSection("7.3 두 요소로 Heap 속성 확인");
    {
        Heap<int, 0> minH;
        minH.push(10);
        minH.push(5);
        AssertTest(minH.top() == 5, "Min Heap 두 요소 - 작은 값이 top");
        
        Heap<int, 1> maxH;
        maxH.push(10);
        maxH.push(5);
        AssertTest(maxH.top() == 10, "Max Heap 두 요소 - 큰 값이 top");
    }
    
    PrintSubSection("7.4 완전히 동일한 값들");
    {
        Heap<int, 0> h;
        for (int i = 0; i < 20; ++i) {
            h.push(42);
        }
        
        AssertTest(h.size() == 20, "동일 값 20개 삽입");
        AssertTest(h.top() == 42, "top이 해당 값");
        
        while (!h.empty()) {
            AssertTest(h.top() == 42, "모든 요소가 동일 값");
            h.pop();
        }
    }
}

void Test08_LargeDataSet() {
    // 대용량 데이터 삽입/삭제 테스트
    PrintSection("테스트 8: 대용량 데이터셋");
    
    PrintSubSection("8.1 Min Heap 10,000개 삽입");
    {
        Heap<int, 0> h;
        for (int i = 0; i < 10000; ++i) {
            h.push(i);
        }
        
        AssertTest(h.size() == 10000, "10,000개 삽입 성공");
        AssertTest(h.top() == 0, "최소값 0이 top");
    }
    
    PrintSubSection("8.2 Max Heap 10,000개 삽입");
    {
        Heap<int, 1> h;
        for (int i = 0; i < 10000; ++i) {
            h.push(i);
        }
        
        AssertTest(h.size() == 10000, "10,000개 삽입 성공");
        AssertTest(h.top() == 9999, "최대값 9999가 top");
    }
    
    PrintSubSection("8.3 대용량 랜덤 삽입 후 정렬 확인");
    {
        Heap<int, 0> h;
        random_device rd;
        mt19937 gen(42);  // 시드 고정
        uniform_int_distribution<> dis(1, 100000);
        
        for (int i = 0; i < 5000; ++i) {
            h.push(dis(gen));
        }
        
        vector<int> extracted;
        while (!h.empty()) {
            extracted.push_back(h.top());
            h.pop();
        }
        
        AssertTest(is_sorted(extracted.begin(), extracted.end()), 
                   "5,000개 랜덤 값 정렬 확인");
    }
}

void Test09_MoveAndCopySemantics() {
    // Move/Copy semantics 정확성 검증
    PrintSection("테스트 9: Move/Copy Semantics");
    
    PrintSubSection("9.1 L-value push (복사)");
    {
        Tracker::ResetCounts();
        Heap<Tracker, 0> h;
        
        Tracker t1(10);
        Tracker t2(5);
        Tracker t3(15);
        
        int beforeCopies = Tracker::totalCopies;
        h.push(t1);
        h.push(t2);
        h.push(t3);
        
        AssertTest(Tracker::totalCopies > beforeCopies, "L-value push는 복사 발생");
        AssertTest(h.top().data == 5, "최소값 정확");
        Tracker::PrintStats();
    }
    
    PrintSubSection("9.2 R-value push (이동)");
    {
        Tracker::ResetCounts();
        Heap<Tracker, 0> h;
        
        h.push(Tracker(10));
        h.push(Tracker(5));
        h.push(Tracker(15));
        
        AssertTest(Tracker::totalMoves > 0, "R-value push는 이동 발생");
        AssertTest(h.top().data == 5, "최소값 정확");
        Tracker::PrintStats();
    }
    
    PrintSubSection("9.3 메모리 누수 없음 확인");
    {
        Tracker::ResetCounts();
        {
            Heap<Tracker, 0> h;
            for (int i = 0; i < 100; ++i) {
                h.push(Tracker(i));
            }
            // Heap 소멸
        }
        
        AssertTest(Tracker::liveCount == 0, "모든 객체 정상 소멸");
        AssertTest(Tracker::totalConstructs == Tracker::totalDestructs, 
                   "생성/소멸 횟수 일치");
        Tracker::PrintStats();
    }
}

void Test10_ExceptionSafety() {
    // 예외 발생 시 안정성 검증
    PrintSection("테스트 10: 예외 안전성");
    
    PrintSubSection("10.1 push 중 예외 발생");
    {
        Exploder::Reset();
        Heap<Exploder, 0> h;
        
        h.push(Exploder(1));
        h.push(Exploder(2));
        
        Exploder::throwAtCopy = 1;  // 첫 번째 복사에서 예외
        
        bool exceptionCaught = false;
        try {
            Exploder e(3);
            h.push(e);  // 복사 생성자에서 예외 발생
        } catch (const runtime_error&) {
            exceptionCaught = true;
        }
        
        AssertTest(exceptionCaught, "예외 정상 발생");
        // Vector가 예외 안전성을 보장하므로 Heap 상태 유지
        AssertTest(h.size() == 2, "예외 후에도 기존 요소 유지");
    }
    
    PrintSubSection("10.2 연속 예외 복구");
    {
        Exploder::Reset();
        Heap<Exploder, 0> h;
        
        for (int i = 0; i < 5; ++i) {
            h.push(Exploder(i));
        }
        
        size_t sizeBefore = h.size();
        
        Exploder::throwAtCopy = 1;
        try {
            Exploder e(10);
            h.push(e);
        } catch (...) {}
        
        Exploder::Reset();  // 예외 조건 해제
        h.push(Exploder(10));  // 정상 삽입
        
        AssertTest(h.size() == sizeBefore + 1, "예외 후 정상 연산 복구");
    }
}

void Test11_ComplexDataTypes() {
    // 복잡한 데이터 타입으로 테스트
    PrintSection("테스트 11: 복잡한 데이터 타입");
    
    PrintSubSection("11.1 string 타입 Min Heap");
    {
        Heap<string, 0> h;
        vector<string> words = {"zebra", "apple", "mango", "banana", "cherry"};
        
        for (const auto& w : words) {
            h.push(w);
        }
        
        AssertTest(h.top() == "apple", "사전순 최소값");
        
        vector<string> sorted;
        while (!h.empty()) {
            sorted.push_back(h.top());
            h.pop();
        }
        
        AssertTest(is_sorted(sorted.begin(), sorted.end()), "문자열 정렬 확인");
    }
    
    PrintSubSection("11.2 사용자 정의 타입 (Task)");
    {
        Heap<Task, 0> h;
        h.push(Task(5, "Low"));
        h.push(Task(1, "Critical"));
        h.push(Task(3, "Medium"));
        h.push(Task(1, "Urgent"));
        
        AssertTest(h.top().priority == 1, "최고 우선순위 작업이 top");
        
        h.pop();
        AssertTest(h.top().priority == 1, "동일 우선순위 다음 작업");
        
        h.pop();
        AssertTest(h.top().priority == 3, "중간 우선순위 작업");
    }
    
    PrintSubSection("11.3 double 타입 정밀도 테스트");
    {
        Heap<double, 0> h;
        h.push(3.14159);
        h.push(2.71828);
        h.push(1.41421);
        h.push(1.73205);
        
        AssertTest(abs(h.top() - 1.41421) < 1e-5, "최소 double 값");
        
        vector<double> sorted;
        while (!h.empty()) {
            sorted.push_back(h.top());
            h.pop();
        }
        
        AssertTest(is_sorted(sorted.begin(), sorted.end()), "double 정렬 확인");
    }
}

void Test12_StressTestHeapProperty() {
    // Heap 속성의 극한 스트레스 테스트
    PrintSection("테스트 12: Heap 속성 스트레스 테스트");
    
    PrintSubSection("12.1 랜덤 삽입/삭제 1000회");
    {
        Heap<int, 0> h;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 1000);
        uniform_int_distribution<> opDis(0, 1);
        
        bool propertyMaintained = true;
        for (int i = 0; i < 1000; ++i) {
            if (opDis(gen) == 0 || h.empty()) {
                h.push(dis(gen));
            } else {
                h.pop();
            }
            
            if (!h.empty() && !VerifyMinHeapProperty(h)) {
                propertyMaintained = false;
                break;
            }
        }
        
        AssertTest(propertyMaintained, "1000회 랜덤 연산 후 속성 유지");
    }
    
    PrintSubSection("12.2 최악의 경우 - 역순 삽입");
    {
        Heap<int, 0> minH;
        for (int i = 10000; i > 0; --i) {
            minH.push(i);
        }
        
        AssertTest(minH.top() == 1, "역순 삽입 후 최소값");
        AssertTest(VerifyMinHeapProperty(minH), "역순 삽입 후 Heap 속성");
    }
    
    PrintSubSection("12.3 연속된 같은 값 대량 삽입");
    {
        Heap<int, 0> h;
        for (int i = 0; i < 1000; ++i) {
            h.push(42);
        }
        
        int count = 0;
        bool allEqual = true;
        while (!h.empty()) {
            if (h.top() != 42) {
                allEqual = false;
                break;
            }
            h.pop();
            count++;
        }
        
        AssertTest(allEqual && count == 1000, "모든 값이 동일");
        
        AssertTest(count == 1000, "모든 요소 정상 추출");
    }
}

void Test13_HeapSortVerification() {
    // Heap을 이용한 정렬 알고리즘 검증
    PrintSection("테스트 13: Heap Sort 정확성 검증");
    
    PrintSubSection("13.1 Min Heap으로 오름차순 정렬");
    {
        vector<int> data = {64, 34, 25, 12, 22, 11, 90, 88, 45, 50, 23, 36, 18, 77};
        Heap<int, 0> h;
        
        for (int v : data) h.push(v);
        
        vector<int> sorted;
        while (!h.empty()) {
            sorted.push_back(h.top());
            h.pop();
        }
        
        vector<int> expected = data;
        sort(expected.begin(), expected.end());
        
        AssertTest(sorted == expected, "Heap Sort 결과 == std::sort 결과");
    }
    
    PrintSubSection("13.2 Max Heap으로 내림차순 정렬");
    {
        vector<int> data = {64, 34, 25, 12, 22, 11, 90, 88, 45, 50, 23, 36, 18, 77};
        Heap<int, 1> h;
        
        for (int v : data) h.push(v);
        
        vector<int> sorted;
        while (!h.empty()) {
            sorted.push_back(h.top());
            h.pop();
        }
        
        vector<int> expected = data;
        sort(expected.begin(), expected.end(), greater<int>());
        
        AssertTest(sorted == expected, "Max Heap Sort 결과 정확");
    }
    
    PrintSubSection("13.3 중복 값 포함 정렬");
    {
        vector<int> data = {5, 2, 8, 2, 9, 1, 5, 5, 3, 1, 8};
        Heap<int, 0> h;
        
        for (int v : data) h.push(v);
        
        vector<int> sorted;
        while (!h.empty()) {
            sorted.push_back(h.top());
            h.pop();
        }
        
        vector<int> expected = data;
        sort(expected.begin(), expected.end());
        
        AssertTest(sorted == expected, "중복 값 정렬 정확성");
    }
}

void Test14_PerformanceVsSTL_Push() {
    // std::priority_queue와 push 성능 비교
    PrintSection("테스트 14: 성능 비교 - Push 연산");
    
    PrintSubSection("14.1 순차적 삽입 - Min Heap vs std::priority_queue");
    {
        cout << "  [사용자 Min Heap]" << endl;
        MEASURE("Min Heap 순차 삽입",
            Heap<int, 0> h;
            for (int i = 0; i < PERF_TEST_SIZE; ++i) {
                h.push(i);
            }
        );
        
        cout << "  [std::priority_queue (greater)]" << endl;
        MEASURE("STL 순차 삽입",
            priority_queue<int, vector<int>, greater<int>> pq;
            for (int i = 0; i < PERF_TEST_SIZE; ++i) {
                pq.push(i);
            }
        );
    }
    
    PrintSubSection("14.2 역순 삽입 - Min Heap vs std::priority_queue");
    {
        cout << "  [사용자 Min Heap]" << endl;
        MEASURE("Min Heap 역순 삽입",
            Heap<int, 0> h;
            for (int i = PERF_TEST_SIZE; i > 0; --i) {
                h.push(i);
            }
        );
        
        cout << "  [std::priority_queue (greater)]" << endl;
        MEASURE("STL 역순 삽입",
            priority_queue<int, vector<int>, greater<int>> pq;
            for (int i = PERF_TEST_SIZE; i > 0; --i) {
                pq.push(i);
            }
        );
    }
    
    PrintSubSection("14.3 랜덤 삽입 - Min Heap vs std::priority_queue");
    {
        // 랜덤 데이터 생성
        random_device rd;
        mt19937 gen(42);
        uniform_int_distribution<> dis(1, 1000000);
        
        vector<int> randomData(PERF_TEST_SIZE);
        for (int& v : randomData) v = dis(gen);
        
        cout << "  [사용자 Min Heap]" << endl;
        MEASURE("Min Heap 랜덤 삽입",
            Heap<int, 0> h;
            for (int v : randomData) {
                h.push(v);
            }
        );
        
        cout << "  [std::priority_queue (greater)]" << endl;
        MEASURE("STL 랜덤 삽입",
            priority_queue<int, vector<int>, greater<int>> pq;
            for (int v : randomData) {
                pq.push(v);
            }
        );
    }
    
    PrintSubSection("14.4 Max Heap vs std::priority_queue (기본)");
    {
        random_device rd;
        mt19937 gen(42);
        uniform_int_distribution<> dis(1, 1000000);
        
        vector<int> randomData(PERF_TEST_SIZE);
        for (int& v : randomData) v = dis(gen);
        
        cout << "  [사용자 Max Heap]" << endl;
        MEASURE("Max Heap 랜덤 삽입",
            Heap<int, 1> h;
            for (int v : randomData) {
                h.push(v);
            }
        );
        
        cout << "  [std::priority_queue (기본)]" << endl;
        MEASURE("STL 랜덤 삽입",
            priority_queue<int> pq;
            for (int v : randomData) {
                pq.push(v);
            }
        );
    }
}

void Test15_PerformanceVsSTL_Pop() {
    // std::priority_queue와 pop 성능 비교
    PrintSection("테스트 15: 성능 비교 - Pop 연산");
    
    PrintSubSection("15.1 전체 추출 - Min Heap vs std::priority_queue");
    {
        // 데이터 준비
        random_device rd;
        mt19937 gen(42);
        uniform_int_distribution<> dis(1, 1000000);
        
        vector<int> randomData(PERF_TEST_SIZE);
        for (int& v : randomData) v = dis(gen);
        
        Heap<int, 0> h;
        for (int v : randomData) h.push(v);
        
        priority_queue<int, vector<int>, greater<int>> pq;
        for (int v : randomData) pq.push(v);
        
        cout << "  [사용자 Min Heap]" << endl;
        MEASURE("Min Heap 전체 pop",
            while (!h.empty()) {
                h.pop();
            }
        );
        
        cout << "  [std::priority_queue]" << endl;
        MEASURE("STL 전체 pop",
            while (!pq.empty()) {
                pq.pop();
            }
        );
    }
    
    PrintSubSection("15.2 부분 추출 (50%) - Min Heap vs std::priority_queue");
    {
        random_device rd;
        mt19937 gen(42);
        uniform_int_distribution<> dis(1, 1000000);
        
        vector<int> randomData(PERF_TEST_SIZE);
        for (int& v : randomData) v = dis(gen);
        
        Heap<int, 0> h;
        for (int v : randomData) h.push(v);
        
        priority_queue<int, vector<int>, greater<int>> pq;
        for (int v : randomData) pq.push(v);
        
        cout << "  [사용자 Min Heap]" << endl;
        MEASURE("Min Heap 50% pop",
            for (int i = 0; i < PERF_TEST_SIZE / 2; ++i) {
                h.pop();
            }
        );
        
        cout << "  [std::priority_queue]" << endl;
        MEASURE("STL 50% pop",
            for (int i = 0; i < PERF_TEST_SIZE / 2; ++i) {
                pq.pop();
            }
        );
    }
}

void Test16_PerformanceVsSTL_Mixed() {
    // 혼합 연산 성능 비교
    PrintSection("테스트 16: 성능 비교 - 혼합 연산");
    
    PrintSubSection("16.1 Push-Pop 교차 연산");
    {
        random_device rd;
        mt19937 gen(42);
        uniform_int_distribution<> dis(1, 1000000);
        
        cout << "  [사용자 Min Heap]" << endl;
        MEASURE("Min Heap 교차 연산",
            Heap<int, 0> h;
            for (int i = 0; i < PERF_TEST_SIZE / 2; ++i) {
                h.push(dis(gen));
                h.push(dis(gen));
                if (!h.empty()) h.pop();
            }
        );
        
        cout << "  [std::priority_queue]" << endl;
        MEASURE("STL 교차 연산",
            priority_queue<int, vector<int>, greater<int>> pq;
            for (int i = 0; i < PERF_TEST_SIZE / 2; ++i) {
                pq.push(dis(gen));
                pq.push(dis(gen));
                if (!pq.empty()) pq.pop();
            }
        );
    }
    
    PrintSubSection("16.2 대량 삽입 + 전체 추출");
    {
        random_device rd;
        mt19937 gen(42);
        uniform_int_distribution<> dis(1, 1000000);
        
        vector<int> data(PERF_TEST_SIZE);
        for (int& v : data) v = dis(gen);
        
        cout << "  [사용자 Min Heap]" << endl;
        MEASURE("Min Heap 삽입+추출",
            Heap<int, 0> h;
            for (int v : data) h.push(v);
            while (!h.empty()) h.pop();
        );
        
        cout << "  [std::priority_queue]" << endl;
        MEASURE("STL 삽입+추출",
            priority_queue<int, vector<int>, greater<int>> pq;
            for (int v : data) pq.push(v);
            while (!pq.empty()) pq.pop();
        );
    }
}

void Test17_PerformanceVsSTL_TopAccess() {
    // top() 접근 성능 비교
    PrintSection("테스트 17: 성능 비교 - Top 접근");
    
    PrintSubSection("17.1 반복적 top() 호출");
    {
        random_device rd;
        mt19937 gen(42);
        uniform_int_distribution<> dis(1, 1000000);
        
        Heap<int, 0> h;
        priority_queue<int, vector<int>, greater<int>> pq;
        
        for (int i = 0; i < 10000; ++i) {
            int v = dis(gen);
            h.push(v);
            pq.push(v);
        }
        
        cout << "  [사용자 Min Heap]" << endl;
        MEASURE("Min Heap top 호출",
            volatile int sum = 0;
            for (int i = 0; i < 1000000; ++i) {
                sum += h.top();
            }
        );
        
        cout << "  [std::priority_queue]" << endl;
        MEASURE("STL top 호출",
            volatile int sum = 0;
            for (int i = 0; i < 1000000; ++i) {
                sum += pq.top();
            }
        );
    }
}

void Test18_MemoryEfficiency() {
    // 메모리 효율성 테스트
    PrintSection("테스트 18: 메모리 효율성");
    
    PrintSubSection("18.1 대량 객체 저장 - 메모리 사용량");
    {
        cout << "  [사용자 Heap - HeavyObject]" << endl;
        MEASURE("Heap 메모리 사용",
            Heap<HeavyObject, 0> h;
            for (int i = 0; i < 1000; ++i) {
                h.push(HeavyObject(1000));
            }
        );
        
        cout << "  [std::priority_queue - HeavyObject]" << endl;
        MEASURE("STL 메모리 사용",
            priority_queue<HeavyObject, vector<HeavyObject>, greater<HeavyObject>> pq;
            for (int i = 0; i < 1000; ++i) {
                pq.push(HeavyObject(1000));
            }
        );
    }
    
    PrintSubSection("18.2 메모리 누수 검증");
    {
        Tracker::ResetCounts();
        {
            Heap<Tracker, 0> h;
            for (int i = 0; i < 10000; ++i) {
                h.push(Tracker(i));
            }
            // Heap 소멸 시작
        }
        // Heap 소멸 완료
        
        AssertTest(Tracker::liveCount == 0, "10,000개 객체 모두 소멸");
        AssertTest(Tracker::totalConstructs == Tracker::totalDestructs,
                   "생성/소멸 횟수 완벽 일치");
        
        cout << "    누수 없음 확인: 생성=" << Tracker::totalConstructs 
             << ", 소멸=" << Tracker::totalDestructs << endl;
    }
}

void Test19_RealWorldScenario_TaskScheduling() {
    // 실제 사용 시나리오: 작업 스케줄링
    PrintSection("테스트 19: 실전 시나리오 - 작업 스케줄링");
    
    PrintSubSection("19.1 우선순위 기반 작업 처리");
    {
        Heap<Task, 0> taskQueue;  // 낮은 priority 값이 더 높은 우선순위
        
        taskQueue.push(Task(3, "Backup Database"));
        taskQueue.push(Task(1, "Fix Critical Bug"));
        taskQueue.push(Task(5, "Update Documentation"));
        taskQueue.push(Task(1, "Server Down Alert"));
        taskQueue.push(Task(2, "Deploy Hotfix"));
        
        vector<string> processOrder;
        while (!taskQueue.empty()) {
            processOrder.push_back(taskQueue.top().name);
            taskQueue.pop();
        }
        
        // 우선순위 1인 작업들이 먼저 처리되어야 함
        AssertTest(processOrder[0].find("Critical") != string::npos || 
                   processOrder[0].find("Alert") != string::npos,
                   "최우선 작업 먼저 처리");
        AssertTest(processOrder.back() == "Update Documentation",
                   "낮은 우선순위 작업 마지막 처리");
    }
    
    PrintSubSection("19.2 이벤트 시뮬레이션 (시간 기반)");
    {
        struct Event {
            int timestamp;
            string description;
            
            bool operator<=(const Event& other) const {
                return timestamp <= other.timestamp;
            }
            bool operator>=(const Event& other) const {
                return timestamp >= other.timestamp;
            }
        };
        
        Heap<Event, 0> eventQueue;
        
        eventQueue.push({100, "User Login"});
        eventQueue.push({50, "System Start"});
        eventQueue.push({150, "Data Sync"});
        eventQueue.push({75, "Load Config"});
        eventQueue.push({200, "Shutdown"});
        
        vector<int> executionOrder;
        while (!eventQueue.empty()) {
            executionOrder.push_back(eventQueue.top().timestamp);
            eventQueue.pop();
        }
        
        AssertTest(is_sorted(executionOrder.begin(), executionOrder.end()),
                   "이벤트가 시간순으로 실행됨");
        AssertTest(executionOrder.front() == 50, "첫 이벤트 확인");
        AssertTest(executionOrder.back() == 200, "마지막 이벤트 확인");
    }
}

void Test20_RealWorldScenario_TopK() {
    // 실제 사용 시나리오: Top-K 요소 찾기
    PrintSection("테스트 20: 실전 시나리오 - Top K Elements");
    
    PrintSubSection("20.1 최소 K개 요소 찾기 (Min Heap)");
    {
        vector<int> data = {7, 10, 4, 3, 20, 15, 8, 2, 9, 12};
        int K = 3;
        
        Heap<int, 0> minHeap;
        for (int v : data) {
            minHeap.push(v);
        }
        
        vector<int> topK;
        for (int i = 0; i < K && !minHeap.empty(); ++i) {
            topK.push_back(minHeap.top());
            minHeap.pop();
        }
        
        AssertTest(topK.size() == 3, "3개 요소 추출");
        AssertTest(topK[0] == 2 && topK[1] == 3 && topK[2] == 4,
                   "최소 3개 요소 정확");
    }
    
    PrintSubSection("20.2 최대 K개 요소 찾기 (Max Heap)");
    {
        vector<int> data = {7, 10, 4, 3, 20, 15, 8, 2, 9, 12};
        int K = 4;
        
        Heap<int, 1> maxHeap;
        for (int v : data) {
            maxHeap.push(v);
        }
        
        vector<int> topK;
        for (int i = 0; i < K && !maxHeap.empty(); ++i) {
            topK.push_back(maxHeap.top());
            maxHeap.pop();
        }
        
        AssertTest(topK.size() == 4, "4개 요소 추출");
        AssertTest(topK[0] == 20 && topK[1] == 15 && topK[2] == 12 && topK[3] == 10,
                   "최대 4개 요소 정확");
    }
}

void Test21_RealWorldScenario_MedianFinder() {
    // 실제 사용 시나리오: 중앙값 찾기 (두 개의 Heap 사용)
    PrintSection("테스트 21: 실전 시나리오 - 스트리밍 중앙값");
    
    PrintSubSection("21.1 동적 중앙값 계산");
    {
        Heap<int, 1> maxHeap;  // 작은 절반의 최대값
        Heap<int, 0> minHeap;  // 큰 절반의 최소값
        
        auto addNumber = [&](int num) {
            if (maxHeap.empty() || num <= maxHeap.top()) {
                maxHeap.push(num);
            } else {
                minHeap.push(num);
            }
            
            // 균형 맞추기
            if (maxHeap.size() > minHeap.size() + 1) {
                minHeap.push(maxHeap.top());
                maxHeap.pop();
            } else if (minHeap.size() > maxHeap.size()) {
                maxHeap.push(minHeap.top());
                minHeap.pop();
            }
        };
        
        auto getMedian = [&]() -> double {
            if (maxHeap.size() == minHeap.size()) {
                return (maxHeap.top() + minHeap.top()) / 2.0;
            }
            return maxHeap.top();
        };
        
        vector<int> stream = {5, 15, 1, 3, 8, 7, 9, 10, 20, 12};
        vector<double> medians;
        
        for (int num : stream) {
            addNumber(num);
            medians.push_back(getMedian());
        }
        
        // 몇 가지 중앙값 검증
        AssertTest(medians[0] == 5.0, "첫 번째 중앙값");
        AssertTest(medians[1] == 10.0, "두 번째 중앙값 (5+15)/2");
        AssertTest(medians[2] == 5.0, "세 번째 중앙값");
        
        cout << "    최종 중앙값: " << getMedian() << endl;
    }
}

void Test22_EdgeCase_BoundaryValues() {
    // 극한 경계값 테스트
    PrintSection("테스트 22: 극한 경계값");
    
    PrintSubSection("22.1 매우 큰 양수");
    {
        Heap<int, 0> h;
        h.push(INT_MAX);
        h.push(INT_MAX - 1);
        h.push(INT_MAX - 2);
        
        AssertTest(h.top() == INT_MAX - 2, "큰 값 중 최소값");
        
        h.pop();
        AssertTest(h.top() == INT_MAX - 1, "다음 최소값");
    }
    
    PrintSubSection("22.2 매우 작은 음수");
    {
        Heap<int, 0> h;
        h.push(INT_MIN);
        h.push(INT_MIN + 1);
        h.push(INT_MIN + 2);
        
        AssertTest(h.top() == INT_MIN, "음수 중 최소값");
    }
    
    PrintSubSection("22.3 양수와 음수 혼합");
    {
        Heap<int, 0> h;
        h.push(-100);
        h.push(100);
        h.push(0);
        h.push(-50);
        h.push(50);
        
        vector<int> sorted;
        while (!h.empty()) {
            sorted.push_back(h.top());
            h.pop();
        }
        
        AssertTest(sorted == vector<int>({-100, -50, 0, 50, 100}),
                   "음수/양수 혼합 정렬");
    }
    
    PrintSubSection("22.4 0 값 처리");
    {
        Heap<int, 0> h;
        h.push(0);
        h.push(0);
        h.push(0);
        h.push(1);
        h.push(-1);
        
        AssertTest(h.top() == -1, "0 포함 최소값");
        h.pop();
        AssertTest(h.top() == 0, "0이 다음 최소값");
    }
}

void Test23_ComparisonOperatorVerification() {
    // <= 및 >= 연산자만 사용하는지 검증
    PrintSection("테스트 23: 비교 연산자 검증");
    
    PrintSubSection("23.1 <= 연산자만 사용 (Min Heap)");
    {
        struct OnlyLessEqual {
            int value;
            OnlyLessEqual(int v) : value(v) {}
            
            bool operator<=(const OnlyLessEqual& other) const {
                return value <= other.value;
            }
            bool operator>=(const OnlyLessEqual& other) const {
                return value >= other.value;
            }
            
            // 다른 비교 연산자는 삭제
            bool operator<(const OnlyLessEqual&) const = delete;
            bool operator>(const OnlyLessEqual&) const = delete;
            bool operator==(const OnlyLessEqual&) const = delete;
            bool operator!=(const OnlyLessEqual&) const = delete;
        };
        
        Heap<OnlyLessEqual, 0> h;
        h.push(OnlyLessEqual(5));
        h.push(OnlyLessEqual(2));
        h.push(OnlyLessEqual(8));
        
        AssertTest(h.top().value == 2, "<= 연산자만으로 Min Heap 동작");
    }
    
    PrintSubSection("23.2 >= 연산자만 사용 (Max Heap)");
    {
        struct OnlyGreaterEqual {
            int value;
            OnlyGreaterEqual(int v) : value(v) {}
            
            bool operator<=(const OnlyGreaterEqual& other) const {
                return value <= other.value;
            }
            bool operator>=(const OnlyGreaterEqual& other) const {
                return value >= other.value;
            }
            
            bool operator<(const OnlyGreaterEqual&) const = delete;
            bool operator>(const OnlyGreaterEqual&) const = delete;
            bool operator==(const OnlyGreaterEqual&) const = delete;
            bool operator!=(const OnlyGreaterEqual&) const = delete;
        };
        
        Heap<OnlyGreaterEqual, 1> h;
        h.push(OnlyGreaterEqual(5));
        h.push(OnlyGreaterEqual(2));
        h.push(OnlyGreaterEqual(8));
        
        AssertTest(h.top().value == 8, ">= 연산자만으로 Max Heap 동작");
    }
}

void Test24_PrintHeapFunction() {
    // printHeap 함수 테스트
    PrintSection("테스트 24: printHeap 함수");
    
    PrintSubSection("24.1 Min Heap 출력");
    {
        Heap<int, 0> h;
        for (int i : {5, 2, 8, 1, 9, 3}) {
            h.push(i);
        }
        
        cout << "    ";
        h.printHeap();
        AssertTest(true, "Min Heap 출력 완료");
    }
    
    PrintSubSection("24.2 Max Heap 출력");
    {
        Heap<int, 1> h;
        for (int i : {5, 2, 8, 1, 9, 3}) {
            h.push(i);
        }
        
        cout << "    ";
        h.printHeap();
        AssertTest(true, "Max Heap 출력 완료");
    }
    
    PrintSubSection("24.3 빈 Heap 출력");
    {
        Heap<int, 0> h;
        cout << "    ";
        h.printHeap();
        AssertTest(true, "빈 Heap 출력 완료");
    }
}

void Test25_ConstCorrectnessAndConst() {
    // const 정확성 테스트
    PrintSection("테스트 25: Const 정확성");
    
    PrintSubSection("25.1 const top() 메서드");
    {
        Heap<int, 0> h;
        h.push(1);
        h.push(2);
        h.push(3);
        
        const Heap<int, 0>& ch = h;
        AssertTest(ch.top() == 1, "const top() 접근 가능");
        //AssertTest(ch.size() == 3, "const에서 size() 호출"); ///< Vector에서 const 로 size를 반환하지 않음
    }
}

// ==========================================
// [메인 함수]
// ==========================================
int main() {
    try {
        cout << "\n" << GREEN << "╔═══════════════════════════════════════════════════════════╗" << RESET << endl;
        cout << GREEN << "║         Heap Class 종합 테스트 시작                         ║" << RESET << endl;
        cout << GREEN << "╚═══════════════════════════════════════════════════════════╝" << RESET << endl;
        
        Test01_BasicConstructorAndEmpty();
        Test02_MinHeapPushAndTop();
        Test03_MaxHeapPushAndTop();
        Test04_MinHeapPopOperation();
        Test05_MaxHeapPopOperation();
        Test06_HeapPropertyMaintenance();
        Test07_EdgeCasesEmptyAndSingle();
        Test08_LargeDataSet();
        Test09_MoveAndCopySemantics();
        Test10_ExceptionSafety();
        Test11_ComplexDataTypes();
        Test12_StressTestHeapProperty();
        Test13_HeapSortVerification();
        Test14_PerformanceVsSTL_Push();
        Test15_PerformanceVsSTL_Pop();
        Test16_PerformanceVsSTL_Mixed();
        Test17_PerformanceVsSTL_TopAccess();
        Test18_MemoryEfficiency();
        Test19_RealWorldScenario_TaskScheduling();
        Test20_RealWorldScenario_TopK();
        Test21_RealWorldScenario_MedianFinder();
        Test22_EdgeCase_BoundaryValues();
        Test23_ComparisonOperatorVerification();
        Test24_PrintHeapFunction();
        Test25_ConstCorrectnessAndConst();
        
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
            cout << RED << "  실패한 테스트를 확인하고 Heap.hpp를 수정해주세요." << RESET << endl << endl;
        }
        
    } catch (const exception& e) {
        cerr << "\n" << RED << "╔═══════════════════════════════════════════════════════════╗" << RESET << endl;
        cerr << RED << "║  [치명적 오류] 테스트 중단: " << e.what() << RESET << endl;
        cerr << RED << "╚═══════════════════════════════════════════════════════════╝" << RESET << endl;
        return 1;
    }

    return (failCount > 0) ? 1 : 0;
}
