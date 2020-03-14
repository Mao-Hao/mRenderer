#pragma once
#include <vector>
#include <string>
#include <array>
#include "mMath.hpp"
#include "mTexture.h"
#include "mColor.h"

class mModel {
private:
    const char * modelPath;
public:
    std::vector<Vec3f> vertices;
    // зЂвт Vec3i (x: vertex, y: uv, z: normal) 
    std::vector<std::vector<Vec3i>> faces;
    std::vector<Vec3f> normals;
    std::vector<Vec2f> texcoord;    // uv
    mTGATexture diffuseMap;
    mTGATexture normalMap;
    mTGATexture specularMap;
public:
    mModel(const char *path, float sz = 1.0f);
    ~mModel();

    bool bindTexture(mTGATexture & tex, const char * suffix);

    int vertSize();
    int facesSize();

    Vec4f getVertex(int index);
    Vec4f getVertex(int faceIndex, int vertIndex);

    std::array<int, 3> getFace(int index);

    Vec2f getTexcoord(int faceIndex, int vertIndex);

    Vec3f getNormal(int faceIndex, int vertIndex);
    Vec3f getNormal(Vec2f _texcoord);   // TODO
    
    mColor diffuse(Vec2f uv);

    float specular( Vec2f uvf );
    mColor getSpecularColor( Vec2f _uv );

    bool loadTexture(const char * path, const char * suffix, mTGATexture & tex);
};
