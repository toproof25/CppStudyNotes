/*
cmath는 수학 연산을 위한 라이브러리로 삼각 함수, 절댓값, 로그 등 수학 연산이 가능하다.
*/

#include <iostream>
#include <cmath> // 수학 연산 라이브러리
using namespace std;

int main()
{
    // 게임 월드 좌표
    double playerX = 10.0, playerY = 20.0;
    double enemyX = 13.0, enemyY = 24.0;


    // sqrt: 제곱근 (거리 계산)
    // 피타고라스의 정리( a² + b² = c² )를 이용해 두 점 사이의 거리를 구한다.
    // 게임에서 가장 빈번하게 사용되는 연산 중 하나.
    double deltaX = enemyX - playerX; // X축 거리차
    double deltaY = enemyY - playerY; // Y축 거리차
    double distance = sqrt(deltaX * deltaX + deltaY * deltaY);

    /*
    거리 계산 및 활용
    - 구현: 두 오브젝트(플레이어, 적, 아이템 등)의 좌표 차이를 각각 제곱하여 더한 후, 제곱근을 구함.
    - 이유: AI가 플레이어를 추격할지, 스킬 사정거리에 들어왔는지, 총알이 적에게 맞았는지 등
             범위(Range)를 확인하는 모든 로직의 기반이 된다.
    - Tip: sqrt는 비교적 무거운 연산. 단순히 거리의 대소만 비교할 때는 굳이 sqrt를 사용하지 않고,
           거리의 '제곱'값(distance_squared = dx*dx + dy*dy)과 사정거리의 '제곱'값(range_squared)을
           비교하는 최적화를 매우 빈번하게 사용한다.
    */
    cout << "1. 플레이어와 적 사이의 거리: " << distance << endl << endl;


    // atan2: 두 점 사이의 각도 구하기 (아크탄젠트2)
    // 한 오브젝트가 다른 오브젝트를 '바라보게' 만들 때 결정적으로 사용된다.
    double angleRadian = atan2(deltaY, deltaX);
    double angleDegree = angleRadian * 180.0 / M_PI; // 라디안(radian)을 도(degree)로 변환

    /*
    특정 방향 바라보기 (Look At)
    - 구현: 두 좌표의 차이(deltaY, deltaX)를 atan2 함수에 넣어주면 두 점을 잇는 직선의 각도가 라디안 값으로 나온다.
    - 이유: 적 포탑이 플레이어를 향해 자동으로 회전하거나, 플레이어 캐릭터의 머리가 특정 아이템을 향해
             돌아보는 등의 '바라보기' 기능을 구현하는 데 표준적으로 사용된다.
             일반 atan(y/x)과 달리, atan2는 360도 모든 방향을 정확하게 표현하고 0으로 나누는 문제도 없어서 훨씬 안정적이다.
    */
    cout << "2. 플레이어가 적을 바라보는 각도: " << angleDegree << "도" << endl << endl;


    // sin, cos: 특정 각도로 이동하기 (삼각함수)
    // '각도'와 '속도'를 가지고 실제 이동할 x, y 좌표를 계산한다.
    double moveAngleDegree = 45.0; // 45도 방향으로
    double moveSpeed = 100.0;      // 100의 속도로 이동
    double moveAngleRadian = moveAngleDegree * M_PI / 180.0; // 각도를 라디안으로 변환
    
    double velocityX = moveSpeed * cos(moveAngleRadian);
    double velocityY = moveSpeed * sin(moveAngleRadian);

    /*
     투사체 발사, 원형 움직임
    - 구현: 이동 방향(각도)과 속력이 주어졌을 때, cos에 각도를 넣으면 x축 속도 성분, sin에 넣으면 y축 속도 성분을 얻는다.
    - 이유: 플레이어가 특정 각도로 총알을 발사하거나, 스킬을 시전할 때 그 투사체의 초기 속도를 계산하는 데 필수적이다.
             혹은 행성이 태양 주위를 도는 것처럼 원형 궤도를 따라 움직이는 오브젝트를 구현할 때도 사용된다.
    */
    cout << "3. 45도 방향, 100의 속도로 이동 시" << endl;
    cout << "   X축 속도: " << velocityX << ", Y축 속도: " << velocityY << endl << endl;

    
    // fabs (float abs): 부동소수점 실수의 절댓값
    // 방향은 무시하고 크기(magnitude)만 필요할 때 사용한다.
    double value = -123.45;
    double absValue = fabs(value);

    /*
     속력, 차이 계산
    - 구현: 속도(velocity)는 +/- 방향을 가지지만, 속력(speed)은 크기만 가지므로 절댓값을 취해 구한다.
    - 이유: 두 값의 차이를 계산할 때 누가 더 큰지 상관없이 순수한 차이값만 필요할 때 유용하다.
    */
    cout << "4. -123.45의 절댓값: " << absValue << endl << endl;


    // fmin, fmax: 두 값 중 최소/최대값 (값 제한-Clamping)
    // 특정 값의 범위를 강제로 제한할 때 매우 유용하다.
    double currentHealth = 80.0;
    double damage = 120.0;
    currentHealth = currentHealth - damage; // 체력이 -40이 됨
    currentHealth = fmax(0.0, currentHealth); // 체력이 0 미만으로 내려가지 않도록 강제함

    /*
     체력(HP), 마나(MP), 스태미나 등의 값 제한
    - 구현: fmax(최소값, 현재값) / fmin(최대값, 현재값) 형태로 사용.
    - 이유: 플레이어가 아무리 큰 데미지를 입어도 체력이 0 밑으로 내려가지 않게 하거나,
             최대 체력 이상으로 회복되지 않도록 값을 특정 범위 [최소, 최대] 안에 묶어두는(clamping) 역할을 한다.
    */
    cout << "5. 120의 데미지를 입은 후 체력: " << currentHealth << endl << endl;


    // pow: 거듭제곱
    // 다양한 물리 공식이나 게임 디자인 공식을 적용할 때 사용된다.
    double lightIntensity = 1.0;
    double lightDistance = 4.0;
    // 빛의 감쇠는 거리의 제곱에 반비례하는 경우가 많다 (Inverse-square law).
    double falloff = 1.0 / pow(lightDistance, 2);

    /*
     빛/소리의 감쇠, 비선형적인 값 변화
    - 구현: pow(밑, 지수) 형태로 사용한다.
    - 이유: 현실의 빛이나 소리는 거리에 따라 선형적으로 약해지지 않고, 거리의 제곱에 반비례해서 감쇠한다.
             이런 물리 현상을 묘사하거나, 캐릭터의 레벨업에 따른 능력치 상승폭을 비선형적으로 디자인하는 등
             단순한 덧셈, 곱셈으로 표현하기 힘든 공식을 구현할 때 사용된다.
    */
    cout << "6. 거리 4만큼 떨어진 곳에서의 빛 감쇠율: " << falloff << endl << endl;


    return 0;
}