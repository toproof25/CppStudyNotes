/*
* 포인터 변수 자체를 상수화 하는 경우
*/
#include <iostream>
#include <windows.h>

int main()
{
    int number1 = 10;
    int number2 = 9999;
    int *const p_number1 = &number1; // 포인터 변수 p_number1를 상수화 함

    std::cout << "number1: " << number1 << std::endl;
    std::cout << "*p_number1: " << *p_number1 << std::endl;
    std::cout << "\n\n";

    // 포인터 변수 자신의 값을 상수화 하였기에 주소를 변경할 수 없음
    //p_number1 = &number2;

    // 가리키는 값은 상수가 아니기에 값 변경이 가능
    *p_number1 = number2;

    std::cout << "number1: " << number1 << std::endl;
    std::cout << "*p_number1: " << *p_number1 << std::endl;
    std::cout << "\n\n";

    std::cout << "\n\n";
    system("pause");
    return 0;
}