#pragma once
#include "vector.h"

struct AABB
{
    Vector3 minPoint;
    Vector3 maxPoint;
    AABB();
    AABB(Vector3 p1, Vector3 p2, Vector3 p3);
};