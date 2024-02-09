#pragma once
#include "vector.h"
#include "matrix.h"

class Transform
{
public:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Transform();
    ~Transform();
    Vector3 up();
    Vector3 forward();
    Vector3 right();
    Matrix4x4 localToWorldMatrix();
    Matrix4x4 worldToLocalMatrix();
    void Rotate();
};