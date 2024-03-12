#pragma once
#include <cmath>
#include <vector>
#include <sstream>

template<int n, typename T>
struct Vector
{
    T data[n] = {};

    Vector() {}
    Vector(std::vector<T> data)
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
    Vector<n, T> normalized() const
    {
        Vector<n, T> norm;
        float mag = this->magnitude();
        for (int i = 0; i < n; i++)
            norm.data[i] = this->data[i] / mag;
        return norm;
    }
    Vector<n, T>& Normalize()
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
    Vector<n, T>& operator +=(const Vector<n, T>& v)
    {
        for (int i = 0; i < n; i++)
            data[i] += v.data[i];
        return *this;
    }
    Vector<n, T>& operator -=(const Vector<n, T>& v)
    {
        for (int i = 0; i < n; i++)
            data[i] -= v.data[i];
        return *this;
    }
    Vector<n, T>& operator *=(float d)
    {
        for (int i = 0; i < n; i++)
            data[i] *= d;
        return *this;
    }
    Vector<n, T>& operator /=(float d)
    {
        for (int i = 0; i < n; i++)
            data[i] /= d;
        return *this;
    }
};

template<typename T>
T Clamp(T value, T min, T max)
{
    if (value > max)
        return max;
    if (value < min)
        return min;
    return value;
}

// 取反
template<int n, typename T>
Vector<n, T> operator -(const Vector<n, T>& v)
{
    Vector<n, T> result;
    for (int i = 0; i < n; i++)
        result[i] = -v[i];
    return result;
}

template<int n, typename T>
Vector<n, T> operator +(const Vector<n, T>& a, const Vector<n, T>& b)
{
    Vector<n, T> result;
    for (int i = 0; i < n; i++)
        result[i] = a[i] + b[i];
    return result;
}

template<int n, typename T>
Vector<n, T> operator -(const Vector<n, T>& a, const Vector<n, T>& b)
{
    Vector<n, T> result;
    for (int i = 0; i < n; i++)
        result[i] = a[i] - b[i];
    return result;
}
// 点乘
template<int n, typename T>
T operator *(const Vector<n, T>& lhs, const Vector<n, T>& rhs)
{
    T result = 0;
    for (int i = 0; i < n; i++)
        result += lhs[i] * rhs[i];
    return result;
}
// 数乘
template<int n, typename T>
Vector<n, T> operator *(const Vector<n, T>& a, float d)
{
    Vector<n, T> result;
    for (int i = 0; i < n; i++)
        result[i] = a[i] * d;
    return result;
}

template<int n, typename T>
Vector<n, T> operator *(float d, const Vector<n, T>& a)
{
    Vector<n, T> result;
    for (int i = 0; i < n; i++)
        result[i] = a[i] * d;
    return result;
}

template<int n, typename T>
Vector<n, T> operator /(const Vector<n, T>& a, float d)
{
    Vector<n, T> result;
    for (int i = 0; i < n; i++)
        result[i] = a[i] / d;
    return result;
}

template<int n, typename T>
std::istream& operator >>(std::istream& is, Vector<n, T>& v)
{
    for (int i = 0; i < n; i++)
        is >> v[i];
    return is;
}

template<int n, typename T>
std::ostream& operator <<(std::ostream& os, const Vector<n, T>& v)
{
    os << "(";
    for (int i = 0; i < n - 1; i++)
        os << v[i] << ", ";
    os << v[n - 1] << ")";
    return os;
}

template<typename T>
struct Vector<2, T>
{
    T x, y;

    Vector() : x(0), y(0) {}
    Vector(T x, T y) : x(x), y(y) {}
    float magnitude() const
    {
        return std::sqrt(x * x + y * y);
    }
    Vector<2, T> normalized() const
    {
        float mag = magnitude();
        return Vector<2, T>(x / mag, y / mag);
    }
    Vector<2, T>& Normalize()
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
    Vector<2, T>& operator +=(const Vector<2, T>& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vector<2, T>& operator -=(const Vector<2, T>& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Vector<2, T>& operator *=(float d)
    {
        x *= d;
        y *= d;
        return *this;
    }
    Vector<2, T>& operator /=(float d)
    {
        x /= d;
        y /= d;
        return *this;
    }
};

template<typename T>
struct Vector<3, T>
{
    T x, y, z;

    Vector() : x(0), y(0), z(0) {}
    Vector(T x, T y, T z) : x(x), y(y), z(z) {}
    Vector(Vector<2, T> v, T z) : x(v.x), y(v.y), z(z) {}
    float magnitude() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }
    Vector<3, T> normalized() const
    {
        float mag = magnitude();
        return Vector<3, T>(x / mag, y / mag, z / mag);
    }
    Vector<3, T>& Normalize()
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
    Vector<3, T>& operator +=(const Vector<3, T>& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vector<3, T>& operator -=(const Vector<3, T>& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    Vector<3, T>& operator *=(float d)
    {
        x *= d;
        y *= d;
        z *= d;
        return *this;
    }
    Vector<3, T>& operator /=(float d)
    {
        x /= d;
        y /= d;
        z /= d;
        return *this;
    }
    template<typename U>
    operator Vector<2, U>()
    {
        return Vector<2, U>(x, y);
    }
};

template<typename T>
struct Vector<4, T>
{
    T x, y, z, w;

    Vector() : x(0), y(0), z(0), w(0) {}
    Vector(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    Vector(Vector<3, T> v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}
    float magnitude() const
    {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }
    Vector<4, T> normalized() const
    {
        float mag = magnitude();
        return Vector<4, T>(x / mag, y / mag, z / mag, w / mag);
    }
    Vector<4, T>& Normalize()
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
    Vector<4, T>& operator +=(const Vector<4, T>& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }
    Vector<4, T>& operator -=(const Vector<4, T>& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }
    Vector<4, T>& operator *=(float d)
    {
        x *= d;
        y *= d;
        z *= d;
        w *= d;
        return *this;
    }
    Vector<4, T>& operator /=(float d)
    {
        x /= d;
        y /= d;
        z /= d;
        w /= d;
        return *this;
    }
    operator Vector<3, float>() const
    {
        return Vector<3, float>(x, y, z);
    }
};

using byte = unsigned char;
using Vector2Int = Vector<2, int>;
using Vector2 = Vector<2, float>;
using Vector3 = Vector<3, float>;
using Vector4 = Vector<4, float>;
using Color = Vector<4, byte>;

template<>
struct Vector<4, byte>
{
    byte B, G, R, A;
    
    Vector() : B(0), G(0), R(0), A(255) {}
    Vector(byte R, byte G, byte B) : B(B), G(G), R(R), A(255) {}
    Vector(byte R, byte G, byte B, byte A) : B(B), G(G), R(R), A(A) {}
    Vector(Vector3 c) : A(255)
    {
        R = Clamp(c.x * 255, 0.0f, 255.0f);
        G = Clamp(c.y * 255, 0.0f, 255.0f);
        B = Clamp(c.z * 255, 0.0f, 255.0f);
    }
    Vector(byte* data, int bit) : B(data[0]), G(data[1]), R(data[2]), A(data[3])
    {
        if (bit == 24)
        {
            B = data[0];
            G = data[1];
            R = data[2];
            A = 255;
        }
        else if(bit == 32)
        {
            B = data[0];
            G = data[1];
            R = data[2];
            A = data[3];
        }
    }
    byte& operator [](int index)
    {
        switch (index)
        {
            case 0 : return B;
            case 1 : return G;
            case 2 : return R;
            case 3 : return A;
            default : throw std::out_of_range("Invalid index");
        }
    }
    byte operator [](int index) const
    {
        switch (index)
        {
            case 0 : return B;
            case 1 : return G;
            case 2 : return R;
            case 3 : return A;
            default : throw std::out_of_range("Invalid index");
        }
    }
    Color& operator +=(const Color& c)
    {
        B = Clamp(B + c.B, 0, 255);
        G = Clamp(G + c.G, 0, 255);
        R = Clamp(R + c.R, 0, 255);
        A = Clamp(A + c.A, 0, 255);
        return *this;
    }
    Color& operator *=(const Color& c)
    {
        B *= c.B / 255.0f;
        G *= c.G / 255.0f;
        R *= c.R / 255.0f;
        A = Clamp(A + c.A, 0, 255);
        return *this;
    }
    Color& operator *=(float d)
    {
        B = Clamp(int(B * d), 0, 255);
        G = Clamp(int(G * d), 0, 255);
        R = Clamp(int(R * d), 0, 255);
        return *this;
    }
    static Color GetRandomColor()
    {
        return Color(std::rand()% 256, std::rand() % 256, std::rand() % 256);
    }
    static Color White;
    static Color Black;
    static Color Red;
    static Color Green;
    static Color Blue;
    static Color Yellow;
};

Color operator +(const Color& a, const Color& b);
Color operator *(const Color& a, const Color& b);
Color operator *(const Color& a, float d);
std::istream& operator >>(std::istream& is, Color& c);

// 叉乘
template<typename T>
float operator ^(const Vector<2, T>& lhs, const Vector<2, T>& rhs)
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
// 叉乘
template<typename T>
Vector<3, T> operator ^(const Vector<3, T>& lhs, const Vector<3, T>& rhs)
{
    return Vector<3, T>(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}

Vector3 BarycentricCoordinate(Vector2 p, Vector2 p1, Vector2 p2, Vector2 p3);
Vector3 BarycentricCoordinate(Vector3 p, Vector3 p1, Vector3 p2, Vector3 p3);
Vector3 Reflect(Vector3 in, Vector3 normal);
float Distance(Vector3 p1, Vector3 p2);
float Distance(Vector2 p1, Vector2 p2);
