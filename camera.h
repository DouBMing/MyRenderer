#pragma once
#include "object.h"
#include "bitmap.h"

class Camera
{
public:
    Vector3 position;
    Vector3 up;
    Vector3 lookAt;
    float fieldOfView;
    float nearClipPlane;
    float farClipPlane;
    Camera(int width, int height);
    void Render(Object& object);
private:
    Vector2Int screenSize;
    Bitmap image;
    std::vector<Vector4> vertexBuffer;
    float** zBuffer;
    bool ZTest(Vector2Int p, float z);
    void VertexTransform(Object& object);   // 顶点变换，最终变换为屏幕空间坐标
    void Rasterization(Vector3 p1, Vector3 p2, Vector3 p3, Color c);
    void DrawWireframe(Object& object);
    void Output();
};