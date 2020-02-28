#pragma once
#include "mDevice.h"

int     mDevice::width          = 800;  // x
int     mDevice::height         = 800;  // y
uint ** mDevice::framebuffer    = nullptr;
float** mDevice::zbuffer        = nullptr;
bool    mDevice::keys[(uint)KeyCode::KEY_NUM]   = {};
bool    mDevice::btns[(uint)MouseBtn::BTN_NUM]  = {};
CallbackFunc mDevice::callbackfuncs = { nullptr, nullptr, nullptr, nullptr };