#include "painter.h"
#include <vector>

Painter::Painter(Bitmap& image) : image(image) {}

Painter::~Painter() {}

// 在图片上画线，使用Bresenham直线算法
void Painter::Line(int x1, int y1, int x2, int y2, Color c)
{
    int dx = abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, derr;    // 误差值

    while (true)
    {
        image.Set(x1, y1, c);   // 从(x1, y1)画起
        if (x1 == x2 && y1 == y2)
            break;
        derr = 2 * err;
        if (derr >= dy)
        {
            err += dy;
            x1 += sx;
        }
        if (derr <= dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void Painter::Line(Vector2Int p1, Vector2Int p2, Color c)
{
    Line(p1.x, p1.y, p2.x, p2.y, c);
}