#include <iostream>
#include <windows.h>

int main()
{

    char c1 = 'A';
    char c2 = 'a';
    
    std::cout << "대문자 -> 소문자 변환 (대문자 or 32)\n";
    for (c1; c1 <= 'Z'; c1++)
    {
        std::cout << c1 << " : " << char(c1|32) << '\n';
    }

    std::cout << "\n소문자 -> 대문자 변환 (소문자 xor 32)\n";
    for (c2; c2 <= 'z'; c2++)
    {
        std::cout << c2 << " : " << char(c2^32) << '\n';
    }

    std::cout << "\n\n";
    system("pause");
    return 0;
}