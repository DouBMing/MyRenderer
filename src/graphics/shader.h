#pragma once
#include "matrix.h"
#include "bitmap.h"

class Model;
class Camera;

class IShader
{
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
protected:
    const Model* model;
    Matrix4x4 M, V, P;
    Matrix4x4 worldToObject;

    Vector3 WorldSpaceViewDir(Vector3 worldPos);
    Vector3 ToWorldNormal(Vector3 normal);
    Vector2 GetUV(Vector3 baryCoord, int faceIdx);
    Vector3 GetWorldNormal(Vector3 baryCoord, int faceIdx);
};

// 平面着色shader
class FlatShader : public IShader
{
public:
    FlatShader(Color cSpecular, float gloss, const Model* model, Camera& camera);
    virtual ~FlatShader() override;
    virtual Vector4 vertex(int faceIdx, int i) override;
    virtual Color fragment(Vector3 baryCoord, int faceIdx) override;
private:
    std::vector<Vector4> worldCoords;
    Color specularColor;
    float gloss;
};

// 高洛德着色shader
class GouraudShader : public IShader
{
public:
    GouraudShader(Color cSpecular, float gloss, const Model* model, Camera& camera);
    virtual ~GouraudShader() override;
    virtual Vector4 vertex(int faceIdx, int i) override;
    virtual Color fragment(Vector3 baryCoord, int faceIdx) override;
private:
    std::vector<Vector4> worldCoords;
    std::vector<Color> lightColors;
    Color specularColor;
    float gloss;
};

// 冯氏着色Shader
class PhongShader : public IShader
{
public:
    PhongShader(Color cSpecular, float gloss, const Model* model, Camera& camera);
    virtual ~PhongShader() override;
    virtual Vector4 vertex(int faceIdx, int i) override;
    virtual Color fragment(Vector3 baryCoord, int faceIdx) override;
private:
    std::vector<Vector4> worldCoords;
    Color specularColor;
    float gloss;
};

class BlinnPhongShader : public IShader
{
public:
    BlinnPhongShader(Color cSpecular, float gloss, const Model* model, Camera& camera);
    virtual ~BlinnPhongShader() override;
    virtual Vector4 vertex(int faceIdx, int i) override;
    virtual Color fragment(Vector3 baryCoord, int faceIdx) override;
private:
    std::vector<Vector4> worldCoords;
    Color specularColor;
    float gloss;
};

class HalfLambertShader : public IShader
{
public:
    HalfLambertShader(Color cSpecular, float gloss, const Model* model, Camera& camera);
    virtual ~HalfLambertShader() override;
    virtual Vector4 vertex(int faceIdx, int i) override;
    virtual Color fragment(Vector3 baryCoord, int faceIdx) override;
private:
    std::vector<Vector4> worldCoords;
    Color specularColor;
    float gloss;
};