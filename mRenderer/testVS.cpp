#include "win32.h"
#include "mDevice.h"
#include "mMath.hpp"
#include "mColor.h"
#include "mPoint.h"
#include "mVertex.h"
#include "mPaint.h"
#include "mFPSCamera.h"
#include "mFPSCameraHelper.h"
#include "testVS.h"

float vertices[108] = {
    0.25f, 0.25f, 0.25f,
    0.25f, 0.25f, 0.75f,
    0.25f, 0.75f, 0.25f,
    0.75f, 0.25f, 0.25f,
    0.25f, 0.25f, 0.75f,
    0.25f, 0.75f, 0.75f,

    0.75f, 0.75f, 0.25f,
    0.75f, 0.25f, 0.75f,
    0.25f, 0.75f, 0.75f,
    0.75f, 0.75f, 0.25f,
    0.75f, 0.25f, 0.25f,
    0.25f, 0.25f, 0.75f,

    0.25f, 0.75f, 0.25f,
    0.75f, 0.25f, 0.25f,
    0.25f, 0.25f, 0.25f,
    0.25f, 0.25f, 0.25f,
    0.25f, 0.25f, 0.25f,
    0.25f, 0.25f, 0.25f,

    0.25f, 0.25f, 0.75f,
    0.25f, 0.75f, 0.75f,
    0.75f, 0.75f, 0.25f,
    0.75f, 0.25f, 0.75f,
    0.25f, 0.75f, 0.75f,
    0.75f, 0.75f, 0.75f,

    0.75f, 0.75f, 0.75f,
    0.75f, 0.75f, 0.75f,
    0.75f, 0.75f, 0.75f,
    0.75f, 0.75f, 0.75f,
    0.75f, 0.75f, 0.25f,
    0.75f, 0.25f, 0.75f,

    0.25f, 0.75f, 0.75f,
    0.75f, 0.75f, 0.25f,
    0.75f, 0.25f, 0.25f,
    0.25f, 0.25f, 0.75f,
    0.25f, 0.75f, 0.25f,
    0.75f, 0.25f, 0.75f
};

struct Attrib {
    Vec3f position;
    Vec3f texcoord;
    Vec3f normal;
};

struct Uniform {
    Mat     model;
    Mat     view;
    Mat     proj;
    Vec3f   cameraPos;
};

struct Varyings {

};

class MyClass
{
public:
    MyClass();
    ~MyClass();

private:

};

MyClass::MyClass()
{
}

MyClass::~MyClass()
{
}


Vec4f mVertexShader(_In_ Attrib & attrib, _In_ Uniform & uniform) {//, _Out_ Varyings & varyings) {
    return Vec4f();
};


int testVS(int argc, char * argv[])
{
    if (mInitWindow() != 0)  return -1;
    mFPSCamera camera(Vec3f(0.0f, 0.0f, 3.0f), Vec3f(0.0f, 1.0f, 0.0f), Vec3f(0.0f, 0.0f, -1.0f));
    setCamera(camera);
    mDevice::callbackfuncs.keyCallback    = FPSCKeyCallback;
    mDevice::callbackfuncs.cursorCallback = FPSCCursorCallback;
    mDevice::callbackfuncs.scrollCallback = FPSCScrollCallback;
    mDevice::mInitZbuffer();

    float prev = getNativeTime();
    while (!mDevice::isKeyPressed(KeyCode::ESC)) {
        float curr = getNativeTime();
        cout << (curr - prev) * 1000 << '\n';
        setRecordsTime(prev, curr);
        prev = curr;

        for (size_t i = 0; i<mDevice::width; i++)
            for (size_t j = 0;j<mDevice::height; j++)
                mDevice::setPixel_rc(i, j, 0);

        //--beg--








        //--end--

        mDevice::freshZBuffer();
        mDispatch();
        mUpdateWindow();
    }
    mDevice::mDestroyZBuffer();
    destroyWindow();
    return 0;
}
