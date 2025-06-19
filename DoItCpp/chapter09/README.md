# Chapter9 객체지향 프로그래밍 - SOLID 원칙
1. 단일 책임 원칙
2. 개방 폐쇄 원칙
3. 리스코프 치환 원칙
4. 인터페이스 분리 원칙
5. 의존성 역전 원칙 


# SRP 단일 책임 원칙 (Single Responsibility Principle)
- ***클래스는 한 가지 기능만 수행해야 하고, 한 가지 이유로만 변경해야 한다***
- 어떤 클래스가 A클래스를 변경할 때도 B클래스를 변경할 때에도 변경되는 현상을 지양해야 한다는 의미

### 산탄총 수술
- 산탄총이 사방에 퍼져 흔적을 남기듯 여러 클래스에 흔적이 있어 수정할 때 문제가 될 수 있는 것

### 설계 측면에서의 단일 책임 원칙
- 다중상속은 클래스를 크게 만들어 상속 보다는 컴포지션과 어그리게이션을 활용한다
- 다중 상속이 필요한 상황에서 컴포지션과 어그리게이션은 수정을 한 클래스에 갇히게 하는 효과

### 리펙토링 측면에서의 단일 책임 원칙
- 거대 클래스를 작은 단위로 나눔
- 논리적 관계는 유지하나 컴포지션과 어그리게이션으로 has-a 관계로 나누는 작업


# OCP 개방 폐쇄 원칙 (Open-Closed principle)
- ***확장에는 열려있고, 수정에는 닫혀있다***
- 추가 기능에는 추가 기능만 수정하며, 기존 코드를 수정을 지양하는 것
- 즉 기존 코드를 유지하면서 추가 기능을 추가할 수 있음을 의미하는 원칙

### 추상 클래스(인터페이스)
- 추상 클래스에서 기본적인 틀을 만들고, 구체적인 내용은 자식 클래스에서 구현하도록 하는 방법
- 위와 같은 방식의 설계를 **템플릿 메서드 패턴**이라 부른다



# LSP 리스코프 치환 원칙 (Liskov Substitution Principle)
- ***하위 클래스는 상위 클래스를 대체할 수 있어야 한다***
- is-a 관계인 클래스는 자식 클래스가 부모 클래스를 대체할 수 있어야 한다는 의미

### 업케스팅 / 멤버 함수의 유지 or 오버라이딩
- 자식 클래스가 부모 클래스로 업케스팅 될 수 있어야 한다
- 기존 부모 클래스의 멤버 함수를 오버라이딩하거나 유지해야 한다
- ex) 어떤 몬스터 클래스이든 최상위 클래스인 `monster`로 업케스팅 되어 같은 흐름으로 동작할 수 있어야 한다 

```cpp
// 리스코프 치환 원칙이 적용된 몬스터 생성 함수
monster* monster_factory::create_monster(const int terrain_type, terrain* terrain_inst) {

  // 상위 클래스를 포인터로 선언
  monster* new_mon = nullptr;

  switch (terrain_type) {
  case forest_terrain_type:
    new_mon = new monster_a();
    break;
  case cyber_terrain_type:
    new_mon = new monster_b();
    break;
  case urban_terrain_type:
    new_mon = new monster_c();
    break;
  }

  // 어떤 몬스터 클래스라도 상위 클래스인 monster타입으로 업케스팅, 오버라이딩이 되어 같은 흐름으로 동작이 가능
  terrain_inst->allocate_monster(new_mon);
  terrain_inst->bost_monster(new_mon);
  mon_list.push_back(new_mon);
  mon_count++;
  return new_mon;
}
```



# ISP 인터페이스 분리 원칙 (Interface Segregation principle)
- ***인터페이스는 작고 섬세해야 한다. 클래스는 필요한 인터페이스만 구현해야 한다***
- C++에는 인터페이스는 없으나 순수 가상 함수로만 구현된 추상 클래스의 경우 인터페이스로 활용이 가능하다
- 즉 추상 클래스에서 보는 단일 책임 원칙으로 생각하면 쉽다



# DIP 의존성 역전 원칙 (Dependency Inversion principle)
- ***DIP는 상위 수준 모듈은 하위 수준 모듈에 의존해서는 안되며, 상하위 모듈 모두 추상 레이어(클래스)에 의존해야 한다***
  
