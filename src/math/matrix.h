#pragma once
#include "vector.h"
#include "quaternion.h"
#include <vector>

struct Matrix4x4
{
public:
    Matrix4x4();
    Matrix4x4(Vector4 r1, Vector4 r2, Vector4 r3, Vector4 r4);
    float determinant();
    Matrix4x4 inverse();
    Matrix4x4 transpose();
    Vector4& operator [](int index);
    Vector4 GetRow(int index) const;
    void SetRow(int index, const Vector4& r);
    Vector4 GetColumn(int index) const;
    void SetColumn(int index, Vector4 c);

    friend Matrix4x4 operator +(const Matrix4x4& m1, const Matrix4x4& m2);
    friend Matrix4x4 operator -(const Matrix4x4& m);
    friend Matrix4x4 operator -(const Matrix4x4& m1, const Matrix4x4& m2);
    friend Matrix4x4 operator *(const Matrix4x4& m1, const Matrix4x4& m2);
    friend Vector4 operator *(const Matrix4x4& m, const Vector4& v);
    friend std::vector<Vector4>& operator *(const Matrix4x4& m, std::vector<Vector4>& vertices);
    friend Matrix4x4 operator *(const Matrix4x4& m, float f);
    friend Matrix4x4 operator /(const Matrix4x4& m, float f);
    friend std::ostream& operator <<(std::ostream& os, const Matrix4x4& m);

    static Matrix4x4 identity();
    static Matrix4x4 Translate(Vector3 p);
    static Matrix4x4 Rotate(Quaternion q);
    static Matrix4x4 Scale(Vector3 s);
    static Matrix4x4 TRS(Vector3 p, Quaternion q, Vector3 s);
    static Matrix4x4 LookAt(Vector3 eye, Vector3 forward, Vector3 up);
    static Matrix4x4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
    static Matrix4x4 Perspective(float fov, float aspect, float zNear, float zFar);
    static Matrix4x4 Frustum(float left, float right, float bottom, float top, float zNear, float zFar);
private:
    Vector4 m[4];   // 按行向量储存
};