/*
C++ 17 버전 이상으로 맞추고 해야 정상적으로 동작
filesystem을 이용하여 디렉토리 생성, 텍스트 파일 생성 및 수정
*/
#include <iostream>
#include <string>
#include <filesystem> // 파일 시스템 헤더 파일
#include <fstream>    // 파일 입출력 헤더 파일
using namespace std;

namespace fs = filesystem;

int main()
{
  // 절대 경로로 설정
  string path = "E:\\CppStudyNotes\\DoItCpp\\chapter11\\MyDirectory";
  fs::create_directories(path);


  ofstream outFile(path + "/test.txt");
  outFile << "Hello, File System!" << endl;
  outFile.close();

  cout << "생성 완료" << endl;

  
  return 0;
}