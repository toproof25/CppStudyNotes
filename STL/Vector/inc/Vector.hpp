/**
 * @file Vector.hpp
 * @date 2026-01-14
 * @author toproof (kmnlmn123@gmail.com)
 * @brief Raw Memory를 이용하여 동적 크기 Vector Class 구현
 * @details 
 * 1. allocator를 이용하여 메모리 공간을 할당/재할당
 * 2. Placement New를 이용하여 메모리 공간에 객체를 생성
 * 3. 메모리 재할당에서 예외 안전성을 구현
 */

#pragma once

#include <memory>
#include <new>
#include <iostream>
#include <utility>
#include <type_traits>
#include <stdexcept>


/**
 * @brief Raw Memory로 객체를 관리하는 동적 Vector Class
 * @tparam T Vector가 담을 요소의 타입
 */
template <typename T>
class Vector
{
  using iterator = T*;
  using const_iterator = const T*;

  private:
    size_t _size = 0;
    size_t _capacity = 0;      
    T* vectorArray = nullptr;  ///< 실제 요소들이 저장된 곳을 가리키는 배열 포인터
    std::allocator<T> alloc;   

  private:
    bool isFull() { return (_size == _capacity); }
    bool isEmpty() { return (_size <= 0); }
    void resize();

  public:

    /** @brief 기본 변수값을 초기화하는 기본 생성자 */
    Vector() noexcept : _size(0), _capacity(0), vectorArray(nullptr) {}

    /** @brief Vector 내 모든 요소를 소멸하고 메모리 공간을 회수하는 소멸자 */
    ~Vector();

    /**
     * @brief 모든 요소를 복사하는 복사 생성자
     * @param other 복사될 Vector<T> 객체
     * @note 
     * 1. other의 크기를 복사
     * 2. other의 모든 요소를 복사 생성
     */
    Vector(const Vector<T>& other);

    /**
     * @brief 요소의 소유권을 이전하는 이동 생성자
     * @param other 소유권을 이전할 R-value로 이동 후 사용 불가한 객체
     * @note R-value의 소유권 이전으로 메모리 비용을 줄이고 효율적으로 이동하기 위해 작성
     */
    Vector(Vector<T>&& other) noexcept;

    /**
     * @brief Copy and Swap 방식으로 복사해오는 생성자
     * @param other 대입 연산자의 우측값으로 원본에 영향은 없습니다
     * @return 체인 작용을 위한 자신을 반환
     * @note Copy and Swap 방식으로 내부 변수를 swap하여 이동합니다
     */
    Vector<T>& operator=(Vector<T> other);


    T& operator[](size_t index) noexcept { return vectorArray[index]; }
    const T& operator[](size_t index) const noexcept { return vectorArray[index]; }

    T& at(size_t index);
    const T& at(size_t index) const;

    T& front() { return vectorArray[0]; }
    const T& front() const { return vectorArray[0]; }

    T& back() { return vectorArray[_size-1]; }
    const T& back() const { return vectorArray[_size-1]; }

    /**
     * @brief 메모리 재할당을 방지하기 위해 vector 메모리 크기를 미리 할당하는 함수입니다
     * @param newCapacity 할당할 메모리 크기 (capacity보다 작은 값은 무시합니다)
     * @throw 메모리 복사/이동 중 오류 발생 시 예외가 발생하며, 생성한 메모리 공간은 회수됩니다
     * @note T타입의 이동 생성자 예외 유무에 따라 복사 혹은 이동을 수행합니다
     */
    void reserve(size_t newCapacity);

    /**
     * @brief vector 마지막 위치에 요소를 추가하는 함수
     * @param value 추가할 요소의 값 (L-value값은 복사, R-value값은 이동)
     * @note 크기를 초과 시 재할당이 발생하고 O(n)의 시간이 소요될 수 있음
     */
    void push_back(const T& value);
    void push_back(T&& value);

    /**
     * @brief vector 객체의 마지막 요소를 제거하는 함수
     * @throw out_of_range (빈 공간에서 호출 시 예외 발생)
     * @warning vector 내 남은 요소를 확인한 후 호출
     */
    void pop_back();

    /**
     * @brief vector 객체의 마지막 위치에 생성자 인자로 객체를 생성하여 추가하는 함수
     * @param args 추가할 요소의 생성자 인자들
     * @note 크기를 초과 시 재할당이 발생하고 O(n)의 시간이 소요될 수 있음
     */
    template <typename ...Args> 
    void emplace_back(Args&&... args);

    /**
     * @brief capacit를 size에 맞게 줄입니다
     * @note size만큼 새로운 메모리에 재할당하여 capacity를 size에 맞춥니다
     */
    void shrink_to_fit();

    /** @brief 모든 요소를 제거하고, _size를 0으로 만드는 함수입니다 */
    void clear();

    /**
     * @brief 특정 위치에 요소의 인자를 받아 객체를 생성합니다
     * @param _ptr 요소를 삽입할 위치
     * @param args 생성자 인자들
     * @note 모든 요소를 소멸하고 size가 0으로 됨
     */
    template <typename ...Args> 
    void emplace(iterator _ptr, Args&&... args);

    /**
     * @brief 특정 위치에 객체를 삽입합니다
     * @param _ptr 요소를 삽입할 위치에 대한 Iterator(포인터) 변수
     * @param value 삽입할 객체 (복사, 이동)
     * @note 마지막 삽입의 경우 기존 push_back과 동일하게 동작합니다
     */
    void insert(iterator _ptr, const T& value);
    void insert(iterator _ptr, T&& value);

    /**
     * @brief 특정 위치에 객체를 제거합니다
     * @param _ptr 제거할 요소 위치에 대한 Iterator(포인터) 변수
     * @details 마지막 요소의 제거인 경우 기존 pop_back과 동일하게 동작합니다
     */
    void erase(iterator _ptr);

    iterator begin() { return vectorArray; }
    iterator end() { return vectorArray + _size; }
    const_iterator begin() const { return vectorArray; }
    const_iterator end() const { return vectorArray + _size; }
    const_iterator cbegin() const { return vectorArray; }
    const_iterator cend() const { return vectorArray + _size; }

    size_t size() { return _size; }
    size_t capacity() { return _capacity; }
};

template <typename T>
Vector<T>::~Vector()
{
  for (size_t i=0; i<_size; ++i)
    std::destroy_at(vectorArray+i);

  if (vectorArray != nullptr)
    alloc.deallocate(vectorArray, _capacity);
}

template <typename T>
Vector<T>::Vector(const Vector<T>& other) :
_capacity(other._capacity),
_size(other._size)
{
  vectorArray = alloc.allocate(_capacity);

  try
  {
    // 이동이 가능하면 이동, 아니면 복사
    if constexpr (std::is_nothrow_move_constructible<T>::value)
      std::uninitialized_move(other.begin(), other.end(), vectorArray);
    else
      std::uninitialized_copy(other.begin(), other.end(), vectorArray);
  }
  catch(const std::exception& e)
  {
    alloc.deallocate(vectorArray, _capacity);
    std::cerr << e.what() << '\n';
    throw;
  }  
}

template <typename T>
Vector<T>::Vector(Vector<T>&& other) noexcept : 
  vectorArray(other.vectorArray),
  _size(other._size),
  _capacity(other._capacity)
{
  other.vectorArray = nullptr;
  other._size = 0;
  other._capacity = 0;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T> other)
{
  std::swap(vectorArray, other.vectorArray);
  std::swap(_size, other._size);
  std::swap(_capacity, other._capacity);
  return *this;
}


template <typename T>
T& Vector<T>::at(size_t index)
{
  if (index >= _size) throw std::out_of_range("index 범위가 배열의 범위를 벗어났습니다");
  return vectorArray[index];
}

template <typename T>
const T& Vector<T>::at(size_t index) const
{
  if (index >= _size) throw std::out_of_range("index 범위가 배열의 범위를 벗어났습니다");
  return vectorArray[index];
}

template <typename T>
void Vector<T>::reserve(size_t newCapacity)
{
  if (_capacity >= newCapacity) return;

  T* newArray = alloc.allocate(newCapacity);

  try
  {
    // 이동이 가능하면 이동, 아니면 복사
    if constexpr (std::is_nothrow_move_constructible<T>::value)
      std::uninitialized_move(begin(), end(), newArray);
    else
      std::uninitialized_copy(begin(), end(), newArray);
  }
  catch(const std::exception& e)
  {
    alloc.deallocate(newArray, newCapacity);
    throw;
  }
  
  std::destroy(begin(), end());
  if (vectorArray != nullptr)
    alloc.deallocate(vectorArray, _capacity);

  _capacity = newCapacity;
  vectorArray = newArray;

}

template <typename T>
void Vector<T>::push_back(const T& value)
{
  if (isFull()) resize();
  new (vectorArray+_size) T(value);
  _size++;
}

template <typename T>
void Vector<T>::push_back(T&& value)
{
  if (isFull()) resize();
  new (vectorArray+_size) T(std::move(value));
  _size++;
}

template <typename T>
void Vector<T>::pop_back()
{
  if (isEmpty()) throw std::out_of_range("빈 공간에서 pop을 할 수 없습니다");
  std::destroy_at(vectorArray+_size-1);
  _size--;
}

template <typename T>
template <typename ...Args> 
void Vector<T>::emplace_back(Args&&... args)
{
  if (isFull()) resize();
  new (vectorArray+_size) T(std::forward<Args>(args)...);
  _size++;
}

template <typename T>
void Vector<T>::shrink_to_fit()
{
  size_t newCapacity = _size;
  T* newArray = alloc.allocate(newCapacity);

  // 새로운 메모리 공간에 기존 값을 복사/이동
  try
  {
    std::uninitialized_move(begin(), begin()+_size, newArray);
  }
  catch(const std::exception& e)
  {
    alloc.deallocate(newArray, newCapacity);
    throw;
  }
  
  std::destroy(begin(), begin()+_size);
  alloc.deallocate(vectorArray, _capacity);
  _capacity = newCapacity;
  vectorArray = newArray;
}

template <typename T>
void Vector<T>::clear()
{
  std::destroy(begin(), begin()+_size);
  _size = 0;
}

template <typename T>
template <typename ...Args> 
void Vector<T>::emplace(iterator _ptr, Args&&... args)
{
  if(isFull()) 
  {
    // resize 발생 시 반복자 무효화가 발생하기에 _ptr 위치를 임시 저장 후 resize가 된 후에 반영
    size_t temp = _ptr - begin();
    resize();
    _ptr = begin() + temp;
  }

  if (_ptr == end())
  {
    new (end()) T(std::forward<Args>(args)...);
    _size++;
    return;
  }

  // 정의되지 않은 공간 - 맨 뒷 공간에 객체 생성
  iterator it = end();
  new (it) T(std::move(*(it-1)));
  --it;

  // 정의된 공간 - _ptr ~ it-1 까지 뒤로 밀기
  std::move_backward(_ptr, it, end());

  // 객체 생성 후 공간에 할당
  *_ptr = T(std::forward<Args>(args)...);
  _size++;
}

template <typename T>
void Vector<T>::insert(iterator _ptr, const T& value)
{
  if(isFull()) 
  {
    // resize 발생 시 반복자 무효화가 발생하기에 _ptr 위치를 임시 저장 후 resize가 된 후에 반영
    size_t temp = _ptr - begin();
    resize();
    _ptr = begin() + temp;
  }

  if (_ptr == end())
  {
    new (end()) T(std::move(value));
    _size++;
    return;
  }

  // 정의되지 않은 공간 - 맨 뒷 공간에 객체 생성
  iterator it = end();
  new (it) T(std::move(*(it-1)));
  --it;

  // 정의된 공간 - _ptr ~ it-1 까지 뒤로 밀기
  std::move_backward(_ptr, it, end());

  // 삽입할 공간에 value를 복사
  *_ptr = value;
  _size++;
}

template <typename T>
void Vector<T>::insert(iterator _ptr, T&& value)
{
  // resize 발생 시 반복자 무효화가 발생하여 _ptr 위치에 값이 제대로 설정되지 않음
  if(isFull()) 
  {
    size_t temp = _ptr - begin();
    resize();
    _ptr = begin() + temp;
  }

  if (_ptr == end())
  {
    new (end()) T(std::move(value));
    _size++;
    return;
  }

  // 정의되지 않은 공간 - 맨 뒷 공간에 객체 생성
  iterator it = end();
  new (it) T(std::move(*(it-1)));
  --it;

  // 정의된 공간 - _ptr ~ it-1 까지 뒤로 밀기
  std::move_backward(_ptr, it, end());

  // 삽입할 공간에 value를 이동
  *_ptr = std::move(value);
  _size++;
}

template <typename T>
void Vector<T>::erase(iterator _ptr)
{
  if(isEmpty()) throw std::out_of_range("Error erase: 소멸할 객체가 없습니다");

  if(_ptr == end()-1)
  {
    std::destroy_at(end()-1);
    _size--;
    return;
  }

  iterator it = end()-1;
  for (; _ptr != it; ++_ptr)
  {
    *_ptr = std::move(*(_ptr+1));
  }

  std::destroy_at(end()-1);
  _size--;
}

template <typename T>
void Vector<T>::resize()
{
  // 1. capacity 크기를 2배로 증가 (기본 상태에서는 2의 거듭제곱)
  size_t newCapacity;
  if (_capacity == 0) newCapacity = 1; /// 크기가 0이라면 1로 설정
  else newCapacity = _capacity << 1;

  // 2. 새로운 크기만큼 메모리 공간을 할당
  T* newArray = alloc.allocate(newCapacity);

  // 3. 새로운 메모리 공간에 기존 값을 이동
  try
  {
    // 이동이 가능하면 이동, 아니면 복사
    if constexpr (std::is_nothrow_move_constructible<T>::value)
      std::uninitialized_move(begin(), end(), newArray);
    else
      std::uninitialized_copy(begin(), end(), newArray);
  }
  catch(const std::exception& e)
  {
    alloc.deallocate(newArray, newCapacity);
    throw;
  }
  
  // 4. 기존 메모리 공간을 회수
  std::destroy(begin(), end());
  if (vectorArray != nullptr) alloc.deallocate(vectorArray, _capacity);
  _capacity = newCapacity;
  vectorArray = newArray;
}

