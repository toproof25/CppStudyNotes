/*
* 상수 선언 및 사용
*/
#include <iostream>

int main()
{
    // 상수 선언
    const double PI = 3.141592;
    std::cout << "PI : " << PI << std::endl;

    // const로 선언된 변수는 수정이 불가하며, 오류가 발생
    // 식이 수정할 수 있는 lvalue여야 한다
    //PI = 3.1415926535;

    return 0;
}