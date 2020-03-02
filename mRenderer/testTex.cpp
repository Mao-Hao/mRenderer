#include "win32.h"
#include "mDevice.h"
#include "mMath.hpp"
#include "mColor.h"
#include "mPoint.h"
#include "mVertex.h"
#include "mPaint.h"
#include "mFPSCamera.h"
#include "mFPSCameraHelper.h"
#include "testTex.h"
#include "mModel.h"

using std::vector;

static inline Vec2i NDC2Screen(Vec4f & p) {
    return Vec2i((p.x) * (mDevice::width), (p.y) * (mDevice::height));
}

int testTex(int argc, char * argv[])
{
    if (mInitWindow() != 0)  return -1;
    mFPSCamera camera(Vec3f(0.0f, 0.0f, 3.0f), Vec3f(0.0f, 1.0f, 0.0f), Vec3f(0.0f, 0.0f, -1.0f));
    setCamera(camera);
    mDevice::callbackfuncs.keyCallback    = FPSCKeyCallback;
    mDevice::callbackfuncs.cursorCallback = FPSCCursorCallback;
    mDevice::callbackfuncs.scrollCallback = FPSCScrollCallback;
    mDevice::mInitZbuffer();

    //auto model = new mModel("obj/african_head/african_head.obj");
    auto model0 = new mModel("obj/assassin/face.obj");
    auto model1 = new mModel("obj/assassin/hair.obj");
    auto model2 = new mModel("obj/assassin/body.obj");
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
        auto m = Mat::translate(0.0f, 0.0f, 0.0f) * camera.getViewMatrix() * camera.getProjMatrix();
        for (size_t i = 0; i != model0->facesSize(); i++) {
            vector<int> face = model0->getFace(i);
            for (int j = 0; j < 3; j++) {
                Vec4f v0 = model0->getVertex(face[j]);
                Vec4f v1 = model0->getVertex(face[(j+1)%3]);
                Vec4xMat4(v0, m);
                Vec4xMat4(v1, m);
                if (!mClip(v0) && !mClip(v0)) {
                    v0 = v0 / v0.w;
                    v1 = v1 / v1.w;
                    auto p0 = NDC2Screen(v0);
                    auto p1 = NDC2Screen(v1);
                    line(p0, p1);
                }
            }
        }
        for (size_t i = 0; i != model1->facesSize(); i++) {
            vector<int> face = model1->getFace(i);
            for (int j = 0; j < 3; j++) {
                Vec4f v0 = model1->getVertex(face[j]);
                Vec4f v1 = model1->getVertex(face[(j+1)%3]);
                Vec4xMat4(v0, m);
                Vec4xMat4(v1, m);
                if (!mClip(v0) && !mClip(v0)) {
                    v0 = v0 / v0.w;
                    v1 = v1 / v1.w;
                    auto p0 = NDC2Screen(v0);
                    auto p1 = NDC2Screen(v1);
                    line(p0, p1);
                }
            }
        }
        for (size_t i = 0; i != model2->facesSize(); i++) {
            vector<int> face = model2->getFace(i);
            for (int j = 0; j < 3; j++) {
                Vec4f v0 = model2->getVertex(face[j]);
                Vec4f v1 = model2->getVertex(face[(j+1)%3]);
                Vec4xMat4(v0, m);
                Vec4xMat4(v1, m);
                if (!mClip(v0) && !mClip(v0)) {
                    v0 = v0 / v0.w;
                    v1 = v1 / v1.w;
                    auto p0 = NDC2Screen(v0);
                    auto p1 = NDC2Screen(v1);
                    line(p0, p1);
                }
            }
        }
        //--end--
        mDevice::freshZBuffer();
        mDispatch();
        mUpdateWindow();
    }
    mDevice::mDestroyZBuffer();
    destroyWindow();
    return 0;
}
