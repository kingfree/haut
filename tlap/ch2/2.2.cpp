#include <iostream>
#include <cmath>
using namespace std;
int main() {
    for(int row = 1; row <= 7; row++) {
        for(int hashNum = 1; hashNum <= 4 - abs(4 - row); hashNum++)
            cout << "#";
        cout << "\n";
    }
    return 0;
}
