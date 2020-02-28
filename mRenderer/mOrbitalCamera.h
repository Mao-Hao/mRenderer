#pragma once

#include "mMath.hpp"

// ��Ӳ���
struct Motion { 
    Vec2f orbit;    // ���
    Vec2f pan;      // ����
    float dolly;    // ǰ��
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
    
    // ֱ�Ӳ���
    Vec3f Position;
    Vec3f Target;
    float Aspect;   // w / h
};