#include "mFPSCameraHelper.h"
#include "win32.h"

static Vec3f initEyePos;

static struct Records
{
    mFPSCamera * camera = nullptr;

    float prevTime = 0;
    float currTime = 0;
    float deltaTime = 0;

    bool  isFstMouse = true;
    float lastXPos = mDevice::width / 2.0f;
    float lastYPos = mDevice::height / 2.0f;
} record;


void setCamera( mFPSCamera & c )
{
    record.camera = &c;
    initEyePos = c.eye;
}

void setRecordsTime( float prev, float curr )
{
    record.prevTime = prev;
    record.currTime = curr;
    record.deltaTime = curr - prev;
}

void FPSCKeyCallback( KeyCode key, bool pressed )
{
    if ( pressed ) {
        if ( key == KeyCode::SPACE ) {
            record.camera->eye = initEyePos;
            record.camera->yaw = Yaw;
            record.camera->pitch = Pitch;
            record.camera->zoom = Zoom;
            record.camera->updateCameraVectors();
            return;
        }
        record.camera->processKeyboard( key, record.deltaTime );
    }
}

void FPSCBtnCallback( MouseBtn btn, bool pressed )
{
    // TODO
}

inline static void onEdge( float xPos, float yPos )
{
    //if ( xPos < 10 || xPos > mDevice::width - 10 ) {

    //}
}

void FPSCCursorCallback( float xPos, float yPos )
{
    if ( record.isFstMouse ) {
        record.lastXPos = xPos;
        record.lastYPos = yPos;
        record.isFstMouse = false;
    }
    float xOffset = xPos - record.lastXPos;
    float yOffset = yPos - record.lastYPos;
    
    record.lastXPos = xPos;
    record.lastYPos = yPos;
    record.camera->processMouseMovement( xOffset, yOffset );
}

void FPSCScrollCallback( float offset )
{
    record.camera->processMouseScroll( offset );
}
