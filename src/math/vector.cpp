#include "vector.h"

Color Color::White(255, 255, 255);
Color Color::Black(0, 0, 0);
Color Color::Red(255, 0, 0);
Color Color::Green(0, 255, 0);
Color Color::Blue(0, 0, 255);
Color Color::Yellow(255, 255, 0);

Color operator +(const Color& a, const Color& b)
{
    Color c;
    c.B = Clamp(a.B + b.B, 0, 255);
    c.G = Clamp(a.G + b.G, 0, 255);
    c.R = Clamp(a.R + b.R, 0, 255);
    c.A = Clamp(a.A + b.A, 0, 255);
    return c;
}

Color operator *(const Color& a, const Color& b)
{
    Color c;
    c.B = a.B * b.B / 255.0f;
    c.G = a.G * b.G / 255.0f;
    c.R = a.R * b.R / 255.0f;
    c.A = Clamp(a.A + b.A, 0, 255);
    return c;
}

Color operator *(const Color& a, float d)
{
    Color c;
    c.B = Clamp(a.B * d, 0.0f, 255.0f);
    c.G = Clamp(a.G * d, 0.0f, 255.0f);
    c.R = Clamp(a.R * d, 0.0f, 255.0f);
    c.A = a.A;
    return c;
}

std::istream& operator >>(std::istream& is, Color& c)
{
    int r, g, b;
    is >> r >> g >> b;
    c.R = Clamp(r, 0, 255);
    c.G = Clamp(g, 0, 255);
    c.B = Clamp(b, 0, 255);
    c.A = 255;
    return is;
}

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