#pragma once
#include "object.h"

class Light : public Object
{
public:
    float intensity;
    Color color;
    
    Light(float intensity, Color c);
    Light(float intensity, Color c, Vector3 position, Vector3 rotation);
    Vector3 direction() const;
};