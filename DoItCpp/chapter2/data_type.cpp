#include <iostream>
#include <windows.h>
#include <string>
#include <iomanip> // setprecision 함수를 사용하기 위한 헤더파일
#include <limits>

int main()
{
    // 정수형 데이터 타입
    char character = 'A';
    short short_number = 32767;
    int number = 2147483647;
    unsigned int unsigned_number = 4294967295;
    long long_number = 2147483647;
    long long long_long_numbe = 9223372036854775807;

    // 실수형 데이터 타입
    float float_number = 0.1234567f;
    double double_number = 0.1234567890123456;
    long double long_double_number = 0.12345678901234567890123456789012345l;

    // 논리형 데이터 타입
    bool boolean = true;

    // 문자열 데이터 타입 - string 클래스 사용
    const char *c_string1 = "Hello, World!"; // char배열로 문자열을 저장할 시 마지막에 개행문자가 들어갈 크기까지 고려
    char c_string2[] = "Hello, World!";
    std::string string = "Hello, World!"; // string 클래스를 이용하여 간단하게 문자열을 저장할 수 있음

    std::cout << "char: " << character << "\nsize : " << sizeof(character) << " byte\n" << std::endl;
    std::cout << "short: " << short_number << "\nsize : " << sizeof(short_number) << " byte\n" << std::endl;
    std::cout << "int: " << number << "\nsize : " << sizeof(number) << " byte\n" << std::endl;
    std::cout << "unsigned int: " << unsigned_number << "\nsize : " << sizeof(unsigned_number) << " byte\n" << std::endl;
    std::cout << "long: " << long_number << "\nsize : " << sizeof(long_number) << " byte\n" << std::endl;
    std::cout << "long long: " << long_long_numbe << "\nsize : " << sizeof(long_long_numbe) << " byte\n" << std::endl;

    std::cout << "float: " << std::setprecision(9) << float_number << "\nsize : " << sizeof(float_number) << " byte\n" << std::endl;
    std::cout << "double: " << std::setprecision(18) << double_number << "\nsize : " << sizeof(double_number) << " byte\n" << std::endl;
    std::cout << "long double: " << std::setprecision(36) << long_double_number << "\nsize : " << sizeof(long_double_number) << " byte\n" << std::endl;

    std::cout << "bool: " << boolean << "\nsize : " << sizeof(boolean) << " byte\n" << std::endl;

    std::cout << "c_string[12]: " << c_string1 << "\nsize : " << sizeof(c_string1) << " byte\n" << std::endl;
    std::cout << "c_string[14]: " << c_string2 << "\nsize : " << sizeof(c_string2) << " byte\n" << std::endl;
    std::cout << "string: " << string << "\nsize : " << sizeof(string) << "\nlength : " << string.length() << " length\n" << std::endl;
    

    std::cout << "\n\n";
    system("pause");
    return 0;
}