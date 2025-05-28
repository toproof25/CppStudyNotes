/*
* 포인터 활용
* 크기 5 정수 배열을 동적 할당 후 포인터 선언
* 동적 할당된 배열에 1~5까지 값 저장
* 할당 배열 내용 출력
* 메모리 해제
*/
#include <iostream>
#include <windows.h>

int main()
{
    int size = 5;
    int *arr = new int[size];
    int *p_arr = arr;

    for(int i=0; i<size; ++i)
    {
      p_arr[i] = i+1;
    }

    for(int i=0; i<size; ++i)
    {
      std::cout << "p_arr[" << i << "]: " << p_arr[i] << std::endl;
    }

    delete[] arr;


    std::cout << "\n\n";
    system("pause");
    return 0;
}