#pragma once

#include <iostream>
#include <memory>
#include "../../Vector/inc/Vector.hpp"


// 0-based index 기준
// 부모 (i - 1) / 2 
// 왼쪽 자식  2*i + 1 
// 오른쪽 자식 2*i + 2

template <typename T>
class Heap
{

private:
  Vector<T> vector;
  T* rootNode = nullptr;

public:
  //void push(T&& data);
  void push(const T& data);
  void pop();


  void printHeap()
  {
    std::cout << "Heap Class List("<< vector.size() <<"): ";
    for (size_t i = 0; i<vector.size(); ++i)
      std::cout << vector[i] << " → ";
    std::cout << '\n';
  }
};


template <typename T>
void Heap<T>::push(const T& data)
{
  /*
  - 삽입 시 항상 자식 노드가 숫자가 더 커야한다
  - 맨 아래층에서 왼쪽 부터 삽입한다

  1. 일단 vector에 삽입한다 (알아서 resize함)
  2. 이후 부모와 값을 비교하여 교체한다
  3. 교체한 후 부모의 값보다 작은 경우에만 교체를 멈춘다
  */

  vector.push_back(data);
  
  size_t currentIndex = vector.size()-1;
  size_t parantIndex = (currentIndex - 1) / 2;

  // 부모보다 값이 큰 경우
  while(currentIndex > 0 && vector[currentIndex] < vector[parantIndex])
  {
    T temp = std::move(vector[currentIndex]);
    vector[currentIndex] = std::move(vector[parantIndex]);
    vector[parantIndex] = std::move(temp);
    
    currentIndex = parantIndex;
    parantIndex = (currentIndex - 1) / 2;
  }
  
}

template <typename T>
void Heap<T>::pop()
{
  if (vector.size() == 0) return;
  if (vector.size() == 1) {
    vector.pop_back();
    return;
  }

  /*
  1. 가장 위에 있는 노드를 제거한다
  2. 마지막 레벨에 가장 우측 노드를 root로 설정한다
  3. root의 자식과 값을 비교하여 교체한다
  */

  // root 노드와 마지막 노드를 교체한 후 제거
  T temp = std::move(vector.back()); ///< 포인터 타입을 저장하는 경우 메모리 누수 발생..?
  vector.back() = std::move(vector.front());
  vector.front() = std::move(temp);
  vector.pop_back();

  size_t currentIndex = 0;
  size_t l_child = 2 * currentIndex + 1;
  size_t r_child = 2 * currentIndex + 2;
  size_t min_child;
  size_t vSize = vector.size()-1;

  if (vSize == currentIndex)
    return;
  else if (vSize < r_child)
    min_child = l_child;
  else
    min_child = (vector[l_child] < vector[r_child]) ? l_child : r_child;

  // 부모 노드가 자식 노드보다 큰 경우
  while (vector[currentIndex] > vector[min_child] )
  {
    // 부모 노드와 교체
    temp = std::move(vector[min_child]);
    vector[min_child] = std::move(vector[currentIndex]);
    vector[currentIndex] = std::move(temp);

    currentIndex = min_child;
    l_child = 2 * currentIndex + 1;
    r_child = 2 * currentIndex + 2;
    
    // 두 자식 중 가장 작은 노드를 구함

    ///****여기에 문제 있음 집에서 수정 */
    if (vSize == currentIndex)
      continue;
    else if (vSize < r_child)
      min_child = l_child;
    else
      min_child = (vector[l_child] < vector[r_child]) ? l_child : r_child;
  }


}