/**
 * @file Queue.hpp
 * @author toproof(kmnlmn123@gmail.com)
 * @date 2026-01-06
 * @brief Raw Memory관리를 바탕으로 Queue STL 직접 구현해보기
 * @version 2.1.0
 * @details 원형 Queue 방식으로 FIFO 구조를 구현.
 * 
 * version 1.0.0 : unique_ptr을 이용하여 메모리 관리를 하는 Queue Class
 * version 2.0.0 : allocator와 Placement New방식을 이용하여 Raw 메모리 관리
 * vwesion 2.1.0 : resize 함수를 구현하여 동적 크기 원형 큐를 구현 / 크기 관련 변수 타입을 size_t 로 변경
 */

#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

/**
 * @class Queue
 * @brief First In, First Out 구조의 원형 큐 클래스
 * @tparam Queue의 요소의 타입
 * @details allocator와 Placement New를 이용한 Raw Memory 원형 큐로 구현
 * push: 요소 삽입
 * pop: 요소 제거
 * front: 맨 앞 요소 반환
 * emplace: 인자를 받아 객체 생성 후 요소 삽입 (객체를 내부에서 생성)
 * getSize: 현재 요소 수를 반환
 * getCapacity: 메모리 공간 크기를 반환
 * 
 */
template<typename T>
class Queue
{
  private:
    int frontIndex = 0;
    int backIndex = 0;

    size_t size = 0;
    size_t capacity = 0;

    T* queueArray;
    std::allocator<T> alloc;

  private:
    /**
     * @brief Queue 최대 사이즈를 늘립니다
     * @throw 메모리 재할당 시 오류가 발생하는 경우 예외가 발생
     * @details Full 상태에서 push할 경우 호출되는 함수
     * - 기존 capacity에서 2배만큼 크기가 증가합니다
     * - frontIndex는 0으로 backIndex는 size와 같게 초기화됩니다.
     */
    void resize();

  public:
    /**
     * @brief 기본 생성자
     * @param capacity 초기 크기 설정
     * @details capacity만큼 allocate로 빈 메모리 공간을 할당
     */
    Queue(int capacity);

    /**
     * @brief 소멸자
     * @details 모든 객체 소멸자 호출 및 메모리 공간 회수 
     */
    ~Queue();

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
     * @warning 최대 크기를 넘어서는 삽입 시 resize로 메모리 재할당 발생
     * @details R-value 형태의 T객체를 std::move를 이용하여 삽입(메모리 주소 이전)
     */
    void push(T&& value);
    
    /**
     * @brief Queue 요소 제거
     * @warning 최대 크기를 넘어서는 삽입 시 resize로 메모리 재할당 발생
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
     * @warning 최대 크기를 넘어서는 삽입 시 resize로 메모리 재할당 발생
     * @details 가변 인자 template로 인자를 받아 내부에서 객체를 생성합니다
     */
    template<typename... Args>
    void emplace(Args&&... args);

    bool isEmpty() noexcept;
    bool isFull() noexcept; 
    
    size_t getSize() noexcept;
    size_t getCapacity() noexcept;
};


template<typename T> 
Queue<T>::Queue(int capacity): capacity(capacity)
{ 
  queueArray = alloc.allocate(capacity);
}

template<typename T> 
Queue<T>::~Queue()
{ 
  
  // 1. 유효한 객체만 소멸
  for (int i=0; i<size; ++i)
  {
    std::destroy_at(&queueArray[frontIndex]);
    frontIndex++;
    if (frontIndex >= capacity) frontIndex = 0;
  }

  // 2. queueArray값이 이동 생성자 등에 의해 초기화된 상태가 아니라면 Queue 전체 메모리 회수
  if (queueArray != nullptr)
    alloc.deallocate(queueArray, capacity);
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
  // 1. other의 메모리 공간 크기만큼 할당
  queueArray = alloc.allocate(this->capacity);

  // 2. 유효한 T 객체만 복사 할당
  int i=0;
  int fIndex=frontIndex;
  try
  {    
    for (; i<size; ++i)
    {
      new (queueArray+fIndex) T(other.queueArray[fIndex]);
      fIndex++;
      if (fIndex >= capacity) fIndex = 0;
    }
  }
  catch(const std::exception& e)
  {
    // try 코드에서 생성한 객체만 제거 후 Queue 메모리 공간 회수
    fIndex--;
    for(--i; i>=0; --i)
    {
      std::destroy_at(&queueArray[fIndex]);
      fIndex--;
      if (fIndex <= 0) fIndex = capacity-1;
    }
    alloc.deallocate(queueArray, capacity);
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
size_t Queue<T>::getSize() noexcept
{
  return size;
}

template<typename T>
size_t Queue<T>::getCapacity() noexcept
{
  return capacity;
}


template<typename T>
void Queue<T>::push(const T& value)
{
  if (isFull()) resize();

  new (&queueArray[backIndex]) T(value);
  backIndex++;
  if (backIndex >= capacity) backIndex = 0;
  size++;
}

template<typename T>
void Queue<T>::push(T&& value)
{
  if (isFull()) resize();

  new (&queueArray[backIndex]) T(std::move(value));
  backIndex++;
  if (backIndex >= capacity) backIndex = 0;
  size++;
}

template<typename T>
void Queue<T>::pop()
{ 
  if (isEmpty()) throw std::out_of_range("Error pop, Index Out of Bounds: " + std::to_string(frontIndex));

  std::destroy_at(&queueArray[frontIndex]);
  frontIndex++;
  if (frontIndex >= capacity) frontIndex = 0;
  size--;
}

template<typename T>
const T& Queue<T>::front()
{
  if (isEmpty()) throw std::out_of_range("Error front, Index Out of Bounds: " + std::to_string(frontIndex+1));
  return queueArray[frontIndex];
}

template<typename T>
template<typename... Args>
void Queue<T>::emplace(Args&&... args)
{
    if (isFull()) resize();
    
    new (&queueArray[backIndex]) T(std::forward<Args>(args)...);
    backIndex++;
    if (backIndex >= capacity) backIndex = 0;
    size++;
}

template<typename T>
void Queue<T>::resize()
{
  // 1. 기존 크기의 2배 공간을 할당한다
  size_t newCapacity {capacity*2};
  T* newQueueArray {nullptr};
  newQueueArray = alloc.allocate(newCapacity);

  // 2. front부터 back요소까지 새로운 공간에 모두 연속적으로 배치한다
  int i=0;
  int fIndex = frontIndex;
  try
  {    
    for (; i<size; ++i)
    {
      // move_if_noexcept를 사용하여 이동 생성자가 호출되도록 구현
      new (newQueueArray+i) T(std::move_if_noexcept(queueArray[fIndex]));
      fIndex++;
      if (fIndex >= capacity) fIndex = 0;
    }
  }
  catch(const std::exception& e)
  {
    // try 코드에서 생성한 객체만 제거 후 Queue 메모리 공간 회수
    for(--i; i>=0; --i)
    {
      std::destroy_at(&newQueueArray[i]);
    }
    alloc.deallocate(newQueueArray, newCapacity);
    throw;
  }

  // 3. 기존 메모리에서 유효한 객체만 소멸 후 메모리 회수
  fIndex=frontIndex;
  for (int i=0; i<size; ++i)
  {
    std::destroy_at(&queueArray[fIndex]);
    fIndex++;
    if (fIndex >= capacity) fIndex = 0;
  }
  alloc.deallocate(queueArray, capacity);

  queueArray = newQueueArray;
  capacity = newCapacity;
  frontIndex = 0;
  backIndex = size;
}

