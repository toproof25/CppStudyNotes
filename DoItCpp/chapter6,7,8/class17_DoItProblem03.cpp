/*
* 가상 함수를 활용하여 커피를 제공하는 클래스를 제작하라
*/
#include <iostream>
#include <windows.h>
using namespace std;


class CoffeeMachine
{
  public:
    virtual void order_coffee();

  private:
};


class UnmannedStore : CoffeeMachine
{
  public:
    virtual void order_coffee() override;
};
void UnmannedStore::order_coffee()
{
  cout << "커피 고르기" << endl;
  cout << "카운터에서 커피 주문하기" << endl;
  cout << "커피 가져가기" << endl;
}

class Franchise : CoffeeMachine
{
  public:
    virtual void order_coffee() override;
};
void Franchise::order_coffee()
{
  cout << "빽다방 커피 고르기" << endl;
  cout << "어플로 적립하고 커피 주문하기" << endl;
  cout << "커피 가져가기" << endl;
}


class Kiosk : CoffeeMachine
{
  public:
    virtual void order_coffee() override;
};
void Kiosk::order_coffee()
{
  cout << "커피 고르기" << endl;
  cout << "키오스크로 커피를 고른 후 결제하기" << endl;
  cout << "커피 가져가기" << endl;
}

int main()
{
  

  std::cout << "\n\n";
  system("pause");
  return 0;
}