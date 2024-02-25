#include "shader.h"
#include "model.h"
#include "camera.h"
#include "scene.h"

IShader::IShader(const Model* model, Camera& camera) : model(model)
{
    M = model->transform.localToWorldMatrix();
    V = camera.worldToViewMatrix();
    P = camera.projectionMatrix();
}

IShader::~IShader() {}

FlatShader::FlatShader(const std::string& texturePath, const Model* model, Camera& camera) : IShader(model, camera)
{
    worldCoords.resize(model->nVerts());
    if (texturePath == "")
    {
        diffuseMap = nullptr;
        return;
    }
    diffuseMap = new Bitmap();
    diffuseMap->Read(texturePath);
}

FlatShader::~FlatShader()
{
    if (diffuseMap != nullptr)
        delete diffuseMap;
}

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
    Face face = model->face(faceIdx);
    Vector3 sa = Vector3(worldCoords[face.vi[1]] - worldCoords[face.vi[0]]);
    Vector3 sb = Vector3(worldCoords[face.vi[2]] - worldCoords[face.vi[0]]);
    Vector3 normal = (sa ^ sb).normalized();

    Color cLight;
    for (Light* light : Scene::current->lights)
    {
        float intensity = std::max(0.0f, normal * light->direction()) * light->intensity;
        cLight += light->color * intensity;
    }

    if (model->HasMaterial())
    {
        Vector2 uv1 = model->texCoord(faceIdx, 0);
        Vector2 uv2 = model->texCoord(faceIdx, 1);
        Vector2 uv3 = model->texCoord(faceIdx, 2);
        float u = uv1[0] * baryCoord[0] + uv2[0] * baryCoord[1] + uv3[0] * baryCoord[2];
        float v = uv1[1] * baryCoord[0] + uv2[1] * baryCoord[1] + uv3[1] * baryCoord[2];
        return cLight * model->SampleKd(faceIdx, u, v);
    }

    if (diffuseMap == nullptr)
        return cLight;

    Vector2 uv1 = model->texCoord(faceIdx, 0);
    Vector2 uv2 = model->texCoord(faceIdx, 1);
    Vector2 uv3 = model->texCoord(faceIdx, 2);
    float u = uv1[0] * baryCoord[0] + uv2[0] * baryCoord[1] + uv3[0] * baryCoord[2];
    float v = uv1[1] * baryCoord[0] + uv2[1] * baryCoord[1] + uv3[1] * baryCoord[2];
    return cLight * diffuseMap->Get(diffuseMap->width * u, diffuseMap->height * v);
}


GouraudShader::GouraudShader(const std::string& texturePath, const Model* model, Camera& camera) : IShader(model, camera)
{
    vertexColors.resize(model->nVerts());
    if (texturePath == "")
    {
        diffuseMap = nullptr;
        return;
    }
    diffuseMap = new Bitmap();
    diffuseMap->Read(texturePath);
}

GouraudShader::~GouraudShader()
{
    if (diffuseMap != nullptr)
        delete diffuseMap;
}

Vector4 GouraudShader::vertex(int faceIdx, int i)
{
    Vector4 worldPos = M * Vector4(model->vert(faceIdx, i), 1);
    
    std::vector<Light*>& lights = Scene::current->lights;
    Color cLight;
    for (Light* light : Scene::current->lights)
    {
        Vector3 normal = M * Vector4(model->normal(faceIdx, i), 0);
        float intensity = std::max(0.0f,  normal * light->direction()) * light->intensity;
        cLight += light->color * intensity;
    }
    vertexColors[model->face(faceIdx).vi[i]] = cLight;

    return P * V * worldPos;
}

Color GouraudShader::fragment(Vector3 baryCoord, int faceIdx)
{
    Face face = model->face(faceIdx);

    Color cLight = vertexColors[face.vi[0]] * baryCoord[0] + vertexColors[face.vi[1]] * baryCoord[1] +
        vertexColors[face.vi[2]] * baryCoord[2];
    
    if (model->HasMaterial())
    {
        Vector2 uv1 = model->texCoord(faceIdx, 0);
        Vector2 uv2 = model->texCoord(faceIdx, 1);
        Vector2 uv3 = model->texCoord(faceIdx, 2);
        float u = uv1[0] * baryCoord[0] + uv2[0] * baryCoord[1] + uv3[0] * baryCoord[2];
        float v = uv1[1] * baryCoord[0] + uv2[1] * baryCoord[1] + uv3[1] * baryCoord[2];
        return cLight * model->SampleKd(faceIdx, u, v);
    }

    if (diffuseMap == nullptr)
        return cLight;

    Vector2 uv1 = model->texCoord(faceIdx, 0);
    Vector2 uv2 = model->texCoord(faceIdx, 1);
    Vector2 uv3 = model->texCoord(faceIdx, 2);
    float u = uv1[0] * baryCoord[0] + uv2[0] * baryCoord[1] + uv3[0] * baryCoord[2];
    float v = uv1[1] * baryCoord[0] + uv2[1] * baryCoord[1] + uv3[1] * baryCoord[2];
    return cLight * diffuseMap->Get(diffuseMap->width * u, diffuseMap->height * v);
}


PhongShader::PhongShader(const std::string& texturePath, const Model* model, Camera& camera) : IShader(model, camera)
{
    if (texturePath == "")
    {
        diffuseMap = nullptr;
        return;
    }
    diffuseMap = new Bitmap();
    diffuseMap->Read(texturePath);
}

PhongShader::~PhongShader()
{
    if (diffuseMap != nullptr)
        delete diffuseMap;
}

Vector4 PhongShader::vertex(int faceIdx, int i)
{
    // MVP变换
    Vector4 worldPos = M * Vector4(model->vert(faceIdx, i), 1);
    return P * V * worldPos;
}

Color PhongShader::fragment(Vector3 baryCoord, int faceIdx)
{
    Vector3 normal = model->normal(faceIdx, 0) * baryCoord[0] + model->normal(faceIdx, 1) * baryCoord[1] +
        model->normal(faceIdx, 2) * baryCoord[2];
    normal = M * Vector4(normal, 0);

    Color cLight;
    for (Light* light : Scene::current->lights)
    {
        float intensity = std::max(0.0f, normal * light->direction()) * light->intensity;
        cLight += light->color * intensity;
    }

    if (model->HasMaterial())
    {
        Vector2 uv1 = model->texCoord(faceIdx, 0);
        Vector2 uv2 = model->texCoord(faceIdx, 1);
        Vector2 uv3 = model->texCoord(faceIdx, 2);
        float u = uv1[0] * baryCoord[0] + uv2[0] * baryCoord[1] + uv3[0] * baryCoord[2];
        float v = uv1[1] * baryCoord[0] + uv2[1] * baryCoord[1] + uv3[1] * baryCoord[2];
        return cLight * model->SampleKd(faceIdx, u, v);
    }

    if (diffuseMap == nullptr)
        return cLight;

    Vector2 uv1 = model->texCoord(faceIdx, 0);
    Vector2 uv2 = model->texCoord(faceIdx, 1);
    Vector2 uv3 = model->texCoord(faceIdx, 2);
    float u = uv1[0] * baryCoord[0] + uv2[0] * baryCoord[1] + uv3[0] * baryCoord[2];
    float v = uv1[1] * baryCoord[0] + uv2[1] * baryCoord[1] + uv3[1] * baryCoord[2];
    return cLight * diffuseMap->Get(diffuseMap->width * u, diffuseMap->height * v);
}