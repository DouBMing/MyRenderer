#pragma once
#include "vector.h"

struct Quaternion
{
    float x, y, z, w;

    Quaternion();
    Quaternion(float x, float y, float z, float w);
    Quaternion(Vector3 eulerAngles);
    Vector3 eulerAngles();
    friend Vector3 operator *(Quaternion q, Vector3 v);
    friend Quaternion operator *(Quaternion lhs, Quaternion rhs);
    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);
};