#pragma once

#include "mMath.hpp"

class mShader {
public:
    Mat Model;
    Mat View;
    Mat Projection;

    virtual ~mShader() {}

    virtual Vec4f vertex(float * p, int n) = 0;

    //virtual bool fragment() = 0;
};