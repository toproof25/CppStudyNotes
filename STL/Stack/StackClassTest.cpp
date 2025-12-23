#include <iostream>
#include <string>
#include <vector>
#include <stack>   // STL stack 비교를 위해 추가
#include <chrono>
#include <cassert>
#include "Stack.hpp"

using namespace std;

// [테스트 도우미 클래스 1] 객체의 생성/소멸/이동을 추적
class Tracker {
public:
    static int liveCount;
    string name;

    Tracker() : name("Default") { liveCount++; }
    Tracker(string n) : name(n) { liveCount++; }
    
    // 복사 생성자
    Tracker(const Tracker& other) : name(other.name + "_copy") { liveCount++; }
    
    // 이동 생성자
    Tracker(Tracker&& other) noexcept : name(std::move(other.name)) {
        other.name = "Moved-from";
        liveCount++;
    }

    ~Tracker() { liveCount--; }
};
int Tracker::liveCount = 0;

// [테스트 도우미 클래스 2] 특정 횟수 복사 시 예외를 던짐 (강한 예외 보증 테스트용)
struct Exploder {
    static int copyCount;
    static int throwAt;
    int id;

    Exploder(int i) : id(i) {}
    Exploder(const Exploder& other) : id(other.id) {
        if (++copyCount == throwAt) throw runtime_error("Booooom!");
    }
};
int Exploder::copyCount = 0;
int Exploder::throwAt = 0;

// ---------------------------------------------------------

void TestBasic() {
    cout << "[1. 기본 기능 및 동적 확장 테스트] ";
    ok::Stack<int> s(2); // 초기 용량 2
    s.push(10);
    s.push(20);
    s.push(30); // resize 발생해야 함

    assert(s.size() == 3);
    assert(s.peek() == 30);
    s.pop();
    assert(s.peek() == 20);
    cout << "▶ 통과" << endl;
}

void TestResourceManagement() {
    cout << "[2. 리소스 관리 및 누수 테스트] ";
    {
        ok::Stack<Tracker> s(5);
        s.push(Tracker("A"));
        s.push(Tracker("B"));
        s.emplace("C");
        
        assert(Tracker::liveCount >= 3); 
    }
    // 스택이 소멸된 후 Tracker 객체들이 모두 destroy 되었는지 확인
    assert(Tracker::liveCount == 0);
    cout << "▶ 통과 (누수 없음)" << endl;
}

void TestCopyAndMove() {
    cout << "[3. Copy-and-Swap 및 이동 테스트] ";
    ok::Stack<string> s1(5);
    s1.push("Hello");
    s1.push("World");

    ok::Stack<string> s2 = s1; // 복사 생성자
    assert(s2.size() == 2);
    assert(s2.peek() == "World");

    ok::Stack<string> s3 = std::move(s1); // 이동 생성자
    assert(s1.size() == 0); // 원본은 비워져야 함
    assert(s3.size() == 2);
    cout << "▶ 통과" << endl;
}

void TestExceptionSafety() {
    cout << "[4. 강한 예외 보증 테스트] ";
    ok::Stack<Exploder> s(2);
    s.push(Exploder(1));
    s.push(Exploder(2));

    Exploder::copyCount = 0;
    Exploder::throwAt = 1; // resize 도중 첫 번째 복사에서 예외 발생 시뮬레이션

    try {
        s.push(Exploder(3)); // 여기서 resize() 호출됨
    } catch (...) {
        // 예외가 발생해도 기존 s의 상태(데이터 2개)는 유지되어야 함 (강한 예외 보증)
        assert(s.size() == 2);
    }
    cout << "▶ 통과" << endl;
}

// [5. STL과의 성능 비교 테스트]
void TestPerformanceComparison() {
    cout << "\n[5. STL 성능 비교 테스트 (1,000,000건 삽입)]" << endl;
    const int count = 1000000; // 데이터 양을 100만 건으로 증량
    
    // 1. ok::Stack (사용자 정의)
    {
        ok::Stack<int> s(10);
        auto start = chrono::high_resolution_clock::now();
        for(int i = 0; i < count; ++i) {
            s.push(i);
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = end - start;
        cout << " - ok::Stack (Vector 기반)   : " << diff.count() << "s" << endl;
    }

    // 2. std::stack (내부적으로 std::vector 사용하도록 설정)
    {
        std::stack<int, std::vector<int>> st;
        auto start = chrono::high_resolution_clock::now();
        for(int i = 0; i < count; ++i) {
            st.push(i);
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = end - start;
        cout << " - std::stack (std::vector)  : " << diff.count() << "s" << endl;
    }

    // 3. std::stack (기본값 - std::deque 기반)
    {
        std::stack<int> st; // 기본값은 deque
        auto start = chrono::high_resolution_clock::now();
        for(int i = 0; i < count; ++i) {
            st.push(i);
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = end - start;
        cout << " - std::stack (std::deque)   : " << diff.count() << "s" << endl;
    }
}

// [6. Emplace 효율성 테스트]
// 무거운 객체를 생성할 때 복사/이동 없이 바로 생성되는지 확인
void TestEmplaceEfficiency() {
    cout << "\n[6. Emplace vs Push 성능 테스트 (무거운 객체)]" << endl;
    const int count = 100000;
    
    // Push 테스트 (임시 객체 생성 및 이동 발생)
    {
        ok::Stack<string> s(10);
        auto start = chrono::high_resolution_clock::now();
        for(int i = 0; i < count; ++i) {
            s.push(string("Very Long String to avoid SSO optimization ") + to_string(i));
        }
        auto end = chrono::high_resolution_clock::now();
        cout << " - push(T&&) 시간    : " << chrono::duration<double>(end - start).count() << "s" << endl;
    }

    // Emplace 테스트 (가변 인자를 통한 직접 생성)
    {
        ok::Stack<string> s(10);
        auto start = chrono::high_resolution_clock::now();
        for(int i = 0; i < count; ++i) {
            // string 생성자에 필요한 인자만 전달하여 내부에서 직접 생성
            s.emplace("Very Long String to avoid SSO optimization " + to_string(i));
        }
        auto end = chrono::high_resolution_clock::now();
        cout << " - emplace(Args&&) 시간 : " << chrono::duration<double>(end - start).count() << "s" << endl;
    }
}

int main() {
    try {
        cout << "=== Stack Class Integrity & Performance Test ===\n" << endl;
        TestBasic();
        TestResourceManagement();
        TestCopyAndMove();
        TestExceptionSafety();
        
        // 성능 테스트 시작
        TestPerformanceComparison();
        TestEmplaceEfficiency();
        
        cout << "\n모든 테스트를 성공적으로 마쳤습니다!" << endl;
    } catch (const exception& e) {
        cerr << "\n테스트 중 치명적 오류 발생: " << e.what() << endl;
    }
    return 0;
}