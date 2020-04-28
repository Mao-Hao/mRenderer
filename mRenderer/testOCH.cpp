#include "win32.h"
#include "mDevice.h"
#include "mMath.hpp"
#include "mColor.h"
#include "mPoint.h"
#include "mPaint.h"
#include "mOrbitalCamera.h"
#include "mOrbitalCameraHelper.h"
#include "mModel.h"
#include "Shader_1.h"
#include <vector>

using std::vector;

static const Vec3f CameraPosition = { 0, 0, 3 };
static const Vec3f CameraTarget = { 0, 0, -1 };

int testOCH( int argc, char * argv[] )
{

    if ( mInitWindow() != 0 )  return -1;
    mOrbitalCamera camera( CameraPosition, CameraTarget, (float)mDevice::width / mDevice::height );
    mDevice::callbackfuncs.keyCallback = OCKeyCallback;
    mDevice::callbackfuncs.btnCallback = OCBtnCallback;
    mDevice::callbackfuncs.scrollCallback = OCScrollCallback;

    mDevice::mInitZbuffer();

    Shader_1_phong * shader = new Shader_1_phong;

    vector<mModel> models;
    auto m = mModel( "obj/african_head/african_head.obj" );
    m.bindTexture( m.diffuseMap, "_diffuse.tga" );
    models.push_back( m );
    m = mModel( "obj/african_head/african_head_eye_inner.obj" );
    m.bindTexture( m.diffuseMap, "_diffuse.tga" );
    models.push_back( m );

    float prev = mGetNativeTime();
    while ( true ) {
        float curr = mGetNativeTime();
        prev = curr;

        for ( size_t i = 0; i < mDevice::width; i++ )
            for ( size_t j = 0; j < mDevice::height; j++ )
                mDevice::setPixel_rc( i, j, 0 );

        updateOCamera( camera );

        //--beg--
        shader->setMatrix(
            Mat::translate( 0, 0, 1 ),
            camera.getViewMatrix(),
            camera.getProjMatrix()
        );

        for ( auto & model : models ) {
            shader->setModel( &model );
            for ( size_t i = 0; i != model.facesSize(); i++ ) {
                mRasterize( shader, i );

            }
        }
        //--end--

        mDevice::freshZBuffer();

        func();

        mUpdateWindow();
    }

    return 0;
}