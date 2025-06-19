/*
* 프렌드 클래스, 함수 사용
*/
#include <iostream>
#include <windows.h>


class sangok
{
  public:
    sangok(int age, std::string number): age(age), number(number) {};

  private:
    int age = 100;
    std::string number;
    std::string hobby = "러닝/게임/독서 등";

    // 프렌드 클래스 선언 - oksang객체에서는 sangok객체 접근 지정자를 무시할 수 있다
    friend class oksang;
};

class minsu
{
  public:
    minsu(int age, std::string number): age(age), number(number) {};

  private:
    int age;
    std::string number;
    std::string hobby = "산책하기";

    // 프렌드 함수 - 전역 함수 minsu_hobby에서는 minsu의 접근 지정자를 무시할 수 있다 
    friend void minsu_hobby(const minsu &minsu);

};

void minsu_hobby(const minsu &minsu)
{
  std::cout << "minsu의 취미는 " << minsu.hobby << std::endl;
}


class oksang
{
  public:
    oksang(int age, std::string number): age(age), number(number) {};

    // sangok 클래스에 프렌드 클래스로 oksang이 선언되어 있기에 oksang에서 sangok객체의 접근 지정자를 무시할 수 있다
    void call_sangok(const sangok &sangok)
    {
      std::cout << "sangok의 취미는 " << sangok.hobby << std::endl;
    }

    // minsu는 프렌드 클래스를 선언하지 않아 private에 접근할 수 없다
    void call_minsu(const minsu &minsu)
    {
      std::cout << "minsu의 취미를 알 수 없습니다." << std::endl;
    }

  private:
    int age;
    std::string number; 
};

int main()
{
  sangok sangok(25, "010-1234-5678");
  minsu minsu(23, "010-4444-5555");
  oksang oksang(30, "010-1111-2222");

  oksang.call_sangok(sangok);
  oksang.call_minsu(minsu);
  minsu_hobby(minsu);

  std::cout << "\n\n";
  system("pause");
  return 0;
}