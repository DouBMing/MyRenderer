#include "matrix.h"
#include <cmath>

Matrix4x4::Matrix4x4() : m({}) {}

Matrix4x4::Matrix4x4(Vector4 r1, Vector4 r2, Vector4 r3, Vector4 r4) : m({r1, r2, r3, r4}) {}

Matrix4x4 Matrix4x4::transpose()
{
    return Matrix4x4(GetColumn(0), GetColumn(1), GetColumn(2), GetColumn(3));
}

Vector4& Matrix4x4::operator [](int index)
{
    return m[index];
}

Vector4 Matrix4x4::GetRow(int index) const
{
    return m[index];
}

void Matrix4x4::SetRow(int index, const Vector4& r)
{
    m[index] = r;
}

Vector4 Matrix4x4::GetColumn(int index) const
{
    return Vector4(m[0][index], m[1][index], m[2][index], m[3][index]);
}

void Matrix4x4::SetColumn(int index, Vector4 c)
{
    m[0][index] = c[0];
    m[1][index] = c[1];
    m[2][index] = c[2];
    m[3][index] = c[3];
}

Matrix4x4 operator +(const Matrix4x4& m1, const Matrix4x4& m2)
{
    return Matrix4x4(m1.GetRow(0) + m2.GetRow(0), m1.GetRow(1) + m2.GetRow(1),
    m1.GetRow(2) + m2.GetRow(2), m1.GetRow(3) + m2.GetRow(3));
}

Matrix4x4 operator -(const Matrix4x4& m)
{
    return Matrix4x4(-m.GetRow(0), -m.GetRow(1), -m.GetRow(2), -m.GetRow(3));
}

Matrix4x4 operator -(const Matrix4x4& m1, const Matrix4x4& m2)
{
    return Matrix4x4(m1.GetRow(0) - m2.GetRow(0), m1.GetRow(1) - m2.GetRow(1),
    m1.GetRow(2) - m2.GetRow(2), m1.GetRow(3) - m2.GetRow(3));
}

Matrix4x4 operator *(const Matrix4x4& m1, const Matrix4x4& m2)
{
    Matrix4x4 m;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] = m1.GetRow(i) * m2.GetColumn(j);
    return m;
}

Vector4 operator *(const Matrix4x4& m, const Vector4& v)
{
    return Vector4(m.GetRow(0) * v, m.GetRow(1) * v, m.GetRow(2) * v, m.GetRow(3) * v);
}

std::vector<Vector4>& operator *(const Matrix4x4& m, std::vector<Vector4>& vertices)
{
    for (int i = 0; i < vertices.size(); i++)
        vertices[i] = m * vertices[i];
    return vertices;
}

Matrix4x4 operator *(const Matrix4x4& m, float f)
{
    return Matrix4x4(m.GetRow(0) * f, m.GetRow(1) * f, m.GetRow(2) * f, m.GetRow(3) * f);
}

Matrix4x4 operator /(const Matrix4x4& m, float f)
{
    return Matrix4x4(m.GetRow(0) / f, m.GetRow(1) / f, m.GetRow(2) / f, m.GetRow(3) / f);
}

std::ostream& operator <<(std::ostream& s, const Matrix4x4& m)
{
    s << m.m[0] << "\n" << m.m[1] << "\n" << m.m[2] << "\n" << m.m[3];
    return s;
}

Matrix4x4 Matrix4x4::identity()
{
    return Matrix4x4({
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}});
}

Matrix4x4 Matrix4x4::Translate(Vector3 p)
{
    return Matrix4x4({
        {1, 0, 0, p.x},
        {0, 1, 0, p.y},
        {0, 0, 1, p.z},
        {0, 0, 0, 1}});
}

Matrix4x4 Matrix4x4::Rotate(Quaternion q)
{
    float x = q.x;
    float y = q.y;
    float z = q.z;
    float w = q.w;
    return Matrix4x4({
        {1 - 2 * y * y - 2 * z * z, 2 * x * y - 2 * z * w, 2 * x * z + 2 * y * w, 0},
        {2 * x * y + 2 * z * w, 1 - 2 * x * x - 2 * z * z, 2 * y * z - 2 * x * w, 0},
        {2 * x * z - 2 * y * w, 2 * y * z + 2 * x * w, 1 - 2 * x * x - 2 * y * y, 0},
        {0, 0, 0, 1}});
}

Matrix4x4 Matrix4x4::Scale(Vector3 s)
{
    return Matrix4x4({
        {s.x, 0, 0, 0},
        {0, s.y, 0, 0},
        {0, 0, s.z, 0},
        {0, 0, 0, 1}});
}

Matrix4x4 Matrix4x4::TRS(Vector3 p, Quaternion q, Vector3 s)
{
    return Translate(p) * Rotate(q) * Scale(s);
}

Matrix4x4 Matrix4x4::LookAt(Vector3 eye, Vector3 forward, Vector3 up)
{
    Matrix4x4 mRView = Matrix4x4({
        Vector4(-forward ^ up, 0),
        Vector4(up, 0),
        Vector4(forward, 0),
        {0, 0, 0, 1}});
    Matrix4x4 mTView = Matrix4x4({
        {1, 0, 0, -eye.x},
        {0, 1, 0, -eye.y},
        {0, 0, 1, -eye.z},
        {0, 0, 0, 1}});
    return mRView * mTView;
}

Matrix4x4 Matrix4x4::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
    float invRL = 1 / (right - left);
    float invTB = 1 / (top - bottom);
    float invFN = 1 / (zFar - zNear);
    return Matrix4x4({
        {2 * invRL, 0, 0, -(right + left) * invRL},
        {0, 2 * invTB, 0, -(top + bottom) * invTB},
        {0, 0, 2 * invFN, -(zFar + zNear) * invFN},
        {0, 0, 0, 1}});
}

Matrix4x4 Matrix4x4::Perspective(float fov, float aspect, float zNear, float zFar)
{
    float tanHalfFov = std::tan(fov * M_PI / 180 / 2);
    float invFN = 1 / (zFar - zNear);
    return Matrix4x4({
        {1 / (aspect * tanHalfFov), 0, 0, 0},
        {0, 1 / tanHalfFov, 0, 0},
        {0, 0, (zNear + zFar) * invFN, -2 * zFar * zNear * invFN},
        {0, 0, 1, 0}});
}

Matrix4x4 Matrix4x4::Frustum(float left, float right, float bottom, float top, float zNear, float zFar)
{
    float invRL = 1 / (right - left);
    float invTB = 1 / (top - bottom);
    float invFN = 1 / (zFar - zNear);
    return Matrix4x4({
        {2 * zNear * invRL, 0, -(right + left) * invRL, 0},
        {0, 2 * zNear * invTB, -(top + bottom) * invTB, 0},
        {0, 0, (zNear + zFar) * invFN, -2 * zFar * zNear * invFN},
        {0, 0, 1, 0}});
}