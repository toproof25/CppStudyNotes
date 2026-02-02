/**
 * @file Array.hpp
 * @author toproof (kmnlmn123@gmail.com)
 * @date 2026-01-07
 * @version 3.0.0
 * @brief STL Array와 유사한 클래스 제작
 * @details 
 * 
 * ### version 1.0.0
 * - allocator와 Placement New 방식을 이용하여 Raw Memory를 이용한 배열 클래스
 * - `[]` 연산자 오버라이드를 적용하여 실제 사용하는 배열처럼 구현
 * - <array> 라이브러리의 기능을 분석한 후 각 기능들을 그대로 구현
 * 
 * ### version 2.0.0
 * - 실제 array의 경우 객체 내부에 인라인으로 작성됨. 즉 new, allocator가 아닌 int num[5] 이런 식으로 작성되어 이렇게 수정하기로 결정
 * - fill 함수는 1개로 고정 (value를 복사하여 모두 적용)
 * - 유니폼 초기화를 지원하기 위한 생성자 초기화 추가
 * 
 * ### version 3.0.0
 * - 요소 반환 함수에서 `const T& index` 와 `int index`의 속도 차이 - const T&의 경우 주소를 가지며 8바이트, 그냥 int는 4바이트로 기본 타입의 경우 복사가 더 유리
 * - 사실상 복사 생성자, 대입 생성자, 이동 생성자의 오버라이드가 의미가 없으므로 제거 -> Zero Overhead
 * - v2.0.0에서 구현한 생성자 초기화를 제거 -> 컴파일러가 집합체 초기화로 만들어서 동작함
 * - "특별한 이유가 없으면 생성자/소멸자/대입연산자를 아예 만들지 마라"는 원칙을 제일 잘 이해할 수 있었던 구현
 * - at, front, back T타입 반환 함수를 복사가 아닌 참조를 반환하도록 수정 T -> T&
 * - 반복자 호환?을 위해 begin, end 함수 구현
 * 
 */

#pragma once
#include <stdexcept>

template <typename T, size_t capacity>
class Array 
{
  public:
    using iterator = T*;

    T array[capacity];

    /**
     * @brief [] 연산자를 오버라이딩하여 요소를 반환합니다
     * @param index 배열의 특정 번째
     * @warning index의 범위를 계산하지 않아 범위에 주의
     */
    T& operator[](size_t index) noexcept;
    const T& operator[](size_t index) const noexcept;

    /**
     * @brief 배열의 index번째 요소를 참조 반환합니다
     * @param index 배열의 특정 번째를 의미
     * @throw 배열 범위에 벗어나는 index인 경우 out_of_range를 throw
     */
    T& at(size_t index);
    const T& at(size_t index) const;

    T& front() noexcept;
    T& back() noexcept;

    /**
     * @brief 배열의 모든 값을 value로 채웁니다
     * @param value 배열을 초기화할 값
     */
    void fill(const T& value) noexcept;

    size_t size() noexcept;

    // iterator 기능
    iterator begin() { return array; }
    iterator end() { return array+capacity; }
    
};

template <typename T, size_t capacity>
T& Array<T, capacity>::operator[](size_t index) noexcept { return array[index]; }

template <typename T, size_t capacity>
const T& Array<T, capacity>::operator[](size_t index) const noexcept { return array[index]; }

template <typename T, size_t capacity>
T& Array<T, capacity>::at(size_t index)
{
  if (index >= capacity) throw std::out_of_range("Error: out_of_range 배열 범위를 벗어났습니다...");
  return array[index];
}
template <typename T, size_t capacity>
const T& Array<T, capacity>::at(size_t index) const
{
  if (index >= capacity) throw std::out_of_range("Error: out_of_range 배열 범위를 벗어났습니다...");
  return array[index];
}

template <typename T, size_t capacity>
void Array<T, capacity>::fill(const T& value) noexcept
{
  for (int i=0; i<capacity; ++i)
    array[i] = value;
}

template <typename T, size_t capacity>
T& Array<T, capacity>::front() noexcept { return array[0]; }

template <typename T, size_t capacity>
T& Array<T, capacity>::back() noexcept { return array[capacity-1]; }

template <typename T, size_t capacity>
size_t Array<T, capacity>::size() noexcept { return capacity; }
