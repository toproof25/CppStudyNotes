#include <iostream>
#include "../inc/Heap.hpp"

Heap<int> h;
Heap<int, 1> h2;

void Test_push()
{
  std::cout << "************************" << "< push Test >" << "************************" << '\n';
  h.printHeap();

  h.push(10);
  h.printHeap();

  h.push(6);
  h.printHeap();

  h.push(5);
  h.printHeap();

  h.push(2);
  h.printHeap();

  h.push(1);
  h.printHeap();

  h.push(7);
  h.printHeap();

  h.push(8);
  h.printHeap();

  h.push(9);
  h.printHeap();

  h.push(3);
  h.printHeap();

  h.push(4);
  h.printHeap();
  std::cout << '\n' << '\n';
}

void Test_pop()
{
  std::cout << "************************" << "< pop Test >" << "************************" << '\n';
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();

  h.pop();
  h.printHeap();
  std::cout << '\n' << '\n';
}

void Test_push_h2()
{
  std::cout << "************************" << "< push Test >" << "************************" << '\n';
  h2.printHeap();

  h2.push(1);
  h2.printHeap();

  h2.push(6);
  h2.printHeap();

  h2.push(5);
  h2.printHeap();

  h2.push(2);
  h2.printHeap();

  h2.push(10);
  h2.printHeap();

  h2.push(7);
  h2.printHeap();

  h2.push(8);
  h2.printHeap();

  h2.push(9);
  h2.printHeap();

  h2.push(3);
  h2.printHeap();

  h2.push(4);
  h2.printHeap();
  std::cout << '\n' << '\n';
}

void Test_pop_h2()
{
  std::cout << "************************" << "< pop Test >" << "************************" << '\n';
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();

  h2.pop();
  h2.printHeap();
  std::cout << '\n' << '\n';
}


int main()
{
  Test_push();
  Test_pop();

  Test_push_h2();
  Test_pop_h2();


  return 0;
}