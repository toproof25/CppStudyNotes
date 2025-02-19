# LSP 리스코프 치환 원칙

### Liskov Substitution Principle
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