#include "transform.h"

Transform::Transform() : position(), rotation(), scale(1, 1, 1) {}

Transform::~Transform() {}

Matrix4x4 Transform::localToWorldMatrix()
{
    Matrix4x4 mLocalToWorldPoint = Matrix4x4({
        {1, 0, 0, position.x},
        {0, 1, 0, position.y},
        {0, 0, 1, position.z},
        {0, 0, 0, 1}});
    return mLocalToWorldPoint;
}