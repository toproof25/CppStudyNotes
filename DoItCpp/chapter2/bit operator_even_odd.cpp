#include <iostream>
#include <windows.h>

int main()
{
    int even_odd;
    std::cout << "홀수 짝수 구분 : ";
    std::cin >> even_odd;
    std::cout << "even_odd : " << (even_odd&1 ? "홀수" : "짝수") << '\n';

    std::cout << "\n\n";
    system("pause");
    return 0;
}