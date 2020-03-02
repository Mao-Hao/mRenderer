#pragma once
#include <vector>
#include <string>
#include "mMath.hpp"

class mModel {
public:
    std::vector<Vec3f> vertices;
    // зЂвт Vec3i (x: vertex, y: uv, z: normal) 
    std::vector<std::vector<Vec3i>> faces;

public:
    mModel(const char *path);
    ~mModel();

    int vertSize();
    int facesSize();

    Vec4f getVertex(int index);
    Vec4f getVertex(int faceIndex, int vertIndex);

    std::vector<int> getFace(int index);
};
