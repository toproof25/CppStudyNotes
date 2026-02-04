/**
 * @file Heap.hpp
 * @date 2026-02-03
 * @author toproof(kmnlmn123@gmail.com)
 * @brief
 * - 이전에 구현한 Vector Class를 이용하여 Heap Class의 요소 저장을 관리하여 Zero-Overhead 원칙을 준수
 * - Vector Class에서 생성과 소멸의 책임을 가져 RAII와 예외 안정성을 보장함
 * - 배열 구조로 Heap을 구현하여 오버헤드가 적고 빠근 접근이 가능하다
 * 
 */

#pragma once

#include <iostream>
#include <memory>
#include "../../Vector/inc/Vector.hpp"

/**
 * @brief Heap 구조로 저장되는 Class
 * @tparam T 저장되는 객체의 타입
 * @tparam _g 0이면 최소값이 우선순위, 아닌 경우 최대값이 우선순위
 */
template <typename T, int _g = 0>
class Heap
{

private:
  Vector<T> vector;

public:

  /**
   * @brief Heap에 우선순위를 고려하여 data를 삽입합니다
   * @param data 저장할 객체
   * @note 
   * 1. Vector 맨 끝에 data를 삽입합니다
   * 2. 해당 위치부터 부모와 값을 비교하여 swap을 반복합니다
   * 3. root이거나 더 이상 부모보다 우선순위가 아닐 경우 종료합니다
   */
  void push(T&& data);
  void push(const T& data);

  /**
   * @brief 가장 높은 우선순위 요소를 제거하며, 내부 요소의 우선순위를 재정렬합니다
   * @note 
   * 1. 최상단 우선순위 요소를 제거합니다
   * 2. 마지막 요소를 root로 만든다.
   * 2. root 요소부터 자식들 값과 우선순위를 비교하여 swap을 반복합니다
   * 3. 마지막 index거나 자식보다 우선순위가 낮은 경우 종료합니다
   */
  void pop();
  
  T& top() { return vector.front(); }
  const T& top() const { return vector.front(); }

  size_t size() { return vector.size(); }
  bool empty() { return vector.size() == 0; }

  
  /** @brief 테스트용 출력 함수 */
  void printHeap()
  {
    std::cout << "Heap Class List("<< vector.size() <<"): ";
    for (size_t i = 0; i<vector.size(); ++i)
      std::cout << vector[i] << " → ";
    std::cout << '\n';
  }
};

template <typename T, int _g>
void Heap<T, _g>::push(T&& data)
{
  vector.push_back(std::forward<T>(data));
  
  size_t currentIndex = vector.size()-1;
  size_t parantIndex = (currentIndex - 1) / 2;

  if constexpr (_g == 0)
  {
    while(currentIndex > 0 && vector[currentIndex] <= vector[parantIndex])
    {
      T temp = std::move(vector[currentIndex]);
      vector[currentIndex] = std::move(vector[parantIndex]);
      vector[parantIndex] = std::move(temp);
      
      currentIndex = parantIndex;
      parantIndex = (currentIndex - 1) / 2;
    }
  }
  else
  {
    while(currentIndex > 0 && vector[currentIndex] >= vector[parantIndex])
    {
      T temp = std::move(vector[currentIndex]);
      vector[currentIndex] = std::move(vector[parantIndex]);
      vector[parantIndex] = std::move(temp);
      
      currentIndex = parantIndex;
      parantIndex = (currentIndex - 1) / 2;
    }
  }


}


template <typename T, int _g>
void Heap<T, _g>::push(const T& data)
{
  vector.push_back(data);
  
  size_t currentIndex = vector.size()-1;
  size_t parantIndex = (currentIndex - 1) / 2;

  if constexpr (_g == 0)
  {
    while(currentIndex > 0 && vector[currentIndex] <= vector[parantIndex])
    {
      T temp = std::move(vector[currentIndex]);
      vector[currentIndex] = std::move(vector[parantIndex]);
      vector[parantIndex] = std::move(temp);
      
      currentIndex = parantIndex;
      parantIndex = (currentIndex - 1) / 2;
    }
  }
  else
  {
    while(currentIndex > 0 && vector[currentIndex] >= vector[parantIndex])
    {
      T temp = std::move(vector[currentIndex]);
      vector[currentIndex] = std::move(vector[parantIndex]);
      vector[parantIndex] = std::move(temp);
      
      currentIndex = parantIndex;
      parantIndex = (currentIndex - 1) / 2;
    }
  }

}

template <typename T, int _g>
void Heap<T, _g>::pop()
{
  if (vector.size() == 0) return;
  if (vector.size() == 1) { vector.pop_back(); return; }

  size_t vSize = vector.size()-1;

  // 맨 앞 요소와 맨 뒤 요소 위치를 바꾼 후 맨 뒤 요소를 제거합니다
  T temp = std::move(vector[vSize]);
  vector[vSize] = std::move(vector[0]);
  vector[0] = std::move(temp);
  vector.pop_back();

  size_t currentIndex = 0;
  size_t l_child = 2 * currentIndex + 1;
  size_t r_child = 2 * currentIndex + 2;
  size_t min_child;
  vSize = vector.size()-1;


  if constexpr (_g == 0)
  {
    if (vSize < r_child)
      min_child = l_child;
    else
      min_child = (vector[l_child] <= vector[r_child]) ? l_child : r_child;

    while (vSize >= min_child && vector[currentIndex] >= vector[min_child] )
    {
      temp = std::move(vector[min_child]);
      vector[min_child] = std::move(vector[currentIndex]);
      vector[currentIndex] = std::move(temp);

      currentIndex = min_child;
      l_child = 2 * currentIndex + 1;
      r_child = 2 * currentIndex + 2;
      
      if (vSize < r_child)
        min_child = l_child;
      else
        min_child = (vector[l_child] <= vector[r_child]) ? l_child : r_child;
    }
  }
  else
  {
    if (vSize < r_child)
      min_child = l_child;
    else
      min_child = (vector[l_child] >= vector[r_child]) ? l_child : r_child;

    while (vSize >= min_child && vector[currentIndex] <= vector[min_child] )
    {
      temp = std::move(vector[min_child]);
      vector[min_child] = std::move(vector[currentIndex]);
      vector[currentIndex] = std::move(temp);

      currentIndex = min_child;
      l_child = 2 * currentIndex + 1;
      r_child = 2 * currentIndex + 2;
      
      if (vSize < r_child)
        min_child = l_child;
      else
        min_child = (vector[l_child] <= vector[r_child]) ? l_child : r_child;
    }
  }


}