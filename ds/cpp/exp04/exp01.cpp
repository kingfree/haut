#include <iostream>
using namespace std;

#include "Date.h"

int main()
{
    Date t(2014, 12, 25);
    t.displayDate();

    t.setYear(2015);
    cout << "改年为" << 2015 << ": " << t.getYear() << endl;
    t.displayDate();

    t.setMonth(2);
    cout << "改月为" << 2 << ": " << t.getMonth() << endl;
    t.displayDate();

    t.setMonth(35); // 这里会自动设置为 1
    cout << "改月为" << 35 << ": " << t.getMonth() << endl;
    t.displayDate();

    t.setDay(18);
    cout << "改日为" << 18 << ": " << t.getDay() << endl;
    t.displayDate();

    return 0;
}
