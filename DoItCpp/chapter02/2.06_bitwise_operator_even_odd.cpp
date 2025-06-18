/*
 * 비트 연산자를 이용한 홀수 짝수 구분 

홀수는 무조건 2진수에 1에 해당하는 비트가 1이어야 한다. (나머지는 모두 짝수에 해당하는 비트)
그렇기에 숫자와 1을 and연산 했을 때 결과가 1이라면 홀수 (1 and 1 이므로)
결과가 0이라면 (0 and 1) 짝수이다. 
비트 연산을 이용하여 홀수 짝수를 구분할 수 있는 방법
 */
#include <iostream>

int main()
{
    int even_odd;
    std::cout << "홀수 짝수 구분 : ";
    std::cin >> even_odd;
    std::cout << "even_odd : " << (even_odd&1 ? "홀수" : "짝수") << '\n';
    return 0;
}