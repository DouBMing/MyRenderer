#pragma once
#include "transform.h"

class Scene;

class Object
{
public:
    Transform transform;
    Object(Vector3 position = Vector3(), Vector3 rotation = Vector3(), Vector3 scale = Vector3(1, 1, 1));
    Object(const Object& obejct);
    ~Object();
protected:
    Scene& scene;   // 物体所属的场景
};