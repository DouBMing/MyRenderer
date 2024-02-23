#include "camera.h"
#include "scene.h"
#include "painter.h"
#include <iostream>
#include <limits>
using std::string;

static float floatMax = std::numeric_limits<float>::max();

Camera::Camera(int pixelWidth, int pixelHeight) : pixelWidth(pixelWidth), pixelHeight(pixelHeight), screen(pixelWidth, pixelHeight, 32),
    fieldOfView(60), nearClipPlane(0.3), farClipPlane(1000)
{
    zBuffer = new float*[pixelHeight];
    for (int i = 0; i < pixelHeight; i++)
    {
        zBuffer[i] = new float[pixelWidth];
        for (int j = 0; j < pixelWidth; j++)
        {
            zBuffer[i][j] = floatMax;
        }
    }
}

Camera::Camera(int pixelWidth, int pixelHeight, Vector3 position, Vector3 rotation) : Object(position, rotation),
    pixelWidth(pixelWidth), pixelHeight(pixelHeight), screen(pixelWidth, pixelHeight, 32), fieldOfView(60), nearClipPlane(0.3), farClipPlane(1000)
{
    zBuffer = new float*[pixelHeight];
    for (int i = 0; i < pixelHeight; i++)
    {
        zBuffer[i] = new float[pixelWidth];
        for (int j = 0; j < pixelWidth; j++)
        {
            zBuffer[i][j] = floatMax;
        }
    }
}

Camera::~Camera()
{
    for (int i = 0; i < pixelHeight; i++)
        delete zBuffer[i];
    delete zBuffer;
}

Matrix4x4 Camera::worldToViewMatrix() const
{
    return MatrixLookAt(transform.position, transform.forward(), transform.up());
}

Matrix4x4 Camera::projectionMatrix() const
{
    float aspect = (float)pixelWidth / pixelHeight; // 横纵比
    return MatrixPerspective(fieldOfView, aspect, nearClipPlane, farClipPlane);
}

void Camera::Render()
{
    for (Model* model : scene.models)
    {
        Render(*model);
    }
    Output();
}

void Camera::Render(const Model& model)
{
    // 几何阶段
    GeometryStage(model);
    // 光栅化阶段
    RasterizationStage(model);
}

void Camera::GeometryStage(const Model& model)
{
    int nVerts = model.nVerts();
    screenCoordsBuffer.resize(nVerts);
    Matrix4x4 mScreen = Matrix4x4({
        {pixelWidth / 2.0f, 0, 0, pixelWidth / 2.0f},
        {0, pixelHeight / 2.0f, 0, pixelHeight / 2.0f},
        {0, 0, 1, 0},
        {0, 0, 0, 1}});
    for (int i = 0; i < model.nFaces(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // 顶点着色
            Vector4 clipPos = model.shader->vertex(i, j);
            clipPos = clipPos / clipPos.w;
            // 屏幕映射
            screenCoordsBuffer[model.face(i).vi[j]] = mScreen * clipPos;
        }
    }
}

void Camera::RasterizationStage(const Model& model)
{
    for (int i = 0; i < model.nFaces(); i++)
    {
        Face face = model.face(i);
        // 三角面屏幕空间的顶点
        Vector3 sp1 = screenCoordsBuffer[face.vi[0]];
        Vector3 sp2 = screenCoordsBuffer[face.vi[1]];
        Vector3 sp3 = screenCoordsBuffer[face.vi[2]];

        Vector2 minPoint, maxPoint;
        minPoint.x = std::min(std::min(sp1.x, sp2.x), sp3.x);
        minPoint.y = std::min(std::min(sp1.y, sp2.y), sp3.y);
        maxPoint.x = std::max(std::max(sp1.x, sp2.x), sp3.x);
        maxPoint.y = std::max(std::max(sp1.y, sp2.y), sp3.y);
        // 剔除完全在边界外的三角形
        if (minPoint.x > pixelWidth || minPoint.y > pixelHeight || maxPoint.x < 0 || maxPoint.y < 0)
            return;

        for (int sy = std::max(0.0f, minPoint.y); sy < std::min((float)pixelHeight, maxPoint.y); sy++)
        {
            for (int sx = std::max(0.0f, minPoint.x); sx < std::min((float)pixelWidth, maxPoint.x); sx++)
            {
                Vector2 p(sx, sy);
                Vector3 baryCoord = BarycentricCoordinate(p, sp1, sp2, sp3);
                if (baryCoord[0] < 0 || baryCoord[1] < 0 || baryCoord[2] < 0)
                    continue;
                float sz = sp1.z * baryCoord[0] + sp2.z * baryCoord[1] + sp3.z * baryCoord[2];
                if (sz < -1 || sz > 1)
                    continue;
                if (zBuffer[sy][sx] < sz)
                    continue;
                zBuffer[sy][sx] = sz;
                // 三角面世界空间的顶点
                Color c = model.shader->fragment(baryCoord, i);
                screen.Set({sx, sy}, c);
            }
        }
    }
}

void Camera::FillRandomColor(const Model& model)
{
    for (int i = 0; i < model.nFaces(); i++)
    {
        Face face = model.face(i);
        Vector3 p1 = screenCoordsBuffer[face.vi[0]];
        Vector3 p2 = screenCoordsBuffer[face.vi[1]];
        Vector3 p3 = screenCoordsBuffer[face.vi[2]];
        Color c = Color::GetRandomColor();

        Vector2 minPoint, maxPoint;
        minPoint.x = std::min(std::min(p1.x, p2.x), p3.x);
        minPoint.y = std::min(std::min(p1.y, p2.y), p3.y);
        maxPoint.x = std::max(std::max(p1.x, p2.x), p3.x);
        maxPoint.y = std::max(std::max(p1.y, p2.y), p3.y);
        // 剔除边界外的三角形
        if (minPoint.x > pixelWidth || minPoint.y > pixelHeight || maxPoint.x < 0 || maxPoint.y < 0)
            return;
        for (int y = minPoint.y; y < maxPoint.y; y++)
        {
            if (y < 0 || y >= pixelHeight)
                continue;
            for (int x = minPoint.x; x < maxPoint.x; x++)
            {
                if (x < 0 || x >= pixelWidth)
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
                    screen.Set({x, y}, c);
                }
            }
        }
    }
}

void Camera::DrawWireframe(const Model& model)
{
    Painter painter(screen);

    Color c(255, 255, 255);
    for (int i = 0; i < model.nFaces(); i++)
    {
        Face face = model.face(i);
        for (int j = 0; j < 3; j++)
        {
            Vector4 p1 = screenCoordsBuffer[face.vi[j]];
            Vector4 p2 = screenCoordsBuffer[face.vi[(j + 1) % 3]];
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
    screen.Write(imagePath + imageName + ".bmp");
    // 输出深度缓冲图像
    char c;
    std::cout << "Output Z-Buffer Image?(Y/N)" << std::endl;
    std::cin >> c;
    if (c == 'N')
        return;
    Bitmap zBufferImage(pixelWidth, pixelHeight, 8);
    for (int i = 0; i < pixelHeight; i++)
    {
        for (int j = 0; j < pixelWidth; j++)
        {
            byte c = zBuffer[i][j] > 1 ? 0 : (1.0 - zBuffer[i][j]) / 2 * 255;
            screen.Set(j, i, c);
        }
    }
    screen.Write(imagePath + imageName + "_ZBuffer.bmp");
}