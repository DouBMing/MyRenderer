#include "matrix.h"
#include <cmath>

Matrix4x4 MatrixTranslate(const Vector3& p)
{
    return Matrix4x4({
        {1, 0, 0, p.x},
        {0, 1, 0, p.y},
        {0, 0, 1, p.z},
        {0, 0, 0, 1}});
}

Matrix4x4 MatrixRotate(const Quaternion& q)
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

Matrix4x4 MatrixScale(const Vector3& s)
{
    return Matrix4x4({
        {s.x, 0, 0, 0},
        {0, s.y, 0, 0},
        {0, 0, s.z, 0},
        {0, 0, 0, 1}});
}

Matrix4x4 MatrixTRS(const Vector3& p, const Quaternion& q, const Vector3& s)
{
    return MatrixTranslate(p) * MatrixRotate(q) * MatrixScale(s);
}

Matrix4x4 MatrixLookAt(const Vector3& eye, const Vector3& forward, const Vector3& up)
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

Matrix4x4 MatrixOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
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

Matrix4x4 MatrixPerspective(float fov, float aspect, float zNear, float zFar)
{
    float tanHalfFov = std::tan(fov * M_PI / 180 / 2);
    float invFN = 1 / (zFar - zNear);
    return Matrix4x4({
        {1 / (aspect * tanHalfFov), 0, 0, 0},
        {0, 1 / tanHalfFov, 0, 0},
        {0, 0, (zNear + zFar) * invFN, -2 * zFar * zNear * invFN},
        {0, 0, 1, 0}});
}

Matrix4x4 MatrixFrustum(float left, float right, float bottom, float top, float zNear, float zFar)
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