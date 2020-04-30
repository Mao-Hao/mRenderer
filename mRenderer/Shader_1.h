#pragma once
// 光照贴图，无阴影
#include "mMath.hpp"
#include "mModel.h"
#include "mColor.h"
#include "mFPSCameraHelper.h"
#include "mShader.h"
#include <array>


class Shader_1_phong : public mShader
{
public:
    mModel * model = nullptr;
    void setModel( mModel * p ) { model = p; }

    #pragma region uniforms
    Mat pvm;
    Vec3f lightPos = { 0, -3, 3 };  // 需要乘一个light的model矩阵
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
        Vec3f n;
        if ( model->normalMap.data == nullptr ) {
            n = interpolate( normals, bc ).normalize();
        }
        else {
            Vec3f bn = interpolate( normals, bc ).normalize();

            mat<3, 3, float> A;
            A[0] = proj<3>( vertices[1] - vertices[0] );
            A[1] = proj<3>( vertices[2] - vertices[0] );
            A[2] = bn;

            mat<3, 3, float> AI = A.invert();

            Vec3f i = AI * Vec3f( texcoords[1].x - texcoords[0].x, texcoords[2].x - texcoords[0].x, 0 );
            Vec3f j = AI * Vec3f( texcoords[1].y - texcoords[0].y, texcoords[2].y - texcoords[0].y, 0 );

            mat<3, 3, float> B;
            B.setCol( 0, i.normalize() );
            B.setCol( 1, j.normalize() );
            B.setCol( 2, bn );

            n = ( B * ( model->normal( uv ).normalize() * 2 - Vec3f( 1, 1, 1 ) ) );
        }

        // color
        mColor diffColor = model->diffuse( uv );
        mColor specColor = White * model->specular( uv );

        // diffuse 
        Vec3f fPos = interpolate( fragPos, bc ).normalize();
        Vec3f l = proj<3>( lightPos - fPos ).normalize();
        float diffuseStrength = mMax( 0.0f, n * l ) + 0.3;
        mColor diffuseColor = diffColor * diffuseStrength;

        // specular
        float specularStrength = 0.5;
        Vec3f viewDir = ( *cameraPosPtr - fPos ).normalize();
        Vec3f r = reflect( -l, n ).normalize();
        float specular = pow( mMax( ( viewDir * r ), 0.0f ), 32 );
        mColor specularColor = specColor * specularStrength * specular;

        color = diffuseColor;
        return false;
    }
    #pragma endregion shaders

public:
    Shader_1_phong() { cameraPosPtr = getCameraPos(); }
    ~Shader_1_phong() {}
};