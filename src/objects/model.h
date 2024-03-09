#pragma once
#include <vector>
#include "object.h"
#include "material.h"
#include "bounds.h"
#include "shader.h"

struct Face
{
    int vi[3];  // 顶点索引
    int ti[3];  // 材质索引
    int ni[3];  // 法线索引
};

std::istringstream& operator >>(std::istringstream& iss, Face& v);

class Model : public Object
{
public:
    IShader* shader;

    Model(const std::string& modelName);
    Model(const std::string& modelName, Vector3 position, Vector3 rotation, Vector3 scale);
    ~Model();
    int nVerts() const;
    int nFaces() const;
    Vector3 vert(int faceIdx, int i) const;
    Vector2 texCoord(int faceIdx, int i) const;
    Vector3 normal(int faceIdx, int i) const;
    Face face(int index) const;
    Vector3 operator [](int index) const;   // 通过索引器获取顶点
    Material* GetMaterial(int faceIdx) const;
private:
    std::vector<Vector3> verts;             // 顶点坐标
    std::vector<Vector2> texCoords;         // 材质坐标
    std::vector<Vector3> normals;           // 法线坐标
    std::vector<Face> faces;                // 三角面信息
    std::vector<Material*> materials;       // 材质信息
    std::vector<int> materialIndices;       // 面对应的材质序号
    Bounds bounds;                          // 边界框
    void LoadOBJ(const std::string& modelName);
    void LoadMTL(const std::string& materialName);
};