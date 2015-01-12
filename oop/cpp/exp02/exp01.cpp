#include <iostream>
using namespace std;

class NumberA
{
private:
    int a, b;

public:
    NumberA(int _a, int _b) : a(_a), b(_b) {}
    int plus() { return a + b; }
    int minus() { return a - b; }
    int times() { return a * b; }
    int divide() { return a / b; }
    int mod() { return a % b; }
};

int main()
{
    int a, b;
    cin >> a >> b;
    NumberA t(a, b);
    cout << t.plus() << endl;
    cout << t.minus() << endl;
    cout << t.times() << endl;
    cout << t.divide() << endl;
    cout << t.mod() << endl;
    return 0;
}
