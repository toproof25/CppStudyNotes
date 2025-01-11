/*
* 포인터 변수 선언, 사용
*/
#include <iostream>
#include <windows.h>

int main()
{
    int int_num = 100;
    int *p_int = &int_num;

    std::cout << "int_num의 값 : " << int_num << std::endl;
    std::cout << "int_num의 주소 : " << &int_num << std::endl;
    std::cout << '\n';
    std::cout << "p_int가 가진 주소 : " << p_int << std::endl;
    std::cout << "p_int가 가리키는 값 : " << *p_int << std::endl;
    std::cout << '\n';
    std::cout << "p_int의 주소 : " << &p_int << std::endl;


    std::cout << "\n\n";
    system("pause");
    return 0;
}