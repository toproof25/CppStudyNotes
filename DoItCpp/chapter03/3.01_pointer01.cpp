/*
* 포인터 변수 선언, 사용
*/
#include <iostream>
#include <windows.h>

int main()
{
    // 스택 메모리에 100을 저장한 int_num 변수
    int int_num = 100;

    // 스택 메모리에 int_num의 주소를 가진 포인터 변수
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