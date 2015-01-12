// Date.cpp
#include <iostream>
using std::cout;
using std::endl;

#include <string>

#include <algorithm>
using std::min;
using std::max;

#include <ctime>

#include "Date.h"

const std::string Date::WEEKS[8] = { "星期日",
    "星期一", "星期二", "星期三",
    "星期四", "星期五", "星期六"
};

Date::Date(int num)
{
    time(&t);
    t = num;
}

Date::Date(int y, int m, int d)
{
    time(&t);
    setYear(y);
    setMonth(m);
    setDay(d);
}

Date::Date(int year, int days)
{
    setYear(year);
    setDays(days);
}

void Date::setYear(int year)
{
    struct tm *m = localtime(&t);
    m->tm_year = year - 1900;
    t = mktime(m);
}

void Date::setMonth(int mon)
{
    if (mon < 1 || mon > 12) {
        mon = 1;
    }
    struct tm *m = localtime(&t);
    m->tm_mon = mon - 1;
    t = mktime(m);
}

void Date::setDay(int day)
{
    if (day < 1 || day > 31) {
        day = 1;
    }
    struct tm *m = localtime(&t);
    m->tm_mday = day;
    t = mktime(m);
}

int Date::getYear() const
{
    struct tm *m = localtime(&t);
    return m->tm_year + 1900;
}

int Date::getMonth() const
{
    struct tm *m = localtime(&t);
    return m->tm_mon + 1;
}

int Date::getDay() const
{
    struct tm *m = localtime(&t);
    return m->tm_mday;
}

void Date::displayDate()
{
    cout << getYear() << "年" << getMonth() << "月" << getDay() << "日";
}

Date& Date::operator+(int day)
{
    static Date newdate(this->t + day * Date::DAY);
    return newdate;
}

Date& Date::operator-(int day)
{
    static Date newdate(this->t - day * Date::DAY);
    return newdate;
}

int Date::operator-(const Date& rhs)
{
    return (int) difftime(t, rhs.getTime()) / 60 / 60 / 24;
}

int Date::getWeekday() const
{
    struct tm *m = localtime(&t);
    return m->tm_wday;
}

int Date::getWeek() const
{
    return this->getDays() / 7;
}

bool Date::isLeap() const
{
    return Date::yearIsLeap(getYear());
}

bool Date::yearIsLeap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

void Date::setDays(int d)
{
    struct tm *m = localtime(&t);
    m->tm_mon = 0;
    m->tm_mday = 1;
    t = mktime(m);
    t += (d - 1) * Date::DAY;
}

int Date::getDays() const
{
    struct tm *m = localtime(&t);
    return m->tm_yday + 1;
}

std::string Date::weekday2s(int weekday)
{
    return Date::WEEKS[weekday % 7];
}
