#pragma once
// π‚’’Ã˘Õº£¨Œﬁ“ı”∞
#include "mMath.hpp"
#include "mModel.h"
#include "mColor.h"
#include "mFPSCameraHelper.h"
#include "mShader.h"
#include <array>


typedef struct
{
    Vec3f lightPos;
    mColor lightColor;

    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
} Light;


class Shader_2_floor : public mShader
{
public:
    mModel * model = nullptr;
    void setModel( mModel * p ) { model = p; }

    #pragma region uniforms
    Mat pvm;
    Vec3f lightPos = { 3, 3, 3 };
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

    virtual void GeometryShader() {}

    virtual bool FrameShader( Vec3f bc, _Out_ mColor & color )
    {
        color = model->diffuse( uv );
        return false;
        Vec3f n;
        if ( model->normalMap.data == nullptr ) {
            n = interpolate( normals, bc ).normalize();
        }
        else {
            n = model->normal( uv );
        }

        //diffuse 
        mColor diffColor = model->diffuse( uv );
        Vec3f fPos = interpolate( fragPos, bc ).normalize();
        //Vec3f n = interpolate( normals, bc ).normalize();
        auto lp = proj<3>(m * embed<4>(lightPos));
        Vec3f l = ( lp - fPos ).normalize();
        float diffuseStrength = mMax( 0.0f, n * l ) + 0.2;
        mColor diffuseColor = diffColor * diffuseStrength;

        mColor specColor = White * model->specular( uv );
        float specularStrength = 0.5;
        Vec3f viewDir = ( *cameraPosPtr - fPos ).normalize();
        Vec3f r = reflect( -l, n ).normalize();
        float specular = pow( mMax( ( viewDir * r ), 0.0f ), 32 );
        mColor specularColor = specColor * specularStrength * specular;

        color = diffuseColor + specularColor;
        return false;
    }
    #pragma endregion shaders

public:
    Shader_2_floor() { cameraPosPtr = getCameraPos(); }
    ~Shader_2_floor() {}
};