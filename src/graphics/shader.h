#pragma once
#include "matrix.h"
#include "bitmap.h"

class Model;
class Camera;

class IShader
{
protected:
    const Model* model;
    Matrix4x4 M, V, P;
public:
    IShader(const Model* model, Camera& camera);
    virtual ~IShader();
    /// @brief 顶点着色器
    /// @param faceIdx 面序号
    /// @param i 顶点序号
    virtual Vector4 vertex(int faceIdx, int i) = 0;
    /// @brief 片元着色器
    /// @param baryCoord 重心坐标
    /// @param faceIdx 面序号
    virtual Color fragment(Vector3 baryCoord, int faceIdx) = 0;
};

// 平面着色shader
class FlatShader : public IShader
{
private:
    std::vector<Vector4> worldCoords;
    Bitmap* diffuseMap;
public:
    FlatShader(const std::string& texturePath, const Model* model, Camera& camera);
    virtual ~FlatShader() override;
    virtual Vector4 vertex(int faceIdx, int i) override;
    virtual Color fragment(Vector3 baryCoord, int faceIdx) override;
};

// 高洛德着色shader
class GouraudShader : public IShader
{
private:
    std::vector<Color> vertexColors;
    Bitmap* diffuseMap;
public:
    GouraudShader(const std::string& texturePath, const Model* model, Camera& camera);
    virtual ~GouraudShader() override;
    virtual Vector4 vertex(int faceIdx, int i) override;
    virtual Color fragment(Vector3 baryCoord, int faceIdx) override;
};