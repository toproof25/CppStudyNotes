#include "Queue.hpp"
#include <iostream>

int main() 
{

  std::cout << "*********************Queue.h 테스트*********************" << '\n';

  Queue<int> queue(10); 
 
  try
  {
    for(int i=1; i<=10; ++i)
    {
      queue.emplace(i);
      std::cout << i << ". Queue push front: " << queue.front() << '\n';
    }
    std::cout << '\n';
    for(int i=1; i<=10; ++i)
    {
      std::cout << i << ". Queue pop front: " << queue.pop() << '\n';
    }
    std::cout << '\n' << '\n' << '\n';
    for (int i=1; i<101; ++i)
    {
      if (i%5==0)
      {
        std::cout << i << ". Queue test pop front: " << queue.pop() << '\n';
      }
      else
      {
        queue.push(i);
        std::cout << i << ". Queue test push front: " << queue.front() << '\n';
      }
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