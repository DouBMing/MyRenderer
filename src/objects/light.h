#pragma once
#include "object.h"

class Light : public Object
{
public:
    Light(float intensity, Color c);
    Light(float intensity, Color c, Vector3 position, Vector3 rotation);
    Vector3 direction() const;
    Color GetColor() const;

    static Color GetAmbient();
    static void SetAmbient(Color c);
private:
    float intensity;
    Color color;
    static Color ambient;   // 环境光
};