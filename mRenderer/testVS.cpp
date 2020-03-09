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
#include "mModel.h"
#include "mShader.h"
#include "mPaint.h"

using std::vector;

int testVS( int argc, char * argv[] )
{
    if ( mInitWindow() != 0 )  return -1;
    mFPSCamera camera( Vec3f( 0.0f, 0.0f, 3.0f ), Vec3f( 0.0f, 1.0f, 0.0f ), Vec3f( 0.0f, 0.0f, -1.0f ) );
    setCamera( camera );
    mDevice::callbackfuncs.keyCallback = FPSCKeyCallback;
    mDevice::callbackfuncs.cursorCallback = FPSCCursorCallback;
    mDevice::callbackfuncs.scrollCallback = FPSCScrollCallback;
    mDevice::mInitZbuffer();

    mShader shader;

    vector<mModel> models;
    //models.push_back( mModel( "obj/african_head/african_head.obj" ) );
    //models.push_back(mModel("obj/african_head/african_head_eye_inner.obj"));
    //models.push_back(mModel("obj/african_head/african_head_eye_outer.obj"));
    //models.push_back(mModel("obj/diablo3_pose/diablo3_pose.obj"));
    models.push_back(mModel("obj/assassin/hair.obj", 0.01f));
    models.push_back(mModel("obj/assassin/face.obj", 0.01f));
    models.push_back(mModel("obj/assassin/body.obj", 0.01f));
    models.push_back(mModel("obj/assassin/weapon.obj", 0.01f));
    float prev = (float)getNativeTime();
    while ( !mDevice::isKeyPressed( KeyCode::ESC ) ) {
        float curr = (float)getNativeTime();
        cout << (curr - prev) * 1000 << '\n';
        setRecordsTime( prev, curr );
        prev = curr;

//#pragma loop(hint_parallel(0))
        for ( int i = 0; i < mDevice::width; i++ )
            for ( int j = 0; j < mDevice::height; j++ )
                mDevice::setPixel_rc( i, j, 0 );

        //--beg--

        shader.setMatrix(
            Mat::translate( 0, 1, 0 ),
            camera.getViewMatrix(),
            camera.getProjMatrix()
        );

        for ( auto & model : models ) {
            shader.setModel( &model );
//#pragma loop(hint_parallel(0))
            for ( size_t i = 0; i != model.facesSize(); i++ ) {
                mRasterize( shader, i );
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
