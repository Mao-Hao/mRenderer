#pragma once
#include "mMath.hpp"
#include "mModel.h"
#include "mColor.h"
#include "mFPSCameraHelper.h"
#include "mShader.h"
#include <array>

class Shader_1 : public mShader
{
public:
    mModel * model = nullptr;
    void setModel( mModel * p ) { model = p; }

    #pragma region uniforms
    Mat pvm;
    Vec3f lightPos = { 3, -3, 3 };  // 需要乘一个light的model矩阵
    mColor lightColor = White;
    Vec3f * cameraPosPtr = nullptr;
    #pragma endregion uniforms

    #pragma region varyings
    std::array<Vec3f, 3> fragPos;
    std::array<Vec3f, 3> normals;
    mat<3, 3, float> normalMat;
    #pragma region varyings

    void setMatrix( Mat _model, Mat _view, Mat _proj )
    {
        m = _model; v = _view; p = _proj;
        pvm = p * v * m;
        normalMat = m.invertTranspose().getMinor( 4, 4 );
        cameraPosPtr = getCameraPos();
    }

    #pragma region shaders
    virtual std::array<Vec4f, 3> VertexShader( int faceIndex )
    {
        auto face = model->getFace( faceIndex );
        for ( size_t i = 0; i < 3; i++ ) {
            vertices[i] = pvm * model->getVertex( face[i] );
            normals[i] = normalMat * model->getNormal( faceIndex, i );
            texcoords[i] = model->getTexcoord( faceIndex, i );
            fragPos[i] = proj<3>( m * model->getVertex( face[i] ) );
        }
        return vertices;
    }

    virtual bool FrameShader( Vec3f bc, _In_ mColor & color )
    {
        // color
        Vec2f uv = interpolate( texcoords, bc );
        mColor diffColor = model->diffuse( uv );
        mColor specColor = White * model->specular( uv );

        // diffuse 
        Vec3f fPos = interpolate( fragPos, bc ).normalize();
        Vec3f n = interpolate( normals, bc ).normalize();
        Vec3f l = proj<3>( lightPos - fPos ).normalize();
        float diffuseStrength = mMax( 0.0f, n * l );
        mColor diffuseColor = diffColor * diffuseStrength;

        // specular
        float specularStrength = 0.6f;
        Vec3f viewDir = ( *cameraPosPtr - fPos ).normalize();
        Vec3f r = reflect( -l, n ).normalize();
        float specular = pow( mMax( ( viewDir * r ), 0.0f ), 32 );
        mColor specularColor = specColor * specularStrength * specular;

        color = diffuseColor + specularColor;
        return false;
    }
    #pragma endregion shaders

public:
    Shader_1() {}
    ~Shader_1() {}
};