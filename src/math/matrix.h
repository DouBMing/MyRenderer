#pragma once
#include "vector.h"
#include "quaternion.h"
#include <vector>

template<int row, int col>
struct Matrix
{
    Matrix() {}
    Matrix(std::vector<Vector<col, float>> rows[row])
    {
        for (int i = 0; i < row; i++)
            m[i] = rows[i];
    }
    Matrix<col, row> transpose() const
    {
        Matrix<col, row> mat;
        for (int i = 0; i < col; i++)
        {
            for (int j = 0; j < row; j++)
                mat.m[i][j] = m[j][i];
        }
        return mat;
    }
    Vector<col, float>& operator [](int index)
    {
        return m[index];
    }
    Vector<col, float> GetRow(int index) const
    {
        return m[index];
    }
    void SetRow(int index, const Vector<col, float>& r)
    {
        m[index] = r;
    }
    Vector<row, float> GetColumn(int index) const
    {
        Vector<row, float> c;
        for (int i = 0; i < row; i++)
            c[i] = m[i][index];
        return c;
    }
    void SetColumn(int index, Vector<row, float> c)
    {
        for (int i = 0; i < row; i++)
            m[i][index] = c[i];
    }
    Matrix<row, col>& operator +=(const Matrix<row, col>& m)
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
                m[i][j] += m.m[i][j];
        }
        return *this;
    }
    Matrix<row, col>& operator -=(const Matrix<row, col>& m)
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
                m[i][j] -= m.m[i][j];
        }
        return *this;
    }
    Matrix<row, col>& operator *=(float d)
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
                m[i][j] *= d;
        }
        return *this;
    }
    Matrix<row, col>& operator /=(float d)
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
                m[i][j] /= d;
        }
        return *this;
    }
    template<int r, int c>
    explicit operator Matrix<r, c>()
    {
        Matrix<r, c> mat;
        for (int i = 0; i < std::min(r, row); i++)
        {
            for (int j = 0; j < std::min(c, col); j++)
            {
                mat[i][j] = m[i][j];
            }
        }
        return mat;
    }

    template<int r, int c>
    friend Matrix<r, c> operator -(const Matrix<r, c>& m);
    template<int r, int c>
    friend Matrix<r, c> operator +(const Matrix<r, c>& m1, const Matrix<r, c>& m2);
    template<int r, int c>
    friend Matrix<r, c> operator -(const Matrix<r, c>& m1, const Matrix<r, c>& m2);
    template<int r, int c, int n> 
    friend Matrix<r, c> operator *(const Matrix<r, n>& lhs, const Matrix<n, c>& rhs);
    template<int r, int c>
    friend Vector<c, float> operator *(const Matrix<r, c>& lhs, const Vector<c, float>& v);
    template<int r, int c>
    friend Vector<r, float> operator *(const Vector<r, float>& v, const Matrix<r, c>& rhs);
    template<int r, int c>
    friend Matrix<r, c> operator *(const Matrix<r, c>& m, float d);
    template<int r, int c>
    friend Matrix<r, c> operator /(const Matrix<r, c>& m, float d);
    template<int r, int c>
    friend std::ostream& operator <<(std::ostream& os, const Matrix<r, c>& m);
private:
    Vector<col, float> m[row];   // 按行向量储存
};

template<int row, int col>
Matrix<row, col> operator -(const Matrix<row, col>& m)
{
    Matrix<row, col> mat;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            mat.m[i][j] = -m.m[i][j];
    }
    return mat;
}

template<int row, int col>
Matrix<row, col> operator +(const Matrix<row, col>& m1, const Matrix<row, col>& m2)
{
    Matrix<row, col> mat;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            mat.m[i][j] = m1.m[i][j] + m2.m[i][j];
    }
    return mat;
}

template<int row, int col>
Matrix<row, col> operator -(const Matrix<row, col>& m1, const Matrix<row, col>& m2)
{
    Matrix<row, col> mat;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            mat.m[i][j] = m1.m[i][j] - m2.m[i][j];
    }
    return mat;
}

template<int row, int col, int n>
Matrix<row, col> operator *(const Matrix<row, n>& lhs, const Matrix<n, col>& rhs)
{
    Matrix<row, col> mat;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            for (int k = 0; k < n; k++)
                mat.m[i][j] += lhs.m[i][k] * rhs.m[k][j];
        }
    }
    return mat;
}

template<int row, int col>
Vector<col, float> operator *(const Matrix<row, col>& lhs, const Vector<col, float>& v)
{
    Vector<col, float> vec;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            vec[i] += lhs.m[i][j] * v[j];
    }
    return vec;
}

template<int row, int col>
Vector<row, float> operator *(const Vector<row, float>& v, const Matrix<row, col>& rhs)
{
    Vector<row, float> vec;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            vec[i] += v[j] * rhs.m[j][i];
    }
    return vec;
}

template<int row, int col>
Matrix<row, col> operator *(const Matrix<row, col>& m, float d)
{
    Matrix<row, col> mat;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            mat.m[i][j] = m.m[i][j] * d;
    }
    return mat;
}

template<int row, int col>
Matrix<row, col> operator /(const Matrix<row, col>& m, float d)
{
    Matrix<row, col> mat;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            mat.m[i][j] = m.m[i][j] / d;
    }
    return mat;
}

template<int row, int col>
std::ostream& operator <<(std::ostream& os, const Matrix<row, col>& m)
{
    for (int i = 0; i < row - 1; i++)
        os << m.m[i] << "\n";
    os << m.m[row - 1];
    return os;
}

template<int n>
struct Matrix<n, n>
{
    Matrix() {}
    Matrix(std::vector<Vector<n, float>> rows)
    {
        for (int i = 0; i < n; i++)
            m[i] = rows[i];
    }
    Matrix<n, n> transpose() const
    {
        Matrix<n, n> trans;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                trans.m[i][j] = m[j][i];
        }
        return trans;
    }
    float determinant() const
    {
        if (n == 2)
            return m[0][0] * m[1][1] -  m[0][1] * m[1][0];
        float det = 0;
        for (int i = 0; i < n; i++)
            det += m[0][i] * cofactor(0, i);
        return det;
    }
    // 去除第r行、第c列后的代数余子式
    float cofactor(int r, int c) const
    {
        if (n == 2)
            return (r != c ? -1 : 1) * m[!r][!c];
        return ((r + c) % 2 ? -1 : 1) * submatrix(r, c).determinant();
    }
    // 去除第r行、第c列后的子矩阵
    Matrix<n - 1, n - 1> submatrix(int r, int c) const
    {
        Matrix<n - 1, n - 1> sub;
        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - 1; j++)
                sub[i][j] = m[i < r ? i : i + 1][j < c ? j : j + 1];
        }
        return sub;
    }
    // 伴随矩阵
    Matrix<n, n> adjugate() const
    {
        Matrix<n, n> adj;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                adj[j][i] = cofactor(i, j);
        }
        return adj;
    }
    // 逆矩阵
    Matrix<n, n> inverse() const
    {
        float det = determinant();
        return adjugate() / det;
    }
    Vector<n, float>& operator [](int index)
    {
        return m[index];
    }
    Vector<n, float> GetRow(int index) const
    {
        return m[index];
    }
    void SetRow(int index, const Vector<n, float>& r)
    {
        m[index] = r;
    }
    Vector<n, float> GetColumn(int index) const
    {
        Vector<n, float> c;
        for (int i = 0; i < n; i++)
            c[i] = m[i][index];
        return c;
    }
    void SetColumn(int index, Vector<n, float> c)
    {
        for (int i = 0; i < n; i++)
            m[i][index] = c[i];
    }
    Matrix<n, n>& operator +=(const Matrix<n, n>& m)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                m[i][j] += m.m[i][j];
        }
        return *this;
    }
    Matrix<n, n>& operator -=(const Matrix<n, n>& m)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                m[i][j] -= m.m[i][j];
        }
        return *this;
    }
    Matrix<n, n>& operator *=(float d)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                m[i][j] *= d;
        }
        return *this;
    }
    Matrix<n, n>& operator /=(float d)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                m[i][j] /= d;
        }
        return *this;
    }
    template<int r, int c>
    explicit operator Matrix<r, c>()
    {
        Matrix<r, c> mat;
        for (int i = 0; i < std::min(r, n); i++)
        {
            for (int j = 0; j < std::min(c, n); j++)
            {
                mat[i][j] = m[i][j];
            }
        }
        return mat;
    }

    static Matrix<n, n> identity()
    {
        Matrix<n, n> mat;
        for (int i = 0; i < n; i++)
            mat.m[i][i] = 1;
        return mat;
    }

    template<int r, int c>
    friend Matrix<r, c> operator -(const Matrix<r, c>& m);
    template<int r, int c>
    friend Matrix<r, c> operator +(const Matrix<r, c>& m1, const Matrix<r, c>& m2);
    template<int r, int c>
    friend Matrix<r, c> operator -(const Matrix<r, c>& m1, const Matrix<r, c>& m2);
    template<int r, int c, int t> 
    friend Matrix<r, c> operator *(const Matrix<r, t>& lhs, const Matrix<t, c>& rhs);
    template<int r, int c>
    friend Vector<c, float> operator *(const Matrix<r, c>& lhs, const Vector<c, float>& v);
    template<int r, int c>
    friend Vector<r, float> operator *(const Vector<r, float>& v, const Matrix<r, c>& rhs);
    template<int r, int c>
    friend Matrix<r, c> operator *(const Matrix<r, c>& m, float d);
    template<int r, int c>
    friend Matrix<r, c> operator /(const Matrix<r, c>& m, float d);
    template<int r, int c>
    friend std::ostream& operator <<(std::ostream& os, const Matrix<r, c>& m);
private:
    Vector<n, float> m[n];   // 按行向量储存
};

template<>
struct Matrix<1, 1>
{
    Matrix() {}
    Matrix(float value)
    {
        m[0] = value;
    }
    Matrix<1, 1> transpose() const
    {
        return *this;
    }
    float determinant() const
    {
        return m[0];
    }
    // 逆矩阵
    Matrix<1, 1> inverse() const
    {
        return Matrix<1, 1>(1 / m[0]);
    }
    Vector<1, float>& operator [](int index)
    {
        return m;
    }
    static Matrix<1, 1> identity()
    {
        return Matrix<1, 1>(1);
    }
private:
    Vector<1, float> m;
};

using Matrix4x4 = Matrix<4, 4>;
using Matrix3x3 = Matrix<3, 3>;

Matrix4x4 MatrixTranslate(const Vector3& p);
Matrix4x4 MatrixRotate(const Quaternion& q);
Matrix4x4 MatrixScale(const Vector3& s);
Matrix4x4 MatrixTRS(const Vector3& p, const Quaternion& q, const Vector3& s);
Matrix4x4 MatrixLookAt(const Vector3& eye, const Vector3& forward, const Vector3& up);
Matrix4x4 MatrixOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
Matrix4x4 MatrixPerspective(float fov, float aspect, float zNear, float zFar);
Matrix4x4 MatrixFrustum(float left, float right, float bottom, float top, float zNear, float zFar);