/**
 * @file List.hpp
 * @date 2026-01-18
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
    T node;
    Node* next_node = nullptr;
    Node* pre_node = nullptr;
    Node(T value) : node(value) {}
  };

private:
  size_t _capacity;

  Node* frontNode = nullptr;
  Node* backNode = nullptr;

public:
  // o
  List() : _capacity(0) {}
  ~List();

  List(const List<T>& other);
  List(List<T>&& other) noexcept;
  List<T>* operator=(List<T> other);

  T& front() { return frontNode->node; }
  T& back() { return backNode->node; }
  size_t size() { return _capacity; }

  // o
  void push_front(const T& value);
  void push_front(T&& value);

  // o
  void push_back(const T& value);
  void push_back(T&& value);

  // o
  template <typename ...Args>
  void emplcae_front(Args&& ...args);

  // o
  template <typename ...Args>
  void emplcae_back(Args&& ...args);

  void pop_front();
  void pop_back();

  void insert();
  void emplace();

  void erase();
  void remove();


  void printList()
  {
    Node* current = frontNode;
    int i=1;
    while (current != nullptr)
    {
      std::cout << '[' << i++ << "] : " << current->node << " → "; 
      current = current->next_node;
    }
    std::cout << "\n"; 
    //std::cout << '[' << i << "] : " << current->node << "\n"; 
  }
};

template <typename T>
List<T>::~List()
{
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
void List<T>::push_front(const T& value)
{
  std::allocator<Node> alloc;

  if (_capacity == 0)
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
    frontNode = node;
    
  }
  
  _capacity++;
}
template <typename T>
void List<T>::push_front(T&& value)
{
  std::allocator<Node> alloc;

  if (_capacity == 0)
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
    frontNode = node;
  }
  
  _capacity++;
}


template <typename T>
void List<T>::push_back(const T& value)
{
  std::allocator<Node> alloc;

  if (_capacity == 0)
  {
    frontNode = alloc.allocate(1);
    new (frontNode) Node(value);
    backNode = frontNode;
  }
  else
  {
    Node* node = alloc.allocate(1);
    new (node) Node(value);
    backNode->next_node = node;
    backNode = node;
  }
  
  _capacity++;
}
template <typename T>
void List<T>::push_back(T&& value)
{
  std::allocator<Node> alloc;

  if (_capacity == 0)
  {
    frontNode = alloc.allocate(1);
    new (frontNode) Node(std::move(value));
    backNode = frontNode;
  }
  else
  {
    Node* node = alloc.allocate(1);
    new (node) Node(std::move(value));
    backNode->next_node = node;
    backNode = node;
  }
  
  _capacity++;
}



// emplace 삽입
template <typename T>
template <typename ...Args>
void List<T>::emplcae_front(Args&& ...args)
{
  std::allocator<Node> alloc;

  if (_capacity == 0)
  {
    frontNode = alloc.allocate(1);
    new (frontNode) Node(std::forward<T>(args)...);
    backNode = frontNode;
  }
  else
  {
    Node* node = alloc.allocate(1);
    new (node) Node(std::forward<T>(args)...);

    node->next_node = frontNode;
    frontNode = node;
  }
  
  _capacity++;
}

template <typename T>
template <typename ...Args>
void List<T>::emplcae_back(Args&& ...args)
{
  std::allocator<Node> alloc;

  if (_capacity == 0)
  {
    frontNode = alloc.allocate(1);
    new (frontNode) Node(std::forward<T>(args)...);
    backNode = frontNode;
  }
  else
  {
    Node* node = alloc.allocate(1);
    new (node) Node(std::forward<T>(args)...);
    backNode->next_node = node;
    backNode = node;
  }
  
  _capacity++;
}


template <typename T>
void List<T>::pop_front()
{
  std::allocator<Node> alloc;
  
  Node* deleteNode = frontNode;
  frontNode = deleteNode->next_node;

  std::destroy_at(deleteNode);
  alloc.deallocate(deleteNode, 1);

  _capacity--;
}

template <typename T>
void List<T>::pop_back()
{
  std::allocator<Node> alloc;
  std::destroy_at(backNode);
  alloc.deallocate(backNode, 1);
  backNode = nullptr;

  _capacity--;
}