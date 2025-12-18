#include <iostream>
#include <memory>
#include <stdexcept>

/*
STL의 컨테이너 어댑터 Queue는 기존 컨테이너를 바탕으로 Queue구조를 사용할 수 있도록 만든 헤더 파일
FIFO(First In First Out) 선입선출의 구조로 사용한다
추가 : push
제거 : pop
맨 앞 데이터 : front
비어있는지 확인 : empty
*/

template<typename T>
class Queue
{
  private:
    int frontIndex = 0;
    int backIndex = 0;
    int count = 0;
    int size;
    std::unique_ptr<T[]> queueArray;  

  private:
    bool isEmpty() noexcept;
    bool isFull() noexcept; 

  public:
    Queue(int size);

    void push(const T& value);
    void push(T&& value);
    
    // STL Queue pop은 반환값이 없음
    // T pop();
    void pop();
    
    const T& front();

    template<typename... Args>
    void emplace(Args&&... args);
};


// 생성자에서 size를 설정하고, size만큼 T타입의 동적 배열을 unique_ptr로 정의함
template<typename T> 
Queue<T>::Queue(int size): size(size){ queueArray = std::make_unique<T[]>(this->size); }

// Queue이 비어있으면 true
template<typename T>
bool Queue<T>::isEmpty() noexcept 
{ 
  if (count <= 0) return true;
  return false;
}

// Queue이 꽉차있으면 true
template<typename T>
bool Queue<T>::isFull() noexcept 
{ 
  if (count >= size) return true;
  return false;
}


template<typename T>
void Queue<T>::push(const T& value)
{
  if (isFull()) throw std::out_of_range("Error Push copy, Index Out of Bounds: " + std::to_string(backIndex));

  queueArray[backIndex] = value;
  backIndex = (backIndex+1) % size;
  count++;
}
template<typename T>
void Queue<T>::push(T&& value)
{
  if (isFull()) throw std::out_of_range("Error Push move, Index Out of Bounds: " + std::to_string(backIndex));

  queueArray[backIndex] = std::move(value);
  backIndex = (backIndex+1) % size;
  count++;
}

// // 공간을 확인 후 제거
// template<typename T>
// T Queue<T>::pop()
// { 
//   if (isEmpty()) throw std::out_of_range("Error pop, Index Out of Bounds: " + std::to_string(frontIndex));

//   T popValue = std::move(queueArray[frontIndex]);
//   queueArray[frontIndex].~T();
//   frontIndex = (frontIndex+1) % size;
//   count--;
//   return popValue;
// }

//공간을 확인 후 제거
template<typename T>
void Queue<T>::pop()
{ 
  if (isEmpty()) throw std::out_of_range("Error pop, Index Out of Bounds: " + std::to_string(frontIndex));

  queueArray[frontIndex].~T();
  frontIndex = (frontIndex+1) % size;
  count--;
}

// 예외 상황은 throw로 반환하여 스택 풀기가 발생하여 main함수에서 catch로 처리됨
template<typename T>
const T& Queue<T>::front()
{
  if (isEmpty()) throw std::out_of_range("Error front, Index Out of Bounds: " + std::to_string(frontIndex+1));
  return queueArray[(frontIndex+1) % size];
}

// `typename... Args`는 받는 타입의 갯수가 일정하지 않을 때 여러 개를 받는 것을 의미한다
template<typename T>
template<typename... Args>
void Queue<T>::emplace(Args&&... args)
{
    if (isFull()) throw std::out_of_range("Error emplace_push, Index Out of Bounds: " + std::to_string(backIndex));
    
    queueArray[backIndex] = T(std::forward<Args>(args)...);
    backIndex = (backIndex+1) % size;
    count++;
}
