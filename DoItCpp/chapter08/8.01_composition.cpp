/*
* Composition 개념 익히기 
*/
#include <iostream>

using namespace std;

// 캐릭터 클래스 (추상화 - 플레이어, 몬스터)
class character {

  // 생성자로 체력과 파워를 초기화
  public:
    character() : hp(100), power(100) { std::cout << "character\n"; };

  // 상속으로 체력, 파워를 설정
  protected:
    int hp;
    int power;
};

//character 클래스를 상속 받은 player 클래스
class player : public character {

  // 빈 생성자
  public:
    player() { std::cout << "player\n"; };
  };

//기본 Monster 클래스
class monster {
  public:
    monster() { cout << "몬스터입니다\n"; };
    void get_damage(int _damage) {};
    void attack(player target_player) {};
    void attack_special(player target_player);
};

// 스페셜 공격 함수 정의
void monster::attack_special(player target_player) {
  cout << "기본 공격 : 데미지 - 10 hp" << endl;
}

//몬스터 A는 기본 Monster 클래스로부터 상속
class monster_a : public monster, character {
  public:
    monster_a() { cout << "저는 몬스터A 입니다" << endl; };

    //상속받은 함수 오버라이딩 
    void attack_special(player target_player);
};

// monster_a 오버라이딩을 이용하여 함수 재정의
void monster_a::attack_special(player target_player) {
  cout << "인텡글 공격 : 데미지 - 15 hp" << endl;
}


// 상속이 아닌 컴포지션을 이용한 몬스터 클래스
class monster_composition {
  private:
    // 멤버 변수로 객체를 사용
    monster monster_base;
    character character_base;

  public:
    monster_composition() { cout << "저는 monster_composition 입니다" << endl; };
    ~monster_composition() {};

    // 상속과 달리 오버라이딩이 아닌 델리게이션으로 사용
    void attack(player target_player);
    void attack_special(player target_player);
};

// 컴포지션 위임 개념으로 기존 일반 공격을 실행
void monster_composition::attack(player target_player) {
  monster_base.attack(target_player);
}

// 스페셜 공격은 오버라이딩이 아닌 함수를 정의하여 사용 (이름은 같게)
void monster_composition::attack_special(player target_player) {
  cout << "monster_composition 공격 : 데미지 - 15 hp" << endl;
}


int main()
{
  player test;

  {
  cout << "\n***기존 monster_a Class***" << endl;
  monster_a mon1;
  mon1.attack(test);
  mon1.attack_special(test);
  }

  // 클래스 멤버 변수로 사용하여 생명 주기가 같음
  {
  cout << "\n***컴포지션 monster Class***" << endl;
  monster_composition mon2;
  mon2.attack(test);
  mon2.attack_special(test);
  }

  return 0;
}