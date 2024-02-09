#pragma once
#include "transform.h"
#include "model.h"

class Object
{
public:
    Model& model;
    Transform transform;
    Object(std::string modelPath);
    ~Object();
};