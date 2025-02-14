/*
* 추상 클래스 활용
- 비행기 추상 클래스를 설계
- 전투기, 대형 항공기, 헬리콥터, 행글라이더 4종류의 클래스를 제작
- 필수 기능으로 이륙, 착륙, 고도 상승, 고도 하강, 불시착
- 공통 기능은 컴포지션, 어그리게이션(기장)으로 활용
*/

#include <iostream>
#include <windows.h>
using namespace std;

// 비행기 엔진 클래스 - 컴포지션
class Engine
{
  public:
    Engine(int max_power) : max_power(max_power) {}
    
    void StartEngine(string name) { power=10; cout << name << "엔진 시동 On : power ->" << power << endl; }
    void StopEngine(string name) { power=0; cout << name << "엔진 시동을 종료합니다 : power ->" << power << endl; }
    void PowerUp() { power = power+50 >= max_power ? max_power : power+50; }
    void PowerDown() { power = power-50 <= 0 ? 0 : power-50; }
    int GetMaxPower() { return max_power; }

  private:
    int max_power;
    int power;
};

class Cockpit
{
  public:
    void TakeOff(string name);    // 이륙
    void Land();                  // 착륙
    void IncreaseAltitude(int fuel);      // 고도 상승
    void DecreaseAltitude(int fuel);      // 고도 하강
    void EmergencyLanding();      // 불시착
};
void Cockpit::TakeOff(string name)
{
  cout << name << "비행기 이륙합니다" << endl;
}
void Cockpit::Land()
{
  cout << "비행기 착륙합니다" << endl;
}
void Cockpit::IncreaseAltitude(int fuel)
{
  cout << "고도 상승!!!! 남은 연료->" << fuel << endl;
}
void Cockpit::DecreaseAltitude(int fuel)
{
  cout << "고도 하강!!!! 남은 연료->" << fuel << endl;
}
void Cockpit::EmergencyLanding()
{
  cout << "불시착.... 비상...비상" << endl;
}


// 기장 클래스 - 어그리게이션으로 기장을 변경
class Captain
{
  public:
    Captain(string name) : name(name) {}
    void GetCaptainName()
    {
      cout << "안녕하세요. " << name << " 기장 입니다." << endl; 
    }

  private:
    string name;
};

// 비행기 추상 클래스
class Airplane
{
  public:
    Airplane(string name, int capacity, Captain *captain) : 
    airplane_name(name), 
    capacity(capacity), 
    captain(captain), 
    aviation_fuel(0.0) 
    {}

    virtual void AirplaneInformation() =0;
    virtual void RefuelAirplane(int fuel) =0; // 순수 가상 함수 (연료 채우는 함수 - 비행기마다 다른 연료를 사용)

  protected:
    string airplane_name; // 비행기 이름
    double aviation_fuel; // 연료
    int capacity;         // 수용인원
    Captain *captain;     // 기장 - 어그리게이션으로 사용
};

class FighterJet : public Airplane
{
  public:
    FighterJet(string name, int capacity, Captain *captain) : Airplane(name, capacity, captain), engine(1000) {}

    virtual void AirplaneInformation() override;

    virtual void RefuelAirplane(int fuel) override;

    // 이륙
    void TakeOff()
    {
      engine.StartEngine(this->airplane_name);
      cockpit.TakeOff(this->airplane_name);
    }
    // 착륙
    void Land()
    {
      cockpit.Land();
      engine.StopEngine(this->airplane_name);
    }
    // 고도 상승
    void IncreaseAltitude()
    {
      engine.PowerUp();
      this->aviation_fuel -= 12;
      if (this->aviation_fuel <= 0)
      {
        EmergencyLanding();
      }
      else
      {
        cockpit.IncreaseAltitude(this->aviation_fuel);
      }
    }
    // 고도 하강
    void DecreaseAltitude()
    {
      engine.PowerDown();
      this->aviation_fuel -= 5;
      if (this->aviation_fuel <= 0)
      {
        EmergencyLanding();
      }
      else
      {
        cockpit.DecreaseAltitude(this->aviation_fuel);
      }
    }
    // 불시착
    void EmergencyLanding()
    {
      cockpit.EmergencyLanding();
    }


    Engine engine;
    Cockpit cockpit;
};

void FighterJet::AirplaneInformation()
{
  cout << "기체이름 : [" << this->airplane_name <<"]" << endl;
  cout << "연료 : [" << this->aviation_fuel <<"]" << endl;
  cout << "수용인원 : [" << this->capacity <<"]" << endl;
  cout << "마력 : [" << this->engine.GetMaxPower() <<"]" << endl;
  captain->GetCaptainName();
  cout << endl;
}
void FighterJet::RefuelAirplane(int fuel)
{
  cout << "[전투기 전용 항공유]" << endl;
  cout << "연료를" << fuel << "L 채웁니다..." << endl;
  this->aviation_fuel += fuel;
  cout << endl;
}


int main()
{
  Captain captain1("김철수");
  Captain captain2("홍길동");


  FighterJet f("F-99", 5, &captain1);
  f.RefuelAirplane(50);
  f.AirplaneInformation();

  f.TakeOff();

  f.IncreaseAltitude();
  f.IncreaseAltitude();
  f.IncreaseAltitude();

  f.DecreaseAltitude();
  f.DecreaseAltitude();
  f.DecreaseAltitude();

  f.Land();
  

  std::cout << "\n\n";
  system("pause");
  return 0;
}
