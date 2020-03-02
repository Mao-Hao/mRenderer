#pragma once
#include "mBase.h"
#include "mColor.h"
#include <functional>

const float ZNear       =   0.1f;
const float invZNear    =   1.0f / ZNear;
const float ZFar        =   100.0f;
const float invZFar     =   1.0f / ZFar;
const float invDZFN      =   1 / (invZFar - invZNear);


enum class KeyCode {
    A, D, E, Q, S, W, SPACE, ESC, KEY_NUM
};

enum class MouseBtn {
    L, R, BTN_NUM
};

struct CallbackFunc{
    std::function<void(KeyCode, bool)>      keyCallback;
    std::function<void(MouseBtn, bool)>     btnCallback;
    std::function<void(float, float)>       cursorCallback;
    std::function<void(float)>              scrollCallback;
};

template <typename T>
inline bool outOfRange(T v, T lo, T hi) {
    if (v < lo || v > hi)
        return true;
    return false;
}

class mDevice {
public:
    static int          width;
    static int          height;
    static uint      ** framebuffer;
    static float     ** zbuffer;
    static bool         keys[(uint)KeyCode::KEY_NUM];
    static bool         btns[(uint)MouseBtn::BTN_NUM];
    static CallbackFunc callbackfuncs;

    // inline
    static __forceinline void setPixel(int x, int y, mColor c) {
        //mAssert(x >= 0); mAssert(x < width);
        //mAssert(y >= 0); mAssert(y < height);
        if (x >= 0 && x < width && y >= 0 && y < height)
            framebuffer[y][x] = toRawColor(c);
    }
    static void setPixel_rc(int x, int y, rawColor c = 0xffffff) {
        //mAssert(x >= 0); mAssert(x < width);
        //mAssert(y >= 0); mAssert(y < height);
        if (x >= 0 && x < width && y >= 0 && y < height)
            framebuffer[y][x] = c;
    }
    static mColor getColor(int x, int y) {
        mAssert(x >= 0); mAssert(x < width);
        mAssert(y >= 0); mAssert(y < height);
        return toMColor(framebuffer[y][x]);
    }
    static rawColor getRawColor(int x, int y) {
        mAssert(x >= 0); mAssert(x < width);
        mAssert(y >= 0); mAssert(y < height);
        return framebuffer[y][x];
    }
    static bool isKeyPressed(KeyCode key) {
        return keys[(uint)key];
    }
    static bool isBtnPressed(MouseBtn btn)  {
        return btns[(uint)btn];
    }
    // zbuffer
    static void mInitZbuffer() {
        mDevice::zbuffer = new float*[mDevice::height];
        for(int i = 0; i < mDevice::width; i++)
            mDevice::zbuffer[i] = new float[mDevice::width];
        for (int i = 0; i < mDevice::height; i++)
            for(int j = 0; j < mDevice::width; j++)
                mDevice::zbuffer[i][j] = -1.0f;
    }
    // z越小，离摄像机越近 大的z表示该物体在前面 false -> discard
    static __forceinline bool mZTest(int x, int y, float & z) {
        if (z - mDevice::zbuffer[y][x] > 0.0f) {
            mDevice::zbuffer[y][x] = z;
            return true;        // 绘制该点
        }
        return false;
    }
    // 待优化
    static void freshZBuffer() {
        for (int i = 0; i < mDevice::height; i++)
            for(int j = 0; j < mDevice::width; j++)
                mDevice::zbuffer[i][j] = -1.0f;
    }
    static void mDestroyZBuffer() {
        delete []mDevice::zbuffer;
    }
private:
     mDevice() = delete;
    ~mDevice() {}
};



