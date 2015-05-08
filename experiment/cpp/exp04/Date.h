// Date.h
class Date
{
private:
    int year;
    int month;
    int day;

public:
    Date(int, int, int);
    void displayDate();

    void setYear(int y) { year = y; };
    void setMonth(int); // 需要特殊处理
    void setDay(int d) { day = d; };

    int getYear() const { return year; };
    int getMonth() const { return month; };
    int getDay() const { return day; };
};
