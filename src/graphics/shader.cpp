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

FlatShader::FlatShader(const Model* model, Camera& camera) : IShader(model, camera)
{

}

Vector4 FlatShader::vertex(int faceIdx, int i)
{
    // MVP变换
    Vector4 worldPos = M * Vector4(model->vert(faceIdx, i), 1);
    Face face = model->face(faceIdx);
    worldCoordsBuffer[model->face(faceIdx).vi[i]] = worldPos;
    return P * V * worldPos;
}

Color FlatShader::fragment(Vector3 baryCoord, int faceIdx)
{
    std::vector<Light*>& lights = Scene::current->lights;
    Face face = model->face(faceIdx);
    Vector3 sa = (Vector3)worldCoordsBuffer[face.vi[1]] - (Vector3)worldCoordsBuffer[face.vi[0]];
    Vector3 sb = (Vector3)worldCoordsBuffer[face.vi[2]] - (Vector3)worldCoordsBuffer[face.vi[0]];
    Vector3 normal = (sa ^ sb).normalized();
    float totalIntensity = 0;
    for (Light* light : Scene::current->lights)
    {
        float intensity = Clamp(normal * light->direction(), 0.0f, 1.0f);
        totalIntensity = Clamp(totalIntensity + intensity, 0.0f, 1.0f);
    }
    return Color::White * totalIntensity;
}