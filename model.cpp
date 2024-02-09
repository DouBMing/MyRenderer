#include "model.h"
#include <iostream>
#include <fstream>
#include <limits>

using std::string;

static float floatMin = std::numeric_limits<float>::min();
static float floatMax = std::numeric_limits<float>::max();

Model::Model(string filePath) : verts(), faces()
{
    std::ifstream in;
    in.open(filePath, std::ifstream::in);
    if (in.fail())
        return;
    
    Vector3 minPoint = Vector3(floatMax, floatMax, floatMax);
    Vector3 maxPoint = Vector3(floatMin, floatMin, floatMin);

    string line;
    while (!in.eof())
    {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v "))
        {
            iss >> trash;
            Vector3 v;
            for (int i = 0; i < 3; i++)
                iss >> v;
            verts.push_back(v);

            minPoint.x = std::min(minPoint.x, v.x);
            minPoint.y = std::min(minPoint.y, v.y);
            minPoint.z = std::min(minPoint.z, v.z);
            maxPoint.x = std::max(maxPoint.x, v.x);
            maxPoint.y = std::max(maxPoint.y, v.y);
            maxPoint.z = std::max(maxPoint.z, v.z);
        }
        else if(!line.compare(0, 2, "f "))
        {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash)
            {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            faces.push_back(f);
        }
    }
    Vector3 boundsCenter = (minPoint + maxPoint) / 2;
    std::cerr << "BoundsCenter:\t" << boundsCenter << "\n";
    for (int i = 0; i < verts.size(); i++)
        verts[i] -= boundsCenter;   // 模型统一以边界框中心为原点
    boundsSize = maxPoint - minPoint;
    std::cerr << "BoundsSize:\t" << boundsSize << "\n";
}

Model::~Model() {}

int Model::nVerts()
{
    return verts.size();
}

int Model::nFaces()
{
    return faces.size();
}

Vector3 Model::vert(int index)
{
    return verts[index];
}

std::vector<int> Model::face(int index)
{
    return faces[index];
}

Vector3 Model::GetBoundsSize()
{
    return boundsSize;
}

Vector3 Model::operator [](int index)
{
    return verts[index];
}