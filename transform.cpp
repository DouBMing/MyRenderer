#include "transform.h"

Transform::Transform(Vector3 position, Vector3 rotation, Vector3 scale) 
    : position(position), rotation(rotation), scale(scale) {}

Transform::~Transform() {}

Vector3 Transform::right()
{
    return rotation * Vector3(1, 0, 0);
}

Vector3 Transform::up()
{
    return rotation * Vector3(0, 1, 0);
}

Vector3 Transform::forward()
{
    return rotation * Vector3(0, 0, 1);
}

Matrix4x4 Transform::localToWorldMatrix()
{
    return Matrix4x4::TRS(position, rotation, scale);
}