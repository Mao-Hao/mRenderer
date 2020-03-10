#pragma once
#include "mBase.h"
#include "mMath.hpp"
#include "mColor.h"
#include "mPoint.h"
#include "mVertex.h"
#include "mShader.h"

// bresenham
// 实际是绘制 p0 to p1 - (1, 1)的线
void line( Vec2i & p0, Vec2i & p1, mColor mc = White );

// 插值版本
void mLine( mPoint2D & p0, mPoint2D & p1 );

// 待优化
void mTriangle( mPoint2D & A, mPoint2D & B, mPoint2D & C );

void mRasterize( mShader * shader, int faceIndex );