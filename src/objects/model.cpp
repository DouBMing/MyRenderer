#include "model.h"
#include <iostream>
#include <fstream>

using std::string;

std::istringstream& operator >>(std::istringstream& iss, Face& f)
{
    char slash;
    for (int i = 0; i < 3; i++)
    {
        iss >> f.vi[i] >> slash >> f.ti[i] >> slash >> f.ni[i];
        // obj文件中的索引是从1开始的，因此需要减1
        f.vi[i]--;
        f.ti[i]--;
        f.ni[i]--;
    }
    return iss;
}

Model::Model(const string& modelName) : shader(nullptr)
{
    LoadOBJ(modelName);
}

Model::Model(const string& modelName, Vector3 position, Vector3 rotation, Vector3 scale)
    : Object(position, rotation, scale), shader(nullptr)
{
    LoadOBJ(modelName);
}

Model::~Model()
{
    if (shader != nullptr)
        delete shader;
}

int Model::nVerts() const
{
    return verts.size();
}

int Model::nFaces() const
{
    return faces.size();
}

Vector3 Model::vert(int faceIdx, int i) const
{
    return verts[faces[faceIdx].vi[i]];
}

Vector2 Model::texCoord(int faceIdx, int i) const
{
    return texCoords[faces[faceIdx].ti[i]];
}

Vector3 Model::normal(int faceIdx, int i) const
{
    return normals[faces[faceIdx].ni[i]];
}

Face Model::face(int index) const
{
    return faces[index];
}

Vector3 Model::operator [](int index) const
{
    return verts[index];
}

Material* Model::GetMaterial(int faceIdx) const
{
    if (materials.size() == 0)
        return nullptr;
    return materials[materialIndices[faceIdx]];
}

void Model::LoadOBJ(const string& modelName)
{
    string modelPath  = "Models/" + modelName + ".obj";

    std::ifstream in;
    in.open(modelPath, std::ifstream::in);
    if (in.fail())
    {
        std::cerr << "Can't open " + modelName + ".obj" << "\n";
        return;
    }
    
    string line;
    int mtlIndex = 0;
    while (!in.eof())
    {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        
        string prefix;
        iss >> prefix;
        if (prefix == "mtllib")
        {
            LoadMTL(modelName);
        }
        else if (prefix == "v")
        {
            Vector3 v;
            iss >> v;
            verts.push_back(v);

            bounds.minPoint.x = std::min(bounds.minPoint.x, v.x);
            bounds.minPoint.y = std::min(bounds.minPoint.y, v.y);
            bounds.minPoint.z = std::min(bounds.minPoint.z, v.z);
            bounds.maxPoint.x = std::max(bounds.maxPoint.x, v.x);
            bounds.maxPoint.y = std::max(bounds.maxPoint.y, v.y);
            bounds.maxPoint.z = std::max(bounds.maxPoint.z, v.z);
        }
        else if (prefix == "vt")
        {
            Vector2 t;
            iss >> t;
            texCoords.push_back(t);
        }
        else if (prefix == "vn")
        {
            Vector3 n;
            iss >> n;
            normals.push_back(n);
        }
        else if (prefix == "f")
        {
            Face f;
            iss >> f;
            faces.push_back(f);
            materialIndices.push_back(mtlIndex);
        }
        else if (prefix == "usemtl")
        {
            string name;
            iss >> name;
            for (int i = 0; i < materials.size(); i++)
            {
                if (materials[i]->name == name)
                {
                    mtlIndex = i;
                    break;
                }
            }
        }
    }
    Vector3 boundsCenter = bounds.center();
    bounds.minPoint -= bounds.center();
    bounds.maxPoint -= bounds.center();
    for (int i = 0; i < verts.size(); i++)
        verts[i] -= boundsCenter;   // 模型统一以边界框中心为原点
}

void Model::LoadMTL(const string& materialName)
{
    string materialPath  = "Models/" + materialName + ".mtl";

    std::ifstream in;
    in.open(materialPath, std::ifstream::in);
    if (in.fail())
    {
        std::cerr << "Can't open " + materialName + ".mtl" << "\n";
        return;
    }
    
    string line;
    Material* current = nullptr;
    while (!in.eof())
    {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        
        string prefix;
        iss >> prefix;
        if (prefix == "newmtl")
        {
            string name;
            iss >> name;
            current = new Material(name);
            materials.push_back(current);
        }
        else if (prefix == "Ka")
        {
            iss >> current->Ka;
        }
        else if (prefix == "Kd")
        {
            iss >> current->Kd;
        }
        else if (prefix == "Ks")
        {
            iss >> current->Ks;
        }
        else if (prefix == "Ns")
        {
            iss >> current->Ns;
        }
        else if (prefix == "d")
        {
            iss >> current->d;
        }
        else if (prefix == "map_Ka")
        {
            string path;
            iss >> path;
            current->ambientMap = new Bitmap();
            current->ambientMap->Read(path);
        }
        else if (prefix == "map_Kd")
        {
            string path;
            iss >> path;
            current->diffuseMap = new Bitmap();
            current->diffuseMap->Read(path);
        }
        else if (prefix == "map_Ks")
        {
            string path;
            iss >> path;
            current->specularMap = new Bitmap();
            current->specularMap->Read(path);
        }
    }
}