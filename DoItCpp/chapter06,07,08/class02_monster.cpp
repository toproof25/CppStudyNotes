/*
* Do It C++ 교재 몬스터 클래스 정의
*/
#include <iostream>
#include <windows.h>

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

//몬스터 B는 기본 Monster 클래스로부터 상속
class monster_b : public monster, character {
  public:
    //상속받은 함수 오버라이딩
    void attack_special(player target_player);
};

// monster_b 오버라이딩을 이용하여 함수 재정의
void monster_b::attack_special(player target_player) {
  cout << "가상 공격 : 데미지 - 0 hp" << endl;
}

//몬스터 C는 기본 Monster 클래스로부터 상속
class monster_c : public monster, character {
public:
  //상속받은 함수 오버라이딩
  void attack_special(player target_player);
};

// monster_c 오버라이딩을 이용하여 함수 재정의
void monster_c::attack_special(player target_player) {
  cout << "강력 뇌전 공격 : 데미지 - 100 hp" << endl;
}

int main() {
  cout << "플레이어 생성\n";
  player player_1;

  cout << "\n몬스터 a 생성\n";
  monster_a forest_monster;
  
  cout << "\n몬스터 b 생성\n";
  monster_b tutorial_monster;
  
  cout << "\n몬스터 c 생성\n";
  monster_c boss_monster;

  cout << "\n몬스터 총 공격" << endl;
  tutorial_monster.attack_special(player_1);
  forest_monster.attack_special(player_1);
  boss_monster.attack_special(player_1);


  std::cout << "\n\n";
  system("pause");
  return 0;
}