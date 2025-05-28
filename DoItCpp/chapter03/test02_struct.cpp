/*
* 구조체 활용
* 5가지 속성을 가진 몬스터 구조체 생성
* 구조체를 파라미터로 받는 함수를 제작후 출력
*/
#include <iostream>
#include <windows.h>

struct Monster
{
  std::string name;
  double HP;
  double ATK;
  double DEF;
  double SPD;
  double LUK;
};


void print_status(Monster *monster)
{
  std::cout << "Name: " << monster->name << std::endl;
  std::cout << "HP: " << monster->HP << std::endl;
  std::cout << "ATK: " << monster->ATK << std::endl;
  std::cout << "DEF: " << monster->DEF << std::endl;
  std::cout << "SPD: " << monster->SPD << std::endl;
  std::cout << "LUK: " << monster->LUK << std::endl;
  std::cout << '\n';
};


int main()
{
    int size = 10;

    std::cout << "몬스터의 수를 입력 : ";
    std::cin >> size;

    Monster *monsters = new Monster[size];

    for (int i=0; i<size; ++i)
    {
        monsters[i].name = "Monster" + std::to_string(i+1);
        monsters[i].HP = 100.0 + i;
        monsters[i].ATK = 10.0 + i;
        monsters[i].DEF = 5.0 + i;
        monsters[i].SPD = 5.0 + i;
        monsters[i].LUK = 0.1 + i;
    }

    for (int i=0; i<size; ++i)
    {
        print_status(&monsters[i]);
    }





    delete[] monsters;

    std::cout << "\n\n";
    system("pause");
    return 0;
}