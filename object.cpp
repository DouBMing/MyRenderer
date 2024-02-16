#include "object.h"
#include "scene.h"

Object::Object(Vector3 position, Vector3 rotation, Vector3 scale)
    : scene(*Scene::current), transform(position, rotation, scale) {}

Object::~Object() {}