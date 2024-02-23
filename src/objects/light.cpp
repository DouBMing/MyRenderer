#include "light.h"

Light::Light(float intensity)
{
    if (intensity > 1.0)
        this->intensity = 1.0;
    else
        this->intensity = intensity;
}

Light::Light(Vector3 position, Vector3 rotation, float intensity) : Object(position, rotation)
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