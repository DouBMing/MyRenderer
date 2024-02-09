#pragma once
#include "vector.h"
#include <vector>

struct Matrix4x4
{
public:
    Matrix4x4() : m({}) {}
    Matrix4x4(Vector4 r1, Vector4 r2, Vector4 r3, Vector4 r4) : m({r1, r2, r3, r4}) {}
    float determinant();
    Matrix4x4 inverse();
    Vector4& operator [](int index)
    {
        return m[index];
    }
    Vector4 GetRow(int index) const
    {
        return m[index];
    }
    void SetRow(int index, const Vector4& r)
    {
        m[index] = r;
    }
    Vector4 GetColumn(int index) const
    {
        return Vector4(m[0][index], m[1][index], m[2][index], m[3][index]);
    }
    void SetColumn(int index, Vector4 c)
    {
        m[0][index] = c[0];
        m[1][index] = c[1];
        m[2][index] = c[2];
        m[3][index] = c[3];
    }

    friend Matrix4x4 operator +(const Matrix4x4& m1, const Matrix4x4& m2)
    {
        return Matrix4x4(m1.GetRow(0) + m2.GetRow(0), m1.GetRow(1) + m2.GetRow(1),
        m1.GetRow(2) + m2.GetRow(2), m1.GetRow(3) + m2.GetRow(3));
    }
    friend Matrix4x4 operator -(const Matrix4x4& m)
    {
        return Matrix4x4(-m.GetRow(0), -m.GetRow(1), -m.GetRow(2), -m.GetRow(3));
    }
    friend Matrix4x4 operator -(const Matrix4x4& m1, const Matrix4x4& m2)
    {
        return Matrix4x4(m1.GetRow(0) - m2.GetRow(0), m1.GetRow(1) - m2.GetRow(1),
        m1.GetRow(2) - m2.GetRow(2), m1.GetRow(3) - m2.GetRow(3));
    }
    friend Matrix4x4 operator *(const Matrix4x4& m1, const Matrix4x4& m2)
    {
        Matrix4x4 m;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m[i][j] = m1.GetRow(i) * m2.GetColumn(j);
        return m;
    }
    friend Vector4 operator *(const Matrix4x4& m, const Vector4& v)
    {
        return Vector4(m.GetRow(0) * v, m.GetRow(1) * v, m.GetRow(2) * v, m.GetRow(3) * v);
    }
    friend std::vector<Vector4>& operator *(const Matrix4x4& m, std::vector<Vector4>& vertices)
    {
        for (int i = 0; i < vertices.size(); i++)
            vertices[i] = m * vertices[i];
        return vertices;
    }
    friend Matrix4x4 operator *(const Matrix4x4& m, float f)
    {
        return Matrix4x4(m.GetRow(0) * f, m.GetRow(1) * f, m.GetRow(2) * f, m.GetRow(3) * f);
    }
    friend Matrix4x4 operator /(const Matrix4x4& m, float f)
    {
        return Matrix4x4(m.GetRow(0) / f, m.GetRow(1) / f, m.GetRow(2) / f, m.GetRow(3) / f);
    }

    static Matrix4x4 identity()
    {
        return Matrix4x4({
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}});
    }
private:
    Vector4 m[4];   // 按行向量储存
};