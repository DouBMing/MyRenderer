#pragma once
#include "object.h"

class Light : public Object
{
public:
    float intensity;
    Color color;
    
    Light(float intensity);
    Light(Vector3 position, Vector3 rotation, float intensity);
    Vector3 direction() const;
};