/*
* 구조체 타입 배열 선언과 사용
*/
#include <iostream>
#include <windows.h>
#include <string>

// x, y, z값을 가지는 구조체 선언
struct Position
{
    // 좌표
    double x = 0.0;
    double y = 1.0;
    double z = 0.0;

    // 좌표 출력
    void display(const std::string &name)
    {
        std::cout << name << " x, y, z : (" << x << ", " << y << ", " << z << ")" << std::endl;
    }   
};

// 구조체 포인터와 좌표를 받아서 값을 변경 - 원본 구조체의 값을 바꾸기 위해 포인터로 받아옴
void set_position(Position *pos, double x, double y, double z)
{
    (*pos).x = x; // *를 이용하여 값에 접근
    (*pos).y = y;
    (*pos).z = z;
}

// 구조체 포인터를 활용하여 좌표 초기화
void reset_position(Position *pos)
{
    pos->x = 0.0; // ->를 이용하여 값에 접근
    pos->y = 0.0;
    pos->z = 0.0; 
}

int main()
{
    Position target[] = 
    {
        {0.0, 0.0, 0.0},
        {1.0, 0.0421, 3120.0},
        {2.0, 0.0, 0.0},
        {3.0, 0.0421, 421.0},
        {5.0, 0.0, 0.0},
        {60.0, 0.0, 0.0},
        {0.0, 124.0, 0.0},
        {0.0, 0.0, 0.3210},
        {0.0421, 0.0, 0.0},
        {0.0, 0.32130, 0.0},
        {0.510, 0.0, 0.3210}
    };

    std::cout << "초기값 좌표" << std::endl;
    int count = 0;
    for(Position &pos : target)
    {
        pos.display("target[" + std::to_string(count++) + "]");
    }
    std::cout << "\n\n";


    std::cout << "set_position 좌표" << std::endl;
    count = 0;
    for(int y=0; y<=10; y++)
    {
        set_position(&target[y], 0, y, 0);
    }
    for(Position &pos : target)
    {
        pos.display("target[" + std::to_string(count++) + "]");
    }
    std::cout << "\n\n";


    std::cout << "reset_position 좌표" << std::endl;
    count = 0;
    for(int y=0; y<=10; y++)
    {
        reset_position(&target[y]);
    }
    for(Position &pos : target)
    {
        pos.display("target[" + std::to_string(count++) + "]");
    }
    std::cout << "\n\n";

    std::cout << "\n\n";
    system("pause");
    return 0;
}