#pragma once
#include "vector.h"

struct Bounds
{
    Vector3 minPoint, maxPoint;
    // 新建一个minPoint(+inf, +inf, +inf), maxPoint(-inf, -inf, -inf)的边界框
    Bounds();
    Bounds(Vector3 minPoint, Vector3 maxPoint);
    Bounds(Vector3 p1, Vector3 p2, Vector3 p3);
    Bounds(Vector2 p1, Vector2 p2, Vector2 p3);
    void AddPoint(Vector3 p);
    Vector3 center();
    Vector3 size();
};