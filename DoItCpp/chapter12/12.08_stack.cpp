/*
자료구조 Stack을 제공하는 컨테이너 어댑터
다른 컨테이너를 기반으로 새로운 기능을 제공하는 것

LIFO (Lask In First Out) 구조로 사용하는 자료구조
추가 : Push
제거 : Pop
맨 위 데이터 : top
비어있는지 확인 : empty

Stack은 top데이터 이외에 다른 요소를 접근하여 조회할 수는 없다
*/
#include <iostream>
#include <stack>
using namespace std;

int main()
{
  stack<int> myStack;

  // stack push
  for (int i=1; i<102; ++i)
  {
    myStack.push(i);
  }

  // top은 stack의 제일 위 데이터
  cout << "top value : " << myStack.top() << endl; 

  // stack pop
  cout << "myStack.pop() 실행 " << endl; 
  myStack.pop();
  cout << "pop 이후 top value : " << myStack.top() << endl; 

  cout << (myStack.empty() ? "스택이 비었습니다" : "스택에 데이터가 있습니다") << endl;

  // stack이 모두 비워질 때 까지 pop
  while(!myStack.empty())
  {
    cout << "반복중... top value : " << myStack.top() << endl; 
    myStack.pop();
  }
  cout << "myStack이 모두 비워졌습니다" << endl;



  return 0;
}