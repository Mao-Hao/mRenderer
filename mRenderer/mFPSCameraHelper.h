#pragma once
#include "mFPSCamera.h"
#include "mDevice.h"
#include "mMath.hpp"


void setCamera(mFPSCamera & c);

void setRecordsTime(float prev, float curr);

void FPSCKeyCallback(KeyCode key, bool pressed);

void FPSCBtnCallback(MouseBtn btn, bool pressed);

void FPSCCursorCallback(float xPos, float yPos);

void FPSCScrollCallback(float offset);