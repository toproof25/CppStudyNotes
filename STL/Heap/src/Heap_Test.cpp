#include <iostream>
#include "../inc/Heap.hpp"

Heap<int> h;

void Test_push()
{
  std::cout << "************************" << "< push Test >" << "************************" << '\n';
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

int main()
{
  Test_push();
  Test_pop();


  return 0;
}