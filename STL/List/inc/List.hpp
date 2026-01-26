/**
 * @file List.hpp
 * @date 2026-01-20
 * @author toproof (kmnlmn123@gmail.com)
 * @brief Raw Memory를 이용한 Doubly Linked List 구조의 Class 구현
 * @note
 * 1. allocator, placement new를 이용하여 메모리 관리
 * 2. Doubly Linked List 구조로 앞, 뒤, 중간에 모든 요소를 삽입, 제거 가능
 * 3. iterator, const_iterator, reverse_iterator 구조체와 각 전위/후위, *, ==, != 연산자 오버라이드를 이용하여 타 STL 함수와 호환
 * 4. 기존 std::list와 같은 기능을 구현
 * 5. 복사 생성자에서 복사 시 오류가 발생하는 경우 복사한 모든 노드의 소멸과 메모리 공간 회수를 구현하여 예외 안전성을 높임
 * 
 * @todo 
 * - 유니폼 초기화 생성자 구현
 * - (size_t, T) 를 이용하여 초기화하는 생성자 구현
 * - resize(), reserve(), sort(), split() 함수 구현
 */

#pragma once

#include <memory>       
#include <new>          
#include <iostream>
#include <utility>      
#include <type_traits>  
#include <stdexcept>    
#include <iterator>


/**
 * @brief Doubly Linked List 클래스
 */
template <typename T>
class List
{
private:
  struct Node
  {
    T value;
    Node* next_node = nullptr; ///< 다음 노드를 가리키는 포인터
    Node* pre_node  = nullptr; ///< 이전 노드를 가리키는 포인터

    Node(const T& v) : value(v) {}
    Node(T&& v) : value(std::move(v)) {}

    template <typename... Args>
    Node(Args&& ...args) : value(std::forward<Args>(args)...) {}
  };

  size_t _size;
  Node* frontNode = nullptr; ///< List의 첫 노트를 가리키는 포인터
  Node* backNode  = nullptr; ///< List의 마지막 노드를 가리키는 포인터

public:

  template <typename _ptr, typename _ref>
  struct ListIterator
  {
    using difference_type = std::ptrdiff_t;           // 반복자 간 거리 타입
    using value_type = typename std::remove_reference<_ref>::type;  // 요소의 타입
    using pointer = value_type*;                      // 포인터 타입
    using reference = _ref;                           // 참조 타입
    using iterator_category = std::bidirectional_iterator_tag;  // 반복자 카테고리
    
    _ptr it;

    ListIterator(const _ptr& _it) : it(_it) {};

    inline ListIterator<_ptr, _ref>& operator++()   
    { 
      it = it->next_node; 
      return *this; 
    }
    inline ListIterator<_ptr, _ref> operator++(int) 
    { 
      ListIterator<_ptr, _ref> temp = *this; 
      ++*this; 
      return temp; }

    inline ListIterator<_ptr, _ref>& operator--()   
    { 
      it = it->pre_node; 
      return *this; 
    }
    inline ListIterator<_ptr, _ref> operator--(int) 
    { 
      ListIterator<_ptr, _ref> temp = *this; 
      --*this; 
      return temp; 
    }

    _ref operator*() { return it->value; }
    
    bool operator==(const ListIterator<_ptr, _ref>& other) { return this->it == other.it; }
    bool operator!=(const ListIterator<_ptr, _ref>& other) { return !(*this == other);    }
  };

  template <typename _ptr, typename _ref>
  struct ReverseListIterator
  {
    using difference_type = std::ptrdiff_t;           // 반복자 간 거리 타입
    using value_type = typename std::remove_reference<_ref>::type;  // 요소의 타입
    using pointer = value_type*;                      // 포인터 타입
    using reference = _ref;                           // 참조 타입
    using iterator_category = std::bidirectional_iterator_tag;  // 반복자 카테고리

    _ptr it;
    ReverseListIterator(const _ptr& _it) : it(_it) {};

    ReverseListIterator<_ptr, _ref>& operator++()    
    { 
      it = it->pre_node; 
      return *this; 
    }
    ReverseListIterator<_ptr, _ref> operator++(int) 
    { 
      ReverseListIterator<_ptr, _ref> temp = *this; 
      ++*this; 
      return temp; 
    }

    ReverseListIterator<_ptr, _ref>& operator--()    
    { it = it->next_node; 
      return *this; 
    }
    ReverseListIterator<_ptr, _ref> operator--(int) 
    { ReverseListIterator<_ptr, _ref> temp = *this; 
      --*this; return temp; 
    }

    _ref operator*() { return it->value; }
    
    bool operator==(const ReverseListIterator<_ptr, _ref>& other) { return this->it == other.it; }
    bool operator!=(const ReverseListIterator<_ptr, _ref>& other) { return !(*this == other); }
  };

  using iterator               = ListIterator<Node*, T&>;
  using const_iterator         = ListIterator<const Node*, const T&>;
  using reverse_iterator       = ReverseListIterator<Node*, T&>;
  using reverse_const_iterator = ReverseListIterator<const Node*, const T&>;

  iterator begin() { return iterator(frontNode); }
  iterator end()   { return iterator(nullptr);   }
  
  const_iterator cbegin() { return const_iterator(frontNode); }
  const_iterator cend()   { return const_iterator(nullptr);   }

  reverse_iterator rbegin() { return reverse_iterator(backNode); }
  reverse_iterator rend()   { return reverse_iterator(nullptr);  }
  
  reverse_const_iterator crbegin() { return reverse_const_iterator(backNode); }
  reverse_const_iterator crend()   { return reverse_const_iterator(nullptr);  }


public:
  List() : _size(0) {}
  ~List();

  /**
   * @brief List other 객체의 모든 요소를 순회하여 요소를 복사하는 복사 생성자입니다 
   * @param other 복사하고자 하는 List 객체
   * @note 복사 중 예외가 발생하는 경우 catch에서 생성했던 모든 노드를 소멸하고 메모리 공간을 회수하여 메모리 누수를 방지 
   */
  List(const List<T>& other);
  List(List<T>&& other) noexcept;

  /**
   * @brief =연산자를 통해 복사/이동을 수행하는 복사 대입 연산자
   * @param other 복사/이동하고자 하는 List 객체
   * @return 연쇄적으로 연산되도록 반환 복사/이동된 자신을 반환하여 
   * @note List<T> other는 일반적으로 복사 생성자가 호출되어 정의되나 std::move를 사용 시 이동 생성자가 호출됩니다
   * - `l = l2` -> 복사 생성자
   * - `l = std::move(l2)` -> 이동 생성자
   * 하나의 =연산자 오버라이드를 통해 복사와 이동의 이점을 누리는 복사 대입 연산자로 구현
   */
  List<T>& operator=(List<T> other);

  T& front()    { return frontNode->value; }
  T& back()     { return backNode->value;  }
  size_t size() const { return _size;        }
  bool empty() const  { return (_size == 0); }

  /**
   * @brief 특정 값을 맨 앞에 삽입하는 함수
   * @param value 삽입할 값
   */
  void push_front(const T& value);
  void push_front(T&& value);

  /**
   * @brief 특정 값을 맨 뒤에 삽입하는 함수
   * @param value 삽입할 값
   */
  void push_back(const T& value);
  void push_back(T&& value);

  /**
   * @brief 생정자 인자를 받아 내부에서 맨 앞에 객체를 생성하는 함수
   * @param args 타입의 생성자 인자들
   */
  template <typename... Args>
  void emplace_front(Args&& ...args);

  /**
   * @brief 생정자 인자를 받아 내부에서 맨 뒤에 객체를 생성하는 함수
   * @param args 타입의 생성자 인자들
   */
  template <typename... Args>
  void emplace_back(Args&& ...args);

  /**
   * @brief 맨 앞 요소를 제거하는 함수
   * @throw 빈 리스트에서 호출 시 out_of_range 예외를 던집니다
   * @warning 빈 리스트에서 호출할 시 예외가 발생합니다
   */
  void pop_front();

  /**
   * @brief 맨 뒤 요소를 제거하는 함수
   * @throw 빈 리스트에서 호출 시 out_of_range 예외를 던집니다
   * @warning 빈 리스트에서 호출할 시 예외가 발생합니다
   */
  void pop_back();

  /**
   * @brief 특정 위치에 객체를 삽입합니다
   * @param _iterator 요소를 삽입할 위치의 반복자 (해당 위치에 삽입됩니다)
   * @param value 삽입할 객체
   * @return 삽입된 위치의 반복자를 반환합니다
   * @note 삽입 위치가 맨 앞, 맨 뒤인 경우 push_front(), push_back()을 호출하여 삽입합니다
   */
  iterator insert(iterator _iterator, const T& value);
  iterator insert(iterator _iterator, T&& value);

  /**
   * @brief 생성자 인자를 받아 특정 위치에 객체를 삽입합니다
   * @param _iterator 요소를 삽입할 위치의 반복자 (해당 위치에 삽입됩니다)
   * @param args 삽입할 객체의 생성자 인자
   * @return 삽입된 위치의 반복자를 반환합니다
   * @note 삽입 위치가 맨 앞, 맨 뒤인 경우 emplace_front(), emplace_back()을 호출하여 삽입합니다
   */
  template <typename... Args>
  iterator emplace(iterator _iterator, Args&& ...args);

  /**
   * @brief 특정 위치의 객체를 제거합니다
   * @param _iterator 제거할 요소의 반복자
   * @return 제거된 위치의 다음 반복자를 반환합니다
   * @note 제거 위치가 맨 앞, 맨 뒤인 경우 pop_front(), pop_back()을 호출하여 제거합니다
   */
  iterator erase(iterator _iterator);

  /**
   * @brief 요소를 순회하며 특정 객체와 같은 객체를 제어합니다 
   * @param value 제거할 객체
   */
  void remove(const T& value);

  /**
   * @brief (테스트용 함수) 간단하게 List내 모든 요소 순회 출력하는 함수
   */
  void printList()
  {
    Node* current = frontNode;
    int i=1;
    std::cout << "size (" << size() << ") - ";
    while (current != nullptr)
    {
      std::cout << '[' << i << "] : " << current->value << (current != backNode ? " → " : ""); 
      current = current->next_node;
      i++;
    }
    std::cout << "\n"; 
  }
};


template <typename T>
List<T>::~List()
{
  if (frontNode == nullptr || backNode == nullptr) return;

  std::allocator<Node> alloc;

  Node* deleteNode = frontNode;
  while(deleteNode != nullptr)
  {
    frontNode = frontNode->next_node;

    std::destroy_at(deleteNode);
    alloc.deallocate(deleteNode, 1);
    
    deleteNode = frontNode;
  }
}


template <typename T>
List<T>::List(const List<T>& other) : _size(other._size)
{
  if (empty()) 
    return;

  std::allocator<Node> alloc;

  Node* start = other.frontNode;
  Node* end = other.backNode;

  Node* currentNode;
  Node* prevNode;

  try
  {
    frontNode = alloc.allocate(1);
    new (frontNode) Node(start->value);

    currentNode = frontNode;
    prevNode = frontNode;

    start = start->next_node;
    while (start != nullptr)
    {
      currentNode = alloc.allocate(1);
      new (currentNode) Node(start->value);

      currentNode->pre_node = prevNode;
      prevNode->next_node = currentNode;
      prevNode = currentNode;

      start = start->next_node;
    }
    backNode = currentNode;
  }
  catch(const std::exception& e)
  {
    currentNode = frontNode;
    while (currentNode != nullptr)
    {
      frontNode = frontNode->next_node;

      std::destroy_at(currentNode);
      alloc.deallocate(currentNode, 1);

      currentNode = frontNode;
    }
    
    throw;
  }
}

template <typename T>
List<T>::List(List<T>&& other) noexcept : frontNode(other.frontNode), backNode(other.backNode), _size(other._size)
{
  other.frontNode = nullptr;
  other.backNode  = nullptr;
  other._size = 0;
}

template <typename T>
List<T>& List<T>::operator=(List<T> other)
{
  std::swap(frontNode, other.frontNode);
  std::swap(backNode, other.backNode);
  std::swap(_size, other._size);
  return *this;
}


template <typename T>
void List<T>::push_front(const T& value)
{
  std::allocator<Node> alloc;

  if (frontNode == nullptr)
  {
    frontNode = alloc.allocate(1);
    new (frontNode) Node(value);
    backNode = frontNode;
    _size++;
    return;
  }

  Node* node = alloc.allocate(1);
  new (node) Node(value);

  node->next_node = frontNode;
  frontNode->pre_node = node;
  frontNode = node;

  _size++;
}
template <typename T>
void List<T>::push_front(T&& value)
{
  std::allocator<Node> alloc;

  if (frontNode == nullptr)
  {
    frontNode = alloc.allocate(1);
    new (frontNode) Node(std::move(value));
    backNode = frontNode;
    _size++;
    return;
  }

  Node* node = alloc.allocate(1);
  new (node) Node(std::move(value));

  node->next_node = frontNode;
  frontNode->pre_node = node;
  frontNode = node;

  _size++;
}


template <typename T>
void List<T>::push_back(const T& value)
{
  std::allocator<Node> alloc;

  if (backNode == nullptr)
  {
    frontNode = alloc.allocate(1);
    new (frontNode) Node(value);
    backNode = frontNode;
    _size++;
    return;
  }

  Node* node = alloc.allocate(1);
  new (node) Node(value);

  node->pre_node = backNode;
  backNode->next_node = node;
  backNode = node;

  _size++;
}
template <typename T>
void List<T>::push_back(T&& value)
{
  std::allocator<Node> alloc;

  if (backNode == nullptr)
  {
    frontNode = alloc.allocate(1);
    new (frontNode) Node(std::move(value));
    backNode = frontNode;
    _size++;
    return;
  }

  Node* node = alloc.allocate(1);
  new (node) Node(std::move(value));

  node->pre_node = backNode;
  backNode->next_node = node;
  backNode = node;
  
  _size++;
}


template <typename T>
template <typename... Args>
void List<T>::emplace_front(Args&& ...args)
{
  std::allocator<Node> alloc;

  if (frontNode == nullptr)
  {
    frontNode = alloc.allocate(1);
    new (frontNode) Node(std::forward<Args>(args)...);
    backNode = frontNode;
    _size++;
    return;
  }

  Node* node = alloc.allocate(1);
  new (node) Node(std::forward<Args>(args)...);

  node->next_node = frontNode;
  frontNode->pre_node = node;
  frontNode = node;

  _size++;
}

template <typename T>
template <typename... Args>
void List<T>::emplace_back(Args&& ...args)
{
  std::allocator<Node> alloc;

  if (backNode == nullptr)
  {
    frontNode = alloc.allocate(1);
    new (frontNode) Node(std::forward<Args>(args)...);
    backNode = frontNode;
    _size++;
    return;
  }

  Node* node = alloc.allocate(1);
  new (node) Node(std::forward<Args>(args)...);

  node->pre_node = backNode;
  backNode->next_node = node;
  backNode = node;
  
  _size++;
}


template <typename T>
void List<T>::pop_front()
{
  if (empty())
    throw std::out_of_range("Error pop_front(): 리스트 내 객체가 없습니다");

  std::allocator<Node> alloc;
  
  if (frontNode->next_node == nullptr)
  {
    std::destroy_at(frontNode);
    alloc.deallocate(frontNode, 1);
    frontNode = nullptr;
    backNode = nullptr;
    _size--;
    return;
  }

  Node* deleteNode;
  frontNode = frontNode->next_node;
  deleteNode = frontNode->pre_node;
  frontNode->pre_node = nullptr;

  std::destroy_at(deleteNode);
  alloc.deallocate(deleteNode, 1);

  _size--;
}

template <typename T>
void List<T>::pop_back()
{
  if (empty())
    throw std::out_of_range("Error pop_back(): 리스트 내 객체가 없습니다");

  std::allocator<Node> alloc;
  
  if (backNode->pre_node == nullptr)
  {
    std::destroy_at(backNode);
    alloc.deallocate(backNode, 1);
    frontNode = nullptr;
    backNode = nullptr;
    _size--;
    return;
  }

  Node* deleteNode;
  backNode = backNode->pre_node;
  deleteNode = backNode->next_node;
  backNode->next_node = nullptr;

  std::destroy_at(deleteNode);
  alloc.deallocate(deleteNode, 1);

  _size--;
}

template <typename T>
typename List<T>::iterator List<T>::insert(iterator _iterator, const T& value)
{
  if (frontNode == _iterator.it)
  {
    push_front(value);
    return iterator(frontNode);
  }
  
  if (end() == _iterator)
  {
    push_back(value);
    return iterator(backNode);
  }

  Node* currentNode = _iterator.it;
  Node* preNode = currentNode->pre_node;

  std::allocator<Node> alloc;
  Node* node = alloc.allocate(1);
  new (node) Node(value);

  preNode->next_node = node;

  node->pre_node = preNode;
  node->next_node = currentNode;

  currentNode->pre_node = node;
  _size++;

  return iterator(node);
}

template <typename T>
typename List<T>::iterator List<T>::insert(iterator _iterator, T&& value)
{
  if (frontNode == _iterator.it)
  {
    push_front(value);
    return iterator(frontNode);
  }
  
  if (end() == _iterator)
  {
    push_back(value);
    return iterator(backNode);
  }

  Node* currentNode = _iterator.it;
  Node* preNode = currentNode->pre_node;

  std::allocator<Node> alloc;
  Node* node = alloc.allocate(1);
  new (node) Node(std::move(value));

  preNode->next_node = node;

  node->pre_node = preNode;
  node->next_node = currentNode;

  currentNode->pre_node = node;
  _size++;

  return iterator(node);
}


template <typename T>
template <typename... Args>
typename List<T>::iterator List<T>::emplace(iterator _iterator, Args&& ...args)
{
  if (frontNode == _iterator.it)
  {
    emplace_front(std::forward<Args>(args)...);
    return iterator(frontNode);
  }
  
  if (end() == _iterator)
  {
    emplace_back(std::forward<Args>(args)...);
    return iterator(backNode);
  }

  Node* currentNode = _iterator.it;
  Node* preNode = currentNode->pre_node;

  std::allocator<Node> alloc;
  Node* node = alloc.allocate(1);
  new (node) Node(std::forward<Args>(args)...);

  preNode->next_node = node;

  node->pre_node = preNode;
  node->next_node = currentNode;

  currentNode->pre_node = node;
  _size++;

  return iterator(node);
}


template <typename T> 
typename List<T>::iterator List<T>::erase(iterator _iterator)
{
  if (empty()) throw std::out_of_range("Error erase(): 빈 리스트에서 제거할 수 없습니다");

  Node* deleteNode = _iterator.it;
  iterator nextIterator = ++_iterator;

  if (frontNode == deleteNode)
  {
    pop_front();
    return nextIterator;
  }

  if (deleteNode == backNode)
  {
    pop_back();
    return nextIterator;
  }

  Node* preNode = deleteNode->pre_node;
  Node* nextNode = deleteNode->next_node;

  preNode->next_node = nextNode;
  nextNode->pre_node = preNode;

  std::allocator<Node> alloc;
  std::destroy_at(deleteNode);
  alloc.deallocate(deleteNode, 1);

  _size--;

  return nextIterator;
}

template <typename T>
void List<T>::remove(const T& value)
{
  std::allocator<Node> alloc;

  Node* currentNode = frontNode;
  Node* preNode;
  Node* nextNode;

  while (currentNode != nullptr)
  {
    nextNode = currentNode->next_node;
    if (currentNode->value == value)
    {
        preNode  = currentNode->pre_node;

        if (preNode) preNode->next_node = nextNode;
        else frontNode = currentNode->next_node;

        if (nextNode) nextNode->pre_node = preNode;
        else backNode = currentNode->pre_node;

        std::destroy_at(currentNode);
        alloc.deallocate(currentNode, 1);
        _size--;
    }
    currentNode = nextNode;
  }
}
