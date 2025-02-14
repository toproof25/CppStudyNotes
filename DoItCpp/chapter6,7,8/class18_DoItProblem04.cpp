/*
* 추상 클래스 활용
- 비행기 추상 클래스를 설계
- 전투기, 대형 항공기, 헬리콥터, 행글라이더 4종류의 클래스를 제작
- 필수 기능으로 이륙, 착륙, 고도 상승, 고도 하강, 불시착
- 공통 기능은 컴포지션, 어그리게이션(기장)으로 활용
*/

#include <iostream>
#include <windows.h>
#include <vector>
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
    int GetPower() { return power; }

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
    void PrintCaptainName()
    {
      cout << "안녕하세요. " << name << " 기장 입니다." << endl; 
    }
    string GetName() { return name; }

  private:
    string name;
};

// 비행기 추상 클래스
class Airplane
{
  public:
    Airplane(string name, int capacity) : 
    airplane_name(name), 
    capacity(capacity), 
    aviation_fuel(0.0) 
    {}

    virtual void AirplaneInformation() =0;
    virtual void RefuelAirplane(int fuel) =0; // 순수 가상 함수 (연료 채우는 함수 - 비행기마다 다른 연료를 사용)
    void ChangeCaptain(Captain *captain) 
    { 
      this->captain = captain;
      cout << "기장이 " << captain->GetName() << "으로 변경됐습니다" << endl;
    }

  protected:
    string airplane_name; // 비행기 이름
    double aviation_fuel; // 연료
    int capacity;         // 수용인원
    Captain *captain;     // 기장 - 어그리게이션으로 사용
};

class FighterJet : public Airplane
{
  public:
    FighterJet(string name, int capacity) : Airplane(name, capacity), engine(1000) {}

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
  captain->PrintCaptainName();
  cout << endl;
}
void FighterJet::RefuelAirplane(int fuel)
{
  cout << "[전투기 전용 항공유]" << endl;
  cout << "연료를" << fuel << "L 채웁니다..." << endl;
  this->aviation_fuel += fuel;
  cout << endl;
}

enum Status {
  CaptainSelect,      // 0 기장 선택
  CaptainAdd,         // 1 기장 추가
  PlaneSelect,        // 2 비행기 선택
  PlaneAdd,           // 3 비행기 추가
  ShowCurrentPlane    // 4 현재 비행기 정보 출력
};

int ChoiceCaptain(vector<Captain> &captains)
{
  int choice;
  cout << "기장을 선택하세요" << endl;
  for (int i=0; i<captains.size(); ++i)
  {
    cout << i+1 << ". " << captains[i].GetName() << endl;
  };
  cout << "시작 기장 선택 >>> ";
  cin >> choice;
  cout << endl;
  return choice-1;
}

int main()
{
  vector<Captain> captains = {Captain("김철수"), Captain("홍길동")};
  string captain_name = "";
  cout << "***비행기 관리 시뮬레이션***" << endl;
  int choice = CaptainSelect;
  
  FighterJet fighter_jet("F-99", 1);

  while(1)
  {
    switch(choice)
    {
      case CaptainSelect:
      {
        cout << "****************[기장 선택]****************" << endl;
        int captain_number = ChoiceCaptain(captains);
        fighter_jet.ChangeCaptain(&captains[captain_number]);
        break;
      }

      case CaptainAdd:
      {
        cout << "****************[기장 추가]****************" << endl;
        cout << "기장 이름을 작성하시오 >>> ";
        cin >> captain_name;
        captains.push_back(Captain(captain_name));
        break;
      }

      case PlaneSelect:
        cout << "비행기 선택" << endl;
        break;
      
      case PlaneAdd:
        cout << "비행기 추가" << endl;
        break;

      case ShowCurrentPlane:
        cout << "현재 비행기 정보 출력" << endl;
        break;
      
      default:
        cout << "잘못 입력됨" << endl;
        break;
    }

    

    cout << "다음 목록을 선택하세요" << endl;
    cout << "1. 기장 선택" << endl;
    cout << "2. 기장 추가" << endl;
    cout << "목록 선택 >>> ";
    cin >> choice;
    cout << endl;
    choice--;
  };



  std::cout << "\n\n";
  system("pause");
  return 0;
}
