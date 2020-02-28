#include "win32.h"
#include "mDevice.h"
#include "mMath.hpp"
#include "mColor.h"
#include "mPoint.h"
#include "mPaint.h"
#include "mOrbitalCamera.h"
//#include "OrbitalCameraHelper.h"

static const Vec3f CAMERA_POSITION = {0, 0, 1.5f};
static const Vec3f CAMERA_TARGET = {0, 0, 0};

Vec2f getPos() {
    float x = -1, y = -1;
    getCursorPos(x, y);
    return Vec2f(x, y);
}

struct OCRecords{
    ///* orbit */
    //int is_orbiting;
    //Vec2f orbit_pos;
    //Vec2f orbitDelta;
    /* pan */
    bool isPanning;
    Vec2f panPos;
    Vec2f panDelta;
    ///* zoom */
    //float dollyDelta;
};

mOrbitalCamera oc(CAMERA_POSITION, CAMERA_TARGET, (float)mDevice::width / mDevice::height);
OCRecords record = {false, {0, 0}, {0, 0}};

static Vec2f get_pos_delta(Vec2f old_pos, Vec2f new_pos) {
    Vec2f delta = new_pos - old_pos;
    return delta / (float)mDevice::height;
}

static void buttonCallback(MouseBtn btn, bool pressed) {
    Vec2f cursor_pos = getPos();
    if (btn == MouseBtn::R) {
        if (pressed) {
            record.isPanning = true;
            record.panPos = cursor_pos;
        } else {
            cout << "no" << '\n';
            Vec2f posDelta = get_pos_delta(record.panPos, cursor_pos);
            record.isPanning = false;
            record.panDelta = record.panDelta + posDelta;
        }
    }
}

Vec2i func(Vec4f p) {
    return Vec2i((p.x + 1) * 400, (p.y + 1) * 400);
}

int main() {
    
    mInitWindow();
    mDevice::callbackfuncs.btnCallback = buttonCallback;

    while (true) {

        for (size_t i = 0; i<mDevice::width; i++)
            for (size_t j = 0;j<mDevice::height; j++)
                mDevice::setPixel_rc(i, j, 0);

        auto m = Mat::translate(0.0f,  0.0f,  0.0f) * oc.getViewMatrix() * oc.getProjMatrix();

        auto p0 = Vec4f( 0.0f, -1.0f,  0.0f, 1.0f);
        auto p1 = Vec4f(-1.0f,  0.75f, 0.0f, 1.0f);
        auto p2 = Vec4f( 1.0f,  0.75f, 0.0f, 1.0f);

        auto p0_ = Vec4f(p0 * m.col(0), p0 * m.col(1), p0 * m.col(2), p0 * m.col(3));
        auto p1_ = Vec4f(p1 * m.col(0), p1 * m.col(1), p1 * m.col(2), p1 * m.col(3));
        auto p2_ = Vec4f(p2 * m.col(0), p2 * m.col(1), p2 * m.col(2), p2 * m.col(3));

        Vec2i a = func(p0_);
        Vec2i b = func(p1_);
        Vec2i c = func(p2_);
        mPoint2D A(a, Blue);
        mPoint2D B(b, Green);
        mPoint2D C(c, Red);

        mTriangle(A, B, C);

        
        Motion motion;
        motion.pan = record.panDelta;

        if (record.isPanning) {
        //record.panDelta = {0, 0};
        oc.orbitUpdate(motion);
        }
        mDispatch();
        mUpdateWindow();

    }

    return 0;
}