#include <iostream>
#include <memory>
#include <stdexcept>

template<typename T>
class Stack
{
  private:
    int top = -1;
    int size;
    std::unique_ptr<T[]> stackArray;  

  private:
    // noexcept는 예외(throw)를 반환하지 않는다는 명시를 의미
    bool isEmpty() noexcept;
    bool isFull() noexcept; 

  public:
    Stack(int size);

    void push(T value);
    T pop();
    T peek();
};

// 생성자에서 size를 설정하고, size만큼 T타입의 동적 배열을 unique_ptr로 정의함
template<typename T> 
Stack<T>::Stack(int size): size(size){ stackArray = std::make_unique<T[]>(this->size); }

// Stack이 비어있으면 true
template<typename T>
bool Stack<T>::isEmpty() noexcept { return top == -1; }

// Stack이 꽉차있으면 true
template<typename T>
bool Stack<T>::isFull() noexcept { return top >= size-1; }

// 공간을 확인 후 삽입
template<typename T>
void Stack<T>::push(T value)
{
  if (!isFull()) stackArray[++top] = value;
  else throw std::out_of_range("Error Push, Index Out of Bounds: " + std::to_string(top));
}

// 공간을 확인 후 제거
template<typename T>
T Stack<T>::pop()
{ 
  // std::unique_prt 스마트 포인터는 `delete`로 하나의 요소만 해제할 수 없으므로 move로 이동한 후 기본값으로 대체하는 방법
  if (!isEmpty()) 
  {
    // `std::move`의 경우 런타임에서 발생하는 것이 아닌 컴파일 과정에서 발생함
    // 간단하게 전체가 이동하는게 아닌 포인터 주소만 이동한 후 이동 생성자에서 초기화가 발생 
    T popValue = std::move(stackArray[top]);
    stackArray[top--] = T();
    return popValue;
  }
  else throw std::out_of_range("Error pop, Index Out of Bounds: " + std::to_string(top));
}

// 예외 상황은 throw로 반환하여 스택 풀기가 발생하여 main함수에서 catch로 처리됨
template<typename T>
T Stack<T>::peek()
{
  if (!isEmpty()) return stackArray[top];
  else throw std::out_of_range("Error peek, Index Out of Bounds: " + std::to_string(top));
}


int main() 
{
  Stack<int> stack(10); 
 
  try
  {
    for(int i=1; i<10; ++i)
    {
      stack.push(i);
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