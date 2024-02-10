#pragma once
#include "transform.h"
#include "model.h"

class Object
{
public:
    Model& model;
    Transform transform;
    Object(std::string modelPath, Vector3 position = Vector3(), Vector3 rotation = Vector3(), Vector3 scale = Vector3(1, 1, 1));
    ~Object();
};