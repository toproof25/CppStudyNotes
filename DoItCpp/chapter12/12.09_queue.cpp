/*
자료구조 queue는 FIFO (First In First Out) 선입선출 방식의 데이터 구조이다.

추가 : push
제거 : pop
맨 앞 데이터 : front
맨 뒤 데이터 : back
비어있는지 확인 : empty

*/
#include <iostream>
#include <queue>
using namespace std;

int main()
{
  queue<int> myQueue;

  for(int i=1; i<102; i++)
    myQueue.push(i);


  // front은 queue의 제일 앞 데이터
  cout << "front value : " << myQueue.front() << endl; 

  // back은 queue의 제일 뒤 데이터
  cout << "back value : " << myQueue.back() << endl; 

  // queue pop
  cout << "myQueue.pop() 실행 " << endl; 
  myQueue.pop();
  cout << "pop 이후 front value : " << myQueue.front() << endl; 
  cout << "pop 이후 back value : " << myQueue.back() << endl; 

  cout << (myQueue.empty() ? "queue이 비었습니다" : "queue에 데이터가 있습니다") << endl;

  // queue이 모두 비워질 때 까지 pop
  while(!myQueue.empty())
  {
    cout << "반복중... front value : " << myQueue.front() << endl; 
    myQueue.pop();
  }
  cout << "myQueue이 모두 비워졌습니다" << endl;

  return 0;
}