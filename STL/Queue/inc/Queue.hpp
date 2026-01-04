/**
 * @file Queue.hpp
 * @author toproof(kmnlmn123@gmail.com)
 * @date 2026-01-04
 * @brief Queue STL 직접 구현해보기
 * @version 2.0.0
 * @details 원형 Queue 방식으로 FIFO 구조를 구현.
 * 
 * version 1.0.0 : unique_ptr을 이용하여 메모리 관리를 하는 Queue Class
 * version 2.0.0 : allocator와 Placement New방식을 이용하여 Raw 메모리 관리
 */

#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

/**
 * @class Queue
 * @brief First IN, First Out 구조의 원형 큐 클래스
 * @tparam Queue의 요소의 타입
 * @details 원형 큐로 구현
 * push: 요소 삽입
 * pop: 요소 제거
 * front: 맨 앞 요소 반환
 * emplace: 인자를 받아 객체 생성 후 요소 삽입 (객체를 내부에서 생성)
 * 
 */
template<typename T>
class Queue
{
  private:
    int frontIndex = 0;
    int backIndex = 0;
    int size = 0;
    int capacity = 0;

    //std::unique_ptr<T[]> queueArray;  
    T* queueArray;
    std::allocator<T> alloc;

  public:
    /**
     * @brief 기본 생성자
     * @param capacity 초기 크기 설정
     * @details make_unique<T[]>(capacity)를 호출하여 메모리 공간을 할당
     */
    Queue(int capacity);

    /**
     * @brief 이동 생성자
     * @param other 이동할 Queue 객체
     * @details 메모리 주소 교체를 통해 이동
     */
    Queue(Queue<T>&& other) noexcept;

    /**
     * @brief 복사 생성자
     * @param other 복사할 Queue 객체
     * @details 모든 요소와 내부 변수를 복사하여 other 객체를 복사
     */
    Queue(const Queue<T>& other);

    /**
     * @brief 복사 대입 생성자
     * @param other 대입 연산자 사용 시 우측 객체
     * @details std::swap을 이용하여 Copy-and-swap을 수행
     */
    Queue& operator=(Queue<T> other);

    bool isEmpty() noexcept;
    bool isFull() noexcept; 
    int getSize() noexcept;

    /**
     * @brief Queue에 L-value 복사 삽입
     * @param value L-value 형태의 T타입
     * @throw 공간이 가득 찬 상태에서 삽입 시 out_of_range 예외 발생
     * @warning 항상 공간(size) 확인 후 삽입 여부 결정
     * @details value를 backIndex번째 배열에 복사 삽입
     */
    void push(const T& value);

    /**
     * @brief Queue에 R-value 이전(move) 삽입
     * @param value R-value 형태의 T타입
     * @throw 공간이 가득 찬 상태에서 삽입 시 out_of_range 예외 발생
     * @warning 항상 공간(size) 확인 후 삽입 여부 결정
     * @details R-value 형태의 T객체를 std::move를 이용하여 삽입(메모리 주소 이전)
     */
    void push(T&& value);
    
    /**
     * @brief Queue 요소 제거
     * @throw 공간이 빈 상태에서 제거 시 out_of_range 예외 발생
     * @warning 항상 빈 공간 확인 후 함수를 실행
     * @details frontIndex번째 요소를 std::destroy_at()으로 소멸한 후 size를 1 감소합니다
     */
    void pop();
    
    /**
     * @brief Queue 맨 앞 요소 반환
     * @return 맨 앞 요소를 반환
     * @throw 공간이 빈 상태에서 호출 시 out_of_range 예외 발생
     * @warning 항상 빈 공간 확인 후 함수를 실행
     */
    const T& front();


    /**
     * @brief 내부에서 객체를 생성하고 삽입합니다
     * @param args 생성자에 필요한 인자들
     * @throw 공간이 가득 찬 상태에서 삽입 시 out_of_range 예외 발생
     * @warning 항상 공간(size) 확인 후 삽입 여부 결정
     * @details 가변 인자 template로 인자를 받아 내부에서 객체를 생성합니다
     */
    template<typename... Args>
    void emplace(Args&&... args);
};


template<typename T> 
Queue<T>::Queue(int capacity): capacity(capacity)
{ 
  // 생성자 메모리 공간 할당을 allocator 방식으로 변경
  //queueArray = std::make_unique<T[]>(this->capacity); 
  queueArray = alloc.allocate(capacity);
  
  T** qq = &queueArray;
  std::uninitialized_fill(queueArray, capacity, nullptr);

  for (int i=0; i<capacity; ++i)
  {
    if (queueArray[i] == nullptr)
    {
      std::cout << "nullptr입니다" << '\n';
    }
  }
}

template<typename T> 
Queue<T>::Queue(Queue<T>&& other) noexcept : 
  queueArray(other.queueArray),
  frontIndex(other.frontIndex),
  backIndex(other.backIndex),
  size(other.size),
  capacity(other.capacity)
{

  // other값 초기화하여 원본 메모리 주소 소멸되는 것을 방지
  other.queueArray = nullptr;
  other.frontIndex = 0;
  other.backIndex = 0;
  other.size = 0;
  other.capacity = 0; 
}

template<typename T> 
Queue<T>::Queue(const Queue<T>& other) :
  frontIndex(other.frontIndex),
  backIndex(other.backIndex),
  size(other.size),
  capacity(other.capacity)
{
  // 메모리 공간 할당을 alloc로 변경
  //queueArray = std::make_unique<T[]>(this->capacity);
  queueArray = alloc.allocate(this->capacity);

  int i=0;
  try
  {
    for (; i<capacity; ++i)
    {
      queueArray[i] = other.queueArray[i];
    }
  }
  catch(const std::exception& e)
  {
    // unique_ptr이 알아서 delete를 수행하여 따로 메모리 관리 불필요
    std::cerr << e.what() << '\n';
    
    alloc.deallocate(capacity);
    throw;
  }
}

template<typename T>
Queue<T>& Queue<T>::operator=(Queue<T> other)
{
  // 1. copy and swap
  std::swap(queueArray, other.queueArray);
  std::swap(frontIndex, other.frontIndex);
  std::swap(backIndex, other.backIndex);
  std::swap(capacity, other.capacity);
  std::swap(size, other.size);

  // 2. 자신을 리턴하여 연쇄 작용 `q = qq = qqq`가 동작하도록 리턴
  return *this;
}


template<typename T>
bool Queue<T>::isEmpty() noexcept 
{ 
  if (size <= 0) return true;
  return false;
}

template<typename T>
bool Queue<T>::isFull() noexcept 
{ 
  if (size >= capacity) return true;
  return false;
}

template<typename T>
int Queue<T>::getSize() noexcept
{
  return size+1;
}

template<typename T>
void Queue<T>::push(const T& value)
{
  if (isFull()) throw std::out_of_range("Error Push copy, Index Out of Bounds: " + std::to_string(backIndex));

  queueArray[backIndex] = value;
  backIndex = (backIndex+1) % capacity;
  size++;
}

template<typename T>
void Queue<T>::push(T&& value)
{
  if (isFull()) throw std::out_of_range("Error Push move, Index Out of Bounds: " + std::to_string(backIndex));

  queueArray[backIndex] = std::move(value);
  backIndex = (backIndex+1) % capacity;
  size++;
}

template<typename T>
void Queue<T>::pop()
{ 
  if (isEmpty()) throw std::out_of_range("Error pop, Index Out of Bounds: " + std::to_string(frontIndex));

  //std::destroy_at(&queueArray[frontIndex]);
  frontIndex = (frontIndex+1) % capacity;
  size--;
}

template<typename T>
const T& Queue<T>::front()
{
  if (isEmpty()) throw std::out_of_range("Error front, Index Out of Bounds: " + std::to_string(frontIndex+1));
  return queueArray[frontIndex % capacity];
}

template<typename T>
template<typename... Args>
void Queue<T>::emplace(Args&&... args)
{
    if (isFull()) throw std::out_of_range("Error emplace_push, Index Out of Bounds: " + std::to_string(backIndex));
    
    queueArray[backIndex] = T(std::forward<Args>(args)...);
    backIndex = (backIndex+1) % capacity;
    size++;
}
