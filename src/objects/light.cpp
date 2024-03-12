#include "light.h"

std::istream& operator >>(std::istream& is, LightType& tLight)
{
    std::string sType;
    is >> sType;
    if (sType == "Directional")
        tLight = Directional;
    else if (sType == "Point")
        tLight = Point;
    return is;
}

Light::Light(float intensity, Color c, LightType type) : intensity(intensity), color(c), type(type) {}

Light::Light(float intensity, Color c, LightType type, Vector3 position, Vector3 rotation) : Object(position, rotation, Vector3(1, 1, 1)),
    intensity(intensity), color(c), type(type) {}

Vector3 Light::Direction(Vector3 worldPos) const
{
    switch (type)
    {
        case Directional:
            return -transform.forward();
        case Point:
            return transform.position - worldPos;
    }
    return Vector3();
}

Color Light::GetColor() const
{
    return color * intensity;
}

Color Light::ambient(54, 58, 66);

Color Light::GetAmbient()
{
    return ambient;
}

void Light::SetAmbient(Color c)
{
    ambient = c;
}