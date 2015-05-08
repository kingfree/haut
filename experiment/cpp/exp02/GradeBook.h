/* GradeBook.h */
#include <string>
using std::string;

class GradeBook
{
public:
    GradeBook(string, string);
    void setCourseName(string);
    string getCourseName();
    void setTeacherName(string);
    string getTeacherName();
    void displayMessage();

private:
    string courseName;
    string teacherName;
};
