/**
 * @file Stack.hpp
 * @author toproof(kmnlmn123@gmail.com)
 * @date 2025-12-30
 * @brief allocator와 Placament New를 기반으로 한 동적 Stack 라이브러리
 * @details allocator를 이용하여 메모리 할당과 객체 생성을 분리하고, Placement New를 이용하여 객체 생성과 해제를 이용하여 성능 확보
 */

#include <iostream>
#include <memory>
#include <stdexcept>
#include <new>
#include <utility>

namespace ok 
{

/**
 * @class Stack
 * @brief 동적으로 메모리 자원 할당과 회수를 하는 LIFO 구조 Stack 클래스
 * @tparam T Stack에 저장될 타입
 * @details allocate와 Placement New를 이용하여 메모리를 효율적으로 할당과 해제하며, 
 * 이동, 복사, 복사 대입 생성자를 구현하여 다양한 상황에서 동작하도록 구현 
 */
template<typename T>
class Stack
{
  private:
    size_t capacity;
    int top = -1;

    /** @brief alloc로 할당한 메모리 공간의 시작 주소  */
    T* stackArray;

    /** @brief Raw Memory를 다루기 위한 객체 */
    std::allocator<T> alloc;

  private:
    bool isFull() noexcept; 
    void resize();

  public:

    /**
     * @brief 기본 생성자
     * @param size 최초 Stack 크기 설정
     * @note size만큼 allocate로 공간을 할당
     */
    Stack(size_t capacity);

    /**
     * @brief 이동 생성자
     * @param other R-value값으로 전달받아 호출 Stack에 값을 이동/복사
     * @note other의 값을 모두 이동/복사 후 초기화 하여 데이터가 소멸되지 않음. 예외를 발생하기 않음을 명시하여 복사와 이동 모두 동작하도록 키워드 명시
     */
    Stack(Stack<T>&& other) noexcept; 

    /**
     * @brief 복사 생성자
     * @param other other의 모든 요소와 상태를 복사하여 가져온다
     * @throw Placement New로 요소를 복사 중 예외가 발생 시 메모리를 회수하고, 예외를 발생
     * @note other의 모든 요소를 복사해온다
     */
    Stack(const Stack<T>& other);     

    /**
     * @brief 복사 대입 연산자 (Copy-and-Swap Idiom 사용)
     * @param other 값으로 전달받아 내부적으로 복사 생성자를 활용함
     * @return Stack& 자기 자신에 대한 참조
     * @note 자기 대입과 예외 안전성을 동시에 해결함
     */
    Stack& operator=(Stack<T> other); 


    /**
     * @brief 소멸자
     * @note Placemant New로 할당한 각 요소를 모두 destroy_at로 해제한 후, allocate로 할당한 Stack 공간을 모두 dealocate로 자원 회수
     */
    ~Stack();

    bool isEmpty() noexcept;
    const size_t getCapacity() { return capacity; };
    const int size() { return top+1; };

    /**
     * @brief Stack 공간에 1개 요소를 복사하여 삽입합니다
     * @param value Stack 요소의 타입
     * @note 공간이 가득 찬 상태에서 호출할 경우 resize()가 동작할 수 있음
     */
    void push(const T& value);

    /**
     * @brief Stack 공간에 1개 요소를 move하여 삽입합니다
     * @param value Stack 요소의 R-value 타입
     * @note 공간이 가득 찬 상태에서 호출할 경우 resize()가 동작할 수 있음
     */
    void push(T&& value);

    /**
     * @brief Stack에서 top번째 요소를 제거
     * @throw 빈 공간에서 호출 시 out_of_range 예외가 발생
     * @warning 빈 공간에서 호출 금지. 항상 size()로 확인
     */
    void pop();
    
    /**
     * @brief Stack에서 가상 상단에 있는 요소를 반환합니다
     * @return Stack의 최상단 요소
     * @throw 빈 공간에서 호출 시 out_of_range 예외가 발생
     * @warning 빈 공간에서 호출 금지. 항상 size()로 확인
     */
    T& peek();

    /**
     * @brief n개의 생성자 인수를 받아 내부에서 객체를 생성 후 요소를 삽입합니다
     * @param args n개의 생성자 인수를 받는 타입의 파라미터
     * @note 공간이 가득 찬 상태에서 호출할 경우 resize()가 동작할 수 있음
     */
    template<typename... Args>
    void emplace(Args&&... args);
};

}


template<typename T> 
ok::Stack<T>::Stack(Stack<T>&& other) noexcept 
    : stackArray(other.stackArray),
      capacity(other.capacity),
      top(other.top)
{
    // 원본 값들을 초기화시켜서 원본 Stack 소멸자가 호출되어서 이전/복사된 값들에 영향이 없도록함
    other.stackArray = nullptr;
    other.capacity = 0;
    other.top = -1;
}

template<typename T> 
ok::Stack<T>::Stack(const Stack<T>& other) : top(other.top), capacity(other.capacity)
{
  // 원본 객체 크기만큼 메모리 공간을 할당
  stackArray = this->alloc.allocate(this->capacity); 

  // i를 따로 정의하는 이유는 catch에서 i번째 까지 만든 객체를 다시 반납하기 위해
  int i=0;
  try
  {
    for (; i<=this->top; ++i)
    {
      new (&(this->stackArray[i])) T(other.stackArray[i]);
    }
  }
  catch(...)
  {
    // 오류 발생 시 i번째까지 할당한 자원을 모두 회수하여 메모리 누수 방지
    std::cout << "복사 생성자 오류 발생" << '\n';
    for (int j=0; j<i; ++j) std::destroy_at(&this->stackArray[j]);
    this->alloc.deallocate(this->stackArray, this->capacity);
    throw;
  }
}

template<typename T> 
ok::Stack<T>& ok::Stack<T>::operator=(Stack<T> other)
{
  // Copy-and-swap 방식으로 안전하게 대입 연산자 복사를 수행
  std::swap(this->stackArray, other.stackArray);
  std::swap(this->capacity, other.capacity);
  std::swap(this->top, other.top);
  return *this;
}

template<typename T> 
ok::Stack<T>::Stack(size_t capacity): capacity(capacity) { stackArray = alloc.allocate(capacity); } 

template<typename T>
ok::Stack<T>::~Stack()
{
  // 남아있는 모든 요소(객체)의 소멸자를 호출
  for (int i=0; i<=top; ++i)
  {
    std::destroy_at(&stackArray[i]);
  }

  // Stack 공간을 반납
  if (stackArray != nullptr)
    alloc.deallocate(stackArray, capacity);
}

template<typename T>
bool ok::Stack<T>::isEmpty() noexcept { return top == -1; }

template<typename T>
bool ok::Stack<T>::isFull() noexcept { return top+1 >= capacity; }

template<typename T>
void ok::Stack<T>::push(const T& value)
{
  if (isFull()) 
  {
    resize();
  }

  new (&stackArray[top+1]) T(value);
  top++;
}

template<typename T>
void ok::Stack<T>::push(T&& value)
{
  if (isFull()) 
  {
    resize();
  }

  new (&stackArray[top+1]) T(std::move(value));
  top++;
}

template<typename T>
void ok::Stack<T>::pop()
{ 
  if (isEmpty()) throw std::out_of_range("pop Error: pop을 할 공간이 없습니다 -> " + std::to_string(top));

  std::destroy_at(&stackArray[top]);
  top--;
}

template<typename T>
T& ok::Stack<T>::peek()
{
  if (isEmpty()) throw std::out_of_range("Error peek, Index Out of Bounds: " + std::to_string(top));
  return stackArray[top];
}

template<typename T>
template<typename... Args>
void ok::Stack<T>::emplace(Args&&... args)
{
    if (isFull()) 
    {
      resize();
    }

    // Placement New 방식을 이용하여 객체를 생성과 동시에 메모리 공간에 할당
    new (&stackArray[top+1]) T(std::forward<Args>(args)...);
    top++;
}

template<typename T>
void ok::Stack<T>::resize()
{
  // 기존 공간보다 2배 큰 메모리 공간을 할당
  int reCapacity = capacity * 2;
  T* resizeArray = alloc.allocate(reCapacity);

  // 예외 처리를 통해 예외 발생 시 안전하게 할당한 메모리를 반납
  int i=0;
  try
  {
    for (; i<=top; ++i)
    {
      // T객체의 이동 생성자가 noexcept인 경우 이동하며, noexcept 키워드가 없다면 안전하게 복사를 수행하는 move_if_noexcept
      new (&resizeArray[i]) T(std::move_if_noexcept(stackArray[i]));
    }
  }
  catch(const std::exception& e)
  {
    // 예외 발생 시 i번째까지 할당한 T객체 모두 소멸자를 호출
    for (int j=0; j<i; ++j) std::destroy_at(&resizeArray[j]);

    // 2배 늘렸던 메모리 공간을 다시 회수
    alloc.deallocate(resizeArray, reCapacity);
    throw;
  }
  
  // 성공적으로 resize가 되면, 기존 공간 객체들의 소멸자를 호출 및 기존 메모리 공간을 회수
  for (i=0; i<=top; ++i)
  {
    std::destroy_at(&stackArray[i]);
  }
  alloc.deallocate(stackArray, capacity);
  
  // 변경된 메모리 공간 T*와 capacity를 설정
  stackArray = resizeArray;
  capacity = reCapacity;
};