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
#include <vector>
using namespace std;

int main()
{
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, choice;
  cin >> N;

  vector<int> stack;
  stack.reserve(N);

  for (int i=0; i<N; ++i)
  {
    cin >> choice;
    switch (choice)
    {
      case 1:
        int number;
        cin >> number;
        stack.push_back(number);
        break;

      case 2:
        if(!stack.empty())
        {
          int pop_int = stack.back();
          stack.pop_back();
          cout << pop_int << '\n';
        }
        else
        {
          cout << -1 << '\n';
        }
        break;

      case 3:
        cout << stack.size() << '\n';
        break;

      case 4:
        cout << (stack.empty()? 1 : 0) << '\n';
        break;

      case 5:
        cout << (stack.empty() ? -1 : stack.back()) << '\n';
        break;
    }
  }

  return 0;
}