#pragma once

#include "mBase.h"
#include "mMath.hpp"
#include "mDevice.h"


const float Fovy        =  mRadiansf(45.0f);
const float Aspect      = (float)mDevice::width / mDevice::height;
const float Yaw         = -90.0f;
const float Pitch       =  0.0f;
const float Speed       =  1.5f;
const float Sensitivity =  0.2f;
const float Zoom        =  45.0f;

class mFPSCamera {
public:

    Vec3f eye;
    Vec3f front;
    Vec3f up;
    Vec3f right;
    Vec3f worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    mFPSCamera(Vec3f _eye = {0, 0, 0}, Vec3f _up = {0, 1, 0}, Vec3f _front = {0, 0, -1},
        float _yaw = Yaw, float _pitch = Pitch) 
        : front(_front), movementSpeed(Speed), mouseSensitivity(Sensitivity), zoom(Zoom) {
        eye     = _eye;
        worldUp = _up;
        yaw     = Yaw;
        pitch   = Pitch;
        updateCameraVectors();
    }
    ~mFPSCamera() {};

    // 存疑，第二个参数需不需要加上eye
    // ! 必须加， 不加移动会歪
    Mat getViewMatrix() { return Mat::lookAt(eye, eye + front, up);}

    Mat getProjMatrix() { return Mat::perspective(mRadiansf(zoom), Aspect, ZNear, ZFar);}

    void processKeyboard(KeyCode k, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        if (k == KeyCode::W)        eye = eye + front * velocity;
        if (k == KeyCode::S)        eye = eye - front * velocity;
        if (k == KeyCode::A)        eye = eye - right * velocity;
        if (k == KeyCode::D)        eye = eye + right * velocity;
        if (k == KeyCode::Q)        eye.y -= velocity;
        if (k == KeyCode::E)        eye.y += velocity;
    }

    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true) {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        yaw   += xOffset;
        pitch += yOffset;

        if (constrainPitch)                 pitch = mClamp(pitch, -89.5f, 89.5f);

        updateCameraVectors();
    }

    void processMouseScroll(float yOffset) {
        if (zoom >= 1.0f && zoom <= 60.0f)  zoom -= yOffset;
        zoom = mClamp(zoom, 1.0f, 60.0f);
    }


    void updateCameraVectors() {
        front = Vec3f(
            cos(mRadiansf(yaw)) * cos(mRadiansf(pitch)), 
            sin(mRadiansf(pitch)), 
            sin(mRadiansf(yaw)) * cos(mRadiansf(pitch))
        ).normalize();
        
        right = cross(front, worldUp).normalize();
        up    = cross(right, front).normalize();
    }
};

