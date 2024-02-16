#pragma once
#include "vector.h"
#include "quaternion.h"
#include "matrix.h"

class Transform
{
public:
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;
    Transform(Vector3 position = Vector3(), Vector3 rotation = Vector3(), Vector3 scale = Vector3(1, 1, 1));
    ~Transform();
    Vector3 right();
    Vector3 up();
    Vector3 forward();
    Matrix4x4 localToWorldMatrix();
    Matrix4x4 worldToLocalMatrix();
};