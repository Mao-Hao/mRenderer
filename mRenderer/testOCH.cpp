#include "win32.h"
#include "mDevice.h"
#include "mMath.hpp"
#include "mColor.h"
#include "mPoint.h"
#include "mPaint.h"
#include "mOrbitalCamera.h"
#include "mOrbitalCameraHelper.h"

static const Vec3f CameraPosition = {0, 0, 3.f};
static const Vec3f CameraTarget   = {0, 0, -1};

static Vec2i NDC2Screen(Vec4f p) {
    return Vec2i((p.x + 1) * mDevice::width / 2, (p.y + 1) * mDevice::height / 2);
}

int testOCH(int argc, char *argv[]) {

    if (mInitWindow() != 0)  return -1;
    mOrbitalCamera camera(CameraPosition, CameraTarget, (float)mDevice::width / mDevice::height);
    mDevice::callbackfuncs.keyCallback    = OCKeyCallback;
    mDevice::callbackfuncs.btnCallback    = OCBtnCallback;
    mDevice::callbackfuncs.scrollCallback = OCScrollCallback;

    float prev = getNativeTime();
    while (true) {
        float curr = getNativeTime();
        prev = curr;

        for (size_t i = 0; i<mDevice::width; i++)
            for (size_t j = 0;j<mDevice::height; j++)
                mDevice::setPixel_rc(i, j, 0);

        updateOCamera(camera);

        auto m = Mat::translate(0.0f,  0.0f,  0.0f) * camera.getViewMatrix() * camera.getProjMatrix();

        auto p0 = Vec4f( 0.0f, -1.0f,  0.0f, 1.0f);
        auto p1 = Vec4f(-1.0f,  0.75f, 0.0f, 1.0f);
        auto p2 = Vec4f( 1.0f,  0.75f, 0.0f, 1.0f);

        auto p0_ = Vec4f(p0 * m.col(0), p0 * m.col(1), p0 * m.col(2), p0 * m.col(3));
        auto p1_ = Vec4f(p1 * m.col(0), p1 * m.col(1), p1 * m.col(2), p1 * m.col(3));
        auto p2_ = Vec4f(p2 * m.col(0), p2 * m.col(1), p2 * m.col(2), p2 * m.col(3));

        p0_ = p0_ / p0_.w;
        p1_ = p1_ / p1_.w;
        p2_ = p2_ / p2_.w;

        Vec2i a = NDC2Screen(p0_);
        Vec2i b = NDC2Screen(p1_);
        Vec2i c = NDC2Screen(p2_);

        mPoint2D A(a, Blue);
        mPoint2D B(b, Green);
        mPoint2D C(c, Red);

        mTriangle(A, B, C);

        func();

        mDispatch();
        mUpdateWindow();
    }

    return 0;
}