/* GradeBook.cpp */
#include <iostream>
using std::cout;
using std::endl;

#include "GradeBook.h"

GradeBook::GradeBook(string name, string teacher)
{
    setCourseName(name);
    setTeacherName(teacher);
}

void GradeBook::setCourseName(string name)
{
    if (name.length() <= 25)
        courseName = name;
    if (name.length() > 25) {
        courseName = name.substr(0, 25);
        cout << "名称\"" << name << "\" 长度超限 (25) 。\n"
            << "截取前 25 个字符。\n" << endl;
    }
}

string GradeBook::getCourseName()
{
    return courseName;
}

void GradeBook::setTeacherName(string name)
{
    if (name.length() <= 25)
        teacherName = name;
    if (name.length() > 25) {
        teacherName = name.substr(0, 25);
        cout << "姓名\"" << name << "\" 长度超限 (25) 。\n"
            << "截取前 25 个字符。\n" << endl;
    }
}

string GradeBook::getTeacherName()
{
    return teacherName;
}

void GradeBook::displayMessage()
{
    cout << "欢迎使用 " << getCourseName() << " 课程表！\n"
        << "任课教师：" << getTeacherName() << endl;
}
