#include "vector.h"

Vector4::Vector4() : x(0), y(0), z(0), w(0) {}

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Vector4::Vector4(Vector3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

float Vector4::magnitude() const 
{
    return std::sqrt(x * x + y * y + z * z + w * w);
}

Vector4 Vector4::normalized() const
{
    float m = magnitude();
    return Vector4(x / m, y / m, z / m, w / m);
}

Vector4& Vector4::Normalize()
{
    return *this = *this / magnitude();
}

float& Vector4::operator [](int index)
{
    switch (index)
    {
        case 0 : return x;
        case 1 : return y;
        case 2 : return z;
        case 3 : return w;
        default : throw std::out_of_range("Invalid index.");
        }
}

float Vector4::operator [](int index) const
{
    switch (index)
    {
        case 0 : return x;
        case 1 : return y;
        case 2 : return z;
        case 3 : return w;
        default : throw std::out_of_range("Invalid index.");
    }
}

Vector4& Vector4::operator +=(const Vector4& v)
{
    return *this = *this + v;
}

Vector4& Vector4::operator -=(const Vector4& v)
{
    return *this = *this - v;
}

Vector4& Vector4::operator *=(float f)
{
    return *this = *this * f;
}

Vector4& Vector4::operator /=(float f)
{
    return *this = *this / f;
}

Vector4::operator Vector3() const
{
    return Vector3(x / w, y / w, z / w);
}

Vector4 operator +(const Vector4& a, const Vector4& b)
{
    return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Vector4 operator -(const Vector4& v)
{
    return Vector4(-v.x, -v.y, -v.z, -v.w);
}

Vector4 operator -(const Vector4& a, const Vector4& b)
{
    return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

float operator *(const Vector4& a, const Vector4& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vector4 operator *(const Vector4& a, float f)
{
    return Vector4(a.x * f, a.y * f, a.z * f, a.w * f);
}

Vector4 operator /(const Vector4& a, float f)
{
    return Vector4(a.x / f, a.y / f, a.z / f, a.w / f);
}

std::ostream& operator <<(std::ostream& s, Vector4 v)
{
    return s << "(" << v.x << ",\t" << v.y << ",\t" << v.z << ",\t" << v.w << ")";
}
