#pragma once
#include "bitmap.h"

class Painter
{
public:
    Painter(Bitmap& image);
    ~Painter();
    void Line(int x1, int y1, int x2, int y2, Color c);
    void Line(Vector2Int p1, Vector2Int p2, Color c);
private:
    Bitmap& image;
};