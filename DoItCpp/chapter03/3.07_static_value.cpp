/*
* Static(정적) 변수 테스트
*/
#include <iostream>
#include <windows.h>


void static_value()
{
    // 정적 변수 선언
    int count = 0;
    static int static_count = 0;

    // 정적 변수 값 증가
    count++;
    static_count++;

    // 정적 변수 값 출력
    std::cout << "count : " << count << std::endl;
    std::cout << "static_count : " << static_count << std::endl;
}

int main()
{
    // 정적 변수는 데이터 메모리 영역에 저장된다 
    static_value();
    static_value();
    static_value();
    static_value();
    static_value();

    std::cout << "\n\n";
    system("pause");
    return 0;
}