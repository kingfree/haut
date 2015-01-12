#include <iostream>
#include <ctime>
using namespace std;

const int day = 24 * 60 * 60;

int main()
{
    time_t t;
    struct tm* m;

    time(&t);
    m = localtime(&t);
    cout << ctime(&t) << endl;

    m->tm_year = 2012 - 1900;
    t = mktime(m);
    cout << ctime(&t) << endl;

    t += 400 * day;
    cout << ctime(&t) << endl;

    return 0;
}
