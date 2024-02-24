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
        // "//"表示注释
        if (line.compare(0, 2, "//") == 0)
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
            std::string modelName, textureName;
            iss >> modelName;
            std::string modelPath = "Models/" + modelName + ".obj";
            std::string texturePath;
            int shaderIndex;
            iss >> shaderIndex;
            switch (shaderIndex)
            {
                case 1:
                case 2:
                case 3:
                    iss >> textureName;
                    texturePath = textureName == "no_texture" ? "" : "Textures/" + textureName + ".bmp";
                    break;
            }

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
                    model->shader = new FlatShader(texturePath, model, *camera);
                    break;
                case 2:
                    model->shader = new GouraudShader(texturePath, model, *camera);
                    break;
                case 3:
                    model->shader = new PhongShader(texturePath, model, *camera);
                    break;
            }
            models.push_back(model);
        }
        else if (type.compare("Light") == 0)
        {
            Vector3 position, rotation;
            float intensity;
            Color c;
            iss >> intensity;
            iss >> c;
            Light* light;
            if (iss.eof())
            {
                light = new Light(intensity, c);
            }
            else
            {
                Vector3 position, rotation;
                iss >> position >> rotation;
                light = new Light(intensity, c, position, rotation);
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