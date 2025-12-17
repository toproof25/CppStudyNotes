#include "Stack.hpp"
#include <iostream>

int main() 
{

  int a = 4;
  int b = -1;
  std::cout << "*********************Stack.h 테스트*********************" << '\n';

  Stack<int> stack(10); 
 
  try
  {
    for(int i=1; i<10; ++i)
    {
      stack.emplace(i);
    }

    for(int i=1; i<20; ++i)
    {
      std::cout << "Stack peek: " << stack.peek() << '\n';
      stack.pop();
    }
  }
  catch(const std::out_of_range& e) 
  {
    std::cerr << "런타임 에러 발생: " << e.what() << '\n';
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

  return 0; 
}