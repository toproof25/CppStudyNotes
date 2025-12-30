#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

// 성능 테스트를 위한 "무거운" 객체
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

namespace v1
{

class Stack
{
  private:
    int top = -1;
    int size;
    HeavyObject* stackArray;

  public:
    // 생성자
    Stack(int N) : size(N) { stackArray = new HeavyObject[N]; }

    // 소멸자
    ~Stack() { delete[] this->stackArray; }

    // 값 삽입
    void push(HeavyObject push_number)
    {
      if (!isFull()) 
        stackArray[++top] = push_number;
    }

    // 값 제거 (최상단 먼저 제거)
    HeavyObject pop()
    {
      if (!isEmpty())
      {
        HeavyObject pop_value = stackArray[top];
        top--;
        return pop_value;
      }
      return HeavyObject();
    }

    // 맨 위 값
    HeavyObject peek()
    {
      return stackArray[top];
    }

    // 비어있는지 확인
    bool isEmpty()
    {
      if (top == -1)
        return true;
      return false;
    }

    // 최대인지 확인
    bool isFull()
    {
      if (top-1 == size)
        return true;
      return false;
    }
};

}

namespace v2
{

class Stack
{
  private:
    int top = -1;
    int size;
    HeavyObject* stackArray;

  public:
    // 생성자
    Stack(int N) : size(N) { stackArray = new HeavyObject[N]; }

    // 소멸자
    ~Stack() { delete[] this->stackArray; }

    // 값 삽입
    void push(HeavyObject push_number)
    {
      if (!isFull()) 
        stackArray[++top] = push_number;
    }

    // 값 제거 (최상단 먼저 제거)
    HeavyObject pop()
    {
      if (!isEmpty())
      {
        HeavyObject pop_value = stackArray[top];

        // v2 수정 사항, pop을 할 때 소멸자를 명시적으로 호출하여 공간을 유지하면서 객체를 지운다
        stackArray[top--].~HeavyObject();
        return pop_value;
      }
      return HeavyObject();
    }

    // 맨 위 값
    HeavyObject peek()
    {
      return stackArray[top];
    }

    // 비어있는지 확인
    bool isEmpty()
    {
      if (top == -1)
        return true;
      return false;
    }

    // 최대인지 확인
    bool isFull()
    {
      if (top-1 == size)
        return true;
      return false;
    }
};

}

namespace v3
{

class Stack
{
  private:
    int top = -1;
    int size;
    std::unique_ptr<HeavyObject[]> stackArray;

  public:
    // 생성자
    Stack(int N) : size(N) { stackArray = std::make_unique<HeavyObject[]>(this->size); }

    // 값 삽입
    void push(HeavyObject push_number)
    {
      if (isFull()) 
        throw std::out_of_range("Error Push, Index Out of Bounds: " + std::to_string(top));
  
      stackArray[++top] = push_number;
    }

    // 값 제거 (최상단 먼저 제거)
    HeavyObject pop()
    {
      if (isEmpty()) 
        throw std::out_of_range("Error pop, Index Out of Bounds: " + std::to_string(top));

      HeavyObject pop_value = std::move(stackArray[top--]);
      return pop_value;
    }

    // 맨 위 값
    HeavyObject peek()
    {
      return stackArray[top];
    }

    // 비어있는지 확인
    bool isEmpty()
    {
      if (top == -1)
        return true;
      return false;
    }

    // 최대인지 확인
    bool isFull()
    {
      if (top == size-1)
        return true;
      return false;
    }
};

}

namespace v4
{

template<typename T>
class Stack
{
  private:
    int top = -1;
    int size;
    std::unique_ptr<T[]> stackArray;

  public:
    // 생성자
    Stack(int size) : size(size) { stackArray = std::make_unique<T[]>(this->size); }

    // 복사 저장
    void push(const T& push_number)
    {
      if (isFull()) 
        throw std::out_of_range("Error Push, Index Out of Bounds: " + std::to_string(top));
  
      stackArray[++top] = push_number;
    }

    // move 저장
    void push(T&& push_number)
    {
      if (isFull()) 
        throw std::out_of_range("Error Push, Index Out of Bounds: " + std::to_string(top));
  
      stackArray[++top] = std::move(push_number);
    }

    // emplace 가변인자 테스트를 위해 구현 저장
    void emplace(int&& int_value, const std::string& string_value)
    {
      if (isFull()) 
        throw std::out_of_range("Error emplace, Index Out of Bounds: " + std::to_string(top));
  
      stackArray[++top] = T(int_value, string_value);
    }

    // 값 제거 (최상단 먼저 제거)
    T pop()
    {
      if (isEmpty()) 
        throw std::out_of_range("Error pop, Index Out of Bounds: " + std::to_string(top));

      T pop_value = std::move(stackArray[top--]);
      return pop_value;
    }

    // 맨 위 값
    T peek()
    {
      return stackArray[top];
    }

    // 비어있는지 확인
    bool isEmpty()
    {
      if (top == -1)
        return true;
      return false;
    }

    // 최대인지 확인
    bool isFull()
    {
      if (top == size-1)
        return true;
      return false;
    }
};

}

namespace v5
{

template<typename T>
class Stack
{
  private:
    int top = -1;
    int size;
    std::unique_ptr<T[]> stackArray;  

  private:
    // noexcept는 예외(throw)를 반환하지 않는다는 명시를 의미
    bool isEmpty() noexcept;
    bool isFull() noexcept; 

  public:
    Stack(int size);

    void push(const T& value);
    void push(T&& value);
    T pop();
    T peek();

    // `typename... Args`는 받는 타입의 갯수가 일정하지 않을 때 여러 개를 받는 것을 의미한다
    template<typename... Args>
    void emplace(Args&&... args);

    template<typename... Args>
    void emplaceNotForward(Args&&... args);

};


// 생성자에서 size를 설정하고, size만큼 T타입의 동적 배열을 unique_ptr로 정의함
template<typename T> 
Stack<T>::Stack(int size): size(size){ stackArray = std::make_unique<T[]>(this->size); }

// Stack이 비어있으면 true
template<typename T>
bool Stack<T>::isEmpty() noexcept { return top == -1; }

// Stack이 꽉차있으면 true
template<typename T>
bool Stack<T>::isFull() noexcept { return top >= size-1; }

// L-value 공간을 확인 후 삽입
// L-value는 원본을 참조하는 value가 있으며, stackArray에 복사되어 저장이 된다
template<typename T>
void Stack<T>::push(const T& value)
{
  if (isFull()) throw std::out_of_range("Error Push, Index Out of Bounds: " + std::to_string(top));
  stackArray[++top] = value;
}
// R-value 공간을 확인 후 삽입
// `&&`는 R-value를 의미하고, R-value는 임시 객체이기에 복사가 아닌 주소만 이동. 힙 영역에서 주소는 그대로고 stackArray에서 해당 주소를 포함한다
template<typename T>
void Stack<T>::push(T&& value)
{
  if (isFull()) throw std::out_of_range("Error Push, Index Out of Bounds: " + std::to_string(top));
  stackArray[++top] = std::move(value);
}

// 공간을 확인 후 제거
template<typename T>
T Stack<T>::pop()
{ 
  if (isEmpty()) throw std::out_of_range("Error pop, Index Out of Bounds: " + std::to_string(top));

  // std::unique_prt 스마트 포인터는 `delete`로 하나의 요소만 해제할 수 없으므로 move로 이동한 후 기본값으로 대체하는 방법
  // `std::move`의 경우 런타임에서 발생하는 것이 아닌 컴파일 과정에서 발생함
  // 간단하게 전체가 이동하는게 아닌 포인터 주소만 이동한 후 이동 생성자에서 초기화가 발생 
  T popValue = std::move(stackArray[top--]);
  return popValue;
}

// 예외 상황은 throw로 반환하여 스택 풀기가 발생하여 main함수에서 catch로 처리됨
template<typename T>
T Stack<T>::peek()
{
  if (isEmpty()) throw std::out_of_range("Error peek, Index Out of Bounds: " + std::to_string(top));
  return stackArray[top];
}


// `typename... Args`는 받는 타입의 갯수가 일정하지 않을 때 여러 개를 받는 것을 의미한다
template<typename T>
template<typename... Args>
void Stack<T>::emplace(Args&&... args)
{
    if (isFull()) 
    {
        throw std::out_of_range("Error emplace_push, Index Out of Bounds: " + std::to_string(top));
    }

    // `std::forward<타입>()`의 경우 들어온 형태 그대로 넘겨주는 것을 의미
    // 여러 개의 요소로 이루어진 경우 `std::forward<Args>(args)...`에서 ...을 붙여서 합쳐진 요소를 풀어주는 것이 필요
    // 컴파일 단계에서 컴파일러가 이를 `T(std::forward<Args>(int), std::forward<Args>(double), std::forward<Args>(string));`으로 함수를 구현한다
    stackArray[++top] = T(std::forward<Args>(args)...);
}

// `typename... Args`는 받는 타입의 갯수가 일정하지 않을 때 여러 개를 받는 것을 의미한다
template<typename T>
template<typename... Args>
void Stack<T>::emplaceNotForward(Args&&... args)
{
    if (isFull()) 
    {
        throw std::out_of_range("Error emplace_push, Index Out of Bounds: " + std::to_string(top));
    }
    stackArray[++top] = T(args...);
}

}





