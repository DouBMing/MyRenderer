#include "vector.h"

template<typename T>
float operator ^(const Vec<2, T>& lhs, const Vec<2, T>& rhs)
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

template<typename T>
const Vec<3, T> operator ^(const Vec<3, T>& lhs, const Vec<3, T>& rhs)
{
    return Vec<3, T>(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}

Vector3 BarycentricCoordinate(Vector2 p, Vector2 p1, Vector2 p2, Vector2 p3)
{
    float s = (p1 - p2) ^ (p3 - p2);
    float a = (p - p2) ^ (p3 - p2) / s;
    float b = (p - p3) ^ (p1 - p3) / s;
    return Vector3(a, b, 1 - a - b);
}
