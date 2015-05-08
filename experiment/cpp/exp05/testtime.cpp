#include <iostream>
#include <ctime>
using namespace std;

const int day = 24 * 60 * 60;

void now()
{
    time_t t;
    struct tm* m;
    time(&t);
    m = localtime(&t);
    cout << m->tm_year + 1900 << "/" << m->tm_mon + 1 << "/" << m->tm_mday << endl;

    char s[80];
    strftime(s, 80, "%Y/%m/%d %H:%M:%S", m);
    cout << s << endl;
}

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

    now();

    return 0;
}
