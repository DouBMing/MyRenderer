#pragma once
#include <vector>
#include "vector.h"

class Model
{
public:
    Model(std::string filePath);
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
};
