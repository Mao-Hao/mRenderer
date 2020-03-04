#pragma once
#include <vector>
#include <string>
#include <array>
#include "mMath.hpp"

class mModel {
public:
    std::vector<Vec3f> vertices;
    // зЂвт Vec3i (x: vertex, y: uv, z: normal) 
    std::vector<std::vector<Vec3i>> faces;
    std::vector<Vec3f> normals;
    std::vector<Vec2f> texcoord;    // uv

public:
    mModel(const char *path, float sz = 1.0f);
    ~mModel();

    int vertSize();
    int facesSize();

    Vec4f getVertex(int index);
    Vec4f getVertex(int faceIndex, int vertIndex);

    std::vector<int> getFace(int index);
    std::array<int, 3> getFace3(int index);
};
