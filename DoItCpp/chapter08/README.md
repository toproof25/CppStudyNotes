# Chapter8 객체지향 프로그래밍 - 객체지향을 돕는 기능들
1. 컴포지션과 어그리게이션
2. 가상 함수와 동적 바인딩
3. 추상 클래스와 정적 클래스 멤버

### Composition (컴포지션)
- 다른 클래스의 객체를 멤버 변수로 두어 사용하는 것
- 생명 주기는 포함하는 객체와 같다
- has-a 관계로 강한 소유가 된다
```cpp
// 상속이 아닌 컴포지션을 이용한 몬스터 클래스
class monster_composition {
 private:
    // 멤버 변수로 객체를 사용
    monster monster_base;
    character character_base;

 public:
    monster_composition() { cout << "저는 monster_composition 입니다" << endl; };
    ~monster_composition() {};

    void attack(player target_player);
    void attack_special(player target_player);
};

// 컴포지션을 이용하여 기존 일반 공격을 실행
void monster_composition::attack(player target_player) {
 monster_base.attack(target_player);
}

// 스페셜 공격은 오버라이딩이 아닌 함수를 정의하여 사용 (이름은 같게)
void monster_composition::attack_special(player target_player) {
 cout << "monster_composition 공격 : 데미지 - 15 hp" << endl;
}
```


### aggregation (어그리게이션)
- 컴포지션과는 다르게 포인터, 레퍼런스 변수를 멤버 변수로 두고 따로 생성된 객체를 참조하게 하여 사용하는 방식
- 생명 주기가 각자 따로다
```cpp
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
```

#### Delegation (위임)
- 특정 작업을 다른 객체에게 맡기는 것으로 컴포지션에서 자주 활용된다.
- 기존 상속의 경우 오버라이딩을 하거나 상위 객체의 함수를 실행하면 되나
- 컴포지션의 경우 객체를 따로 두기 때문에 해당 객체의 함수를 호출해야한다.


### Virtual (가상함수)
- 가상함수를 사용하면 객체의 가장 마지막에 오버라이드된 함수를 실행한다. 부모의 타입으로 자식을 참조하더라도 객체의 오버라이드된 함수를 동작하여 **다형성**을 유지할 수 있다.
- `virtual 함수이름(매개변수) {}`
>- 예를 들어 "휴대폰"이라는 클래스를 상속받는 "갤럭시", "아이폰"이 있다.
>- "휴대폰"의 *정보출력*이라는 함수는 "휴대폰 입니다"를 출력한다
>- "갤럭시"와 "아이폰"은 오버라이드하며 각 "갤럭시 입니다"라고 나온다.
>- 그럴 때 `Phone *test = &iphone` 즉 "휴대폰"자료형으로 자식인 아이폰을 참조할 때 *test.정보출력*함수를 실행한다면, 아이폰의 오버라이드된 함수가 아닌 부모의 함수가 호출된다.
>- **문제는 그렇게 되면 만일 휴대폰에 대한 자료를 처리할 때 `Iphone *iph`, `Galaxy *gal`로 각 타입을 구분하여 코드를 작성해야한다. 만일 모든 휴대폰 회사의 타입을 지정한다면 상당히 힘듦**
>- 가상함수를 사용하면  `Phone *test = &iphone` 로 작성했을 때 참조하는 객체의 최종 오버라이드된 함수를 실행하게 된다. 즉 하나의 타입으로 자식 타입을 처리하더라도 ***다형성***을 그대로 유지하며 사용할 수 있는 것
>- 가상함수를 사용했을 때 오버라이드된 함수가 아닌 상위 클래스의 함수를 호출하는 방법은 namespace를 사용한다
>- `_human.Human::print()` - `변수명.상위클래스이름::함수`

### virtual destructor (가상 함수 소멸자)
- 일반 상속 관계에서 소멸자의 경우 업케스팅이 발생했을 때 자식 소멸자가 호출되지 않는다.
- 자식 객체의 소멸자를 호출하기 위해서는 소멸자 앞에 `virtual` 키워드를 작성하여 가상 함수로 만들면 된다.

### Virtual Function Table (가상 함수 테이블)
- 가상 함수를 사용하는 객체는 가상 함수 테이블을 사용하여 함수를 호출한다
- 가상 함수 테이블은 `_vfptr`라는 포인터 변수가 자동으로 생겨 가상 함수 테이블을 가리킨다. 

### Pure Virtual Function (순수 가상 함수)
- `virtual 함수이름() = 0;`
- 가상 함수 선언 후 뒤에 `= 0`을 붙이면 순수 가상 함수로 자식 클래스에게 오버라이딩을 강제할 수 있다.

### abstract class (추상 클래스)
- 순수 가상 함수를 포함하는 클래스를 추상 클래스라고 의미한다
- 추상 클래스는 객체를 생성할 수 없으며, 자식 클래스에서 선언된다
- (순수 가상 함수는 오버라이딩을 해야 하기에 추상 클래스 그대로 객체가 될 수 없다)