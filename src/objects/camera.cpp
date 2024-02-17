#include "camera.h"
#include "scene.h"
#include "matrix.h"
#include "painter.h"
#include <iostream>
#include <limits>
using std::string;

#define _DEBUG

static float floatMax = std::numeric_limits<float>::max();

Camera::Camera(int width, int height) : screenSize(width, height), image(width, height, 32),
    fieldOfView(60), nearClipPlane(0.3), farClipPlane(1000)
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

Camera::Camera(int width, int height, Vector3 position, Vector3 rotation) : Object(position, rotation),
    screenSize(width, height), image(width, height, 32), fieldOfView(60), nearClipPlane(0.3), farClipPlane(1000)
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

Camera::~Camera()
{
    for (int i = 0; i < screenSize.y; i++)
        delete zBuffer[i];
    delete zBuffer;
}

void Camera::Render()
{
    for (int i = 0; i < scene.models.size(); i++)
    {
        Render(scene.models[i]);
    }
    Output();
}

void Camera::Render(Model& model)
{
    // 顶点变换
    VertexTransform(model);
    // 采样并绘制
    Rasterization(model);
}

void Camera::VertexTransform(Model& model)
{
#ifdef DEBUG
    int nVerts = model.nVerts();
    vertexBuffer.reserve(nVerts);
    std::cerr << "Point :\t" << model[0] << "\n";
    std::cerr << "R :\t" << model.transform.rotation << "\n";
    std::cerr << "mR :\t" << Matrix4x4::Rotate(model.transform.rotation) << "\n";
    // 模型坐标到世界坐标
    Matrix4x4 mLTW = model.transform.localToWorldMatrix();
    std::cerr << "mLTW :\t" << mLTW << "\n";
    for (int i = 0; i < nVerts; i++)
        vertexBuffer.push_back(mLTW * Vector4(model[i], 1));
    std::cerr << "Point :\t" << vertexBuffer[0] << "\n";
    // 世界空间到观察空间
    Matrix4x4 mView = Matrix4x4::LookAt(transform.position, transform.forward(), transform.up());
    std::cerr << "mView :\t" << mView << "\n";
    vertexBuffer = mView * vertexBuffer;
    std::cerr << "Point :\t" << vertexBuffer[0] << "\n";
    // 观察空间到投影空间
    float aspect = (float)screenSize.x / screenSize.y; // 横纵比
    Matrix4x4 mPersp = Matrix4x4::Perspective(fieldOfView, aspect, nearClipPlane, farClipPlane);
    vertexBuffer = mPersp * vertexBuffer;
    // 投影空间到NDC
    for (int i = 0; i < nVerts; i++)
        vertexBuffer[i] = vertexBuffer[i] / vertexBuffer[i].w;
    // NDC到屏幕空间
    Matrix4x4 mScreen = Matrix4x4({
        {screenSize.x / 2.0f, 0, 0, screenSize.x / 2.0f},
        {0, screenSize.y / 2.0f, 0, screenSize.y / 2.0f},
        {0, 0, 1, 0},
        {0, 0, 0, 1}});
    vertexBuffer = mScreen * vertexBuffer;
    std::cerr << "Point :\t" << vertexBuffer[0] << "\n";
#else
    int nVerts = model.nVerts();
    vertexBuffer.resize(nVerts);
    // 模型坐标到世界坐标
    Matrix4x4 mLTW = model.transform.localToWorldMatrix();
    for (int i = 0; i < nVerts; i++)
        vertexBuffer[i] = mLTW * Vector4(model[i], 1);
    // 世界空间到观察空间
    Matrix4x4 mView = Matrix4x4::LookAt(transform.position, transform.forward(), transform.up());
    vertexBuffer = mView * vertexBuffer;
    // 观察空间到投影空间
    float aspect = (float)screenSize.x / screenSize.y; // 横纵比
    Matrix4x4 mPersp = Matrix4x4::Perspective(fieldOfView, aspect, nearClipPlane, farClipPlane);
    vertexBuffer = mPersp * vertexBuffer;
    // 投影空间到NDC
    for (int i = 0; i < nVerts; i++)
        vertexBuffer[i] = vertexBuffer[i] / vertexBuffer[i].w;
    // NDC到屏幕空间
    Matrix4x4 mScreen = Matrix4x4({
        {screenSize.x / 2.0f, 0, 0, screenSize.x / 2.0f},
        {0, screenSize.y / 2.0f, 0, screenSize.y / 2.0f},
        {0, 0, 1, 0},
        {0, 0, 0, 1}});
    vertexBuffer = mScreen * vertexBuffer;
#endif
}

void Camera::Rasterization(Model& model)
{
    for (int i = 0; i < model.nFaces(); i++)
    {
        std::vector<int> face = model.face(i);
        Vector3 p1 = vertexBuffer[face[0]];
        Vector3 p2 = vertexBuffer[face[1]];
        Vector3 p3 = vertexBuffer[face[2]];
        Color c = Color::GetRandomColor();

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
                Vector3 baryCoord = BarycentricCoordinate(p, p1, p2, p3);
                if (baryCoord[0] < 0 || baryCoord[1] < 0 || baryCoord[2] < 0)
                    continue;
                float z = p1.z * baryCoord[0] + p2.z * baryCoord[1] + p3.z * baryCoord[2];
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

void Camera::DrawWireframe(Model& model)
{
    Painter painter(image);

    Color c(255, 255, 255);
    for (int i = 0; i < model.nFaces(); i++)
    {
        std::vector<int> face = model.face(i);
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
    char c;
    std::cout << "Output Z-Buffer Image?(Y/N)" << std::endl;
    std::cin >> c;
    if (c == 'N')
        return;
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