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

void Camera::Render()
{
    for (int i = 0; i < scene.models.size(); i++)
    {
        Render(scene.models[i]);
    }
    Output();
}

Matrix4x4 Camera::worldToScreenMatrix() const
{
    // 世界空间到观察空间
    Matrix4x4 mView = MatrixLookAt(transform.position, transform.forward(), transform.up());
    // 观察空间到投影空间
    float aspect = (float)pixelWidth / pixelHeight; // 横纵比
    Matrix4x4 mPersp = MatrixPerspective(fieldOfView, aspect, nearClipPlane, farClipPlane);
    // 投影空间到屏幕空间
    Matrix4x4 mScreen = Matrix4x4({
        {pixelWidth / 2.0f, 0, 0, pixelWidth / 2.0f},
        {0, pixelHeight / 2.0f, 0, pixelHeight / 2.0f},
        {0, 0, 1, 0},
        {0, 0, 0, 1}});
    return mScreen * mPersp * mView;
}

Matrix4x4 Camera::screenToWorldMatrix() const
{
    return worldToScreenMatrix().inverse();
}

void Camera::Render(const Model& model)
{
    // 顶点变换
    VertexProcessing(model);
    // 采样并绘制
    Rasterization(model);
}

#define _DEBUGVertexProcessing

void Camera::VertexProcessing(const Model& model)
{
#ifdef DEBUGVertexProcessing
    int nVerts = model.nVerts();
    vertexBuffer_WorldCoords.resize(nVerts);
    vertexBuffer_ScreenCoords.resize(nVerts);
    std::cerr << "Point :\t" << model[0] << "\n";
    std::cerr << "R :\t" << model.transform.rotation << "\n";
    std::cerr << "mR :\n" << MatrixRotate(model.transform.rotation) << "\n";
    // 模型坐标到世界坐标
    Matrix4x4 mLTW = model.transform.localToWorldMatrix();
    for (int i = 0; i < nVerts; i++)
        vertexBuffer_WorldCoords[i] = mLTW * Vector4(model[i], 1);
    std::cerr << "mLTW :\n" << mLTW << "\n";
    std::cerr << "Point :\t" << vertexBuffer_WorldCoords[0] << "\n";
    // 世界空间到观察空间
    Matrix4x4 mView = MatrixLookAt(transform.position, transform.forward(), transform.up());
    TransformVertices(mView, vertexBuffer_WorldCoords, vertexBuffer_ScreenCoords);
    std::cerr << "mView :\n" << mView << "\n";
    std::cerr << "Point :\t" << vertexBuffer_ScreenCoords[0] << "\n";
    // 观察空间到投影空间
    float aspect = (float)pixelWidth / pixelHeight; // 横纵比
    Matrix4x4 mPersp = MatrixPerspective(fieldOfView, aspect, nearClipPlane, farClipPlane);
    TransformVertices(mPersp, vertexBuffer_ScreenCoords, vertexBuffer_ScreenCoords);
    std::cerr << "mPersp :\n" << mPersp << "\n";
    std::cerr << "Point :\t" << vertexBuffer_ScreenCoords[0] << "\n";
    // 投影空间到NDC
    for (int i = 0; i < nVerts; i++)
        vertexBuffer_ScreenCoords[i] = vertexBuffer_ScreenCoords[i] / vertexBuffer_ScreenCoords[i].w;
    std::cerr << "Point :\t" << vertexBuffer_ScreenCoords[0] << "\n";
    // NDC到屏幕空间
    Matrix4x4 mScreen = Matrix4x4({
        {pixelWidth / 2.0f, 0, 0, pixelWidth / 2.0f},
        {0, pixelHeight / 2.0f, 0, pixelHeight / 2.0f},
        {0, 0, 1, 0},
        {0, 0, 0, 1}});
    TransformVertices(mScreen, vertexBuffer_ScreenCoords, vertexBuffer_ScreenCoords);
    std::cerr << "mScreen :\n" << mScreen << "\n";
    std::cerr << "Point :\t" << vertexBuffer_ScreenCoords[0] << "\n";
#else
    int nVerts = model.nVerts();
    vertexBuffer_WorldCoords.resize(nVerts);
    vertexBuffer_ScreenCoords.resize(nVerts);
    // 模型坐标到世界坐标
    Matrix4x4 mLTW = model.transform.localToWorldMatrix();
    for (int i = 0; i < nVerts; i++)
        vertexBuffer_WorldCoords[i] = mLTW * Vector4(model[i], 1);
    // 世界空间到屏幕空间
    Matrix4x4 mWTS = worldToScreenMatrix();
    TransformVertices(mWTS, vertexBuffer_WorldCoords, vertexBuffer_ScreenCoords);
#endif
}

void Camera::TransformVertices(const Matrix4x4& m, const std::vector<Vector4>& src, std::vector<Vector4>& dst)
{
    for (int i = 0; i < src.size(); i++)
    {
        dst[i] = m * src[i];
    }
}

void Camera::Rasterization(const Model& model)
{
    if (model.diffuseMap == nullptr)
    {
        FillRandomColor(model);
        return;
    }
    TextureMapping(model);
}

#define _DEBUGTextureMapping

void Camera::TextureMapping(const Model& model)
{
#ifdef DEBUGTextureMapping
    Matrix4x4 mSTW = screenToWorldMatrix();
    for (int i = 0; i < model.nFaces(); i++)
    {
        Face face = model.face(i);
        // 三角面屏幕空间的顶点
        Vector3 sp1 = vertexBuffer_ScreenCoords[face.vi[0]];
        Vector3 sp2 = vertexBuffer_ScreenCoords[face.vi[1]];
        Vector3 sp3 = vertexBuffer_ScreenCoords[face.vi[2]];

        Vector2 minPoint, maxPoint;
        minPoint.x = std::min(std::min(sp1.x, sp2.x), sp3.x);
        minPoint.y = std::min(std::min(sp1.y, sp2.y), sp3.y);
        maxPoint.x = std::max(std::max(sp1.x, sp2.x), sp3.x);
        maxPoint.y = std::max(std::max(sp1.y, sp2.y), sp3.y);
        // 剔除边界外的三角形
        if (minPoint.x > pixelWidth || minPoint.y > pixelHeight || maxPoint.x < 0 || maxPoint.y < 0)
            return;

        for (int sy = minPoint.y; sy < maxPoint.y; sy++)
        {
            if (sy < 0 || sy >= pixelHeight)
                continue;
            for (int sx = minPoint.x; sx < maxPoint.x; sx++)
            {
                if (sx < 0 || sx >= pixelWidth)
                    continue;
                Vector2 p(sx, sy);
                Vector3 baryCoord_Screen = BarycentricCoordinate(p, sp1, sp2, sp3);
                if (baryCoord_Screen[0] < 0 || baryCoord_Screen[1] < 0 || baryCoord_Screen[2] < 0)
                {
                    screen.Set({sx, sy}, Color::Red);
                    continue;
                }
                float sz = sp1.z * baryCoord_Screen[0] + sp2.z * baryCoord_Screen[1] + sp3.z * baryCoord_Screen[2];
                if (sz < -1 || sz > 1)
                    continue;
                if (zBuffer[sy][sx] < sz)
                    continue;
                zBuffer[sy][sx] = sz;
                // 三角面世界空间的顶点
                Vector3 wp1 = vertexBuffer_WorldCoords[face.vi[0]];
                Vector3 wp2 = vertexBuffer_WorldCoords[face.vi[1]];
                Vector3 wp3 = vertexBuffer_WorldCoords[face.vi[2]];
                Vector3 wp = mSTW * Vector4(sx, sy, sz, 1);
                Vector3 baryCoord_World = BarycentricCoordinate(wp, wp1, wp2, wp3);

                Vector2 uv1 = model.texCoord(face.ti[0]);
                Vector2 uv2 = model.texCoord(face.ti[1]);
                Vector2 uv3 = model.texCoord(face.ti[2]);
                float u = uv1[0] * baryCoord_World[0] + uv2[0] * baryCoord_World[1] + uv3[0] * baryCoord_World[2];
                float v = uv1[1] * baryCoord_World[0] + uv2[1] * baryCoord_World[1] + uv3[1] * baryCoord_World[2];
                Color c = model.diffuseMap->Get(u * model.diffuseMap->width, v * model.diffuseMap->height);
                screen.Set({sx, sy}, c);
            }
        }
    }

#else
    Matrix4x4 mSTW = screenToWorldMatrix();
    for (int i = 0; i < model.nFaces(); i++)
    {
        Face face = model.face(i);
        // 三角面屏幕空间的顶点
        Vector3 sp1 = vertexBuffer_ScreenCoords[face.vi[0]];
        Vector3 sp2 = vertexBuffer_ScreenCoords[face.vi[1]];
        Vector3 sp3 = vertexBuffer_ScreenCoords[face.vi[2]];

        Vector2 minPoint, maxPoint;
        minPoint.x = std::min(std::min(sp1.x, sp2.x), sp3.x);
        minPoint.y = std::min(std::min(sp1.y, sp2.y), sp3.y);
        maxPoint.x = std::max(std::max(sp1.x, sp2.x), sp3.x);
        maxPoint.y = std::max(std::max(sp1.y, sp2.y), sp3.y);
        // 剔除边界外的三角形
        if (minPoint.x > pixelWidth || minPoint.y > pixelHeight || maxPoint.x < 0 || maxPoint.y < 0)
            return;

        for (int sy = minPoint.y; sy < maxPoint.y; sy++)
        {
            if (sy < 0 || sy >= pixelHeight)
                continue;
            for (int sx = minPoint.x; sx < maxPoint.x; sx++)
            {
                if (sx < 0 || sx >= pixelWidth)
                    continue;
                Vector2 p(sx, sy);
                Vector3 baryCoord_Screen = BarycentricCoordinate(p, sp1, sp2, sp3);
                if (baryCoord_Screen[0] < 0 || baryCoord_Screen[1] < 0 || baryCoord_Screen[2] < 0)
                    continue;
                float sz = sp1.z * baryCoord_Screen[0] + sp2.z * baryCoord_Screen[1] + sp3.z * baryCoord_Screen[2];
                if (sz < -1 || sz > 1)
                    continue;
                if (zBuffer[sy][sx] < sz)
                    continue;
                zBuffer[sy][sx] = sz;
                // 三角面世界空间的顶点
                Vector3 wp1 = vertexBuffer_WorldCoords[face.vi[0]];
                Vector3 wp2 = vertexBuffer_WorldCoords[face.vi[1]];
                Vector3 wp3 = vertexBuffer_WorldCoords[face.vi[2]];
                Vector3 wp = mSTW * Vector4(sx, sy, sz, 1);
                Vector3 baryCoord_World = BarycentricCoordinate(wp, wp1, wp2, wp3);

                Vector2 uv1 = model.texCoord(face.ti[0]);
                Vector2 uv2 = model.texCoord(face.ti[1]);
                Vector2 uv3 = model.texCoord(face.ti[2]);
                float u = uv1[0] * baryCoord_World[0] + uv2[0] * baryCoord_World[1] + uv3[0] * baryCoord_World[2];
                float v = uv1[1] * baryCoord_World[0] + uv2[1] * baryCoord_World[1] + uv3[1] * baryCoord_World[2];
                Color c = model.diffuseMap->Get(u * model.diffuseMap->width, v * model.diffuseMap->height);
                screen.Set({sx, sy}, c);
            }
        }
    }

#endif
}

void Camera::FillRandomColor(const Model& model)
{
    for (int i = 0; i < model.nFaces(); i++)
    {
        Face face = model.face(i);
        Vector3 p1 = vertexBuffer_ScreenCoords[face.vi[0]];
        Vector3 p2 = vertexBuffer_ScreenCoords[face.vi[1]];
        Vector3 p3 = vertexBuffer_ScreenCoords[face.vi[2]];
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
            Vector4 p1 = vertexBuffer_ScreenCoords[face.vi[j]];
            Vector4 p2 = vertexBuffer_ScreenCoords[face.vi[(j + 1) % 3]];
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
    Bitmap zBufferImage(pixelWidth, pixelHeight, 32);
    for (int i = 0; i < pixelHeight; i++)
    {
        for (int j = 0; j < pixelWidth; j++)
        {
            byte c = zBuffer[i][j] > 1 ? 0 : (1.0 - zBuffer[i][j]) / 2 * 255;
            screen.Set({j, i}, Color{c, c, c});
        }
    }
    screen.Write(imagePath + imageName + "_ZBuffer.bmp");
}