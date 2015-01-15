#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class SHAPE
{
public:
    virtual double GETX() const = 0;
    virtual double GETY() const = 0;
    virtual string GETNAME() = 0;
    virtual double AREA() const { return 0; };
    virtual double VOLUME() const { return 0; };
    virtual void SHOWSHAPINFO() = 0;

protected:
    double _x;
    double _y;
    double _vol;
    double _area;
};

class POINT : public SHAPE
{
public:
    POINT(double x, double y)
    {
        _x = x;
        _y = y;
    }

    virtual double GETX() const
    {
        return _x;
    }

    virtual double GETY() const
    {
        return _y;
    }

    string GETNAME()
    {
        return "点";
    }

    void SHOWSHAPINFO()
    {
        cout << "名称: " << GETNAME() << endl;
        cout << "坐标: (" << GETX() << ", " << GETY() << ")" << endl;
        cout << "面积: " << AREA() << endl;
        cout << "体积: " << VOLUME() << endl;
    }
};

class RECTANGLE : public POINT
{
public:
    RECTANGLE(double x, double y) : POINT(x, y)
    {
        _x = x;
        _y = y;
    }

    double AREA() const
    {
        return (_x * _y);
    }

    string GETNAME()
    {
        return "矩形";
    }
};

class ELLIPSE : public RECTANGLE
{
public:
    ELLIPSE(double x, double y) :RECTANGLE(x, y)
    {
        _x = x;
        _y = y;
    }

    double AREA() const
    {
        return M_PI * (_x / 2) * (_y / 2);
    }

    string GETNAME()
    {
        return "椭圆";
    }
};

class CIRCLE : public ELLIPSE
{
private:
    double radius;

public:
    CIRCLE(double r) :ELLIPSE(r, r), radius(r) {};

    double AREA() const
    {
        return M_PI * radius * radius;
    }

    string GETNAME()
    {
        return "圆";
    }

};

class CYLINDER : public CIRCLE
{
public:
    CYLINDER(double x, double y) :CIRCLE(x)
    {
        _x = x;
        _y = y;
    }

    double AREA() const
    {
        return 2 * M_PI * _x * _y;
    }

    double VOLUME() const
    {
        return M_PI * _x * _x * _y;
    }

    string GETNAME()
    {
        return "圆柱";
    }
};

int main()
{
    SHAPE *shape[5];
    shape[0] = new POINT(2, 6);
    shape[1] = new RECTANGLE(2, 6);
    shape[2] = new ELLIPSE(2, 6);
    shape[3] = new CIRCLE(2);
    shape[4] = new CYLINDER(2, 6);

    for (int i = 0; i < 5; i++) {
        cout << "shape[" << i << "]" << endl;
        shape[i]->SHOWSHAPINFO();
        cout << endl;
    }

    for (int i = 0; i < 5; i++) {
        delete shape[i];
    }
    return 0;
}
