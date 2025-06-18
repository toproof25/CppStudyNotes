/*
* 포인터를 이용하여 두 값을 변환하는 방법
변수, 포인터, 참조 변수 차이점 
*/
#include <iostream>

// Call by Value - 값을 복사하여 해당 함수내에 지역변수로 바뀜 - 원본은 안바뀜
void swap(int num1, int num2)
{
    int temp = num1;
    num1 = num2;
    num2 = temp;
}

// Call by Address - 값의 주소를 포인터 변수로 받아와서 해당 주소의 값을 바꿈 - 원본도 바뀜
void p_swap(int *num1, int *num2)
{
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

// Call by Reference - 값의 주소 자체를 받아와서 해당 주소의 값을 바꿈 - 원본도 바뀜
void ref_swap(int &ref_num1, int &ref_num2)
{
    int temp = ref_num1;
    ref_num1 = ref_num2;
    ref_num2 = temp;
}


int main()
{
    int num1 = 100;
    int num2 = 999;

    std::cout << "num1 : " << num1 << std::endl;
    std::cout << "num2 : " << num2 << std::endl;
    std::cout << '\n';

    swap(num1, num2);

    std::cout << "swap 함수 호출" << std::endl;
    std::cout << "num1 : " << num1 << std::endl;
    std::cout << "num2 : " << num2 << std::endl;
    std::cout << '\n';

    p_swap(&num1, &num2);

    std::cout << "p_swap(&num1, &num2) 호출 - 포인터를 이용하여 변경" << std::endl;
    std::cout << "num1 : " << num1 << std::endl;
    std::cout << "num2 : " << num2 << std::endl;
    std::cout << '\n';

    ref_swap(num1, num2);

    std::cout << "ref_swap(num1, num2) 호출 - 직접 변수에 참조하여 변경" << std::endl;
    std::cout << "num1 : " << num1 << std::endl;
    std::cout << "num2 : " << num2 << std::endl;
    std::cout << '\n';

    return 0;
}