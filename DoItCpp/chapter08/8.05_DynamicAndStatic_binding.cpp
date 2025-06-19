/*
* 정적 바인딩, 동적 바인딩
*/
#include <iostream>
#include <windows.h>

using namespace std;

//기본 Monster 클래스
class monster {
  public:
    virtual void attack_special();
};

// 스페셜 공격 함수 정의
void monster::attack_special() {
  cout << "기본 공격 : 데미지 - 10 hp" << endl;
}

//몬스터 A는 기본 Monster 클래스로부터 상속
class monster_a : public monster {
  public:

    //상속받은 함수 오버라이딩 
    virtual void attack_special() override;
};

// monster_a 오버라이딩을 이용하여 함수 재정의
void monster_a::attack_special() {
  cout << "인텡글 공격 : 데미지 - 15 hp" << endl;
}


int main()
{
  monster mon_base;
  monster_a mon_a;

  // 정적 바인딩된 함수를 호출
  mon_base.attack_special();

  // mon변수에 자료형에 따라 가상함수가 동적으로 바인딩(연결)됨
  cout << "\n***업케스팅 후 가상함수 호출***" << endl;
  monster *mon = &mon_a; 
  mon->attack_special();

  cout << "\n***기본 가상함수 호출***" << endl;
  mon = &mon_base;
  mon->attack_special();

  std::cout << "\n\n";
  system("pause");
  return 0;
}