#pragma once
#include "vector.h"

class IShader
{
public:
    virtual Vector4 vertex() = 0;
    virtual bool fragment() = 0;
};

// 平面着色shader
class FlatShader : public IShader
{

};