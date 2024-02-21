#pragma once
#include "model.h"
#include "bitmap.h"

class Camera : public Object
{
public:
    Camera(int pixelWidth, int pixelHeight);
    Camera(int pixelWidth, int pixelHeight, Vector3 position, Vector3 rotation);
    ~Camera();
    void AllowMSAA(bool value);
    void Render();
    void RenderWithShader();
private:
    int pixelWidth;
    int pixelHeight;
    float fieldOfView;
    float nearClipPlane;
    float farClipPlane;
    Matrix4x4 worldToScreenMatrix() const;  // 世界坐标到屏幕空间的矩阵
    Matrix4x4 screenToWorldMatrix() const;  // 屏幕空间到世界空间的矩阵
    std::vector<Vector4> vertexBuffer_WorldCoords;
    std::vector<Vector4> vertexBuffer_ScreenCoords;
    float** zBuffer;
    Bitmap screen;
    void Render(const Model& model);
    void VertexProcessing(const Model& model);   // 顶点变换，最终变换为屏幕空间坐标
    void TransformVertices(const Matrix4x4& m, const std::vector<Vector4>& src, std::vector<Vector4>& dst);
    void Rasterization(const Model& model);
    void TextureMapping(const Model& model);
    void FillRandomColor(const Model& model);
    void DrawWireframe(const Model& model);
    void Output();
};