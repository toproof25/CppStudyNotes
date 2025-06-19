/*
* 코드 설명 작성
*/
#include <iostream>
#include <string>
#include <filesystem> // 파일 시스템 헤더 파일
#include <fstream>    // 파일 입출력 헤더 파일
using namespace std;

namespace fs = filesystem;

int main()
{
  fs::create_directories("MyDirectory");

  ofstream outFile("MyDirectory/test.txt");
  outFile << "Hello, File System!" << endl;
  outFile.close();

  
  return 0;
}