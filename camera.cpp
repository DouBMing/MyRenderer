#include "camera.h"
#include "matrix.h"
#include "painter.h"
#include <iostream>
#include <limits>

using std::string;

static float floatMax = std::numeric_limits<float>::max();

Camera::Camera(int width, int height) : screenSize(width, height), image(width, height, 32),
position(), up(0, 1, 0), lookAt(0, 0, 1), fieldOfView(60), nearClipPlane(0.3), farClipPlane(1000)
{
    zBuffer = new float*[height];
    for (int i = 0; i < screenSize.y; i++)
    {
        zBuffer[i] = new float[width];
        for (int j = 0; j < screenSize.x; j++)
        {
            zBuffer[i][j] = floatMax;
        }
    }
}

void Camera::Render(Object& object)
{
    // 顶点变换
    VertexTransform(object);
    // 采样并绘制
    for (int i = 0; i < object.model.nFaces(); i++)
    {
        std::vector<int> face = object.model.face(i);
        Rasterization(vertexBuffer[face[0]], vertexBuffer[face[1]], vertexBuffer[face[2]], Color::GetRandomColor());
    }
    Output();
}

void Camera::VertexTransform(Object& object)
{
    int nVerts = object.model.nVerts();
    vertexBuffer.reserve(nVerts);
    // 模型坐标到世界坐标
    Matrix4x4 mLTW = object.transform.localToWorldMatrix();
    for (int i = 0; i < nVerts; i++)
        vertexBuffer.push_back(mLTW * Vector4(object.model[i], 1));
    // 世界空间到观察空间
    Matrix4x4 mRView = Matrix4x4({
        Vector4(-lookAt ^ up, 0),
        Vector4(up, 0),
        Vector4(lookAt, 0),
        {0, 0, 0, 1}
    });
    Matrix4x4 mTView = Matrix4x4({
        {1, 0, 0, -position.x},
        {0, 1, 0, -position.y},
        {0, 0, 1, -position.z},
        {0, 0, 0, 1}
    });
    Matrix4x4 mWorldToViewPoint = mRView * mTView;
    vertexBuffer = mWorldToViewPoint * vertexBuffer;
    // 观察空间到投影空间
    float aspect = (float)screenSize.x / screenSize.y; // 横纵比
    float top = nearClipPlane * std::tan(fieldOfView / 2 * M_PI / 180);
    float right = top * aspect;
    Matrix4x4 mOrth = Matrix4x4({
        {1 / right, 0, 0, 0},
        {0, 1 / top, 0, 0},
        {0, 0, 2 / (farClipPlane - nearClipPlane), -(nearClipPlane + farClipPlane) / (farClipPlane - nearClipPlane)},
        {0, 0, 0, 1}
    });
    Matrix4x4 mProj = mOrth * Matrix4x4({
        {nearClipPlane, 0, 0, 0},
        {0, nearClipPlane, 0, 0},
        {0, 0, nearClipPlane + farClipPlane, -nearClipPlane * farClipPlane},
        {0, 0, 1, 0}
    });
    vertexBuffer = mProj * vertexBuffer;
    // 投影空间到NDC
    for (int i = 0; i < nVerts; i++)
        vertexBuffer[i] = vertexBuffer[i] / vertexBuffer[i].w;
    // NDC到屏幕空间
    Matrix4x4 mScreen = Matrix4x4({
        {screenSize.x / 2.0f, 0, 0, screenSize.x / 2.0f},
        {0, screenSize.y / 2.0f, 0, screenSize.y / 2.0f},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    });
    vertexBuffer = mScreen * vertexBuffer;
}

void Camera::Rasterization(Vector3 p1, Vector3 p2, Vector3 p3, Color c)
{
    Vector2 minPoint, maxPoint;
    minPoint.x = std::min(std::min(p1.x, p2.x), p3.x);
    minPoint.y = std::min(std::min(p1.y, p2.y), p3.y);
    maxPoint.x = std::max(std::max(p1.x, p2.x), p3.x);
    maxPoint.y = std::max(std::max(p1.y, p2.y), p3.y);
    // 剔除边界外的三角形
    if (minPoint.x > screenSize.x || minPoint.y > screenSize.y || maxPoint.x < 0 || maxPoint.y < 0)
        return;
    for (int y = minPoint.y; y < maxPoint.y; y++)
    {
        if (y < 0 || y >= screenSize.y)
            continue;
        for (int x = minPoint.x; x < maxPoint.x; x++)
        {
            if (x < 0 || x >= screenSize.x)
                continue;
            Vector2 p(x, y);
            float cross1 = Vector2(p2 - p1) ^ (p - (Vector2)p1);
            float cross2 = Vector2(p3 - p2) ^ (p - (Vector2)p2);
            float cross3 = Vector2(p1 - p3) ^ (p - (Vector2)p3);
            if (cross1 > 0 && cross2 > 0 && cross3 > 0 || cross1 < 0 && cross2 < 0 && cross3 < 0)
            {
                float det = (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);
                float lambda1 = ((p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y)) / det;
                float lambda2 = ((p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y)) / det;
                float lambda3 = 1 - lambda1 - lambda2;
                float z = lambda1 * p1.z + lambda2 * p2.z + lambda3 * p3.z;
                if (z < -1 || z > 1)
                    continue;
                if (zBuffer[y][x] > z)
                {
                    zBuffer[y][x] = z;
                    image.Set({x, y}, c);
                }
            }
        }
    }
}

void Camera::DrawWireframe(Object& object)
{
    Painter painter(image);

    Color c(255, 255, 255);
    for (int i = 0; i < object.model.nFaces(); i++)
    {
        std::vector<int> face = object.model.face(i);
        for (int j = 0; j < 3; j++)
        {
            Vector4 p1 = vertexBuffer[face[j]];
            Vector4 p2 = vertexBuffer[face[(j + 1) % 3]];
            painter.Line(Vector2Int(p1.x, p1.y), Vector2Int(p2.x, p2.y), c);
        }
    }
}

void Camera::Output()
{
    string imagePath = "Outputs/";
    string imageName;
    std::cout << "image name:" << std::endl;
    std::cin >> imageName;
    image.Write(imagePath + imageName + ".bmp");
    // 输出深度缓冲图像
    Bitmap zBufferImage(screenSize.x, screenSize.y, 32);
    for (int i = 0; i < screenSize.y; i++)
    {
        for (int j = 0; j < screenSize.x; j++)
        {
            byte c = zBuffer[i][j] > 1 ? 0 : (1.0 - zBuffer[i][j]) / 2 * 255;
            image.Set({j, i}, Color{c, c, c});
        }
    }
    image.Write(imagePath + imageName + "_ZBuffer.bmp");
}