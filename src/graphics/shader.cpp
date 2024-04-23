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

Vector3 IShader::WorldSpaceViewDir(const Vector3& worldPos)
{
    return (Scene::current->camera->transform.position - worldPos).normalized();
}

Vector3 IShader::ToWorldNormal(const Vector3& normal)
{
    return (normal * (Matrix3x3)worldToObject).normalized();
}

Vector2 IShader::GetUV(const Vector3& baryCoord, int faceIdx)
{
    Vector2 uv1 = model->texCoord(faceIdx, 0);
    Vector2 uv2 = model->texCoord(faceIdx, 1);
    Vector2 uv3 = model->texCoord(faceIdx, 2);
    return baryCoord * Matrix<3, 2>({uv1, uv2, uv3});
}

Vector3 IShader::GetWorldNormal(const Vector3& baryCoord, int faceIdx)
{
    Vector3 normal1 = model->normal(faceIdx, 0);
    Vector3 normal2 = model->normal(faceIdx, 1);
    Vector3 normal3 = model->normal(faceIdx, 2);
    Vector3 normal = baryCoord * Matrix3x3({normal1, normal2, normal3});
    return ToWorldNormal(normal);
}


FlatShader::FlatShader(Color cSpecular, float gloss, const Model* model, Camera& camera) :
    IShader(model, camera), specularColor(cSpecular), gloss(gloss)
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
    Material* material = model->GetMaterial(faceIdx);
    Face face = model->face(faceIdx);
    // 计算法线
    Vector3 sa = Vector3(worldCoords[face.vi[1]] - worldCoords[face.vi[0]]);
    Vector3 sb = Vector3(worldCoords[face.vi[2]] - worldCoords[face.vi[0]]);
    Vector3 normal = (sa ^ sb).normalized();

    // 采样反射率
    Vector2 uv = GetUV(baryCoord, faceIdx);
    Color albedo = material->SampleKd(uv.x, uv.y);

    // 环境光
    Color ambient = Light::GetAmbient();
    // 漫反射
    Color diffuse, specular;
    for (Light* light : Scene::current->lights)
    {
        // 面位置
        Vector3 worldPos = (worldCoords[face.vi[0]] + worldCoords[face.vi[1]] + worldCoords[face.vi[2]]) / 3;
        diffuse += light->GetColor() * std::max(0.0f, normal * light->Direction(worldPos));
        Vector3 reflectDir = Reflect(-light->Direction(worldPos), normal).normalized();
        Vector3 viewDir = WorldSpaceViewDir(worldPos);
        specular += light->GetColor() * specularColor * std::pow(std::max(0.0f, reflectDir * viewDir), gloss);
    }

    return (ambient + diffuse + specular) * albedo;
}


GouraudShader::GouraudShader(Color cSpecular, float gloss, const Model* model, Camera& camera) :
    IShader(model, camera), specularColor(cSpecular), gloss(gloss)
{
    worldCoords.resize(model->nVerts());
    lightColors.resize(model->nVerts());
}

GouraudShader::~GouraudShader() {}

Vector4 GouraudShader::vertex(int faceIdx, int i)
{
    Vector4 worldPos = M * Vector4(model->vert(faceIdx, i), 1);
    Face face = model->face(faceIdx);
    worldCoords[model->face(faceIdx).vi[i]] = worldPos;
    
    Vector3 normal = ToWorldNormal(model->normal(faceIdx, i));
    // 环境光
    Color ambient = Light::GetAmbient();
    // 漫反射
    Color diffuse, specular;
    for (Light* light : Scene::current->lights)
    {
        diffuse += light->GetColor() * std::max(0.0f, normal * light->Direction(worldPos));
        Vector3 reflectDir = Reflect(-light->Direction(worldPos), normal).normalized();
        Vector3 viewDir = WorldSpaceViewDir(worldPos);
        specular += light->GetColor() * specularColor * std::pow(std::max(0.0f, reflectDir * viewDir), gloss);
    }
    lightColors[model->face(faceIdx).vi[i]] = ambient + diffuse + specular;

    return P * V * worldPos;
}

Color GouraudShader::fragment(Vector3 baryCoord, int faceIdx)
{
    Material* material = model->GetMaterial(faceIdx);
    Face face = model->face(faceIdx);

    // 采样反射率
    Vector2 uv = GetUV(baryCoord, faceIdx);
    Color albedo = material->SampleKd(uv.x, uv.y);

    Color lightColor = lightColors[face.vi[0]] * baryCoord[0] + lightColors[face.vi[1]] * baryCoord[1] +
        lightColors[face.vi[2]] * baryCoord[2];

    return lightColor * albedo;
}


PhongShader::PhongShader(Color cSpecular, float gloss, const Model* model, Camera& camera) :
    IShader(model, camera), specularColor(cSpecular), gloss(gloss)
{
    worldCoords.resize(model->nVerts());
}

PhongShader::~PhongShader() {}

Vector4 PhongShader::vertex(int faceIdx, int i)
{
    // MVP变换
    Vector4 worldPos = M * Vector4(model->vert(faceIdx, i), 1);
    Face face = model->face(faceIdx);
    worldCoords[model->face(faceIdx).vi[i]] = worldPos;
    return P * V * worldPos;
}

Color PhongShader::fragment(Vector3 baryCoord, int faceIdx)
{
    Material* material = model->GetMaterial(faceIdx);
    Face face = model->face(faceIdx);
    // 计算法线
    Vector3 normal = GetWorldNormal(baryCoord, faceIdx);
    // 像素位置
    Vector3 worldPos = worldCoords[face.vi[0]] * baryCoord[0] + worldCoords[face.vi[1]] * baryCoord[1] + worldCoords[face.vi[2]] * baryCoord[2];
    // 采样反射率
    Vector2 uv = GetUV(baryCoord, faceIdx);
    Color albedo = material->SampleKd(uv.x, uv.y);

    // 环境光
    Color ambient = Light::GetAmbient();
    // 漫反射、高光
    Color diffuse, specular;
    for (Light* light : Scene::current->lights)
    {
        diffuse += light->GetColor() * std::max(0.0f, normal * light->Direction(worldPos));
        Vector3 reflectDir = Reflect(-light->Direction(worldPos), normal).normalized();
        Vector3 viewDir = WorldSpaceViewDir(worldPos);
        specular += light->GetColor() * specularColor * std::pow(std::max(0.0f, reflectDir * viewDir), gloss);
    }

    return (ambient + diffuse + specular) * albedo;
}


BlinnPhongShader::BlinnPhongShader(Color cSpecular, float gloss, const Model* model, Camera& camera) :
    IShader(model, camera), specularColor(cSpecular), gloss(gloss)
{
    worldCoords.resize(model->nVerts());
}

BlinnPhongShader::~BlinnPhongShader() {}

Vector4 BlinnPhongShader::vertex(int faceIdx, int i)
{
    // MVP变换
    Vector4 worldPos = M * Vector4(model->vert(faceIdx, i), 1);
    Face face = model->face(faceIdx);
    worldCoords[model->face(faceIdx).vi[i]] = worldPos;
    return P * V * worldPos;
}

Color BlinnPhongShader::fragment(Vector3 baryCoord, int faceIdx)
{
    Material* material = model->GetMaterial(faceIdx);
    Face face = model->face(faceIdx);
    // 计算法线
    Vector3 normal = GetWorldNormal(baryCoord, faceIdx);
    // 像素位置
    Vector3 worldPos = worldCoords[face.vi[0]] * baryCoord[0] + worldCoords[face.vi[1]] * baryCoord[1] + worldCoords[face.vi[2]] * baryCoord[2];
    // 采样反射率
    Vector2 uv = GetUV(baryCoord, faceIdx);
    Color albedo = material->SampleKd(uv.x, uv.y);

    // 环境光
    Color ambient = Light::GetAmbient();
    // 漫反射、高光
    Color diffuse, specular;
    for (Light* light : Scene::current->lights)
    {
        diffuse += light->GetColor() * std::max(0.0f, normal * light->Direction(worldPos));
        Vector3 viewDir = WorldSpaceViewDir(worldPos);
        Vector3 halfDir = (light->Direction(worldPos) + viewDir).normalized();
        specular += light->GetColor() * specularColor * std::pow(std::max(0.0f, normal * halfDir), gloss);
    }

    return (ambient + diffuse + specular) * albedo;
}


HalfLambertShader::HalfLambertShader(Color cSpecular, float gloss, const Model* model, Camera& camera) :
    IShader(model, camera), specularColor(cSpecular), gloss(gloss)
{
    worldCoords.resize(model->nVerts());
}

HalfLambertShader::~HalfLambertShader() {}

Vector4 HalfLambertShader::vertex(int faceIdx, int i)
{
    // MVP变换
    Vector4 worldPos = M * Vector4(model->vert(faceIdx, i), 1);
    Face face = model->face(faceIdx);
    worldCoords[model->face(faceIdx).vi[i]] = worldPos;
    return P * V * worldPos;
}

Color HalfLambertShader::fragment(Vector3 baryCoord, int faceIdx)
{
    Material* material = model->GetMaterial(faceIdx);
    Face face = model->face(faceIdx);
    // 计算法线
    Vector3 normal = GetWorldNormal(baryCoord, faceIdx);
    // 像素位置
    Vector3 worldPos = worldCoords[face.vi[0]] * baryCoord[0] + worldCoords[face.vi[1]] * baryCoord[1] + worldCoords[face.vi[2]] * baryCoord[2];
    // 采样反射率
    Vector2 uv = GetUV(baryCoord, faceIdx);
    Color albedo = material->SampleKd(uv.x, uv.y);

    // 环境光
    Color ambient = Light::GetAmbient();
    // 漫反射、高光
    Color diffuse, specular;
    for (Light* light : Scene::current->lights)
    {
        diffuse += light->GetColor() * (normal * light->Direction(worldPos) * 0.5 + 0.5);
        Vector3 reflectDir = Reflect(-light->Direction(worldPos), normal).normalized();
        Vector3 viewDir = WorldSpaceViewDir(worldPos);
        specular += light->GetColor() * specularColor * std::pow(std::max(0.0f, reflectDir * viewDir), gloss);
    }

    return (ambient + diffuse + specular) * albedo;
}