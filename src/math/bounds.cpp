#include "bounds.h"
#include <limits>

static float floatMin = std::numeric_limits<float>::min();
static float floatMax = std::numeric_limits<float>::max();

Bounds::Bounds()
{
    minPoint = Vector3(floatMax, floatMax, floatMax);
    maxPoint = Vector3(floatMin, floatMin, floatMin);
}

Bounds::Bounds(Vector3 minPoint, Vector3 maxPoint) : minPoint(minPoint), maxPoint(maxPoint) {}

Bounds::Bounds(Vector2 p1, Vector2 p2, Vector2 p3)
{
    minPoint.x = std::min(std::min(p1.x, p2.x), p3.x);
    minPoint.y = std::min(std::min(p1.y, p2.y), p3.y);
    maxPoint.x = std::max(std::max(p1.x, p2.x), p3.x);
    maxPoint.y = std::max(std::max(p1.y, p2.y), p3.y);
}

Bounds::Bounds(Vector3 p1, Vector3 p2, Vector3 p3)
{
    minPoint.x = std::min(std::min(p1.x, p2.x), p3.x);
    minPoint.y = std::min(std::min(p1.y, p2.y), p3.y);
    minPoint.z = std::min(std::min(p1.z, p2.z), p3.z);
    maxPoint.x = std::max(std::max(p1.x, p2.x), p3.x);
    maxPoint.y = std::max(std::max(p1.y, p2.y), p3.y);
    maxPoint.z = std::max(std::max(p1.z, p2.z), p3.z);
}

Vector3 Bounds::center()
{
    return (maxPoint + minPoint) / 2;
}

Vector3 Bounds::size()
{
    return maxPoint - minPoint;
}