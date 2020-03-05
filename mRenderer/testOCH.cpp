#include "win32.h"
#include "mDevice.h"
#include "mMath.hpp"
#include "mColor.h"
#include "mPoint.h"
#include "mPaint.h"
#include "mOrbitalCamera.h"
#include "mOrbitalCameraHelper.h"
#include "mModel.h"
#include "mShader.h"
#include <vector>

using std::vector;

static const Vec3f CameraPosition = {0, 0, 3};
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

    mDevice::mInitZbuffer();

    mShader shader;

    vector<mModel> models;
    models.push_back(mModel("obj/diablo3_pose/diablo3_pose.obj"));

    float prev = getNativeTime();
    while (true) {
        float curr = getNativeTime();
        prev = curr;

        for (size_t i = 0; i<mDevice::width; i++)
            for (size_t j = 0;j<mDevice::height; j++)
                mDevice::setPixel_rc(i, j, 0);

        updateOCamera(camera);
  
        //--beg--
        shader.setMatrix(Mat::rotate_z(mRadiansf(90)), camera.getViewMatrix(), camera.getProjMatrix());
        for (auto & model : models) {
            shader.setModel(&model);
            for (size_t i = 0; i != model.facesSize(); i++) {
                mRasterize(shader, i);

            } 
        }
        //--end--

        mDevice::freshZBuffer();

        func();

        mDispatch();
        mUpdateWindow();
    }

    return 0;
}