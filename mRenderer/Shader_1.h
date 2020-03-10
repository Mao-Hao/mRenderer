#pragma once
#include "mMath.hpp"
#include "mModel.h"
#include "mColor.h"
#include "mShader.h"
#include <array>

class Shader_1 : public mShader
{
public:
    mModel * model = nullptr;

    Vec3f lightDirection = { 1, 1, 1 };

    Shader_1() {}
    ~Shader_1() {}

    void setMatrix( Mat _model, Mat _view, Mat _proj )
    {
        m = _model; v = _view; p = _proj;
        pvm = p * v * m;
    }

    void setModel( mModel * p )
    {
        model = p;
    }

    #pragma region shaders
    virtual std::array<Vec4f, 3> VertexShader( int faceIndex )
    {
        auto face = model->getFace( faceIndex );
        vertices[0] = pvm * model->getVertex( face[0] );
        vertices[1] = pvm * model->getVertex( face[1] );
        vertices[2] = pvm * model->getVertex( face[2] );

        weight[0] = mMax( 0.f, model->getNormal( faceIndex, 0 ) * lightDirection );
        weight[1] = mMax( 0.f, model->getNormal( faceIndex, 1 ) * lightDirection );
        weight[2] = mMax( 0.f, model->getNormal( faceIndex, 2 ) * lightDirection );

        texcoords = { model->getTexcoord( faceIndex, 0 ),
                      model->getTexcoord( faceIndex, 1 ),
                      model->getTexcoord( faceIndex, 2 ) };

        return vertices;
    }

    // true -> discard
    virtual bool FrameShader( Vec3f bc, _In_ mColor & color )
    {
        float currWeight = weight * bc;
        Vec2f uv = texcoords[0] * bc.x + texcoords[1] * bc.y + texcoords[2] * bc.z;
        color = model->diffuse( uv ) * currWeight;
        return false;
    }
    #pragma endregion shaders
};
