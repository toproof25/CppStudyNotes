/*
* 동적 배열 선언과 사용법 학습
*/
#include <iostream>
#include <windows.h>

int main()
{
    // 고정 배열은 크기가 고정되어 있어서 동적으로 크기를 변경할 수 없다 
    std::cout << "고정 배열 선언" << std::endl;
    int fixed_arr_int[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for(const int &num : fixed_arr_int)
    {
        std::cout << num << ", ";
    }
    std::cout << "\n\n";

    // 동적 배열 선언
    int size;
    std::cout << "동적 배열 크기 입력 >>> ";
    std::cin >> size;
    std::cout << "동적 배열 크기 : " << size << std::endl;

    // new를 통해 동적 배열을 생성
    int *danamic_arr_int = new int[size];
    for(int i = 0; i < size; i++)
    {
        danamic_arr_int[i] = i + 1;
    }

    // 동적으로 할당된 배열
    for(int i = 0; i < size; i++)
    {
        std::cout << danamic_arr_int[i] << ", ";
    }
    std::cout << "\n\n";

    // 꼭 delete로 힙 메모리 할당된 동적 메모리를 해제 해주어야 함
    delete[] danamic_arr_int;

    std::cout << "\n\n";
    system("pause");
    return 0;
}