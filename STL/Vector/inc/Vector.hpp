/**
 * @file Vector.hpp
 * @date 2026-01-13
 * @author toproof (kmnlmn123@gmail.com)
 * @brief Raw Memory Vector Class 구현
 */

#pragma ones

#include <memory>
#include <new>
#include <iostream>
#include <utility>


/**
 * @brief Raw Memory Vector Class
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
    T* vectorArray = nullptr;
    std::allocator<T> alloc;

  private:
    bool isFull() { return (_size == _capacity); }
    bool isEmpty() { return (_size <= 0); }
    void resize();

  public:
    /**
     * @brief 기본 생성자
     * @note 기본 크기 변수들 초기화
     */
    Vector() noexcept : _size(0), _capacity(0), vectorArray(nullptr) {}

    /**
     * @brief 소멸자
     * @note 모든 요소와 메모리 공간을 회수
     */
    ~Vector();

    /**
     * @brief 복사 생성자
     * @param other 복사될 Vector<T> 객체
     * @note 
     * 1. other의 크기를 복사
     * 2. other의 모든 요소를 복사 생성
     */
    Vector(const Vector<T>& other);

    /**
     * @brief 이동 생성자
     * @param other 이동할 R-value other 객체
     * @note other의 메모리 공간을 그대로 이전
     */
    Vector(Vector<T>&& other) noexcept;

    /**
     * @brief 복사 대입 생성자
     * @param other 대입 연산자의 우측값
     * @return 체인 작용을 위한 자신을 반환
     * @note 복사된 other의 내부 변수를 swap하여 이동
     */
    Vector<T>& operator=(Vector<T> other);

    /**
     * @brief [] 연산자 오버라이드
     * @param index 반환할 요소의 특정 위치
     * @return index 위치의 참조 값
     * @warning 범위에 대한 예외처리를 하지 않으므로 범위에 주의
     * @note index위치의 참조 값을 반환하여 수정, 읽기 전용 오버로딩
     */
    T& operator[](size_t index) noexcept { return vectorArray[index]; }
    const T& operator[](size_t index) const noexcept { return vectorArray[index]; }

    /**
     * @brief index 위치 요소를 참조 반환
     * @param index 반환할 요소의 특정 위치
     * @return index 위치의 참조 값
     * @throw out_of_range (index 범위에 따라 반환)
     * @note index위치의 참조 값을 반환하여 수정, 읽기 전용 오버로딩
     */
    T& at(size_t index);
    const T& at(size_t index) const;

    /**
     * @brief 첫번째 요소를 반환하는 함수입니다
     * @return vector 객체의 첫번째 요소를 반환합니다
     * @throw out_of_range (빈 공간에서 호출 시 예외 발생)
     * @note 수정과 읽기 변수로 오버로딩
     */
    T& front() { return vectorArray[0]; }
    const T& front() const { return vectorArray[0]; }

    /**
     * @brief 마지막 요소를 반환하는 함수입니다
     * @return vector 객체의 마지막 요소를 반환합니다
     * @throw out_of_range (빈 공간에서 호출 시 예외 발생)
     * @note 수정과 읽기 변수로 오버로딩
     */
    T& back() { return vectorArray[_size-1]; }
    const T& back() const { return vectorArray[_size-1]; }

    /**
     * @brief 객체의 크기를 미리 할당하는 함수
     * @param _capacity 할당할 크기
     */
    void reserve(size_t _capacity);

    /**
     * @brief vector 객체의 마지막에 요소를 추가하는 함수
     * @param value 추가할 요소의 값 (복사)
     * @warning 크기를 초과 시 재할당이 발생하고 O(n)의 시간이 소요될 수 있음
     */
    void push_back(const T& value);
    void push_back(T&& value);

    /**
     * @brief vector 객체의 마지막 요소를 제거하는 함수
     * @throw out_of_range (빈 공간에서 호출 시 예외 발생)
     */
    void pop_back();

    /**
     * @brief vector 객체의 마지막에 요소를 생성자 인자로 추가하는 함수
     * @param args 추가할 요소의 생성자 인자들
     * @warning 크기를 초과 시 재할당이 발생하고 O(n)의 시간이 소요될 수 있음
     */
    template <typename ...Args> 
    void emplace_back(Args&&... args);

    /**
     * @brief capacit를 size에 맞게 줄입니다
     * @note 메모리 재할당을 이용하여 capacity를 size에 맞춤
     */
    void shrink_to_fit();

    /**
     * @brief 모든 요소를 제거합니다
     * @note 모든 요소를 소멸하고 size가 0으로 됨
     */
    void clear();

    template <typename ...Args> 
    void emplace(iterator _ptr, Args&&... args);
    void insert(iterator _ptr, const T& value);
    void insert(iterator _ptr, T&& value);
    void erase(iterator _ptr);

    /**
     * @brief 반복자
     * @return 각 요소에 대한 반복자(포인터)를 반환
     */
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

  int i=0;
  try
  {
    for (size_t i=0; i<_size; ++i)
      new (vectorArray+i) T(other.vectorArray[i]);
  }
  catch(const std::exception& e)
  {
    for (; i>=0; --i)
      std::destroy_at(vectorArray[i]);
    alloc.deallocate(vectorArray, _capacity);
    std::cerr << e.what() << '\n';
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
  if (index < 0 || index >= _size) throw std::out_of_range("index 범위가 배열의 범위를 벗어났습니다");
  return vectorArray[index];
}

template <typename T>
const T& Vector<T>::at(size_t index) const
{
  if (index < 0 || index >= _size) throw std::out_of_range("index 범위가 배열의 범위를 벗어났습니다");
  return vectorArray[index];
}


template <typename T>
void Vector<T>::reserve(size_t newCapacity)
{
  if (_capacity >= newCapacity) return;

  T* newArray = alloc.allocate(newCapacity);

  try
  {
    std::uninitialized_move(begin(), end(), newArray);
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
  int i=0;
  try
  {
    for (; i<_size; ++i)
      new (newArray+i) T(std::move_if_noexcept(vectorArray[i]));
  }
  catch(const std::exception& e)
  {
    for (; i>=0; --i) std::destroy_at(newArray+i);
    alloc.deallocate(newArray, newCapacity);
    throw;
  }
  
  alloc.deallocate(vectorArray, _capacity);
  _capacity = newCapacity;
  vectorArray = newArray;
}

template <typename T>
void Vector<T>::clear()
{
  for (size_t i=0; i<_size; ++i)
    std::destroy_at(vectorArray+i);
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
    std::uninitialized_move(begin(), end(), newArray);
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

