#pragma once
#include "mMath.hpp"
#include <array>

class mShader
{
public:
    Mat m, v, p;
    Mat pvm;

    #pragma region varyings
    std::array<Vec4f, 3> vertices;
    std::array<Vec2f, 3> texcoords;
    Vec3f                weight;
    #pragma endregion varyings

    virtual std::array<Vec4f, 3> VertexShader( int faceIndex ) = 0;

    virtual bool FrameShader( Vec3f bc, _In_ Vec3f & color ) = 0;
};