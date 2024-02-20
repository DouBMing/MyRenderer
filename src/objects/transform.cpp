#include "transform.h"

Transform::Transform(Vector3 position, Vector3 rotation, Vector3 scale) 
    : position(position), rotation(rotation), scale(scale) {}

Transform::~Transform() {}

Vector3 Transform::right() const
{
    return rotation * Vector3(1, 0, 0);
}

Vector3 Transform::up() const
{
    return rotation * Vector3(0, 1, 0);
}

Vector3 Transform::forward() const
{
    return rotation * Vector3(0, 0, 1);
}

Matrix4x4 Transform::localToWorldMatrix() const
{
    return Matrix4x4::TRS(position, rotation, scale);
}