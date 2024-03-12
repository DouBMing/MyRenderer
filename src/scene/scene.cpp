#include "scene.h"
#include <iostream>
#include <fstream>

enum ShaderType
{
    SNone,
    SFlat,
    SGouraud,
    SPhong,
    SBlinnPhong,
    SHalfLambert
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
    float fov, intensity, gloss;
    Color cLight, specular;
    LightType tLight;
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
        
        // 判断物体类型并设置全局变量
        if (prefix == "Camera:")
        {
            TObject = TCamera;
            continue;
        }
        if (prefix == "Light:")
        {
            TObject = TLight;
            continue;
        }
        if (prefix == "Model:")
        {
            TObject = TModel;
            continue;
        }
        if (prefix == "AmbientColor:")
        {
            Color c;
            iss >> c;
            Light::SetAmbient(c);
            continue;
        }

        switch (TObject)
        {
            case TCamera:
                if(prefix == "ScreenSize:")
                {
                    iss >> screenSize;
                    continue;
                }
                else if(prefix == "FOV:")
                {
                    iss >> fov;
                    continue;
                }
            case TLight:
                if(prefix == "Intensity:")
                {
                    iss >> intensity;
                    continue;
                }
                else if(prefix == "Color:")
                {
                    iss >> cLight;
                    continue;
                }
                else if (prefix == "Type:")
                {
                    iss >> tLight;
                    continue;
                }
            case TModel:
                if(prefix == "ObjName:")
                {
                    iss >> modelName;
                    continue;
                }
                else if(prefix == "Shader:")
                {
                    std::string shaderName;
                    iss >> shaderName;
                    if (shaderName == "NoShader")
                        TShader = SNone;
                    else if (shaderName == "FlatShader")
                        TShader = SFlat;
                    else if (shaderName == "GouraudShader")
                        TShader = SGouraud;
                    else if (shaderName == "PhongShader")
                        TShader = SPhong;
                    else if (shaderName == "BlinnPhongShader")
                        TShader = SBlinnPhong;
                    else if (shaderName == "HalfLambertShader")
                        TShader = SHalfLambert;
                    continue;
                }
                else if (prefix == "Specular:")
                {
                    iss >> specular;
                    continue;
                }
                else if (prefix == "Gloss:")
                {
                    iss >> gloss;
                    continue;
                }
        }

        if(prefix == "Position:")
        {
            iss >> position;
            continue;
        }
        if(prefix == "Rotation:")
        {
            iss >> rotation;
            continue;
        }
        if(prefix == "Scale:")
        {
            iss >> scale;
            continue;
        }
        if(prefix == "End")
        {
            switch (TObject)
            {
                case TCamera:
                    camera = new Camera(screenSize.x, screenSize.y, fov, position, rotation);
                    break;
                case TLight:
                    lights.push_back(new Light(intensity, cLight, tLight, position, rotation));
                    break;
                case TModel:
                    {
                        Model* model;
                        model = new Model(modelName, position, rotation, scale);
                        switch (TShader)
                        {
                            case SFlat:
                                model->shader = new FlatShader(specular, gloss, model, *camera);
                                break;
                            case SGouraud:
                                model->shader = new GouraudShader(specular, gloss, model, *camera);
                                break;
                            case SPhong:
                                model->shader = new PhongShader(specular, gloss, model, *camera);
                                break;
                            case SHalfLambert:
                                model->shader = new HalfLambertShader(specular, gloss, model, *camera);
                                break;
                        }
                        models.push_back(model);
                    }
                    break;
            }
            // 恢复默认值
            position = Vector3();
            rotation = Vector3();
            scale = Vector3(1, 1, 1);
            intensity = 1.0;
            cLight = Color::White;
            TShader = SNone;
            specular = Color::White;
            gloss = 20;
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