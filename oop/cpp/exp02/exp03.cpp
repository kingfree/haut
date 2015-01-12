#include <iostream>
using std::cout;
using std::endl;

#include "GradeBook.h"

int main()
{
    GradeBook gradeBook1("CS101 C++ 导论", "王");
    GradeBook gradeBook2("CS102 C++ 数据结构", "李");

    gradeBook1.displayMessage();
    gradeBook2.displayMessage();

    gradeBook1.setCourseName("CS101 C++ 编程");

    gradeBook1.displayMessage();
    gradeBook2.displayMessage();
    return 0;
}
