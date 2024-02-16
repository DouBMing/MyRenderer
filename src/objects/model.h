#pragma once
#include <vector>
#include "object.h"

class Model : public Object
{
public:
    Model(const std::string& modelFile);
    Model(const std::string& modelFile, Vector3 position, Vector3 rotation, Vector3 scale);
    ~Model();
    int nVerts();
    int nFaces();
    Vector3 vert(int index);
    std::vector<int> face(int index);
    Vector3 GetBoundsSize();
    Vector3 operator [](int index); // 通过索引器获取顶点
private:
    std::vector<Vector3> verts;
    std::vector<std::vector<int>> faces;
    Vector3 boundsSize; // 边界框大小
    void Read(const std::string& modelFile);
};
