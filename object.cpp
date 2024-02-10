#include "object.h"
#include "matrix.h"

Object::Object(std::string modelPath, Vector3 position, Vector3 rotation, Vector3 scale)
    : model(*new Model(modelPath)), transform(position, rotation, scale) {}

Object::~Object()
{
    delete &model;
}