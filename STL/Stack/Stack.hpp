#include <iostream>
#include <memory>
#include <stdexcept>
#include <new>
#include <utility>

/*

emplace를 제대로 구현해보기
*/

namespace ok 
{

template<typename T>
class Stack
{
  private:
    int capacity;
    int top = -1;
    //std::unique_ptr<T[]> stackArray;  
    T* stackArray;
    std::allocator<T> alloc;

  private:
    // noexcept는 예외(throw)를 반환하지 않는다는 명시를 의미
    bool isEmpty() noexcept;
    bool isFull() noexcept; 
    void resize();

  public:
    Stack(int size);

    // 이동 생성자
    Stack(Stack<T>&& other) noexcept;

    // 복사 생성자
    Stack(const Stack<T>& other);

    // 복사 대입 연산자
    Stack& operator=(Stack<T> other);

    ~Stack();

    int size();
    void push(const T& value);
    void push(T&& value);
    void pop();
    T& peek();

    // `typename... Args`는 받는 타입의 갯수가 일정하지 않을 때 여러 개를 받는 것을 의미한다
    template<typename... Args>
    void emplace(Args&&... args);

    template<typename... Args>
    void emplaceNotForward(Args&&... args);

};

}

// 이동 생성자 
template<typename T> 
ok::Stack<T>::Stack(Stack<T>&& other) noexcept 
    : stackArray(other.stackArray),  // 원본의 값들을 가져오기
      capacity(other.capacity),
      top(other.top)
{
    // 원본 값들을 초기화시킴 (안 그러면 원본 소멸 시 메모리가 해제됨)
    other.stackArray = nullptr;
    other.capacity = 0;
    other.top = -1;
}

// 복사 생성자 s = Stack(10)
template<typename T> 
ok::Stack<T>::Stack(const Stack<T>& other) : top(other.top), capacity(other.capacity)
{
  // 생성자는 내부에서 호출 불가
  //this->Stack(other.capacity);
  stackArray = this->alloc.allocate(this->capacity); 

  // i를 따로 정의하는 이유는 catch에서 i번째 까지 만든 객체를 다시 반납하기 위해
  int i=0;
  try
  {
    for (; i<=this->top; ++i)
    {
      new (&(this->stackArray[i])) T(other.stackArray[i]);
    }
  }
  catch(...)
  {
    std::cout << "복사 생성자 오류 발생" << '\n';
    for (int j=0; i<i; ++j) std::destroy_at(&this->stackArray[j]);
    this->alloc.deallocate(this->stackArray, this->capacity);
    throw;
  }
}

// 복사 대입 연산자 s=ss
// other을 복사해서 가져오든. 가져와서 복사를 하든 복사 후에 swap하기 (Copy-and-swap)
template<typename T> 
ok::Stack<T>& ok::Stack<T>::operator=(Stack<T> other)
{
  std::swap(this->stackArray, other.stackArray);
  std::swap(this->capacity, other.capacity);
  std::swap(this->top, other.top);
  return *this;
}



// 생성자에서 size를 설정하고, size만큼 T타입의 동적 배열을 unique_ptr로 정의함
template<typename T> 
ok::Stack<T>::Stack(int size): capacity(size)
{ 
  stackArray = alloc.allocate(capacity);
  //stackArray = std::make_unique<T[]>(this->size);
} 
template<typename T>
ok::Stack<T>::~Stack()
{
  // new로 할당하면 delete로 해제를 해야 하며, malloc은 free로
  // allocator.allocate로 빌리면 allocator.deallocate로 반납해야 한다.
  for (int i=0; i<=top; ++i)
  {
    std::destroy_at(&stackArray[i]);
  }

  if (stackArray != nullptr)
    alloc.deallocate(stackArray, capacity);
}


// 현재 사이즈 반환
template<typename T> 
int ok::Stack<T>::size()
{
  return top;
}

// Stack이 비어있으면 true
template<typename T>
bool ok::Stack<T>::isEmpty() noexcept { return top == -1; }

// Stack이 꽉차있으면 true
template<typename T>
bool ok::Stack<T>::isFull() noexcept { return top >= capacity-1; }

// L-value 공간을 확인 후 삽입
// L-value는 원본을 참조하는 value가 있으며, stackArray에 복사되어 저장이 된다
template<typename T>
void ok::Stack<T>::push(const T& value)
{
  if (isFull()) 
  {
    resize();
    //throw std::out_of_range("Error: push할 공간이 없습니다 -> " + std::to_string(top));
  }
  //stackArray[top] = value;

  // 객체 생성 후 top을 증가
  // &을 명시하여 주소값을 명확하게 전달함
  new (&stackArray[top+1]) T(value);
  top++;
}
// R-value 공간을 확인 후 삽입
// `&&`는 R-value를 의미하고, R-value는 임시 객체이기에 복사가 아닌 주소만 이동. 힙 영역에서 주소는 그대로고 stackArray에서 해당 주소를 포함한다
template<typename T>
void ok::Stack<T>::push(T&& value)
{
  if (isFull()) throw std::out_of_range("Error: push할 공간이 없습니다 -> " + std::to_string(top));
  //stackArray[top] = value;

  new (&stackArray[top+1]) T(std::move(value));
  top++;
}

// 공간을 확인 후 제거
template<typename T>
void ok::Stack<T>::pop()
{ 
  if (isEmpty()) throw std::out_of_range("Error pop, Index Out of Bounds: " + std::to_string(top));

  // std::unique_prt 스마트 포인터는 `delete`로 하나의 요소만 해제할 수 없으므로 move로 이동한 후 기본값으로 대체하는 방법
  // `std::move`의 경우 런타임에서 발생하는 것이 아닌 컴파일 과정에서 발생함
  // 간단하게 전체가 이동하는게 아닌 포인터 주소만 이동한 후 이동 생성자에서 초기화가 발생 
  //T popValue = std::move(stackArray[top]);

  // 현재 메모리 주소만 할당을 해제
  std::destroy_at(&stackArray[top]);
  top--;
}

// 예외 상황은 throw로 반환하여 스택 풀기가 발생하여 main함수에서 catch로 처리됨
template<typename T>
T& ok::Stack<T>::peek()
{
  if (isEmpty()) throw std::out_of_range("Error peek, Index Out of Bounds: " + std::to_string(top));
  return *stackArray[top];
}


// `typename... Args`는 받는 타입의 갯수가 일정하지 않을 때 여러 개를 받는 것을 의미한다
template<typename T>
template<typename... Args>
void ok::Stack<T>::emplace(Args&&... args)
{
    if (isFull()) 
    {
        throw std::out_of_range("Error emplace_push, Index Out of Bounds: " + std::to_string(top));
    }
    new (&stackArray[top+1]) T(std::forward<Args>(args)...);
    top++;

    // `std::forward<타입>()`의 경우 들어온 형태 그대로 넘겨주는 것을 의미
    // 여러 개의 요소로 이루어진 경우 `std::forward<Args>(args)...`에서 ...을 붙여서 합쳐진 요소를 풀어주는 것이 필요
    // 컴파일 단계에서 컴파일러가 이를 `T(std::forward<Args>(int), std::forward<Args>(double), std::forward<Args>(string));`으로 함수를 구현한다
    //stackArray[top] = T(std::forward<Args>(args)...);
    //top++;
}

template<typename T>
void ok::Stack<T>::resize()
{
  int reCapacity = capacity * 2;
  T* resizeArray = alloc.allocate(reCapacity);

  // 예외 처리를 통해 예외 발생 시 안전하게 할당한 메모리를 반납
  try
  {
    for (int i=0; i<=top; ++i)
    {
      // noexcept를 통해 예외를 발생하지 않음을 명시하는 move를 사용 -> 복사가 아닌 이동이 발생하기 위함
      // 예외를 발생시키면 복사를 사용함
      //new (&resizeArray[i]) T(std::move(stackArray[i]));
      new (&resizeArray[i]) T(std::move_if_noexcept(stackArray[i]));
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << "resize 실패 : " << e.what() << '\n';
    alloc.deallocate(resizeArray, reCapacity);
    throw;
  }
  
  for (int i=0; i<=top; ++i)
  {
    std::destroy_at(&stackArray[i]);
  }

  
  if (stackArray != nullptr)
    alloc.deallocate(stackArray, capacity);
  stackArray = resizeArray;
  capacity = reCapacity;
};