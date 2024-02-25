#pragma once
#include "model.h"
#include "bitmap.h"

class Camera : public Object
{
public:
    Camera(int pixelWidth, int pixelHeight, float fov);
    Camera(int pixelWidth, int pixelHeight, float fov, Vector3 position, Vector3 rotation);
    ~Camera();
    Matrix4x4 worldToViewMatrix() const;    // 世界空间到观察空间的矩阵
    Matrix4x4 projectionMatrix() const;     // 投影矩阵
    void Render();
private:
    int pixelWidth;
    int pixelHeight;
    float fieldOfView;
    float nearClipPlane;
    float farClipPlane;

    std::vector<Vector4> screenCoordsBuffer;
    float** zBuffer;
    Bitmap screen;
    void Render(const Model& model);
    void GeometryStage(const Model& model);
    void RasterizationStage(const Model& model);
    void FillRandomColor(const Model& model);
    void DrawWireframe(const Model& model);
    void Output();
};