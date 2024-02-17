#include "scene.h"
#include <iostream>
#include <fstream>

Scene* Scene::current = nullptr;

Scene::Scene(const std::string& scenePath)
{
    current = this;

    std::ifstream in;
    in.open(scenePath, std::ifstream::in);
    if (in.fail())
    {
        std::cerr << "Can't open file." << "\n";
        return;
    }

    std::string line;
    while (!in.eof())
    {
        std::getline(in, line);
        if (line.compare(0, 2, "//") == 0) // "//"表示注释
            continue;
        std::istringstream iss(line.c_str());
        std::string type;
        iss >> type;
        if (type.compare("Camera") == 0)
        {
            int width, height;
            iss >> width >> height;
            if (iss.eof())
            {
                camera = new Camera(width, height);
            }
            else
            {
                Vector3 position, rotation;
                iss >> position >> rotation;
                camera = new Camera(width, height, position, rotation);
            }
        }
        else if (type.compare("Model") == 0)
        {
            std::string modelName, textrueName;
            iss >> modelName >> textrueName;
            std::string modelPath = "Models/" + modelName + ".obj";
            std::string textruePath = textrueName == "no_texture" ? "" : "Textures/" + textrueName + ".bmp";
            if (iss.eof())
            {
                Model model(modelPath, textruePath);
                models.push_back(model);
            }
            else
            {
                Vector3 position, rotation, scale;
                iss >> position >> rotation >> scale;
                Model model(modelPath, textruePath, position, rotation, scale);
                models.push_back(model);
            }
        }
        else if (type.compare("Light") == 0)
        {
            Vector3 position, rotation;
            if (iss.eof())
            {
                Light light;
                lights.push_back(light);
            }
            else
            {
                Vector3 position, rotation;
                iss >> position >> rotation;
                Light light(position, rotation);
                lights.push_back(light);
            }
        }
    }
}

Scene::~Scene()
{
    delete camera;
}