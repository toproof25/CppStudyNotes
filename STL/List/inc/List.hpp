/**
 * @file List.hpp
 * @date 2026-01-19
 * @author toproof (kmnlmn123@gmail.com)
 * @brief Raw Memory를 이용한 List 구조의 Class 구현
 */

#pragma once

#include <memory>       // allocator
#include <new>          // placement new
#include <iostream>
#include <utility>      // swap
#include <type_traits>  // type 체크
#include <stdexcept>    // 오류

template <typename T>
class List
{
private:
  struct Node
  {
    T value;
    Node* next_node = nullptr;
    Node* pre_node = nullptr;
    Node(T value) : value(value) {}
  };

  size_t _capacity;
  Node* frontNode = nullptr;
  Node* backNode = nullptr;

public:
  template <typename _ptr, typename _ref>
  struct ListIterator
  {
    _ptr it;

    ListIterator(_ptr _it) : it(_it) {};

    ListIterator<_ptr, _ref>& operator++()    { it = it->next_node; return *this; }
    ListIterator<_ptr, _ref> operator++(int) { ListIterator<_ptr, _ref> temp = *this; ++*this; return temp; }

    ListIterator<_ptr, _ref>& operator--()    { it = it->pre_node; return *this; }
    ListIterator<_ptr, _ref> operator--(int) { ListIterator<_ptr, _ref> temp = *this; --*this; return temp; }

    _ref operator*() { return it->value; }
    
    bool operator==(const ListIterator<_ptr, _ref>& other) { return this->it == other.it; }
    bool operator!=(const ListIterator<_ptr, _ref>& other) { return !(*this == other); }
  };


  template <typename _ptr, typename _ref>
  struct ReverseListIterator
  {
    _ptr it;
    ReverseListIterator(_ptr _it) : it(_it) {};

    ReverseListIterator<_ptr, _ref>& operator++()    { it = it->pre_node; return *this; }
    ReverseListIterator<_ptr, _ref> operator++(int) { ReverseListIterator<_ptr, _ref> temp = *this; ++*this; return temp; }

    ReverseListIterator<_ptr, _ref>& operator--()    { it = it->next_node; return *this; }
    ReverseListIterator<_ptr, _ref> operator--(int) { ReverseListIterator<_ptr, _ref> temp = *this; --*this; return temp; }

    _ref operator*() { return it->value; }
    
    bool operator==(const ReverseListIterator<_ptr, _ref>& other) { return this->it == other.it; }
    bool operator!=(const ReverseListIterator<_ptr, _ref>& other) { return !(*this == other); }
    bool operator>(const ReverseListIterator<_ptr, _ref>& other) { return this->it->value >= other->it->value; }
  };

  using iterator = ListIterator<Node*, T&>;
  using const_iterator = ListIterator<const Node*, const T&>;
  
  using reverse_iterator = ReverseListIterator<Node*, T&>;
  using reverse_const_iterator = ReverseListIterator<const Node*, const T&>;

  iterator begin() { return iterator(frontNode); }
  iterator end() { return iterator(backNode->next_node); }
  
  const_iterator cbegin() { return const_iterator(frontNode); }
  const_iterator cend() { return const_iterator(backNode->next_node); }

  reverse_iterator rbegin() { return reverse_iterator(backNode); }
  reverse_iterator rend() { return reverse_iterator(frontNode->pre_node); }
  
  reverse_const_iterator crbegin() { return reverse_const_iterator(backNode); }
  reverse_const_iterator crend() { return reverse_const_iterator(frontNode->pre_node); }


public:

  List() : _capacity(0) {}
  ~List();

  List(const List<T>& other);
  List(List<T>&& other) noexcept;
  List<T>* operator=(List<T> other);

  T& front() { return frontNode->value; }
  T& back() { return backNode->value; }
  size_t size() { return _capacity; }

  // o
  void push_front(const T& value);
  void push_front(T&& value);

  // o
  void push_back(const T& value);
  void push_back(T&& value);

  // o
  template <typename... Args>
  void emplace_front(Args&& ...args);

  // o
  template <typename... Args>
  void emplace_back(Args&& ...args);

  // o
  void pop_front();
  void pop_back();

  void insert(iterator& _iterator, const T& value);
  void insert(iterator& _iterator, T&& value);

  template <typename... Args>
  void emplace(iterator& _iterator, Args&& ...args);

  void erase(iterator& _iterator);

  void remove(const T& value);


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
  if (frontNode == nullptr)
  {
    return;
  }

  std::allocator<Node> alloc;

  Node* deleteNode = frontNode;
  while(deleteNode != backNode)
  {
    deleteNode = frontNode->next_node;
    std::destroy_at(frontNode);
    alloc.deallocate(frontNode, 1);
  }

  std::destroy_at(backNode);
  alloc.deallocate(backNode, 1);
}


template <typename T>
List<T>::List(const List<T>& other)
{
  std::allocator<Node> alloc;

  try
  {
    /* code */
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  
  
}

template <typename T>
List<T>::List(List<T>&& other) noexcept : frontNode(other.frontNode), backNode(other.backNode), _capacity(other._capacity)
{
  other.frontNode = nullptr;
  other.backNode = nullptr;
  other._capacity = 0;
}

template <typename T>
List<T>* List<T>::operator=(List<T> other)
{
  std::swap(frontNode, other.frontNode);
  std::swap(backNode, other.backNode);
  std::swap(_capacity, other._capacity);
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
  }
  else
  {
    Node* node = alloc.allocate(1);
    new (node) Node(value);

    node->next_node = frontNode;
    frontNode->pre_node = node;
    frontNode = node;
  }
  
  _capacity++;
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
  }
  else
  {
    Node* node = alloc.allocate(1);
    new (node) Node(std::move(value));

    node->next_node = frontNode;
    frontNode->pre_node = node;
    frontNode = node;
  }
  
  _capacity++;
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
  }
  else
  {
    Node* node = alloc.allocate(1);
    new (node) Node(value);
    node->pre_node = backNode;
    backNode->next_node = node;
    backNode = node;
  }
  
  _capacity++;
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
  }
  else
  {
    Node* node = alloc.allocate(1);
    new (node) Node(std::move(value));
    node->pre_node = backNode;
    backNode->next_node = node;
    backNode = node;
  }
  
  _capacity++;
}



// emplace 삽입
template <typename T>
template <typename... Args>
void List<T>::emplace_front(Args&& ...args)
{
  std::allocator<Node> alloc;

  if (frontNode == nullptr)
  {
    frontNode = alloc.allocate(1);
    new (frontNode) Node({std::forward<Args>(args)...});
    backNode = frontNode;
  }
  else
  {
    Node* node = alloc.allocate(1);
    new (node) Node({std::forward<Args>(args)...});

    node->next_node = frontNode;
    frontNode->pre_node = node;
    frontNode = node;
  }
  
  _capacity++;
}

template <typename T>
template <typename... Args>
void List<T>::emplace_back(Args&& ...args)
{
  std::allocator<Node> alloc;

  if (backNode == nullptr)
  {
    frontNode = alloc.allocate(1);
    new (frontNode) Node({std::forward<Args>(args)...});
    backNode = frontNode;
  }
  else
  {
    Node* node = alloc.allocate(1);
    new (node) Node({std::forward<Args>(args)...});
    node->pre_node = backNode;
    backNode->next_node = node;
    backNode = node;
  }
  
  _capacity++;
}


template <typename T>
void List<T>::pop_front()
{
  std::allocator<Node> alloc;
  
  if (frontNode->next_node == nullptr)
  {
    std::destroy_at(frontNode);
    alloc.deallocate(frontNode, 1);
    frontNode = nullptr;
    backNode = nullptr;
    _capacity--;
    return;
  }

  Node* deleteNode;
  frontNode = frontNode->next_node;
  deleteNode = frontNode->pre_node;
  frontNode->pre_node = nullptr;

  std::destroy_at(deleteNode);
  alloc.deallocate(deleteNode, 1);

  _capacity--;
}

template <typename T>
void List<T>::pop_back()
{
  std::allocator<Node> alloc;
  
  if (backNode->pre_node == nullptr)
  {
    std::destroy_at(backNode);
    alloc.deallocate(backNode, 1);
    frontNode = nullptr;
    backNode = nullptr;
    _capacity--;
    return;
  }

  Node* deleteNode;
  backNode = backNode->pre_node;
  deleteNode = backNode->next_node;
  backNode->next_node = nullptr;

  std::destroy_at(deleteNode);
  alloc.deallocate(deleteNode, 1);

  _capacity--;
}

template <typename T>
void List<T>::insert(iterator& _iterator, const T& value)
{
  // 첫번째 위치라면 push_front
  if (frontNode == _iterator.it)
  {
    push_front(value);
    return;
  }
  
  // 마지막 위치라면 push_back
  if (backNode == _iterator.it)
  {
    push_back(value);
    return;
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
  _capacity++;
}

template <typename T>
void List<T>::insert(iterator& _iterator, T&& value)
{
  // 첫번째 위치라면 push_front
  if (frontNode == _iterator.it)
  {
    push_front(value);
    return;
  }
  
  // 마지막 위치라면 push_back
  if (backNode == _iterator.it)
  {
    push_back(value);
    return;
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
  _capacity++;
}


template <typename T>
template <typename... Args>
void List<T>::emplace(iterator& _iterator, Args&& ...args)
{
  // 첫번째 위치라면 push_front
  if (frontNode == _iterator.it)
  {
    emplace_front(std::forward<Args>(args)...);
    return;
  }
  
  // 마지막 위치라면 push_back
  if (backNode == _iterator.it)
  {
    emplace_back(std::forward<Args>(args)...);
    return;
  }

  Node* currentNode = _iterator.it;
  Node* preNode = currentNode->pre_node;

  std::allocator<Node> alloc;
  Node* node = alloc.allocate(1);
  new (node) Node({std::forward<Args>(args)...});

  preNode->next_node = node;

  node->pre_node = preNode;
  node->next_node = currentNode;

  currentNode->pre_node = node;
  _capacity++;
}


template <typename T>
void List<T>::erase(iterator& _iterator)
{
  if (frontNode == _iterator.it)
  {
    pop_front();
    return;
  }

  if (backNode == _iterator.it)
  {
    pop_back();
    return;
  }

  Node* deleteNode = _iterator.it;

  Node* preNode = deleteNode->pre_node;
  Node* nextNode = deleteNode->next_node;

  preNode->next_node = nextNode;
  nextNode->pre_node = preNode;

  std::allocator<Node> alloc;
  std::destroy_at(deleteNode);
  alloc.deallocate(deleteNode, 1);

  _capacity--;
}

template <typename T>
void List<T>::remove(const T& value)
{
  Node* currentNode = frontNode;

  std::allocator<Node> alloc;

  Node* deleteNode;
  Node* preNode;
  Node* nextNode;

  while (currentNode != backNode->next_node)
  {
    if (currentNode->value == value)
    {
      if (currentNode == frontNode) 
        pop_front();
      else if (currentNode == backNode) 
        pop_back();
      else
      {
        deleteNode = currentNode;
        preNode = deleteNode->pre_node;
        nextNode = deleteNode->next_node;

        preNode->next_node = nextNode;
        nextNode->pre_node = preNode;

        std::destroy_at(deleteNode);
        alloc.deallocate(deleteNode, 1);
        _capacity--;
      }
    }

    currentNode = currentNode->next_node;
  }
}