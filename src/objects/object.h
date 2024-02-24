#pragma once
#include "transform.h"

class Scene;

class Object
{
public:
    Transform transform;
    Object();
    Object(Vector3 position, Vector3 rotation, Vector3 scale);
    Object(const Object& obejct);
    ~Object();
protected:
    Scene& scene;   // 物体所属的场景
};