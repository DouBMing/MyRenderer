#include "material.h"

Material::Material(const std::string& name) : name(name)
{
    Ka = Vector3(1, 1, 1);
    Kd = Vector3(1, 1, 1);
    Ks = Vector3(1, 1, 1);
    Ns = 1;
    d = 1;
    illum = 1;
    ambientMap = nullptr;
    diffuseMap = nullptr;
    specularMap = nullptr;
    bumpMap = nullptr;
}

Material::~Material()
{
    if (ambientMap != nullptr)
        delete ambientMap;
    if (diffuseMap != nullptr)
        delete diffuseMap;
    if (specularMap != nullptr)
        delete specularMap;
    if (bumpMap != nullptr)
        delete bumpMap;
}

Color Material::SampleKa(float u, float v)
{
    if (ambientMap != nullptr)
        return Color(Ka) * ambientMap->Get(ambientMap->width * u, ambientMap->height * v);
    return Color(Ka);
}

Color Material::SampleKd(float u, float v)
{
    if (diffuseMap == nullptr)
        return Color(Kd);
    return Color(Kd) * diffuseMap->Get(diffuseMap->width * u, diffuseMap->height * v);
}

Color Material::SampleKs(float u, float v)
{
    if (specularMap != nullptr)
        return Color(Ks) * specularMap->Get(specularMap->width * u, specularMap->height * v);
    return Color(Ks);
}