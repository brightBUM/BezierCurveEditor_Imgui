#pragma once
#include<math.h>
class Point3f
{
public:
    float X, Y, Z;
    bool selected;
    Point3f() {}
    Point3f(float, float,float);
    static Point3f LerpPoint(Point3f p1, Point3f p2, float);
    static float Distance(Point3f p1, Point3f p2);
    Point3f operator+(const Point3f& pt)const;
    Point3f operator-(const Point3f& pt)const;
    Point3f operator-()const;
    Point3f operator*(const float& num);
    Point3f operator/(const float& num);
    ~Point3f();
};

