/*
* 몬스터 클래스를 어그리게시연스로 변경
*/
#include <iostream>
#include <windows.h>
using namespace std;

class monster_a;
class monster_b;
class monster_c;


class monster
{
  public:
    monster(string name , int power) : name(name), power(power) { cout << "monster" << endl; }
    ~monster() { cout << "monster 소멸" << endl; }

    void print_name() { cout << "name : " << name << "/ power : " << power << endl; }

    void use_skill(string skill="일반")
    {
      cout << name << " " << skill << "스킬 시전" << endl;
    }

    string name;
    int power;

};

// monster, monster_a - Aggregation
class monster_a
{
  public:
    monster_a(monster *monster) : monster_base(monster) { cout << "monster_a Aggregation" << endl; }
    ~monster_a() { cout << "monster_a 소멸" << endl; }
    
    // 위임
    void print_name() { monster_base->print_name(); }
    void user_skill() { monster_base->use_skill("얼음"); }
    
  private:
    // 어그리게이션 포인터 변수
    monster *monster_base;
};


// monster, monster_b - Composition
class monster_b
{
  public:
    monster_b(string name, int power) : monster_base(name, power) { cout << "monster_b Composition" << endl; }
    ~monster_b() { cout << "monster_b 소멸" << endl; }

    // 위임
    void print_name() { monster_base.print_name(); }
    void user_skill() { monster_base.use_skill("파이어"); }
    
  private:
    // 컴포지션 객체 변수
    monster monster_base;
};


int main()
{
  {
  // 몬스터 클래스 어그리게이션 변경
  monster mon_base("a몬스터", 100);
  cout << endl;
    {
    monster_a mon_a(&mon_base);
    mon_a.user_skill();
    mon_a.print_name();
    }

  cout << endl;

    // 컴포지션은 생명 주기가 같다
    {
    // 몬스터 클래스 컴포지션 
    monster_b mon_b("bbb 몬스터", 9999);
    mon_b.user_skill();
    mon_b.print_name();
    }

  cout << endl;
  
  }

  std::cout << "\n\n";
  system("pause");
  return 0;
}