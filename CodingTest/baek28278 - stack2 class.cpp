/*
백준 스택 2 (https://www.acmicpc.net/problem/28278)

정수를 저장하는 스택을 구현한 다음, 입력으로 주어지는 명령을 처리하는 프로그램을 작성하시오.

명령은 총 다섯 가지이다.

1 X: 정수 X를 스택에 넣는다. (1 ≤ X ≤ 100,000)
2: 스택에 정수가 있다면 맨 위의 정수를 빼고 출력한다. 없다면 -1을 대신 출력한다.
3: 스택에 들어있는 정수의 개수를 출력한다.
4: 스택이 비어있으면 1, 아니면 0을 출력한다.
5: 스택에 정수가 있다면 맨 위의 정수를 출력한다. 없다면 -1을 대신 출력한다.
*/
#include <iostream>
using namespace std;

class Stack
{
  public:
  
    // 생성자
    Stack(int N) : size(N)
    {
      array = new int[N];
    }

    // 소멸자
    ~Stack()
    {
      delete[] array;
    }

    // 값 삽입
    void push(int push_number)
    {
      if (!isFull()) 
      {
        array[++top] = push_number;
      }
    }

    // 값 제거 (최상단 먼저 제거)
    int pop()
    {
      if (!isEmpty())
      {
        int pop_value = array[top];
        array[top--] = 0;
        return pop_value;
      }
      return -1;
    }

    // 맨 위 값
    int peek()
    {
      return array[top];
    }

    // 비어있는지 확인
    bool isEmpty()
    {
      if (top == -1)
      {
        return true;
      }
      return false;
    }

    // 최대인지 확인
    bool isFull()
    {
      if (top-1 == size)
      {
        return true;
      }
      return false;
    }

    // 스택 사이즈 확인
    int getSize()
    {
      return top+1;
    }

  private:
    int top = -1;
    int size;
    int *array;

};

int main()
{
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, choice;
  cin >> N;

  Stack stack(N);

  for (int i=0; i<N; ++i)
  {
    cin >> choice;
    switch (choice)
    {
      case 1:
        int number;
        cin >> number;
        stack.push(number);
        break;

      case 2:
        if(!stack.isEmpty())
        {
          cout << stack.pop() << '\n';
        }
        else
        {
          cout << -1 << '\n';
        }
        break;

      case 3:
        cout << stack.getSize() << '\n';
        break;

      case 4:
        cout << (stack.isEmpty()? 1 : 0) << '\n';
        break;

      case 5:
        cout << (stack.isEmpty() ? -1 : stack.peek()) << '\n';
        break;
    }
  }

  return 0;
}