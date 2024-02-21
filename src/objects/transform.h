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
    Vector3 right() const;
    Vector3 up() const;
    Vector3 forward() const;
    Matrix4x4 localToWorldMatrix() const;
    Matrix4x4 worldToLocalMatrix() const;
};