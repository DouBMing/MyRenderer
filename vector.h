#pragma once
#include <cmath>
#include <sstream>

template<typename T> struct Vec3;

template<typename T>
struct Vec2
{
    T x, y;

    Vec2() : x(0), y(0) {}
    Vec2(T x, T y) : x(x), y(y) {}
    float magnitude() const { return std::sqrt(x * x + y * y); }
    Vec2<T> normalized() const
    {
        float m = magnitude();
        return Vec2<T>(x / m, y / m);
    }
    Vec2<T>& Normalize() { return *this = *this / magnitude(); }
    T& operator [](int index)
    {
        switch (index)
        {
            case 0 : return x;
            case 1 : return y;
            default : throw std::out_of_range("Invalid index");
        }
    }
    Vec2<T>& operator +=(const Vec2<T>& v) { return *this = *this + v; }
    Vec2<T>& operator -=(const Vec2<T>& v) { return *this = *this - v; }
    Vec2<T>& operator *=(float f) { return *this = *this * f; }
    Vec2<T>& operator /=(float f) { return *this = *this / f; }
    template<typename U>
    operator Vec2<U>() const { return Vec2<U>(x, y); }
    explicit operator Vec3<T>() const { return Vec3<T>(x, y, 0); }
};

template<typename T>
Vec2<T> operator +(const Vec2<T>& a, const Vec2<T>& b)
{
    return Vec2<T>(a.x + b.x, a.y + b.y);
}

template<typename T>
Vec2<T> operator -(const Vec2<T>& v)
{
    return Vec2<T>(-v.x, -v.y);
}

template<typename T>
Vec2<T> operator -(const Vec2<T>& a, const Vec2<T>& b)
{
    return Vec2<T>(a.x - b.x, a.y - b.y);
}

template<typename T>
T operator *(const Vec2<T>& a, const Vec2<T>& b)
{
    return a.x * b.x + a.y * b.y;
}

template<typename T>
Vec2<T> operator *(const Vec2<T>& a, float f)
{
    return Vec2<T>(a.x * f, a.y * f);
}

template<typename T>
Vec2<T> operator /(const Vec2<T>& a, float f)
{
    return Vec2<T>(a.x / f, a.y / f);
}

template<typename T>
T operator ^(const Vec2<T>& a, const Vec2<T>& b)
{
    return a.x * b.y - a.y * b.x;
}

template<typename T>
struct Vec3
{
    T x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
    Vec3(Vec2<T> v, T z) : x(v.x), y(v.y), z(z) {}
    float magnitude() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3<T> normalized() const
    {
        float m = magnitude();
        return Vec3<T>(x / m, y / m, z / m);
    }
    Vec3<T>& Normalize() { return *this = *this / magnitude(); }
    T& operator [](int index)
    {
        switch (index)
        {
            case 0 : return x;
            case 1 : return y;
            case 2 : return z;
            default : throw std::out_of_range("Invalid index.");
        }
    }
    Vec3<T>& operator +=(const Vec3<T>& v) { return *this = *this + v; }
    Vec3<T>& operator -=(const Vec3<T>& v){ return *this = *this - v; }
    Vec3<T>& operator *=(float f) { return *this = *this * f; }
    Vec3<T>& operator /=(float f) { return *this = *this / f; }
    operator Vec2<T>() const { return Vec2<T>(x, y); }
    template<typename U>
    operator Vec3<U>() const { return Vec3<U>(x, y, z); }
};

template<typename T>
Vec3<T> operator +(const Vec3<T>& a, const Vec3<T>& b)
{
    return Vec3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template<typename T>
Vec3<T> operator -(const Vec3<T>& v)
{
    return Vec3<T>(-v.x, -v.y, -v.z);
}

template<typename T>
Vec3<T> operator -(const Vec3<T>& a, const Vec3<T>& b)
{
    return Vec3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}
// 点乘
template<typename T>
T operator *(const Vec3<T>& a, const Vec3<T>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<typename T>
Vec3<T> operator *(const Vec3<T>& a, float f)
{
    return Vec3<T>(a.x * f, a.y * f, a.z * f);
}

template<typename T>
Vec3<T> operator /(const Vec3<T>& a, float f)
{
    return Vec3<T>(a.x / f, a.y / f, a.z / f);
}

// 叉乘
template<typename T>
Vec3<T> operator ^(const Vec3<T>& a, const Vec3<T>& b)
{
    return Vec3<T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

template<typename T>
std::istringstream& operator >>(std::istringstream& s, Vec3<T>& v)
{
    s >> v.x >> v.y >> v.z;
    return s;
}

template<typename T>
std::ostream& operator <<(std::ostream& s, Vec3<T>& v)
{
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return s;
}

typedef Vec2<int> Vector2Int;
typedef Vec2<float> Vector2;
typedef Vec3<int> Vector3Int;
typedef Vec3<float> Vector3;

struct Vector4
{
    float x, y, z, w;

    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vector4(Vector3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
    float magnitude() const { return std::sqrt(x * x + y * y + z * z + w * w); }
    Vector4 normalized() const
    {
        float m = magnitude();
        return Vector4(x / m, y / m, z / m, w / m);
    }
    Vector4& Normalize() { return *this = *this / magnitude(); }
    float& operator [](int index)
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
    float operator [](int index) const
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
    Vector4& operator +=(const Vector4& v) { return *this = *this + v; }
    Vector4& operator -=(const Vector4& v) { return *this = *this - v; }
    Vector4& operator *=(float f) { return *this = *this * f; }
    Vector4& operator /=(float f) { return *this = *this / f; }
    template<typename T>
    operator Vec3<T>() const { return Vec3<T>(x / w, y / w, z / w); }
    friend Vector4 operator +(const Vector4& a, const Vector4& b)
    {
        return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }
    friend Vector4 operator -(const Vector4& v)
    {
        return Vector4(-v.x, -v.y, -v.z, -v.w);
    }
    friend Vector4 operator -(const Vector4& a, const Vector4& b)
    {
        return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }
    // 点乘
    friend float operator *(const Vector4& a, const Vector4& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }
    friend Vector4 operator *(const Vector4& a, float f)
    {
        return Vector4(a.x * f, a.y * f, a.z * f, a.w * f);
    }
    friend Vector4 operator /(const Vector4& a, float f)
    {
        return Vector4(a.x / f, a.y / f, a.z / f, a.w / f);
    }
};