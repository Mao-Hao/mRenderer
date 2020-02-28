#pragma once

#include "mMath.hpp"

// 间接参数
struct Motion { 
    Vec2f orbit;    // 轨道
    Vec2f pan;      // 左右
    float dolly;    // 前后
};

class mOrbitalCamera {
public:
	mOrbitalCamera (Vec3f position, Vec3f target, float aspect);
	~mOrbitalCamera ();

#pragma region set & get
    void setTransform(Vec3f position, Vec3f target);
    Vec3f getFront();
    Mat getViewMatrix();
    Mat getProjMatrix();
#pragma endregion set & get

    void orbitUpdate(Motion motion);
    
    // 直接参数
    Vec3f Position;
    Vec3f Target;
    float Aspect;   // w / h
};