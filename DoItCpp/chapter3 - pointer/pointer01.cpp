/*
* 포인터 변수 선언
*/
#include <iostream>
#include <windows.h>

int main()
{
    int int_num = 100;
    int *p_int = &int_num;

    std::cout << p_int << std::endl;

    std::cout << "\n\n";
    system("pause");
    return 0;
}