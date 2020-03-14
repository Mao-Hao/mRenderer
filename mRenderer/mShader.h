#pragma once
#include "mMath.hpp"
#include <array>

class mShader
{
public:
    #pragma region uniforms
    Mat m, v, p;
    #pragma endregion uniforms

    #pragma region varyings
    std::array<Vec4f, 3> vertices;
    std::array<Vec2f, 3> texcoords;
    #pragma endregion varyings

    #pragma region shaders
    virtual std::array<Vec4f, 3> VertexShader( int faceIndex ) = 0;
    // true -> discard
    virtual bool FrameShader( Vec3f bc, _Out_ Vec3f & color ) = 0;   
    #pragma endregion shaders
};