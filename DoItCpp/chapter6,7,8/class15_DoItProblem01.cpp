/*
* 클래스 객체 곱하기 연산자 오버로딩

a, b, c 몬스터 클래스를 * 연산자 오버라이딩을 이용하여 진화를 하는 코드를 작성

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
    void print_name() { cout << "name : " << name << "/ power : " << power << endl << endl; }

  protected:
    string name;
    int power;

};

class monster_a : public monster
{
  public:
    monster_a(string name, int power) : monster(name, power) { cout << "monster_a" << endl; }

    // a몬스터 * ~몬스터의 진화 오버라이딩
    monster_a operator*(const monster_a &ref_monster) { return monster_a(this->name + "a*a 진화", this->power*2); }
    monster_a operator*(const monster_b &ref_monster) { return monster_a(this->name + "a*b 진화", this->power*3); }
    monster_a operator*(const monster_c &ref_monster) { return monster_a(this->name + "a*c 진화", this->power*0); }
};

class monster_b : public monster
{
  public:
    monster_b(string name, int power) : monster(name, power) { cout << "monster_b" << endl; }

    // b몬스터 * ~몬스터의 진화 오버라이딩
    monster_b operator*(const monster_a &ref_monster) { return monster_b(this->name + "b*a 진화", this->power*5); }
    monster_b operator*(const monster_b &ref_monster) { return monster_b(this->name + "b*b 진화", this->power*2); }
    monster_b operator*(const monster_c &ref_monster) { return monster_b(this->name + "b*c 진화", this->power*0); }
};

class monster_c : public monster
{
  public:
    monster_c(string name, int power) : monster(name, power) { cout << "monster_c" << endl; }

    // c몬스터 * ~몬스터의 진화 오버라이딩
    monster_c operator*(const monster_a &ref_monster) { return monster_c(this->name + "c*a 진화", this->power*0); }
    monster_c operator*(const monster_b &ref_monster) { return monster_c(this->name + "c*b 진화", this->power*4); }
    monster_c operator*(const monster_c &ref_monster) { return monster_c(this->name + "c*c 진화", this->power*2); }
};

int main()
{
  monster_a mon_a("a 몬스터", 100);
  mon_a.print_name();

  monster_b mon_b("b 몬스터", 200);
  mon_b.print_name();
  
  monster_c mon_c("c 몬스터", 300);
  mon_c.print_name();

  // 연산자 오버라이딩을 이용한 진화 코드
  monster_a mon_aa = mon_a * mon_a;
  mon_aa.print_name();
  monster_a mon_ab = mon_a * mon_b;
  mon_ab.print_name();
  monster_a mon_ac = mon_a * mon_c;
  mon_ac.print_name();

  monster_b mon_ba = mon_b * mon_a;
  mon_ba.print_name();
  monster_b mon_bb = mon_b * mon_b;
  mon_bb.print_name();
  monster_b mon_bc = mon_b * mon_c;
  mon_bc.print_name();

  monster_c mon_ca = mon_c * mon_a;
  mon_ca.print_name();
  monster_c mon_cb = mon_c * mon_b;
  mon_cb.print_name();
  monster_c mon_cc = mon_c * mon_c;
  mon_cc.print_name();


  std::cout << "\n\n";
  system("pause");
  return 0;
}