#include "win32.h"
#include "mDevice.h"
#include "mMath.hpp"
#include "mColor.h"
#include "mPoint.h"
#include "mVertex.h"
#include "mPaint.h"
#include "mFPSCamera.h"
#include "mFPSCameraHelper.h"
#include "testFPSH.h"

static Vec2i NDC2Screen(Vec4f & p) {
    return Vec2i((p.x) * mDevice::width, (p.y) * mDevice::height);
}

int testFPSH(int argc, char * argv[])
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

        auto m = Mat::translate(0.0f,  0.0f,  0.0f) * camera.getViewMatrix() * camera.getProjMatrix();

        auto p0 = Vec4f(0.0f, 0.0f, 0.1f, 1.0f);
        auto p1 = Vec4f(0.0f, 1.0f, 0.1f, 1.0f);
        auto p2 = Vec4f(1.0f, 1.0f, 0.1f, 1.0f);

        auto p3 = Vec4f(0.3f, 0.3f, 0.0f, 1.0f);
        auto p4 = Vec4f(0.3f, 0.7f, 0.0f, 1.0f);
        auto p5 = Vec4f(0.7f, 0.7f, 0.0f, 1.0f);

        Vec4xMat4(p0, m);   auto z0 = p0.z;
        Vec4xMat4(p1, m);   auto z1 = p1.z;
        Vec4xMat4(p2, m);   auto z2 = p2.z;

        Vec4xMat4(p3, m);   auto z3 = p3.z;
        Vec4xMat4(p4, m);   auto z4 = p4.z;
        Vec4xMat4(p5, m);   auto z5 = p5.z;

        //if (!(mClip(p0))) {   

            p0 = p0 / p0.w;
            p1 = p1 / p1.w;
            p2 = p2 / p2.w;

            p3 = p3 / p3.w;
            p4 = p4 / p4.w;
            p5 = p5 / p5.w;
            p0.z = z0;
            p1.z = z1;
            p2.z = z2;

            p3.z = z3;
            p4.z = z4;
            p5.z = z5;
            mVertex a = {p0, Red};
            mVertex b = {p1, Red};
            mVertex c = {p2, Red};

            mVertex d = {p3, Green};
            mVertex e = {p4, Green};
            mVertex f = {p5, Green};

            mTriangleZ(a, b, c);
            mTriangleZ(d, e, f);

            mDevice::freshZBuffer();
        //}
        mDispatch();
        mUpdateWindow();
    }
    mDevice::mDestroyZBuffer();
    destroyWindow();
    return 0;
}
