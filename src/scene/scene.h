#pragma once
#include <vector>
#include "camera.h"
#include "model.h"
#include "light.h"

class Scene
{
public:
    static Scene* current;

    std::vector<Model*> models;
    std::vector<Light*> lights;
    Camera* camera;
    
    Scene(const std::string& scenePath);
    ~Scene();
};