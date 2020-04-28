#include "win32.h"
#include "mDevice.h"
#include "mMath.hpp"
#include "mColor.h"
#include "mPoint.h"
#include "mVertex.h"
#include "mPaint.h"
#include "mFPSCamera.h"
#include "mFPSCameraHelper.h"
#include "Shader_2.h"
#include"modelPreproccessor.h"
#include "test_2_shadow.h"

using std::vector;

int test_2_shadow( int argc, char * argv[] )
{
    if ( mInitWindow() != 0 )  return -1;
    mFPSCamera camera( Vec3f( 0.0f, -1.2f, 3.5f ), Vec3f( 0.0f, 1.0f, 0.0f ), Vec3f( 0.0f, 0.0f, -1.0f ) );
    setCamera( camera );
    mDevice::callbackfuncs.keyCallback = FPSCKeyCallback;
    mDevice::callbackfuncs.cursorCallback = FPSCCursorCallback;
    mDevice::callbackfuncs.scrollCallback = FPSCScrollCallback;
    mDevice::mInitZbuffer();

    Shader_2_shadow * shader = new Shader_2_shadow;

    //vector<mModel *> models = loadModels( "floor" );
    //vector<mModel *> models = loadModels( "box" );
    vector<mModel *> models = loadModels( "assassin" );

    float prev = (float)mGetNativeTime();
    while ( !mDevice::isKeyPressed( KeyCode::ESC ) ) {
        float curr = (float)mGetNativeTime();
        std::cout << ( curr - prev ) * 1000 << '\n';
        setRecordsTime( prev, curr );
        prev = curr;

        mDevice::freshFrameBuffer();
        //--beg--

        shader->setMatrix(
            Mat::translate( 0, 0, 0 ),
            camera.getViewMatrix(),
            camera.getProjMatrix()
        );

        for ( auto & model : models ) {
            shader->setModel( model );
            for ( size_t i = 0; i != model->facesSize(); i++ ) {
                mRasterize( shader, i );
            }
        }

        //--end--
        mDevice::freshZBuffer();
        mUpdateWindow();
    }
    mDestroyWindow();
    return 0;
}