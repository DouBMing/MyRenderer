#include "scene.h"
#include <iostream>
#include <fstream>

enum ShaderType
{
    None,
    SFlat,
    SGouraud,
    SPhong
};

enum ObjectType
{
    TCamera,
    TLight,
    TModel
};

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
    ObjectType TObject;
    Vector2Int screenSize;
    float fov, intensity;
    Color cLight;
    std::string modelName;
    ShaderType TShader;
    Vector3 position, rotation, scale;
    while (!in.eof())
    {
        std::getline(in, line);
        // "//"表示注释
        if (line.compare(0, 2, "//") == 0)
            continue;

        std::istringstream iss(line.c_str());
        std::string prefix;
        iss >> prefix;
        
        if (prefix == "Camera:")
            TObject = TCamera;
        else if (prefix == "Light:")
            TObject = TLight;
        else if (prefix == "Model:")
            TObject = TModel;

        switch (TObject)
        {
            case TCamera:
                if(prefix == "ScreenSize:")
                    iss >> screenSize;
                else if(prefix == "FOV:")
                    iss >> fov;
                break;
            case TLight:
                if(prefix == "Intensity:")
                    iss >> intensity;
                else if(prefix == "Color:")
                    iss >> cLight;
                break;
            case TModel:
                if(prefix == "ObjName:")
                {
                    iss >> modelName;
                }
                else if(prefix == "Shader:")
                {
                    std::string shaderName;
                    iss >> shaderName;
                    if (shaderName == "NoShader")
                        TShader = None;
                    else if (shaderName == "FlatShader")
                        TShader = SFlat;
                    else if (shaderName == "GouraudShader")
                        TShader = SGouraud;
                    else if (shaderName == "PhongShader")
                        TShader = SPhong;
                }
                break;
        }

        if(prefix == "Position:")
        {
            iss >> position;
        }
        else if(prefix == "Rotation:")
        {
            iss >> rotation;
        }
        else if(prefix == "Scale:")
        {
            iss >> scale;
        }
        else if(prefix == "End")
        {
            switch (TObject)
            {
                case TCamera:
                    camera = new Camera(screenSize.x, screenSize.y, fov, position, rotation);
                    break;
                case TLight:
                    lights.push_back(new Light(intensity, cLight, position, rotation));
                    break;
                case TModel:
                    {
                        Model* model;
                        model = new Model(modelName, position, rotation, scale);
                        switch (TShader)
                        {
                            case SFlat:
                                model->shader = new FlatShader(model, *camera);
                                break;
                            case SGouraud:
                                model->shader = new GouraudShader(model, *camera);
                                break;
                            case SPhong:
                                model->shader = new PhongShader(model, *camera);
                                break;
                        }
                        models.push_back(model);
                    }
                    break;
            }
            position = Vector3();
            rotation = Vector3();
            scale = Vector3(1, 1, 1);
            intensity = 1.0;
            cLight = Color(255, 255, 255);
            TShader = None;
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