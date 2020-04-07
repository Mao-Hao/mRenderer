#include "win32.h"
#include "mDevice.h"
#include "mMath.hpp"
#include "mColor.h"
#include "mPoint.h"
#include "mVertex.h"
#include "mPaint.h"
#include "mFPSCamera.h"
#include "mFPSCameraHelper.h"
#include "test_1_phong.h"
#include "mModel.h"
#include "Shader_1.h"
#include "mPaint.h"
#include"modelPreproccessor.h"

using std::vector;

int test_1_phong( int argc, char * argv[] )
{
    if ( mInitWindow() != 0 )  return -1;
    mFPSCamera camera( Vec3f( 0.0f, -1.2f, 3.5f ), Vec3f( 0.0f, 1.0f, 0.0f ), Vec3f( 0.0f, 0.0f, -1.0f ) );
    setCamera( camera );
    mDevice::callbackfuncs.keyCallback = FPSCKeyCallback;
    mDevice::callbackfuncs.cursorCallback = FPSCCursorCallback;
    mDevice::callbackfuncs.scrollCallback = FPSCScrollCallback;
    mDevice::mInitZbuffer();

    Shader_1_phong * shader = new Shader_1_phong;

    vector<mModel *> models = loadModels("assassin");
    //vector<mModel *> models = loadModels( "box" );
    float prev = (float)getNativeTime();
    while ( !mDevice::isKeyPressed( KeyCode::ESC ) ) {
        float curr = (float)getNativeTime();
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
            render<RenderMode::NORMAL>(model, shader);

        }

        //--end--
        mDevice::freshZBuffer();
        mDispatch();
        mUpdateWindow();
    }
    destroyWindow();
    return 0;
}
