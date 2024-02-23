#include "bitmap.h"
#include <windows.h>
#include <cstring>
#include <memory>
#include <iostream>
#include <fstream>

const static byte fillColor = 0x0;

BITMAPFILEHEADER GetFileHeader(int width, int height, int bit)
{
    int nColor = (bit == 24 || bit == 32 ? 0 : 1 << bit);
    int colorTableSize = nColor * sizeof(Color);

    BITMAPFILEHEADER fileHeader;
    fileHeader.bfType = 0x4D42;
	fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
			colorTableSize + width * height * bit / 8;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 
		colorTableSize;
    return fileHeader;
}

BITMAPINFOHEADER GetInfoHeader(int width, int height, int bit)
{
    BITMAPINFOHEADER bmiHeader;
    bmiHeader.biSize = 40;
	bmiHeader.biWidth = width;
	bmiHeader.biHeight = height;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = bit;
	bmiHeader.biCompression = 0;
	bmiHeader.biSizeImage = width * height * bit / 8;
	bmiHeader.biXPelsPerMeter = 0;
	bmiHeader.biYPelsPerMeter = 0;
	bmiHeader.biClrUsed = 0;
	bmiHeader.biClrImportant = 0;
    return bmiHeader;
}

int GetColorCount(int bit)
{
    return bit == 24 || bit == 32 ? 0 : 1 << bit;
}

Bitmap::Bitmap() : width(0), height(0), bit(0), colorTable(nullptr), data(nullptr) {}

Bitmap::Bitmap(int width, int height, int bit) : width(width), height(height), bit(bit)
{
    int nColor = GetColorCount(bit);
    int colorTableSize = nColor * sizeof(Color);

    colorTable = new Color[nColor];
	std::memset((void*)colorTable, fillColor, colorTableSize);

	if (bit == 8)
	{
		// 生成灰度调色板
		for (int i = 0; i < 256; i++)
			colorTable[i] = Color(i, i, i, 0);
	}
	data = (byte*)::operator new(width * height * bit / 8);
	std::memset(data, fillColor, width * height * bit / 8);
}

Bitmap::Bitmap(const Bitmap& srcImage)
{
    width = srcImage.width;
    height = srcImage.height;
    bit = srcImage.bit;
    int nColor = GetColorCount(bit);
    int colorTableSize = nColor * sizeof(Color);
    colorTable = new Color[nColor];
    if (bit == 8)
        std::memcpy(colorTable, srcImage.colorTable, colorTableSize);
    data = (byte*)::operator new(width * height * bit / 8);
    std::memcpy(data, srcImage.data, width * height * bit / 8);
}

Bitmap::~Bitmap()
{
    if (colorTable)
        delete[] colorTable;
    if (data)
        ::operator delete(data);
}

void Bitmap::Read(const std::string& filePath)
{
    if (colorTable)
        delete[] colorTable;
    if (data)
        ::operator delete(data);
    colorTable = nullptr;
    data = nullptr;
    
    std::ifstream in;
    // 打开文件
    in.open(filePath, std::ios::binary);
    if (!in.is_open())
    {
		std::cerr << "Can't open file " << filePath << "\n";
		in.close();
		return;
	}
    
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER bmiHeader;
    in.read((char*)&fileHeader, sizeof(BITMAPFILEHEADER));  // 读取文件信息头
    in.read((char*)&bmiHeader, sizeof(BITMAPINFOHEADER));   // 读取图像信息头
    width = bmiHeader.biWidth;
    height = bmiHeader.biHeight;
    bit = bmiHeader.biBitCount;
    
    int nColor = GetColorCount(bit);
    int colorTableSize = nColor * sizeof(Color);
    colorTable = new Color[nColor];
    in.read((char*)colorTable, colorTableSize);             // 读取调色板

    in.seekg(fileHeader.bfOffBits, std::ios::beg);          // 跳到像素数据开始位置
    int linePixelByte = bmiHeader.biWidth * bmiHeader.biBitCount / 8;
    int deltaByte = (4 - linePixelByte % 4) % 4; 	        // 计算每行字节偏差值
    data = (byte*)::operator new(linePixelByte * bmiHeader.biHeight);
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        in.read((char*)data + linePixelByte * i, linePixelByte);
        in.seekg(deltaByte, std::ios::cur);                 // 忽略补齐4字节所用的字节数
    }

    in.close(); // 关闭图片
}

void Bitmap::Write(const std::string& filePath)
{
    std::ofstream out;
    // 新建图片
    out.open(filePath, std::ios::binary);
    if (!out.is_open())
    {
		std::cerr << "Can't open file " << filePath << "\n";
		out.close();
		return;
	}

    BITMAPFILEHEADER fileHeader = GetFileHeader(width, height, bit);
    BITMAPINFOHEADER bmiHeader = GetInfoHeader(width, height, bit);
    out.write((char*)&fileHeader, sizeof(BITMAPFILEHEADER));    // 写入文件信息头
    out.write((char*)&bmiHeader, sizeof(BITMAPINFOHEADER));     // 写入图像信息头

    int colorTableSize = GetColorCount(bit) * sizeof(Color);
    out.write((char*)colorTable, colorTableSize);               // 写入调色板

    int linePixelByte = bmiHeader.biWidth * bmiHeader.biBitCount / 8;
    int deltaByte = (4 - linePixelByte % 4) % 4;                // 计算每行字节偏差值
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        out.write((char*)data + linePixelByte * i, linePixelByte);			
        out.write("\0\0", deltaByte);                           // 忽略补齐4字节所用的字节数							
    }

	out.close();     											// 关闭图片
}

Color Bitmap::Get(int x, int y) // 屏幕空间的x, y
{
    if (!data || x < 0 || y < 0 || x >= width || y >= height)
        return Color();

    if (bit == 8)
    {
        return colorTable[data[(x + y * width) * bit / 8]];
    }
    return Color(data + (x + y * width) * bit / 8, bit);
}

Color Bitmap::Get(Vector2Int p)
{
    if (!data || p.x < 0 || p.y < 0 || p.x >= width || p.y >= height)
        return Color();

    if (bit == 8)
    {
        return colorTable[data[(p.x + p.y * width) * bit / 8]];
    }
    return Color(data + (p.x + p.y * width) * bit / 8, bit);
}

void Bitmap::Set(int x, int y, Color c)
{
    if (bit == 8)
        return;
    if (!data || x < 0 || y < 0 || x >= width || y >= height)
        return;

    std::memcpy(data + (x + y * width) * bit / 8, (const char*)&c, bit / 8);
}

void Bitmap::Set(int x, int y, byte grayScale)
{
    if (bit != 8)
        return;
    if (!data || x < 0 || y < 0 || x >= width || y >= height)
        return;

    data[(x + y * width) * bit / 8] = grayScale;
}

void Bitmap::Set(Vector2Int p, Color c)
{
    if (bit == 8)
        return;
    if (!data || p.x < 0 || p.y < 0 || p.x >= width || p.y >= height)
        return;
    
    std::memcpy(data + (p.x + p.y * width) * bit / 8, (const char*)&c, bit / 8);
}

void Bitmap::Set(Vector2Int p, byte grayScale)
{
    if (bit != 8)
        return;
    if (!data || p.x < 0 || p.y < 0 || p.x >= width || p.y >= height)
        return;

    data[(p.x + p.y * width) * bit / 8] = grayScale;
}

void Bitmap::Clear()
{
    std::memset(data, fillColor, width * height * bit / 8);
}