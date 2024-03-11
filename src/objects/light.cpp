#include "light.h"

Light::Light(float intensity, Color c) : intensity(intensity), color(c) {}

Light::Light(float intensity, Color c, Vector3 position, Vector3 rotation) : Object(position, rotation, Vector3(1, 1, 1)),
    intensity(intensity), color(c) {}

Vector3 Light::direction() const
{
    return -transform.forward();
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