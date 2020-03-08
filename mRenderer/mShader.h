#pragma once
#include "mMath.hpp"
#include "mModel.h"
#include "mColor.h"
#include <array>

class mShader
{
public:
    Mat m;
    Mat v;
    Mat p;
    Mat pvm;

    mModel * model = nullptr;

#pragma region varyings

    std::array<Vec4f, 3> vertices;
    std::array<Vec2f, 3> texcoords;

#pragma endregion varyings

    mShader() {}
    ~mShader() {}

    void setMatrix( Mat _model, Mat _view, Mat _proj )
    {
        m = _model; v = _view; p = _proj;
        pvm = p * v * m;
    }
    void setModel( mModel * p )
    {
        model = p;
    }
    std::array<Vec4f, 3> VertexShader( int faceIndex )
    {
        auto face = model->getFace( faceIndex );
        vertices[0] = pvm * model->getVertex( face[0] );
        vertices[1] = pvm * model->getVertex( face[1] );
        vertices[2] = pvm * model->getVertex( face[2] );

        texcoords = { model->getTexcoord( faceIndex, 0 ),
                      model->getTexcoord( faceIndex, 1 ),
                      model->getTexcoord( faceIndex, 2 ) };

        return vertices;
    }

    // true -> discard
    bool FrameShader( Vec3f bc, _In_ mColor & color )
    {
        Vec2f uv = texcoords[0] * bc.x + texcoords[1] * bc.y + texcoords[2] * bc.z;
        color = model->diffuse( uv );
        return false;
    }
};
