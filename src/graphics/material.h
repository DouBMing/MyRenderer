#pragma once
#include "bitmap.h"

class Material
{
public:
    Material(const std::string& name);
    ~Material();
    Color SampleKa(float u, float v);
    Color SampleKd(float u, float v);
    Color SampleKs(float u, float v);

    friend class Model;
private:
    std::string name;
    Vector3 Ka, Kd, Ks;
    float Ns, d;
    int illum;
    Bitmap* ambientMap;
    Bitmap* diffuseMap;
    Bitmap* specularMap;
};