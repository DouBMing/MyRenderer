#include "vector.h"

Color Color::White(255, 255, 255);
Color Color::Black(0, 0, 0);
Color Color::Red(255, 0, 0);
Color Color::Green(0, 255, 0);
Color Color::Blue(0, 0, 255);
Color Color::Yellow(255, 255, 0);

Vector3 BarycentricCoordinate(Vector2 p, Vector2 p1, Vector2 p2, Vector2 p3)
{
    float s = (p1 - p2) ^ (p3 - p2);
    float a = (p - p2) ^ (p3 - p2) / s;
    float b = (p - p3) ^ (p1 - p3) / s;
    return Vector3(a, b, 1 - a - b);
}

Vector3 BarycentricCoordinate(Vector3 p, Vector3 p1, Vector3 p2, Vector3 p3)
{
    float s = ((p1 - p2) ^ (p3 - p2)).magnitude();
    float a = ((p - p2) ^ (p3 - p2)).magnitude() / s;
    float b = ((p - p3) ^ (p1 - p3)).magnitude() / s;
    return Vector3(a, b, 1 - a - b);
}