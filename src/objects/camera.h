#pragma once
#include "object.h"
#include "model.h"
#include "bitmap.h"

class Camera : public Object
{
public:
    Camera(int width, int height);
    Camera(int width, int height, Vector3 position, Vector3 rotation);
    ~Camera();
    void Render();
private:
    Vector2Int screenSize;
    float fieldOfView;
    float nearClipPlane;
    float farClipPlane;
    Bitmap image;
    std::vector<Vector4> vertexBuffer;
    float** zBuffer;
    void Render(Model& model);
    void VertexTransform(Model& model);   // 顶点变换，最终变换为屏幕空间坐标
    void Rasterization(Model& model);
    void TextureMapping(Model& model);
    void FillRandomColor(Model& model);
    void DrawWireframe(Model& model);
    void Output();
};