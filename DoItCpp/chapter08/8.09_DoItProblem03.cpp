/*
* 가상 함수를 활용하여 커피를 제공하는 클래스를 제작하라 
*/
#include <iostream>
#include <windows.h>
using namespace std;


class CoffeeMachine
{
  public:
    // 순수 가상 함수 - 자식 클래스에서 오버라이드를 강제하여 다형성을 구현
    virtual void order_coffee()=0;

  private:
};


class UnmannedStore : public CoffeeMachine
{
  public:
    // 가상 함수로 다형성을 구현
    virtual void order_coffee() override;
};
void UnmannedStore::order_coffee()
{
  cout << "[일반 커피 결제]" << endl;
  cout << "커피 고르기" << endl;
  cout << "카운터에서 커피 주문하기" << endl;
  cout << "커피 가져가기" << endl;
}

class Franchise : public CoffeeMachine
{
  public:
    virtual void order_coffee() override;
};
void Franchise::order_coffee()
{
  cout << "[컴포즈 커피 결제]" << endl;
  cout << "컴포즈 커피 고르기" << endl;
  cout << "어플로 적립하고 커피 주문하기" << endl;
  cout << "커피 가져가기" << endl;
}


class Kiosk : public CoffeeMachine
{
  public:
    virtual void order_coffee() override;
};
void Kiosk::order_coffee()
{
  cout << "[키오스크 커피 결제]" << endl;
  cout << "커피 고르기" << endl;
  cout << "키오스크로 커피를 고른 후 결제하기" << endl;
  cout << "커피 가져가기" << endl;
}

int main()
{
  UnmannedStore coffee1;
  Franchise coffee2;
  Kiosk coffee3;

  // 각 클래스마다 오버라이드된 함수가 실행됨
  coffee1.order_coffee();
  cout << endl;
  coffee2.order_coffee();
  cout << endl;
  coffee3.order_coffee();

  std::cout << "\n\n";

  // 가상 함수를 이용하여 다형성을 구현
  // 추상 클래스는 객체를 만들 수 없지만, 포인터 변수로 자식 객체를 가리킬 수 있다.
  CoffeeMachine *cm = &coffee1;
  cm->order_coffee();
  cout << endl;

  cm = &coffee2;
  cm->order_coffee();
  cout << endl;

  cm = &coffee3;
  cm->order_coffee();

  std::cout << "\n\n";
  system("pause");
  return 0;
}
