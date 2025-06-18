/*
* C++에서 고정 배열과 포인터의 관계
*/
#include <iostream>
#include <windows.h>

int main()
{
    // 정적 배열 선언
    int arr_int[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 배열의 공간 할당
    std::cout << "arr_int는 int형 배열로 (int의 크기 * 요소의 수)만큼 공간을 할당" << std::endl;
    std::cout << "sizeof(int) * 10 : " << sizeof(int) * 10 << std::endl;
    std::cout << "arr_int의 사이즈 : " << sizeof(arr_int) << std::endl;
    std::cout << "\n\n";

    // 배열의 이름은 첫 주소를 의미
    std::cout << "arr_int는 배열의 시작 주소를 의미 : " << arr_int << " == " << &arr_int << " -> &arr_int" << std::endl;
    std::cout << "즉 역참조를 통해 첫번째 요소의 값을 알 수 있음 *arr_int : " << *arr_int << std::endl;
    std::cout << "\n\n";

    // 포인터로 접근 하기 - 배열을 나타내는 변수 자체는 해당 배열의 첫번째 요소의 주소를 의미. 해당 주소+n은 자료형 크기에 맞게 증가
    std::cout << "값의 주소를 알 수 있으며, 역참조가 가능하니 다른 요소의 값도 알 수 있다 (주소+n은 자료형 크기에 맞게 크기가 증가)" << std::endl;
    std::cout << "*arr_int : " << *arr_int << std::endl;
    std::cout << "*(arr_int+1) : " << *(arr_int+1) << " -> 소괄호의 의미는 우선 연산자를 고려하기 때문 *arr_int+1 는 *arr_int값인 1에 +1을 하여 2가 됨 " << std::endl;
    std::cout << "*(arr_int+4) : " << *(arr_int+4) << std::endl;
    std::cout << "*(arr_int+7) : " << *(arr_int+7) << std::endl;
    std::cout << "\n\n";

    std::cout << "배열 이름은 첫 주소를 의미하는 포인터로, 각 요소마다 주소가 4byte만큼 증가" << std::endl;
    std::cout << "arr_int : " << arr_int << std::endl;
    std::cout << "arr_int+1 : " << arr_int+1 << std::endl;
    std::cout << "arr_int+2 : " << arr_int+2 << std::endl;
    std::cout << "arr_int+3 : " << arr_int+3 << std::endl;
    std::cout << "\n\n";

    // 배열의 접근 방식
    std::cout << "배열의 접근 방식을 통해 접근이 가능" << std::endl;
    std::cout << "arr_int[0] : " << arr_int[0] << std::endl;
    std::cout << "arr_int[1] : " << arr_int[1] << std::endl;
    std::cout << "arr_int[4] : " << arr_int[4] << std::endl;
    std::cout << "arr_int[7] : " << arr_int[7] << std::endl;
    std::cout << "\n\n";

    std::cout << "int의 크기 4byte에 맞게 각 요소의 주소도 크기에 따라 증가하는 것을 확인" << std::endl;
    std::cout << "&arr_int[0] : " << &arr_int[0] << std::endl;
    std::cout << "&arr_int[1] : " << &arr_int[1] << std::endl;
    std::cout << "&arr_int[2] : " << &arr_int[2] << std::endl;
    std::cout << "&arr_int[3] : " << &arr_int[3] << std::endl;
    std::cout << "\n\n";

    std::cout << "포인터 변수로 배열을 가리키기" << std::endl;
    int *p_arr_int = arr_int;
    std::cout << "arr_int : " << arr_int << std::endl;
    std::cout << "p_arr_int : " << p_arr_int << std::endl;
    std::cout << "*p_arr_int : " << *p_arr_int << std::endl;
    std::cout << "\n\n";
    
    std::cout << "배열 변수와 포인터 변수의 사이즈는 다르다 - 배열 변수는 전체 사이즈를, 포인터 변수는 포인터 사이즈를 나타냄" << std::endl;
    std::cout << "arr_int size : " << sizeof(arr_int) << std::endl;
    std::cout << "p_arr_int size : " << sizeof(p_arr_int) << std::endl;
    std::cout << "\n\n";

    std::cout << "포인터 변수로 배열의 요소에 접근하기" << std::endl;
    std::cout << "p_arr_int[0] : " << p_arr_int[0] << std::endl;
    std::cout << "p_arr_int[1] : " << p_arr_int[1] << std::endl;
    std::cout << "p_arr_int[4] : " << p_arr_int[4] << std::endl;
    std::cout << "p_arr_int[7] : " << p_arr_int[7] << std::endl;
    std::cout << "\n\n";




    std::cout << "\n\n";
    system("pause");
    return 0;
}