#pragma once
#include "object.h"

enum LightType
{
    Directional,
    Point
};

std::istream& operator >>(std::istream& is, LightType& tLight);

class Light : public Object
{
public:
    Light(float intensity, Color c, LightType type);
    Light(float intensity, Color c, LightType type, Vector3 position, Vector3 rotation);
    Vector3 Direction(Vector3 worldPos) const;
    Color GetColor() const;

    static Color GetAmbient();
    static void SetAmbient(Color c);
private:
    float intensity;
    Color color;
    LightType type;
    static Color ambient;   // 环境光
};