/*
* Composition, Aggregation 개념 익히기
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


// 어그리게이션을 이용한 몬스터 클래스
class monster_aggregation {
  private:
    monster &monster_base;
    character *character_base;

  public:
    monster_aggregation(monster &ref_monster_base, character *pointer_character_base) : 
    monster_base(ref_monster_base), 
    character_base(pointer_character_base)
    { cout << "저는 monster_aggregation 입니다" << endl; };
    ~monster_aggregation() {};


  void attack(player target_player);
  void attack_special(player target_player);
};

// 어그리게이션 이용하여 기존 일반 공격을 실행
void monster_aggregation::attack(player target_player) {
  monster_base.attack(target_player);
}

// 스페셜 공격은 오버라이딩이 아닌 함수를 정의하여 사용 (이름은 같게)
void monster_aggregation::attack_special(player target_player) {
  cout << "어그리게이션 공격 : 데미지 - 115 hp" << endl;
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

  // 외부에서 객체 생성 후 내부로 전달 - 생명 주기가 다름
  {
  cout << "\n***어그리게이션 monster Class***" << endl;
  monster ref_monster;
  character ref_character;
  monster_aggregation mon3(ref_monster, &ref_character);
  mon3.attack(test);
  mon3.attack_special(test);
  }

  return 0;
}