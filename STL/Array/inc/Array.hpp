/**
 * @file Array.hpp
 * @author toproof (kmnlmn123@gmail.com)
 * @date 2026-01-07
 * @version 1.0.0
 * @brief STL Array와 유사한 클래스 제작 - Raw Memory를 이용하여 고정 크기 배열 (Array) 구현하기
 * @details 
 * 
 * ### version 1.0.0
 * - allocator와 Placement New 방식을 이용하여 Raw Memory를 이용한 배열 클래스
 * - `[]` 연산자 오버라이드를 적용하여 실제 사용하는 배열처럼 구현
 * - <array> 라이브러리의 기능을 분석한 후 각 기능들을 그대로 구현
 * 
 * ### version 2.0.0
 * - 실제 array의 경우 힙 기반 메모리가 아닌 스택 기반 메모리에 저장한다는 
 * 
 * 
 */

#include <new>
#include <memory>
#include <utility>

template <typename T, size_t capacity>
class Array
{
  private:
    T* array;
    std::allocator<T> alloc;

  public:
    Array();
    Array(const Array<T, capacity>& other);
    Array(Array<T, capacity>&& other);
    Array<T, capacity> operator=(Array<T, capacity> other);

    T& operator[](const int& index);
    const T& operator[](const int& index) const;

    T at(const int& index) const;

    T front();
    T back();

    void fill(T&& value);
    void fill(const T& value);

    size_t size();

    Array<T, capacity>& data() const;
    
    //bool empty();
};

template <typename T, size_t capacity>
Array<T, capacity>::Array()
{
  array = alloc.allocate(capacity);
}

template <typename T, size_t capacity>
Array<T, capacity>::Array(const Array<T, capacity>& other)
{
  // 1. 고정 크기만큼 메모리 공간을 할당
  array = alloc.allocate(capacity);

  // 2. other의 요소를 모두 복사
  int i=0;
  try
  {
    for (; i<capacity; ++i)
    {
      new (array+i) T(*(other.array+i));
    }
  }
  catch(const std::exception& e)
  {
    for(--i; i>=0; --i) std::destroy_at(array+i);
    alloc.deallocate(array, capacity);
    throw;
  }
  
}

template <typename T, size_t capacity>
Array<T, capacity>::Array(Array<T, capacity>&& other) : array(other.array)
{
  other.array = nullptr;
}

template <typename T, size_t capacity>
T& Array<T, capacity>::operator[](const int& index)
{
  return *(array+index);
}

template <typename T, size_t capacity>
const T& Array<T, capacity>::operator[](const int& index) const
{
  return *(array+index);
}

template <typename T, size_t capacity>
Array<T, capacity> Array<T, capacity>::operator=(Array<T, capacity> other)
{
  array = std::move(other);
  return *this;
}

template <typename T, size_t capacity>
T Array<T, capacity>::at(const int& index) const
{
  if (index < 0 || index >= capacity) throw std::out_of_range("배열 범위를 벗어났습니다...");
  return *(array+index);
}

template <typename T, size_t capacity>
T Array<T, capacity>::front() { return array[0]; }

template <typename T, size_t capacity>
T Array<T, capacity>::back() { return array[capacity-1]; }

template <typename T, size_t capacity>
void Array<T, capacity>::fill(T&& value)
{
  for (int i=0; i<capacity; ++i)
  {
    new (array+i) T(std::move(value));
  }
}

template <typename T, size_t capacity>
void Array<T, capacity>::fill(const T& value)
{
  for (int i=0; i<capacity; ++i)
  {
    new (array+i) T(value);
  }
}

template <typename T, size_t capacity>
size_t Array<T, capacity>::size()
{
  size_t size = capacity;
  return size;
}
