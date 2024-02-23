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
            std::string modelName;
            iss >> modelName;
            std::string modelPath = "Models/" + modelName + ".obj";
            int shaderIndex;
            iss >> shaderIndex;
            
            Model* model;
            if (iss.eof())
            {
                model = new Model(modelPath);
            }
            else
            {
                Vector3 position, rotation, scale;
                iss >> position >> rotation >> scale;
                model = new Model(modelPath, position, rotation, scale);
            }
            
            switch (shaderIndex)
            {
                case 1:
                    model->shader = new FlatShader(model, *camera);
                    break;
            }
            models.push_back(model);
        }
        else if (type.compare("Light") == 0)
        {
            Vector3 position, rotation;
            float intensity;
            iss >> intensity;
            Light* light;
            if (iss.eof())
            {
                light = new Light(intensity);
            }
            else
            {
                Vector3 position, rotation;
                iss >> position >> rotation;
                light = new Light(position, rotation, intensity);
            }
            lights.push_back(light);
        }
    }
}

Scene::~Scene()
{
    for (Model* m : models)
        delete m;
    for (Light* l : lights)
        delete l;
    delete camera;
}