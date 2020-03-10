#include "mOrbitalCamera.h"

static const float zNEAR = 0.1f;
static const float zFAR  = 1000;
static const float FOVY  = mRadiansf(60);
static const Vec3f UP    = {0, 1, 0};


mOrbitalCamera::mOrbitalCamera(Vec3f position, Vec3f target, float aspect) {
    mAssert((position - target).norm() > Epsilon && aspect > 0);
    Position = position;
    Target = target;
    Aspect = aspect;
}

mOrbitalCamera::~mOrbitalCamera() {}


static Vec3f calPan(Vec3f fromCamera, Motion motion) {
    Vec3f forward   = fromCamera.normalize();
    Vec3f left      = cross(UP, forward);
    Vec3f up        = cross(forward, left);

    float distance  = fromCamera.norm();
    float factor    = distance * (float)tan(FOVY / 2) * 2;
    Vec3f dx        = left * (motion.pan.x * factor);
    Vec3f dy        = up * (motion.pan.y * factor);
    return dx + dy;
}

static Vec3f calOffset(Vec3f fromTarget, Motion motion) {
    float radius    = fromTarget.norm();
    float theta     = atan2(fromTarget.x, fromTarget.z);  // azimuth 
    float phi       = acos(fromTarget.y / radius);         // polar 
    float factor    = Pi * 2.0f;
    Vec3f offset;

    radius  *= pow(0.95f, motion.dolly);
    theta   -= motion.orbit.x * factor;
    phi     -= motion.orbit.y * factor;
    phi      = mClamp(phi, Epsilon, Pi - Epsilon);

    offset.x = radius * sin(phi) * sin(theta);
    offset.y = radius * cos(phi);
    offset.z = radius * sin(phi) * cos(theta);

    return offset;
}


void mOrbitalCamera::setTransform(Vec3f position, Vec3f target) {
    mAssert((position - target).norm() > Epsilon);
    Position = position;
    Target = target;
}

Vec3f mOrbitalCamera::getFront() {
    return Target - Position.normalize();
}

Mat mOrbitalCamera::getViewMatrix() {
    return Mat::lookAt(Position, Target, UP);
}

Mat mOrbitalCamera::getProjMatrix() {
    return Mat::perspective(FOVY, Aspect, zNEAR, zFAR);
}

void mOrbitalCamera::orbitUpdate(Motion motion) {
    Vec3f from_target   = Position - Target;
    Vec3f from_camera   = Target - Position;
    Vec3f pan           = calPan(from_camera, motion);
    Vec3f offset        = calOffset(from_target, motion);
    //      ??? t - p   
    Target              = Target + pan;
    Position            = Target + offset;
}
