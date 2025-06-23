/*
string 라이브러리 기본 사용법과 기능들 익히기 
- 객체 초기화
- length(), size(), append(), empty(), find(), compare(), replace(), 
*/
#include <iostream>
#include <string> // string 문자열 라이브러리
using namespace std;

int main()
{
  // string 객체 초기화
  string str1("생성자 호출 초기화");
  string str2 = "추가할 문자열의 길이를 정할 수 있습니다";

  // length - 문자열의 길이 구하기 
  cout << str1.length() << endl;

  // size - 문자열의 메모리 크기 구하기
  cout << str1.size() << endl;

  // empty - 문자열이 비어있는 지 확인
  cout << (str1.empty() ? "비어있습니다" : "문자열이 존재합니다") << endl << endl;

  // append - 문자열을 추가하기
  str1.append("\n새로 추가한 첫번째 문자열입니다\n");
  cout << str1 << endl;

  // 한글은 UTF-8에서 한문자당 3byte를 차지하기에 "추가할"까지만 추가된다
  str1.append(str2, 0, 9);
  cout << str1 << endl;

  str1.append(str2, 10, str2.size());
  cout << str1 << endl;

  // find - 문자열 찾기 (찾는 문자열이 없으면 string::npos를 반환)
  // 실제로 static const size_t npos = -1;
  cout << "추가할 시작 위치 : " << str1.find("추가할") << endl; 
  cout << "없으면 npos를 반환 " << (str1.find("add") == string::npos) << endl << endl;

  // compare - 문자열 비교 (같으면 0, 대장 문자열이 더 크거나 첫문자가 크면 양수, 아니면 음수)
  string str3 = str2;
  cout << str2 << " compare " << str1 << " | " << str2.compare(str1) << endl << endl;
  cout << str2 << " compare " << str3 << " | " << str2.compare(str3) << endl << endl;
  cout << str2 << " == " << str3 << " | " << (str2==str3) << endl << endl;

  // replace - 문자열 교체 (시작 위치, 바꿀 문자열 크기, 문자열)
  str2.replace(0, 32, "리플레이스 된 문자열");
  cout << str2 << endl << endl;

  str1.replace(str1.find("추가할"), 39, "----replace로 바꾸었습니다----");
  cout << str1 << endl << endl;



  return 0;
}