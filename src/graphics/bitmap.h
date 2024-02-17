#pragma once
#include "vector.h"

typedef unsigned char byte;

struct Color
{
    byte B, G, R, A;
    Color() : B(0), G(0), R(0), A(0xFF) {}
    Color(byte R, byte G, byte B) : B(B), G(G), R(R), A(0xFF) {}
    Color(byte R, byte G, byte B, byte A) : B(B), G(G), R(R), A(A) {}
    Color(byte* data) : B(data[0]), G(data[1]), R(data[2]), A(data[3]) {}
    static Color GetRandomColor()
    {
        return Color(std::rand()% 256, std::rand() % 256, std::rand() % 256);
    }
};

class Bitmap
{
public:
    int width;
    int height;
    int bit;

    Bitmap();
    Bitmap(int width, int height, int bitCount);
    Bitmap(const Bitmap& srcImage);
    ~Bitmap();
    void Read(std::string filePath);
    void Write(std::string filePath);
    Color Get(int x, int y);
    Color Get(Vector2Int p);
    void Set(int x, int y, Color c);
    void Set(Vector2Int p, Color c);
    void Clear();                   // 清除图像
private:
    Color* colorTable;              //调色板
    byte* data;                     //数据
};