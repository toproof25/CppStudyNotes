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
  
  // 메인 알고리즘
  while (true)
  {
    isVPS = true;
    std::getline(std::cin, parenthesis);

    if (parenthesis == ".")
      break;

    for (const char& c : parenthesis)
    {
      switch (c)
      {
        case '[': case '(':
          result.push(c);
          break;
          
        case ']':
          if (result.empty() || result.top() == '(')
            isVPS = false;
          else
            result.pop();
          break;

        case ')':
          if (result.empty() || result.top() == '[')
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
      std::cout << "yes" << '\n';
    else
      std::cout << "no" << '\n';
  }





  return 0;
}