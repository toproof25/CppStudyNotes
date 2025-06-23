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

  // test.txt 파일 생성
  ofstream outFile(path + "/test.txt");
  outFile << "Hello, File System!" << endl;
  outFile.close();
  cout << "생성 완료 > 경로 : " + path  << endl;
  cout << endl;


  // 디렉토리 내 파일 확인
  // fs::directory_interator : 반복을 위한 반복자
  // fs::directory_entry : 디렉토리 내 항목에 대한 각 클래스
  cout << "파일 확인하기"  << endl;
  for (const fs::directory_entry &entry : fs::directory_iterator(path))
  {
    // 경로에 해당하는 것이 파일인지 확인
    if (entry.is_regular_file())
    {
      cout << entry.path().filename() << endl;
    }
  }
  cout << endl;

  // 파일 읽기
  cout << "파일 읽기"  << endl;
  ifstream inFile(path+"/test.txt");
  string line;
  while (getline(inFile, line))
  {
    cout << line << endl;
  }
  inFile.close();
  cout << endl;


  fs::remove_all(path);

  
  return 0;
}