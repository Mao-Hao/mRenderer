#include "mOrbitalCameraHelper.h"
#include "win32.h"

static const Vec3f OCPosition   = {0, 0, 1.5f};
static const Vec3f OCTarget     = {0, 0, 0};

static const float CLICK_DELAY = 0.25f;

struct OCRecords{
    // orbit
    bool    isOrbiting;
    Vec2f   orbitPos;
    Vec2f   orbitDelta;
    // pan 
    bool    isPanning;
    Vec2f   panPos;
    Vec2f   panDelta;
    // zoom 
    float   dollyDelta;
    // click
    float   pressTime;
    float   releaseTime;
    Vec2f   pressPos;
    Vec2f   releasePos;
    bool    singleClick;
    bool    doubleClick;
    Vec2f   clickPos;
};

static OCRecords record = {
    false, {.0f, .0f}, {.0f, .0f},                              // orbit
    false, {.0f, .0f}, {.0f, .0f},                              // pan
    .0f,
    .0f, .0f, {.0f, .0f}, {.0f, .0f}, false, false, {.0f, .0f}  // click
};


static Vec2f getMousePos() {
    float x = -1, y = -1;
    getCursorPos(x, y);
    return Vec2f(x, y);
}

static Vec2f getPosDelta(Vec2f oldPos, Vec2f newPos) {
    Vec2f delta = newPos - oldPos;
    return delta / (float)mDevice::height;
}

static Vec2i NDC2Screen(Vec4f p) {
    return Vec2i((p.x + 1) * mDevice::width / 2, (p.y + 1) * mDevice::height / 2);
}

void OCKeyCallback(KeyCode key, bool pressed) {
    // TODO change light
}

void OCBtnCallback(MouseBtn btn, bool pressed) {
    Vec2f cursorPos = getMousePos();
    if (btn == MouseBtn::L) {
        float currTime = (float)getNativeTime();
        if (pressed) {
            record.isOrbiting = true;
            record.orbitPos   = cursorPos;
            record.pressTime  = currTime;
            record.pressPos   = cursorPos;
        } else {
            float prevTime = record.releaseTime;
            Vec2f posDelta = getPosDelta(record.orbitPos, cursorPos);
            record.isOrbiting = false;
            record.orbitDelta = record.orbitDelta + posDelta;
            if (prevTime && (currTime - prevTime < CLICK_DELAY)) {
                record.doubleClick = true;
                record.releaseTime = false;
            } else {
                record.releaseTime = currTime;
                record.releasePos  = cursorPos;
            }
        } 
    }
    else if (btn == MouseBtn::R) {
        if (pressed) {
            record.isPanning = true;
            record.panPos    = cursorPos;
        } else {
            Vec2f posDelta   = getPosDelta(record.panPos, cursorPos);
            record.isPanning = false;
            record.panDelta  = record.panDelta + posDelta;
        }
    }
}

void OCScrollCallback(float offset) {
    record.dollyDelta += offset;
}

void updateOCamera(mOrbitalCamera & camera) {
    Vec2f cursorPos = getMousePos();
    if (record.isOrbiting) {
        Vec2f posDelta = getPosDelta(record.orbitPos, cursorPos);
        record.orbitDelta = record.orbitDelta + posDelta;
        record.orbitPos = cursorPos;
    }
    if (record.isPanning) {
        Vec2f posDelta = getPosDelta(record.panPos, cursorPos);
        record.panDelta = record.panDelta + posDelta;
        record.panPos = cursorPos;
    }
    if (mDevice::isKeyPressed(KeyCode::SPACE)) {
        camera.setTransform({0, 0, 3}, {0, 0, 0});
    }
    else {
        Motion motion;
        motion.orbit = record.orbitDelta;
        motion.pan = record.panDelta;
        motion.dolly = record.dollyDelta;
        camera.orbitUpdate(motion);
    }
}

void updateClick(float currTime) {
    float lastTime = record.releaseTime;
    if (lastTime && currTime - lastTime > CLICK_DELAY) {
        Vec2f posDelta = record.releasePos - record.pressPos;
        if (posDelta.norm() < 5)    record.singleClick = true;
        record.releaseTime = 0;
    }
    if (record.singleClick || record.doubleClick) {
        float clickX = record.releasePos.x / mDevice::width;
        float clickY = record.releasePos.y / mDevice::height;
        record.clickPos = {clickX, 1 - clickY};
    }
}

// test
void toContext(Context & context, float currTime, float deltaTime) {
    context.click_pos = record.clickPos;
    context.single_click = record.singleClick;
    context.double_click = record.doubleClick;
    context.frame_time = currTime;
    context.delta_time = deltaTime;
}

void func() {
    record.orbitDelta = {0, 0};
    record.panDelta = {0, 0};
    record.dollyDelta = 0;
    record.singleClick = 0;
    record.doubleClick = 0;
}