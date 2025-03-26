/*
template로 범용성 있는 class 제작
*/
#include <iostream>
#include <windows.h>
using namespace std;

template<typename T>
class Stack
{
  public:
    // 생성자 소멸자
    Stack(int size);
    ~Stack();

    // 스택 기본 기능
    void push(T value);
    T pop();
    T peek();
    bool isEmpty();
    bool isFull();
    void print_stack();

  private:
    // 데이터 배열, 위치, 사이즈
    T *array;
    int top;
    int size;
};

// 생성자 구현 (템플릿 필요!)
template<typename T>
Stack<T>::Stack(int size) : size(size), top(-1) {
    array = new T[size]; 
}

// 소멸자 구현 (메모리 해제)
template<typename T>
Stack<T>::~Stack() {
    delete[] array;
}

// Stack push 구현
template<typename T>
void Stack<T>::push(T value)
{
  // 스택에 공간이 있으면 push
  if(!isFull())
  {
    array[++top] = value;
    cout << value << " push 완료" << endl;
  }
  else
  {
    cout << "공간이 부족하여 push를 할 수 없습니다" << endl;
  }
}
// Stack pop 구현
template<typename T>
T Stack<T>::pop()
{
  // 스택에서 pop을 할 데이터가 있으면 pop
  if(!isEmpty())
  {
    T pop_value = peek();
    array[top--] = T();
    cout << pop_value << " pop 완료" << endl;
    return pop_value;
  }
  cerr << "pop을 할 데이터가 없습니다" << endl;
  return T();  // 기본값 반환 (int -> 0, double -> 0.0, string -> "")
}
// Stack peek 구현
template<typename T>
T Stack<T>::peek()
{
  if (!isEmpty())
  {
    return array[top];
  }
  cout << "데이터가 존재하지 않습니다" << endl;
  return T();
}
// Stack isEmpty 구현
template<typename T>
bool Stack<T>::isEmpty()
{
  if (top == -1)
  {
    return true;
  }
  return false;
}
// Stack isFull 구현
template<typename T>
bool Stack<T>::isFull()
{
  if (top == size-1)
  {
    return true;
  }
  return false;
}
// Stack print_stack 구현 - 모든 데이터 출력
template<typename T>
void Stack<T>::print_stack()
{
  for (int i=0; i<=top; ++i)
  {
    cout << array[i] << " : ";
  }
  cout << endl;
}



int main()
{
  // int 형으로 Stack을 생성
  cout << "template<int> 로 클래스 생성" << endl;
  Stack<int> stack_int(10);

  stack_int.push(1);
  stack_int.push(22);
  stack_int.push(333);
  stack_int.push(4444);
  stack_int.push(55555);
  stack_int.push(666666);
  stack_int.push(55555);
  stack_int.push(4444);
  stack_int.push(333);
  stack_int.push(22);
  stack_int.push(1);

  stack_int.print_stack();

  cout << "stack_int 의 peek값 : " << stack_int.peek() << endl;

  cout << "stack_int pop : " << stack_int.pop() << endl;
  cout << "stack_int pop : " << stack_int.pop() << endl;
  cout << "stack_int pop : " << stack_int.pop() << endl;
  cout << "stack_int pop : " << stack_int.pop() << endl;
  cout << "stack_int pop : " << stack_int.pop() << endl;
  cout << "stack_int pop : " << stack_int.pop() << endl;
  stack_int.print_stack();
  

  std::cout << "\n\n";

  // string 형으로 Stack을 생성
  cout << "template<string> 로 클래스 생성" << endl;
  Stack<string> stack_string(5);

  stack_string.push("가");
  stack_string.push("나");
  stack_string.push("다");
  stack_string.push("라");
  stack_string.push("마");
  stack_string.push("바");

  stack_string.print_stack();

  cout << "stack_string 의 peek값 : " << stack_string.peek() << endl;

  cout << "stack_string pop : " << stack_string.pop() << endl;
  cout << "stack_string pop : " << stack_string.pop() << endl;
  cout << "stack_string pop : " << stack_string.pop() << endl;
  cout << "stack_string pop : " << stack_string.pop() << endl;
  cout << "stack_string pop : " << stack_string.pop() << endl;
  cout << "stack_string pop : " << stack_string.pop() << endl;
  stack_string.print_stack();


  


  std::cout << "\n\n";
  system("pause");
  return 0;
}


