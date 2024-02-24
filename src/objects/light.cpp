#include "light.h"

Light::Light(float intensity, Color c) : intensity(intensity), color(c) {}

Light::Light(float intensity, Color c, Vector3 position, Vector3 rotation) : Object(position, rotation, Vector3(1, 1, 1)),
    intensity(intensity), color(c) {}

Vector3 Light::direction() const
{
    return -transform.forward();
}