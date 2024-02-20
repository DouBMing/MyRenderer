#pragma once
#include <cmath>
#include <sstream>

template<int n, typename T>
struct Vec
{
    T data[n];

    Vec() : data({}) {}
    Vec(T data[n])
    {
        for (int i = 0; i < n; i++)
            this->data[i] = data[i];
    }
    float magnitude() const
    {
        float mag = 0;
        for (int i = 0; i < n; i++)
            mag += data[i] * data[i];
        return std::sqrt(mag);
    }
    Vec<n, T> normalized() const
    {
        Vec<n, T> result;
        float mag = this->magnitude();
        for (int i = 0; i < n; i++)
            result.data[i] = this->data[i] / mag;
        return result;
    }
    Vec<n, T>& Normalize()
    {
        float mag = this->magnitude();
        for (int i = 0; i < n; i++)
            data[i] /= mag;
        return *this;
    }
    T& operator [](int index)
    {
        if (index < 0 || index >= n)
            throw std::out_of_range("Invalid index");
        return data[index];
    }
    T operator [](int index) const
    {
        if (index < 0 || index >= n)
            throw std::out_of_range("Invalid index");
        return data[index];
    }
    Vec<n, T>& operator +=(const Vec<n, T>& v)
    {
        for (int i = 0; i < n; ++i)
            data[i] += v.data[i];
        return *this;
    }
    Vec<n, T>& operator -=(const Vec<n, T>& v)
    {
        for (int i = 0; i < n; ++i)
            data[i] -= v.data[i];
        return *this;
    }
    Vec<n, T>& operator *=(float d)
    {
        for (int i = 0; i < n; ++i)
            data[i] *= d;
        return *this;
    }
    Vec<n, T>& operator /=(float d)
    {
        for (int i = 0; i < n; ++i)
            data[i] /= d;
        return *this;
    }
};

// 取反
template<int n, typename T>
Vec<n, T> operator -(const Vec<n, T>& v)
{
    Vec<n, T> result;
    for (int i = 0; i < n; i++)
        result.data[i] = -v.data[i];
    return result;
}

template<int n, typename T>
Vec<n, T> operator +(const Vec<n, T>& a, const Vec<n, T>& b)
{
    Vec<n, T> result;
    for (int i = 0; i < n; i++)
        result.data[i] = a.data[i] + b.data[i];
    return result;
}

template<int n, typename T>
Vec<n, T> operator -(const Vec<n, T>& a, const Vec<n, T>& b)
{
    Vec<n, T> result;
    for (int i = 0; i < n; i++)
        result.data[i] = a.data[i] - b.data[i];
    return result;
}
// 点乘
template<int n, typename T>
T operator *(const Vec<n, T>& lhs, const Vec<n, T>& rhs)
{
    T result = 0;
    for (int i = 0; i < n; i++)
        result += lhs.data[i] * rhs.data[i];
    return result;
}
// 数乘
template<int n, typename T>
Vec<n, T> operator *(const Vec<n, T>& a, float d)
{
    Vec<n, T> result;
    for (int i = 0; i < n; i++)
        result.data[i] = a.data[i] * d;
    return result;
}

template<int n, typename T>
Vec<n, T> operator /(const Vec<n, T>& a, float d)
{
    Vec<n, T> result;
    for (int i = 0; i < n; i++)
        result.data[i] = a.data[i] / d;
    return result;
}

template<int n, typename T>
std::istringstream& operator >>(std::istringstream& iss, Vec<n, T>& v)
{
    for (int i = 0; i < n; i++)
        iss >> v.data[i];
    return iss;
}

template<int n, typename T>
std::ostream& operator <<(std::ostream& os, const Vec<n, T>& v)
{
    os << "(";
    for (int i = 0; i < n - 1; i++)
        os << v.data[i] << ", ";
    os << v.data[n - 1] << ")";
    return os;
}

template<typename T>
struct Vec<2, T>
{
    union
    {
        T data[2];
        struct { T x, y; };
    };

    Vec() : x(0), y(0) {}
    Vec(T x, T y) : x(x), y(y) {}
    float magnitude() const
    {
        return std::sqrt(x * x + y * y);
    }
    Vec<2, T> normalized() const
    {
        float m = magnitude();
        return Vec<2, T>(x / m, y / m);
    }
    Vec<2, T>& Normalize()
    {
        return *this = *this / magnitude();
    }
    T& operator [](int index)
    {
        switch (index)
        {
            case 0 : return x;
            case 1 : return y;
            default : throw std::out_of_range("Invalid index");
        }
    }
    T operator [](int index) const
    {
        switch (index)
        {
            case 0 : return x;
            case 1 : return y;
            default : throw std::out_of_range("Invalid index");
        }
    }
    Vec<2, T>& operator +=(const Vec<2, T>& v)
    {
        return *this = *this + v;
    }
    Vec<2, T>& operator -=(const Vec<2, T>& v)
    {
        return *this = *this - v;
    }
    Vec<2, T>& operator *=(float d)
    {
        return *this = *this * d;
    }
    Vec<2, T>& operator /=(float d)
    {
        return *this = *this / d;
    }
};

template<typename T>
struct Vec<3, T>
{
    union
    {
        T data[3];
        struct { T x, y, z; };
    };

    Vec() : x(0), y(0), z(0) {}
    Vec(T x, T y, T z) : x(x), y(y), z(z) {}
    Vec(Vec<2, T> v, T z) : x(v.x), y(v.y), z(z) {}
    float magnitude() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }
    Vec<3, T> normalized() const
    {
        float m = magnitude();
        return Vec<3, T>(x / m, y / m, z / m);
    }
    Vec<3, T>& Normalize()
    {
        return *this = *this / magnitude();
    }
    T& operator [](int index)
    {
        switch (index)
        {
            case 0 : return x;
            case 1 : return y;
            case 2 : return z;
            default : throw std::out_of_range("Invalid index");
        }
    }
    T operator [](int index) const
    {
        switch (index)
        {
            case 0 : return x;
            case 1 : return y;
            case 2 : return z;
            default : throw std::out_of_range("Invalid index");
        }
    }
    Vec<3, T>& operator +=(const Vec<3, T>& v)
    {
        return *this = *this + v;
    }
    Vec<3, T>& operator -=(const Vec<3, T>& v)
    {
        return *this = *this - v;
    }
    Vec<3, T>& operator *=(float d)
    {
        return *this = *this * d;
    }
    Vec<3, T>& operator /=(float d)
    {
        return *this = *this / d;
    }
    template<typename T>
    operator Vec<2, T>()
    {
        return Vec<2, T>(x, y);
    }
};

template<typename T>
struct Vec<4, T>
{
    union
    {
        T data[4];
        struct { T x, y, z, w; };
    };

    Vec() : x(0), y(0), z(0), w(0) {}
    Vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    Vec(Vec<3, T> v, T w) : x(v.x), y(v.y), z(v.z), w(v.w) {}
    float magnitude() const
    {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }
    Vec<4, T> normalized() const
    {
        float m = magnitude();
        return Vec<4, T>(x / m, y / m, z / m, w / m);
    }
    Vec<4, T>& Normalize()
    {
        return *this = *this / magnitude();
    }
    T& operator [](int index)
    {
        switch (index)
        {
            case 0 : return x;
            case 1 : return y;
            case 2 : return z;
            case 3 : return w;
            default : throw std::out_of_range("Invalid index");
        }
    }
    T operator [](int index) const
    {
        switch (index)
        {
            case 0 : return x;
            case 1 : return y;
            case 2 : return z;
            case 3 : return w;
            default : throw std::out_of_range("Invalid index");
        }
    }
    Vec<4, T>& operator +=(const Vec<4, T>& v)
    {
        return *this = *this + v;
    }
    Vec<4, T>& operator -=(const Vec<4, T>& v)
    {
        return *this = *this - v;
    }
    Vec<4, T>& operator *=(float d)
    {
        return *this = *this * d;
    }
    Vec<4, T>& operator /=(float d)
    {
        return *this = *this / d;
    }
    operator Vec<3, float>() const
    {
        return Vec<3, float>(x / w, y / w, z / w);
    }
};

using byte = unsigned char;
using Vector2Int = Vec<2, int>;
using Vector2 = Vec<2, float>;
using Vector3 = Vec<3, float>;
using Vector4 = Vec<4, float>;
using Color = Vec<4, byte>;

template<>
struct Vec<4, byte>
{
    byte B, G, R, A;
    Vec() : B(0), G(0), R(0), A(255) {}
    Vec(byte R, byte G, byte B) : B(B), G(G), R(R), A(255) {}
    Vec(byte R, byte G, byte B, byte A) : B(B), G(G), R(R), A(A) {}
    Vec(byte* data) : B(data[0]), G(data[1]), R(data[2]), A(data[3]) {}
    static Color GetRandomColor()
    {
        return Color(std::rand()% 256, std::rand() % 256, std::rand() % 256);
    }
};

// 叉乘
template<typename T>
float operator ^(const Vec<2, T>& lhs, const Vec<2, T>& rhs);
// 叉乘
template<typename T>
const Vec<3, T> operator ^(const Vec<3, T>& lhs, const Vec<3, T>& rhs);
Vector3 BarycentricCoordinate(Vector2 p, Vector2 p1, Vector2 p2, Vector2 p3);
Vector3 BarycentricCoordinate(Vector3 p, Vector3 p1, Vector3 p2, Vector3 p3);
float Distance(Vector3 p1, Vector3 p2);
float Distance(Vector2 p1, Vector2 p2);
