#pragma once
#include <vector>
#include "object.h"
#include "bitmap.h"
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

    Model(const std::string& modelFile);
    Model(const std::string& modelFile, Vector3 position, Vector3 rotation, Vector3 scale);
    Model(const Model& model);
    ~Model();
    int nVerts() const;
    int nFaces() const;
    Vector3 vert(int faceIdx, int i) const;
    Vector2 texCoord(int faceIdx, int i) const;
    Vector3 normal(int faceIdx, int i) const;
    Face face(int index) const;
    Vector3 GetBoundsSize() const;
    Vector3 operator [](int index) const;  // 通过索引器获取顶点
private:
    std::vector<Vector3> verts;      // 顶点坐标
    std::vector<Vector2> texCoords;  // 材质坐标
    std::vector<Vector3> normals;    // 法线坐标
    std::vector<Face> faces;
    Vector3 boundsSize;              // 边界框大小
    void LoadOBJ(const std::string& modelFile);
};
