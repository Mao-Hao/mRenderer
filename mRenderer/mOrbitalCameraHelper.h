#pragma once
#include "mOrbitalCamera.h"
#include "mDevice.h"
#include "mMath.hpp"

//----------------------------------------------------------------
// test
struct Context {
    Vec2f click_pos;
    int single_click;
    int double_click;
    float frame_time;
    float delta_time;
};

void toContext(Context & context, float currTime, float deltaTime);
//----------------------------------------------------------------

void OCKeyCallback(KeyCode key, bool pressed);

void OCBtnCallback(MouseBtn btn, bool pressed);

void OCScrollCallback(float offset);

void updateOCamera(mOrbitalCamera & camera);

void updateClick(float currTime);

void func();