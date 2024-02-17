#pragma once
#include <vector>
#include "object.h"
#include "bitmap.h"

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
    Model(const std::string& modelFile);
    Model(const std::string& modelFile, Vector3 position, Vector3 rotation, Vector3 scale);
    ~Model();
    int nVerts();
    int nFaces();
    Vector3 vert(int index);
    Face face(int index);
    Vector3 GetBoundsSize();
    Vector3 operator [](int index); // 通过索引器获取顶点
private:
    std::vector<Vector3> verts;      // 顶点坐标
    std::vector<Vector2> texCoords;  // 材质坐标
    std::vector<Vector3> normals;    // 法线坐标
    std::vector<Face> faces;
    Vector3 boundsSize; // 边界框大小
    void ReadOBJ(const std::string& modelFile);
    Bitmap diffuseMap;  // 漫反射贴图
};
