#include "shader.h"
#include "model.h"
#include "camera.h"
#include "scene.h"

IShader::IShader(const Model* model, Camera& camera) : model(model)
{
    M = model->transform.localToWorldMatrix();
    V = camera.worldToViewMatrix();
    P = camera.projectionMatrix();
    worldToObject = model->transform.worldToLocalMatrix();
}

IShader::~IShader() {}


FlatShader::FlatShader(const Model* model, Camera& camera) : IShader(model, camera)
{
    worldCoords.resize(model->nVerts());
}

FlatShader::~FlatShader() {}

Vector4 FlatShader::vertex(int faceIdx, int i)
{
    // MVP变换
    Vector4 worldPos = M * Vector4(model->vert(faceIdx, i), 1);
    Face face = model->face(faceIdx);
    worldCoords[model->face(faceIdx).vi[i]] = worldPos;
    return P * V * worldPos;
}

Color FlatShader::fragment(Vector3 baryCoord, int faceIdx)
{
    std::vector<Light*>& lights = Scene::current->lights;
    Material* material = model->GetMaterial(faceIdx);
    Face face = model->face(faceIdx);
    // 计算法线
    Vector3 sa = Vector3(worldCoords[face.vi[1]] - worldCoords[face.vi[0]]);
    Vector3 sb = Vector3(worldCoords[face.vi[2]] - worldCoords[face.vi[0]]);
    Vector3 normal = (sa ^ sb).normalized();

    // 采样反射率
    Vector2 uv1 = model->texCoord(faceIdx, 0);
    Vector2 uv2 = model->texCoord(faceIdx, 1);
    Vector2 uv3 = model->texCoord(faceIdx, 2);
    float u = uv1[0] * baryCoord[0] + uv2[0] * baryCoord[1] + uv3[0] * baryCoord[2];
    float v = uv1[1] * baryCoord[0] + uv2[1] * baryCoord[1] + uv3[1] * baryCoord[2];
    Color albedo = material->SampleKd(u, v);

    // 环境光
    Color ambient = Light::GetAmbient() * albedo;
    // 漫反射
    Color diffuse;
    for (Light* light : Scene::current->lights)
    {
        diffuse += light->GetColor() * std::max(0.0f, normal * light->direction());
    }
    diffuse *= albedo;

    return ambient + diffuse;
}


GouraudShader::GouraudShader(const Model* model, Camera& camera) : IShader(model, camera)
{
    lightColors.resize(model->nVerts());
}

GouraudShader::~GouraudShader() {}

Vector4 GouraudShader::vertex(int faceIdx, int i)
{
    std::vector<Light*>& lights = Scene::current->lights;
    // 环境光
    Color ambient = Light::GetAmbient();
    // 漫反射
    Color diffuse;
    for (Light* light : Scene::current->lights)
    {
        Vector3 normal = (model->normal(faceIdx, i) * (Matrix3x3)worldToObject).normalized();
        diffuse += light->GetColor() * std::max(0.0f, normal * light->direction());
    }
    lightColors[model->face(faceIdx).vi[i]] = ambient + diffuse;

    return P * V * M * Vector4(model->vert(faceIdx, i), 1);
}

Color GouraudShader::fragment(Vector3 baryCoord, int faceIdx)
{
    Material* material = model->GetMaterial(faceIdx);
    Face face = model->face(faceIdx);

    // 采样反射率
    Vector2 uv1 = model->texCoord(faceIdx, 0);
    Vector2 uv2 = model->texCoord(faceIdx, 1);
    Vector2 uv3 = model->texCoord(faceIdx, 2);
    float u = uv1[0] * baryCoord[0] + uv2[0] * baryCoord[1] + uv3[0] * baryCoord[2];
    float v = uv1[1] * baryCoord[0] + uv2[1] * baryCoord[1] + uv3[1] * baryCoord[2];
    Color albedo = material->SampleKd(u, v);

    Color lightColor = lightColors[face.vi[0]] * baryCoord[0] + lightColors[face.vi[1]] * baryCoord[1] +
        lightColors[face.vi[2]] * baryCoord[2];

    return lightColor * albedo;
}


PhongShader::PhongShader(const Model* model, Camera& camera) : IShader(model, camera) {}

PhongShader::~PhongShader() {}

Vector4 PhongShader::vertex(int faceIdx, int i)
{
    // MVP变换
    return P * V * M * Vector4(model->vert(faceIdx, i), 1);
}

Color PhongShader::fragment(Vector3 baryCoord, int faceIdx)
{
    Material* material = model->GetMaterial(faceIdx);

    // 计算法线
    Vector3 normal = model->normal(faceIdx, 0) * baryCoord[0] + model->normal(faceIdx, 1) * baryCoord[1] +
        model->normal(faceIdx, 2) * baryCoord[2];
    normal = (normal * (Matrix3x3)worldToObject).normalized();

    // 采样反射率
    Vector2 uv1 = model->texCoord(faceIdx, 0);
    Vector2 uv2 = model->texCoord(faceIdx, 1);
    Vector2 uv3 = model->texCoord(faceIdx, 2);
    float u = uv1[0] * baryCoord[0] + uv2[0] * baryCoord[1] + uv3[0] * baryCoord[2];
    float v = uv1[1] * baryCoord[0] + uv2[1] * baryCoord[1] + uv3[1] * baryCoord[2];
    Color albedo = material->SampleKd(u, v);

    // 环境光
    Color ambient = Light::GetAmbient() * albedo;
    // 漫反射
    Color diffuse;
    for (Light* light : Scene::current->lights)
    {
        diffuse += light->GetColor() * std::max(0.0f, normal * light->direction());
    }
    diffuse *= albedo;

    return ambient + diffuse;
}


HalfLambertShader::HalfLambertShader(const Model* model, Camera& camera) : IShader(model, camera) {}

HalfLambertShader::~HalfLambertShader() {}

Vector4 HalfLambertShader::vertex(int faceIdx, int i)
{
    // MVP变换
    Vector4 worldPos = M * Vector4(model->vert(faceIdx, i), 1);
    return P * V * worldPos;
}

Color HalfLambertShader::fragment(Vector3 baryCoord, int faceIdx)
{
    Material* material = model->GetMaterial(faceIdx);

    // 计算法线
    Vector3 normal = model->normal(faceIdx, 0) * baryCoord[0] + model->normal(faceIdx, 1) * baryCoord[1] +
        model->normal(faceIdx, 2) * baryCoord[2];
    normal = (normal * (Matrix3x3)worldToObject).normalized();

    // 采样反射率
    Vector2 uv1 = model->texCoord(faceIdx, 0);
    Vector2 uv2 = model->texCoord(faceIdx, 1);
    Vector2 uv3 = model->texCoord(faceIdx, 2);
    float u = uv1[0] * baryCoord[0] + uv2[0] * baryCoord[1] + uv3[0] * baryCoord[2];
    float v = uv1[1] * baryCoord[0] + uv2[1] * baryCoord[1] + uv3[1] * baryCoord[2];
    Color albedo = material->SampleKd(u, v);

    // 环境光
    Color ambient = Light::GetAmbient() * albedo;
    // 漫反射
    Color diffuse;
    for (Light* light : Scene::current->lights)
    {
        diffuse += light->GetColor() * (normal * light->direction() * 0.5 + 0.5);
    }
    diffuse *= albedo;

    return ambient + diffuse;
}