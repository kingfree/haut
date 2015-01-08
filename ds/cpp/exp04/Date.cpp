// Date.cpp
#include <iostream>
using std::cout;
using std::endl;

#include "Date.h"

Date::Date(int y, int m, int d)
{
    setYear(y);
    setMonth(m);
    setDay(d);
}

void Date::setMonth(int m)
{
    if (m < 1 || m > 12) {
        m = 1;
    }
    month = m;
}

void Date::displayDate()
{
    cout << year << '/' << month << '/' << day << endl;
}
