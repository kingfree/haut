#include <iostream>
using namespace std;
int main() {
    char outputChacter;
    enum modeType {UPPERCASE, LOWERCASE, PUNCTUATION};
    modeType mode = UPPERCASE;
    char digitChar;
    char punctuation[] = " !?,. ;\"' ";
    cout << "请输入一系列逗号分隔的数字编码：";
    // (i): 18,12312,171,763,98423,1208,216,11,500,18,241,0,32,20620,27,10
    // (o): Right? Yes!
    do {
        digitChar = cin.get();
        int number = digitChar - '0';
        digitChar = cin.get();
        while(digitChar != '\r' && digitChar != '\n' && digitChar != ',') {
            number = number * 10 + digitChar - '0';
            digitChar = cin.get();
        }
        switch(mode) {
            case UPPERCASE:
                number %= 27;
                outputChacter = number + 'A' - 1;
                if(number == 0) {
                    mode = LOWERCASE;
                    continue;
                }
                break;
            case LOWERCASE:
                number %= 27;
                outputChacter = number + 'a' - 1;
                if(number == 0) {
                    mode = PUNCTUATION;
                    continue;
                }
                break;
            case PUNCTUATION:
                number %= 9;
                outputChacter = punctuation[number];
                if(number == 0) {
                    mode = UPPERCASE;
                    continue;
                }
                break;
        }
        cout << outputChacter;
    } while(digitChar != '\r' && digitChar != '\n');
    cout << "\n";
    return 0;
}
