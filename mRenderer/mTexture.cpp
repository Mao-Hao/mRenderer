#include <fstream>
#include "mTexture.h"

using namespace std;

mTGATexture::mTGATexture() 
    : data(nullptr)
    , width(0)
    , height(0)
    , bytesPerPixel(0)
{}

mTGATexture::mTGATexture(int w, int h, int bpp) 
    : data(NULL)
    , width(w)
    , height(h)
    , bytesPerPixel(bpp) {
    ulong nbytes = width * height * bytesPerPixel;
    data = new uchar[nbytes];
    memset(data, 0, nbytes);
}

mTGATexture::mTGATexture(const mTGATexture & tex) 
    : data(NULL)
    , width(tex.width)
    , height(tex.height)
    , bytesPerPixel(tex.bytesPerPixel) {
    ulong nbytes = width * height * bytesPerPixel;
    data = new uchar[nbytes];
    memcpy(data, tex.data, nbytes);
}

mTGATexture::~mTGATexture() {
    if (data) delete [] data;
}

Vec4f mTGATexture::getColor(int x, int y) {
    if (!data || x < 0 || y < 0 || x >= width || y >= height)
        return Vec4f();
    auto p = data + (x + y * width) * bytesPerPixel;
    uchar bgra[4] = {0, 0, 0, 0};
    for (int i = 0; i < bytesPerPixel; i++)
        bgra[i] = p[i];
    return {(float)bgra[2], (float)bgra[1], (float)bgra[0], (float)bgra[3]};
}

bool mTGATexture::loadData(ifstream & in) {
    ulong pixelcount = width * height;
    ulong currentpixel = 0;
    ulong currentbyte  = 0;
    uchar bgra[4];
    do {
        uchar chunkheader = 0;
        chunkheader = in.get();
        if (!in.good()) {
            std::cerr << "an error occured while reading the data\n";
            return false;
        }
        if (chunkheader < 128) {
            chunkheader++;
            for (int i = 0; i < chunkheader; i++) {
                in.read((char *)bgra, bytesPerPixel);
                if (!in.good()) {
                    std::cerr << "an error occured while reading the header\n";
                    return false;
                }
                for (int t = 0; t < bytesPerPixel; t++)
                    data[currentbyte++] = bgra[t];
                currentpixel++;
                if (currentpixel>pixelcount) {
                    std::cerr << "Too many pixels read\n";
                    return false;
                }
            }
        } 
        else {
            chunkheader -= 127;
            in.read((char *)bgra, bytesPerPixel);
            if (!in.good()) {
                std::cerr << "an error occured while reading the header\n";
                return false;
            }
            for (int i = 0; i < chunkheader; i++) {
                for (int t = 0; t < bytesPerPixel; t++)
                    data[currentbyte++] = bgra[t];
                currentpixel++;
                if (currentpixel > pixelcount) {
                    std::cerr << "Too many pixels read\n";
                    return false;
                }
            }
        }
    } while (currentpixel < pixelcount);
    return true;
}

bool mTGATexture::loadTGAImage(const char * path) {
    if (data) { 
        delete [] data;
        data = nullptr;
    }
    ifstream in;
    in.open (path, std::ios::binary);
    if (!in.is_open()) {
        cerr << "can't open file " << path << "\n";
        in.close();
        return false;
    }
    TGAHeader header;
    in.read((char *)&header, sizeof(header));
    if (!in.good()) {
        in.close();
        std::cerr << "an error occured while reading the header\n";
        return false;
    }
    width   = header.ImageWidth;
    height  = header.ImageHeight;
    bytesPerPixel = header.PixelDepth >> 3;
    if (width <= 0 || height <= 0 || 
       (bytesPerPixel != GRAYSCALE && bytesPerPixel != RGB && bytesPerPixel !=RGBA)) {
        in.close();
        std::cerr << "bad bpp (or width/height) value\n";
        return false;
    }
    ulong bytesNum = bytesPerPixel * width * height;
    data = new uchar[bytesNum];
    if (3 == header.ImageType || 2 == header.ImageType) {
        in.read((char *)data, bytesNum);
        if (!in.good()) {
            in.close();
            std::cerr << "an error occured while reading the data\n";
            return false;
        }
    } else if (10 == header.ImageType || 11 == header.ImageType) {
        if (!loadData(in)) {
            in.close();
            std::cerr << "an error occured while reading the data\n";
            return false;
        }
    } else {
        in.close();
        std::cerr << "Unknown ImageType " << (int)header.ImageType << "\n";
        return false;
    }
    std::cerr << width << "x" << height << "/" << bytesPerPixel * 8 << "\n";
    in.close();
    return true;
}

bool mTGATexture::flip_vertically()
{
    if ( !data ) return false;
    unsigned long bytes_per_line = width * bytesPerPixel;
    unsigned char * line = new unsigned char[bytes_per_line];
    int half = height >> 1;
    for ( int j = 0; j < half; j++ ) {
        unsigned long l1 = j * bytes_per_line;
        unsigned long l2 = ( height - 1 - j ) * bytes_per_line;
        memmove( (void *)line, (void *)( data + l1 ), bytes_per_line );
        memmove( (void *)( data + l1 ), (void *)( data + l2 ), bytes_per_line );
        memmove( (void *)( data + l2 ), (void *)line, bytes_per_line );
    }
    delete[] line;
    return true;
}