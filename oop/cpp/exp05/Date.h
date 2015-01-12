// Date.h
#include <string>
#include <ctime>

class Date
{
private:
    const static std::string WEEKS[8];

    time_t t;

    const static int DAY = 24 * 60 * 60;

public:
    Date(int, int, int);
    Date(int, int);
    Date(int);
    void displayDate();

    void setYear(int);
    void setMonth(int);
    void setDay(int);

    time_t getTime() const { return t; }

    int getYear() const;
    int getMonth() const;
    int getDay() const;

    Date& operator+(int);
    int operator-(const Date&);
    Date& operator-(int);

    int getWeekday() const;
    int getWeek() const;

    bool isLeap() const;

    int getDays() const;
    void setDays(int d);

    static std::string weekday2s(int weekday);
    static bool yearIsLeap(int year);
};
