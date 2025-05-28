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

// 상태 관리 상수
enum Status {
  CaptainSelect,       // 0 기장 선택
  CaptainAdd,          // 1 기장 추가
  PlaneSelect,         // 2 비행기 선택
  PlaneAdd,            // 3 비행기 추가
  ShowCurrentPlane,    // 4 현재 비행기 정보 출력
  ShowFightJetCount,    // 5 총 전투기 수 확인
  ShowHelicopterCount    // 6 총 전투기 수 확인
};




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

// 조종실 - 조종 부분
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
    Captain(string name="") : name(name) {}
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
    Airplane(string name="", int capacity=0) : 
    airplane_name(name), 
    capacity(capacity), 
    aviation_fuel(0.0),
    captain(nullptr)
    {}

    virtual void AirplaneInformation() =0;
    virtual void RefuelAirplane(int fuel) =0; // 순수 가상 함수 (연료 채우는 함수 - 비행기마다 다른 연료를 사용)
    string GetAirpaneName() { return airplane_name; } 
    void ChangeCaptain(Captain *captain) 
    { 
      this->captain = captain;
      if (this->captain != nullptr)
      {
        cout << "기장이 " << captain->GetName() << "으로 변경됐습니다" << endl;
      }
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
    // 생성자, 소멸자에서 전투기 수 변수의 값을 조절
    FighterJet(string name, int capacity) : Airplane(name, capacity), engine(1000) { fighter_jet_count++; }
    ~FighterJet() { fighter_jet_count--; }

    virtual void AirplaneInformation() override
    {
      cout << "기체이름 : [" << this->airplane_name <<"]" << endl;
      cout << "연료 : [" << this->aviation_fuel <<"]" << endl;
      cout << "수용인원 : [" << this->capacity <<"]" << endl;
      cout << "마력 : [" << this->engine.GetMaxPower() <<"]" << endl;
      if (captain != nullptr)
      {
        captain->PrintCaptainName();
      }
      cout << endl;
    }
    virtual void RefuelAirplane(int fuel) override
    {
      cout << "[전투기 전용 항공유]" << endl;
      cout << "연료를" << fuel << "L 채웁니다..." << endl;
      this->aviation_fuel += fuel;
      cout << endl;
    }

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

    // 정적 멤버 변수 활용 - 전투기 수 
    static int fighter_jet_count;

    // 컴포지션 활용
    Engine engine;
    Cockpit cockpit;
};
class Helicopter : public Airplane
{
  public:
    Helicopter(string name, int capacity) : Airplane(name, capacity), engine(5555) { helicopter_count++; }
    ~Helicopter() { helicopter_count--; }

    virtual void AirplaneInformation() override
    {
      cout << "기체이름 : [" << this->airplane_name <<"]" << endl;
      cout << "연료 : [" << this->aviation_fuel <<"]" << endl;
      cout << "수용인원 : [" << this->capacity <<"]" << endl;
      cout << "마력 : [" << this->engine.GetMaxPower() <<"]" << endl;
      if (captain != nullptr)
      {
        captain->PrintCaptainName();
      }
      cout << endl;
    }
    virtual void RefuelAirplane(int fuel) override
    {
      cout << "[전투기 전용 항공유]" << endl;
      cout << "연료를" << fuel << "L 채웁니다..." << endl;
      this->aviation_fuel += fuel;
      cout << endl;
    }

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

    // 정적 멤버 변수 활용 - 전투기 수 
    static int helicopter_count;

    // 컴포지션 활용
    Engine engine;   // 기체 엔진
    Cockpit cockpit; // 조종석
};

// 정적 멤버 변수는 전역에서 정의
int FighterJet::fighter_jet_count = 0;
int Helicopter::helicopter_count = 0;



// 기장 선택
Captain* ChoiceCaptain(vector<Captain> &captains)
{
  if (captains.size() == 0)
  {
    return nullptr;
  }

  int choice;
  cout << "기장을 선택하세요" << endl;
  for (int i=0; i<captains.size(); ++i)
  {
    cout << i+1 << ". " << captains[i].GetName() << endl;
  };
  cout << "시작 기장 선택 >>> ";
  cin >> choice;
  cout << endl;
  return &captains[choice-1];
}

// 비행기 선택
Airplane* ChoiceAirplanes(vector<Airplane*> &airplanes)
{
  if (airplanes.size() == 0)
  {
    return nullptr;
  }

  int choice;
  cout << "비행기를 선택하세요" << endl;
  for (int i=0; i<airplanes.size(); ++i)
  {
    cout << i+1 << ". " << airplanes[i]->GetAirpaneName() << endl;
  };
  cout << "시작 비행기 선택 >>> ";
  cin >> choice;
  cout << endl;
  return airplanes[choice-1];
}


int main()
{
  vector<Airplane*> airplanes = {new FighterJet("F-99", 1), new Helicopter("Helicopter1", 4)};
  Airplane *current_airplane = airplanes[0];

  vector<Captain> captains;
  int choice = -1;
  
  cout << "***비행기 관리 시뮬레이션***" << endl;

  while(1)
  {
    switch(choice)
    {
      case CaptainSelect:
      {
        cout << "****************[기장 선택]****************" << endl;
        Captain *captain_pointer = ChoiceCaptain(captains);
        if (captain_pointer == nullptr)
        {
          cout << "****************기장을 먼저 추가해주세요****************" << endl;
        }
        else
        {
          current_airplane->ChangeCaptain(captain_pointer);
          cout << "****************기장 선택 완료****************" << endl;
        }
        break;
      }

      case CaptainAdd:
      {
        cout << "****************[기장 추가]****************" << endl;
        cout << "기장 이름을 작성하시오 >>> ";
        string captain_name;
        cin >> captain_name;
        captains.push_back(Captain(captain_name));
        cout << "****************기장 추가 완료****************" << endl;
        break;
      }

      case PlaneSelect:
      {
        cout << "****************[기체 선택]****************" << endl;
        Airplane *airplane_pointer = ChoiceAirplanes(airplanes);
        if (airplane_pointer == nullptr)
        {
          cout << "****************기체를 먼저 추가해주세요****************" << endl;
        }
        else
        {
          current_airplane = airplane_pointer;
          cout << "****************기체 변경 완료****************" << endl;
        }
        break;
      }
      
      case PlaneAdd:
      {
        cout << "****************[기체 추가]****************" << endl;
        
        string plane_name;
        int capacity;

        cout << "기체 이름을 작성하시오 >>> ";
        cin >> plane_name;
     
        cout << "기체 수용인원을 작성하시오 >>> ";
        cin >> capacity;

        airplanes.push_back(new Helicopter(plane_name, capacity));

        cout << "****************기체 추가 완료****************" << endl;
        break;
      }
      
      case ShowCurrentPlane:
      {
        cout << "****************[현재 비행기 정보 출력]****************" << endl;
        current_airplane->AirplaneInformation();
        cout << "****************현재 비행기 정보 출력 완료****************" << endl;
        break;
      }

      case ShowFightJetCount:
      {
        cout << "****************[총 전투기 기체 수 출력]****************" << endl;
        cout << "현재 전투기는 " << FighterJet::fighter_jet_count << "대 있습니다" << endl;
        cout << "****************총 전투기 기체 수 출력 완료****************" << endl;
        break;
      }

      case ShowHelicopterCount:
      {
        cout << "****************[총 헬리콥터 기체 수 출력]****************" << endl;
        cout << "현재 헬리콥터는 " << Helicopter::helicopter_count << "대 있습니다" << endl;
        cout << "****************총 헬리콥터 기체 수 출력 완료****************" << endl;
        break;
      }

      default:
      {
        break;
      }
    }
    

    current_airplane->RefuelAirplane(111);

    cout << "\n다음 목록을 선택하세요 [현재 기체 - " << current_airplane->GetAirpaneName() << "]" << endl;
    cout << "1. 기장 선택" << endl;
    cout << "2. 기장 추가" << endl;
    cout << "3. 비행기 선택" << endl;
    cout << "4. 비행기 추가" << endl;
    cout << "5. 현재 비행기 정보 출력" << endl;
    cout << "6. 전투기 기체 수 확인" << endl;
    cout << "7. 헬리콥터 기체 수 확인" << endl;
    cout << "목록 선택 >>> ";
    cin >> choice;
    choice--;
    cout << endl;
  };

  // 동적 메모리 해제
  for (auto airplane : airplanes)
  {
    delete airplane;
  }


  std::cout << "\n\n";
  system("pause");
  return 0;
}
