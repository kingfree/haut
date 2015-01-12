// Date.h
#include <string>

class Date
{
private:
    int year;
    int month;
    int day;

    const static int MONTHS[2][13];
    const static std::string WEEKS[8];

public:
    Date(int, int, int);
    Date(int, int);
    void displayDate();

    void setYear(int);
    void setMonth(int);
    void setDay(int);

    int getYear() const { return year; };
    int getMonth() const { return month; };
    int getDay() const { return day; };

    Date& operator+(const int);
    int operator-(const Date&);
    Date& operator-(const int);

    int getWeekday() const;
    int getWeek() const

    bool isLeap() const;

    int getDays() const;
    void setDays(int d);

    static std::string weekday2s(int weekday);
    static bool yearIsLeap(int year);
};
