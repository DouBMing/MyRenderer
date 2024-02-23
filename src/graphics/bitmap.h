#pragma once
#include "vector.h"

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
    void Read(const std::string& filePath);
    void Write(const std::string& filePath);
    Color Get(int x, int y);
    Color Get(Vector2Int p);
    void Set(int x, int y, Color c);
    void Set(int x, int y, byte grayScale);
    void Set(Vector2Int p, Color c);
    void Set(Vector2Int p, byte grayScale);
    void Clear();                   // 清除图像
private:
    Color* colorTable;              //调色板
    byte* data;                     //数据
};