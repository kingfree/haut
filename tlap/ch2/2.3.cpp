#include <iostream>
using namespace std;
int doubleDigitValue(int digit) {
    int doubleDigit = digit * 2;
    int sum;
    sum = doubleDigit >= 10 ? 1 + doubleDigit % 10 : doubleDigit;
    return sum;
}
int main() {
    char digit;
    int oddLengthChecksum = 0;
    int evenLengthChecksum = 0;
    int position = 1;
    cout << "请输入一个数：";
    while((digit = cin.get()) != '\r' && digit != '\n') {
        if(position % 2 == 0) {
            oddLengthChecksum += doubleDigitValue(digit - '0');
            evenLengthChecksum += digit - '0';
        } else {
            oddLengthChecksum += digit - '0';
            evenLengthChecksum += doubleDigitValue(digit - '0');
        }
        position++;
    }
    int checksum = position % 2 ? evenLengthChecksum : oddLengthChecksum;
    cout << "检查结果为 " << checksum << "。\n";
    if(checksum % 10 == 0)
        cout << "检查结果可整除 10，验证成功。\n";
    else
        cout << "检查结果不可整除 10，验证失败。\n";
    return 0;
}
