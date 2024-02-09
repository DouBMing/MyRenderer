#include "object.h"
#include "matrix.h"

Object::Object(std::string modelPath) : model(*new Model(modelPath)), transform() {}

Object::~Object()
{
    delete &model;
}