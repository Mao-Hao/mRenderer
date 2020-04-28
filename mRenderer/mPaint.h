#pragma once
#include "mBase.h"
#include "mMath.hpp"
#include "mColor.h"
#include "mPoint.h"
#include "mVertex.h"
#include "mModel.h"
#include "mShader.h"

// bresenham
// 实际是绘制 p0 to p1 - (1, 1)的线
void line( Vec2i p0, Vec2i p1, mColor mc = White );

// 插值版本
void mLine( mPoint2D & p0, mPoint2D & p1 );

void mRasterize( mShader * shader, int faceIndex );

enum class RenderMode { NORMAL, POINTS, WIRE_FRAME, DEPTH };
template <RenderMode M> void render( mModel * model, mShader * shader );
template <> void render<RenderMode::NORMAL>( mModel * model, mShader * shader );
template <> void render<RenderMode::POINTS>( mModel * model, mShader * shader );
template <> void render<RenderMode::WIRE_FRAME>( mModel * model, mShader * shader );
template <> void render<RenderMode::DEPTH>( mModel * model, mShader * shader );
//template <> void render<RenderMode::PICK_UP>( mModel * model, mShader * shader );