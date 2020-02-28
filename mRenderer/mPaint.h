#pragma once
#include "mBase.h"
#include "mMath.hpp"
#include "mColor.h"
#include "mPoint.h"
#include "mVertex.h"

// bresenham
// 实际是绘制 p0 to p1 - (1, 1)的线
void line(Vec2i & p0, Vec2i & p1, mColor mc = White);

// 插值版本
void mLine(mPoint2D & p0, mPoint2D & p1);

// void triangle(Vec2i & a, Vec2i & b, Vec2i & c, mColor mc = White);
void triangle(mPoint2D & A, mPoint2D & B, mPoint2D & C);

// 待优化
void mTriangle(mPoint2D & A, mPoint2D & B, mPoint2D & C);

void mTriangleZ(mVertex & A, mVertex & B, mVertex & C);

// true -> discard
inline bool mClip(Vec4f v) {
    if (v.x < 0 || v.x > v.w || v.y < 0 || v.y > v.w || v.z < 0 || v.z > v.w)
        return true;
    return false;
}