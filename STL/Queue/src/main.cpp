#include "../inc/Queue.hpp"
#include <iostream>

int main() 
{

  std::cout << "*********************1. 원형 Queue push, pop 테스트*********************" << '\n';

  {
  Queue<int> queue(10); 
 
  try
  {
    for(int i=0; i<10; ++i)
    {
      queue.emplace(i);
      std::cout << i << ". Queue push front:  " << queue.front() << '\n';
    }
    std::cout << '\n';
    for(int i=0; i<9; ++i)
    {
      queue.pop();
      std::cout << i << ". Queue pop front: " << queue.front() << '\n';
    }
    std::cout << '\n' << '\n' << '\n';
    for (int i=1; i<101; ++i)
    {
      if (i%2==0)
      {
        queue.pop();
        std::cout << i << ". Queue test pop front: " << queue.front() << '\n';
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

  std::cout << "\n\n*********************2. 복사 테스트 *********************" << '\n';

  Queue<int> copy(5);
  copy.push(1);
  copy.push(12);
  copy.push(123);
  copy.push(1234);
  copy.push(12345);

  Queue<int> copy_queue(copy);
  while(!copy_queue.isEmpty())
  {
    std::cout << "copy_queue.pop " << copy_queue.front() << '\n';
    copy_queue.pop();
  }

  std::cout << "\n\n*********************3. 복사 대입 테스트 *********************" << '\n';

  Queue<int> sub_queue(0);
  sub_queue = copy;
  while(!sub_queue.isEmpty())
  {
    std::cout << "sub_queue.pop " << sub_queue.front() << '\n';
    sub_queue.pop();
  }

  std::cout << "\n\n*********************4. 이동 생성자 테스트 *********************" << '\n';
  
  Queue<int> move_queue(std::move(copy));
  std::cout << "move_queue.getSize : " << move_queue.getSize() << '\n';
  }
  
  return 0; 
}