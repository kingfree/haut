#include <iostream>
#include <string>
using namespace std;

class OperaN
{
private:
    int num;

public:
    char mark;
    OperaN(int _x = 1) : num(_x) { getMark(); }
    char getMark()
    {
        static string marks = "+-*/";
        return mark = marks[num % 4];
    }
};

int main()
{
    int n;
    cin >> n;
    OperaN t(n);
    cout << t.getMark() << endl;
    cout << t.mark << endl;
    return 0;
}
