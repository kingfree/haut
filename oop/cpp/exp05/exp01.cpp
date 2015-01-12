#include <iostream>
using namespace std;

#include "Date.h";

int main()
{
    Date t(2012, 12, 25);
    t.displayDate();
    cout << Date::weekday2s(t.getWeekday()) << endl;
    cout << (t.isLeap() ? "闰年" : "平年");
    cout << " 是" << t.getYear() << "年的第" << t.getWeek() << "周" << endl;

    Date s(2015, 1, 10);
    s.displayDate();
    cout << Date::weekday2s(s.getWeekday()) << endl;
    cout << (s.isLeap() ? "闰年" : "平年") << endl;

    cout << "相差" << s - t << "天" << endl;

    Date a = s - 40;
    s.displayDate();
    cout << " 减去" << 40 << "天 = ";
    a.displayDate();
    cout << endl;

    Date b = t + 20;
    t.displayDate();
    cout << " 加上" << 20 << "天 = ";
    b.displayDate();
    cout << endl;

    Date c(2015, 1);
    cout << c.getYear() << "年的第" << c.getDays() << "天 ";
    c.displayDate();
    cout << endl;

    Date d(2015, 128);
    cout << d.getYear() << "年的第" << d.getDays() << "天 ";
    d.displayDate();
    cout << endl;

    return 0;
}
