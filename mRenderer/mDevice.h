#pragma once
#include "mBase.h"
#include "mColor.h"
#include <functional>

const float ZNear = 0.1f;
const float invZNear = 1.0f / ZNear;
const float ZFar = 100.0f;
const float invZFar = 1.0f / ZFar;
const float invDZFN = 1 / ( invZFar - invZNear );


enum class KeyCode
{
    A, D, E, Q, S, W, SPACE, ESC, KEY_NUM
};

enum class MouseBtn
{
    L, R, BTN_NUM
};

struct CallbackFunc
{
    std::function<void( KeyCode, bool )>      keyCallback;
    std::function<void( MouseBtn, bool )>     btnCallback;
    std::function<void( float, float )>       cursorCallback;
    std::function<void( float )>              scrollCallback;
};

template <typename T>
inline bool outOfRange( T v, T lo, T hi )
{
    if ( v < lo || v > hi )
        return true;
    return false;
}

class mDevice
{
public:
    inline static const int          width = 800;
    inline static const int          height = 600;
    inline static uint ** framebuffer = nullptr;
    inline static float ** zbuffer = nullptr;
    inline static bool         keys[(uint)KeyCode::KEY_NUM];
    inline static bool         btns[(uint)MouseBtn::BTN_NUM];
    inline static CallbackFunc callbackfuncs;

    // inline
    static __forceinline void setPixel( int x, int y, mColor c )
    {
        mAssert( x >= 0 ); mAssert( x < width );
        mAssert( y >= 0 ); mAssert( y < height );
        #ifndef mNDebug
        if ( x >= 0 && x < width && y >= 0 && y < height )
            #endif
            framebuffer[y][x] = toRawColor( c );
    }

    static void setPixel_rc( int x, int y, rawColor c = 0xffffff )
    {
        mAssert( x >= 0 ); mAssert( x < width );
        mAssert( y >= 0 ); mAssert( y < height );
        #ifndef mNDebug
        if ( x >= 0 && x < width && y >= 0 && y < height )
            #endif
            framebuffer[y][x] = c;
    }

    static mColor getColor( int x, int y )
    {
        mAssert( x >= 0 ); mAssert( x < width );
        mAssert( y >= 0 ); mAssert( y < height );
        return toMColor( framebuffer[y][x] );
    }

    static rawColor getRawColor( int x, int y )
    {
        mAssert( x >= 0 ); mAssert( x < width );
        mAssert( y >= 0 ); mAssert( y < height );
        return framebuffer[y][x];
    }

    static bool isKeyPressed( KeyCode key )
    {
        return keys[(uint)key];
    }

    static bool isBtnPressed( MouseBtn btn )
    {
        return btns[(uint)btn];
    }

    // framebuffer
    static __forceinline void freshFrameBuffer()
    {
        memset( framebuffer[0], 0, (size_t)width * height * sizeof( uint ) );
    }

    // zbuffer
    static void mInitZbuffer()
    {

    }

    // z越小，离摄像机越近 大的z表示该物体在前面 false -> discard
    static __forceinline bool mZTest( int x, int y, float & z )
    {
        if ( z - mDevice::zbuffer[y][x] > 0.0f ) {
            mDevice::zbuffer[y][x] = z;
            return true;        // 绘制该点
        }
        return false;
    }

    static __forceinline void freshZBuffer()
    {
        memset( zbuffer[0], 0, (size_t)width * height * sizeof( float ) );
    }
private:
    mDevice() = delete;
    ~mDevice() {}
};