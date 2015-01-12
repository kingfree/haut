// Date.cpp
#include <iostream>
using std::cout;
using std::endl;

#include <string>

#include <algorithm>
using std::min;
using std::max;

#include "Date.h"

const int Date::MONTHS[2][13] = {
    { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};
const std::string Date::WEEKS[8] = { "星期日",
    "星期一", "星期二", "星期三",
    "星期四", "星期五", "星期六"
};

Date::Date(int y, int m, int d)
{
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
    this->year = year;
}

void Date::setMonth(int m)
{
    if (m < 1 || m > 12) {
        m = 1;
    }
    this->month = m;
}

void Date::setDay(int day)
{
    if (day < 1 || day > Date::MONTHS[isLeap()][this->month]) {
        day = 1;
    }
    this->day = day;
}

void Date::displayDate()
{
    cout << year << "年" << month << "月" << day << "日";
}

Date& Date::operator+(const int day)
{
    return Date(this->year, this->getDays() + day);
}

int Date::operator-(const Date& rhs)
{
    int d = 0;
    for (int i = rhs.year; i < this->year; i++) {
        d += 365 + (int) yearIsLeap(i);
    }
    d += this->getDays();
    return d;
}

Date& Date::operator-(const int day)
{
    return Date(this->year, this->getDays() - day);
}

int Date::getWeekday() const
{
    int y = this->year;
    int m = this->month;
    int d = this->day;
    if (m < 3) {
        m += 12;
        y -= 1;
    }
    int week = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
    return (week + 1) % 7;
}

int Date::getWeek() const
{
    return this->getDays() / 7;
}

bool Date::isLeap() const
{
    return Date::yearIsLeap(this->year);
}

bool Date::yearIsLeap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

void Date::setDays(int d)
{
    int y = 365;
    while (d > y) {
        d -= y + (int) yearIsLeap(++this->year);
    }
    while (d < 0) {
        d += y + (int) yearIsLeap(--this->year);
    }

    int i;
    for (i = 0; i <= 12; i++) {
        if (d <= Date::MONTHS[isLeap()][i]) {
            break;
        }
        d -= Date::MONTHS[isLeap()][i];
    }
    this->setMonth(i);
    this->setDay(d);
}

int Date::getDays() const
{
    int d = this->day;
    for (int i = 1; i < this->month; i++) {
        d += Date::MONTHS[isLeap()][i];
    }
    return d;
}

std::string Date::weekday2s(int weekday)
{
    return Date::WEEKS[weekday % 7];
}
