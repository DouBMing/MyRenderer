#include "light.h"

Light::Light(float intensity, Color c) : color(c)
{
    if (intensity > 1.0)
        this->intensity = 1.0;
    else
        this->intensity = intensity;
}

Light::Light(float intensity, Color c, Vector3 position, Vector3 rotation) : Object(position, rotation, Vector3(1, 1, 1)), color(c)
{
    if (intensity > 1.0)
        this->intensity = 1.0;
    else
        this->intensity = intensity;
}

Vector3 Light::direction() const
{
    return -transform.forward();
}