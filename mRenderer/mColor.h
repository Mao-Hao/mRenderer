#pragma once

#include "mBase.h"
#include "mMath.hpp"

using mColor = Vec3f; 

const mColor Black  (0, 0, 0);
const mColor White  (255, 255, 255);
const mColor Red    (255, 0, 0);
const mColor Green  (0, 255, 0);
const mColor Blue   (0, 0, 255);

template <typename T> 
constexpr bool is_mColor() { 
    return is_same<std::decay<T>::type, mColor>(); 
}

inline rawColor toRawColor(const mColor & mc) {
    auto tmp = mClamp(mc, Black, White);
    uchar r = static_cast<uchar>(tmp.x);
    uchar g = static_cast<uchar>(tmp.y);
    uchar b = static_cast<uchar>(tmp.z);
    return (r << 16) | (g << 8) | (b);
}

inline mColor toMColor(rawColor c) {
    mColor mc(0, 0, 0);
    rawColor mask	= 0x0000ff;
    mc.z = c & mask;
    c = c >> 8;
    mc.y = c & mask;
    c = c >> 8;
    mc.x = c;
    return mc;
}