/*
* 구조체 선언과 사용
*/
#include <iostream>
#include <windows.h>

struct Position
{
    double x = 0.0;
    double y = 1.0;
    double z = 0.0;
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
    Position table;
    Position mouse = {2.1, 3.12, 54.123};
    Position keyboard;

    std::cout << "초기값 좌표" << std::endl;
    std::cout << "table의 x, y, z : (" << table.x << ", " << table.y << ", " << table.z << ")" << std::endl;
    std::cout << "mouse의 x, y, z : (" << mouse.x << ", " << mouse.y << ", " << mouse.z << ")" << std::endl;
    std::cout << "keyboard의 x, y, z : (" << keyboard.x << ", " << keyboard.y << ", " << keyboard.z << ")" << std::endl;
    std::cout << "\n\n";

    set_position(&table, 1.132, 2.51, 3.321);
    set_position(&mouse, 4.51, 5.0321, 6.05151);
    set_position(&keyboard, 7.3410, 8.4310, 19.6160);

    std::cout << "set_position 좌표" << std::endl;
    std::cout << "table의 x, y, z : (" << table.x << ", " << table.y << ", " << table.z << ")" << std::endl;
    std::cout << "mouse의 x, y, z : (" << mouse.x << ", " << mouse.y << ", " << mouse.z << ")" << std::endl;
    std::cout << "keyboard의 x, y, z : (" << keyboard.x << ", " << keyboard.y << ", " << keyboard.z << ")" << std::endl;
    std::cout << "\n\n";

    reset_position(&table);
    reset_position(&mouse);
    reset_position(&keyboard);

    std::cout << "reset_position 좌표" << std::endl;
    std::cout << "table의 x, y, z : (" << table.x << ", " << table.y << ", " << table.z << ")" << std::endl;
    std::cout << "mouse의 x, y, z : (" << mouse.x << ", " << mouse.y << ", " << mouse.z << ")" << std::endl;
    std::cout << "keyboard의 x, y, z : (" << keyboard.x << ", " << keyboard.y << ", " << keyboard.z << ")" << std::endl;
    
    std::cout << "\n\n";
    system("pause");
    return 0;
}