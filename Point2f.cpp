#include "Point2f.h"


Point3f::Point3f(float x, float y,float z) :X(x), Y(y),Z(z) {
    selected = false;
}

Point3f Point3f::LerpPoint(Point3f p1, Point3f p2, float t)
{
    Point3f result;
    return result = p1 * (1 - t) + p2 * t;
}

float Point3f::Distance(Point3f p1, Point3f p2)
{
    return sqrt(pow((p2.X - p1.X), 2) + pow((p2.Y - p1.Y), 2)+ pow((p2.Z - p1.Z), 2));
}

Point3f Point3f::operator+(const Point3f& pt) const
{
    Point3f result;
    result.X = this->X + pt.X;
    result.Y = this->Y + pt.Y;
    result.Z = this->Z + pt.Z;
    return result;
}
Point3f Point3f::operator-(const Point3f& pt) const
{
    Point3f result;
    result.X = this->X - pt.X;
    result.Y = this->Y - pt.Y;
    result.Z = this->Z - pt.Z;
    return result;
}
Point3f Point3f::operator-() const
{
    Point3f result;
    result.X = -this->X;
    result.Y = -this->Y;
    result.Z = -this->Z;
    return result;
}

Point3f Point3f::operator*(const float& num)
{
    this->X *= num;
    this->Y *= num;
    this->Z *= num;
    return Point3f(this->X, this->Y,this->Z);
}

Point3f Point3f::operator/(const float& num)
{
    this->X /= num;
    this->Y /= num;
    this->Z /= num;
    return Point3f(this->X, this->Y, this->Z);
}

Point3f::~Point3f()
{

}