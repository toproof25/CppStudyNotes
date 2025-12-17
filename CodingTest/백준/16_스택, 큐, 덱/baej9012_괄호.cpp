#include <iostream>
#include <string>
#include <stack>




int main()
{
  // 입출력 설정
  std::cin.tie(0);
  std::ios::sync_with_stdio(false);

  // 필요한 변수 설정
  std::stack<char> result;
  std::string parenthesis;
  bool isVPS;
  int number; std::cin >> number;
  
  // 메인 알고리즘
  for (int i=0; i<number; ++i)
  {
    isVPS = true;
    std::cin >> parenthesis;
    for (const char& c : parenthesis)
    {
      switch (c)
      {
        case '(':
          result.push(c);
          break;
          
        case ')':
          if (result.empty())
            isVPS = false;
          else
            result.pop();
          break;
      }

      if (!isVPS)
        break;
    }
    while(!result.empty())
    {
      isVPS = false;
      result.pop();
    }

    if (isVPS)
      std::cout << "YES" << '\n';
    else
      std::cout << "NO" << '\n';
  }





  return 0;
}