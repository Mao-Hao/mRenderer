#include "mFPSCameraHelper.h"
#include "win32.h"

static struct Records {
    mFPSCamera * camera = nullptr;

    float prevTime  = 0;
    float currTime  = 0;
    float deltaTime = 0;

    bool  isFstMouse = true;
    float lastXPos = mDevice::width / 2.0f;
    float lastYPos = mDevice::height / 2.0f;
} record;


void setCamera(mFPSCamera & c) {
    record.camera = &c;
}

void setRecordsTime(float prev, float curr) {
    record.prevTime  = prev;
    record.currTime  = curr;
    record.deltaTime = curr - prev;
}

void FPSCKeyCallback(KeyCode key, bool pressed) {
    if (pressed)    record.camera->processKeyboard(key, record.deltaTime);
}

void FPSCBtnCallback(MouseBtn btn, bool pressed) {
    // TODO
}

void FPSCCursorCallback(float xPos, float yPos) {
    if (record.isFstMouse) {
        record.lastXPos   = xPos;
        record.lastYPos   = yPos;
        record.isFstMouse = false;
    }
    float xOffset = xPos - record.lastXPos;
    float yOffset = yPos - record.lastYPos;
    record.lastXPos = xPos;
    record.lastYPos = yPos;
    record.camera->processMouseMovement(xOffset, yOffset);
}

void FPSCScrollCallback(float offset) {
    record.camera->processMouseScroll(offset);
}
