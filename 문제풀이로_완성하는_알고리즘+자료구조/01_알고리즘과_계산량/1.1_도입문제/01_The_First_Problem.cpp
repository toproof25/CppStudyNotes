/*
문제 설명
- 정수 N이 주어졌을 때 한 변의 길이가 N인 정사각형의 넒이를 출력하는 프로그램을 작성하시오
*/
#include <iostream>
using namespace std;

int main()
{
cin.tie(0);
ios::sync_with_stdio(false);

int N;
cin >> N;
cout << N*N;
return 0;
}