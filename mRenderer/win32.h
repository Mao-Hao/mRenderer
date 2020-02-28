#pragma once
#include "mBase.h"
#include <windows.h>
#include "mDevice.h"

// 基于GDI的双缓冲绘制操作
// 打开一个窗口，并根据接收在屏幕上绘制

// 指向一块 width * height * sizeof(uint) 大小的内存块的首地址
// 每个uint代表着一个像素的颜色，0000 0000 RRRR RRRR GGGG GGGG BBBB BBBB

int mInitWindow();

void mDispatch();

void mUpdateWindow();

void destroyWindow();

bool windowShouldClose();

void getCursorPos(float & xpos, float & ypos);

double getNativeTime();