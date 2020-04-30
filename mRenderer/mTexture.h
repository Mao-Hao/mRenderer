#pragma once
#include "mBase.h"
#include "mMath.hpp"

#pragma pack(push, 1)
struct TGAHeader {
    char  IDLength;
    char  ColorMapType;
    char  ImageType;
    // Color map specification, 5 bytes
    short FirstEntryIndex;
    short ColorMapLength;
    char  ColorMapEntrySize;
    // Image specification, 10 bytes
    short X_Origin;
    short Y_Origin;
    short ImageWidth;
    short ImageHeight;
    char  PixelDepth;
    char  ImageDescriptor;
};
#pragma pack(pop)

// .TGA
class mTGATexture {
public:
    mTGATexture();
    mTGATexture(int w, int h, int bpp);
    mTGATexture(const mTGATexture & tex);
    ~mTGATexture();
    uchar * data;
    int width;
    int height;
    int bytesPerPixel;

    enum Format { GRAYSCALE = 1, RGB = 3, RGBA = 4};

    Vec4f getColor(int x, int y);

    bool loadData(std::ifstream &in);
    bool loadTGAImage(const char * path);
    bool flip_vertically();
};

