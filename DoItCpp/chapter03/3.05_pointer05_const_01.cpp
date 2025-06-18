/*
* 포인터 변수 앞에 const를 붙이는 경우 
*/
#include <iostream>
#include <windows.h>

int main()
{
    int number1 = 10;
    const int *p_number1 = &number1; // p_number1이 가리키는 값을 상수화 함

    std::cout << "number1: " << number1 << std::endl;
    std::cout << "*p_number1: " << *p_number1 << std::endl;
    std::cout << "\n\n";

    // 포인터 변수가 가리키는 값을 상수화 했기 때문에 값을 변경할 수 없음
    //*p_number1 = 9999;

    // 하지만 number1 자체는 상수화가 되어 있지 않아 값 변경이 가능하다
    number1 = 20;

    std::cout << "number1: " << number1 << std::endl;
    std::cout << "*p_number1: " << *p_number1 << std::endl;
    std::cout << "\n\n";

    std::cout << "\n\n";
    system("pause");
    return 0;
}