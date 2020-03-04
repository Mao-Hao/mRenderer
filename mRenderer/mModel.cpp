#include <iostream>
#include <fstream>
#include <sstream>
#include "mModel.h"

using namespace std;

mModel::mModel(const char * path, float sz) {
    ifstream input;
    input.open(path, ifstream::in);
    if (input.fail())   return;
    string line;
    while (!input.eof()) {
        getline(input, line);
        istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) 
                iss >> v[i];
            vertices.push_back(v * sz);
        }
        else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3f norm;
            for (int i=0;i<3;i++) iss >> norm[i];
            normals.push_back(norm);
        } 
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f uv;
            for (int i=0;i<2;i++) iss >> uv[i];
            texcoord.push_back(uv);
        }
        else if (!line.compare(0, 2, "f ")) {
            vector<Vec3i> f;
            Vec3i tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                for (int i=0; i<3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
                f.push_back(tmp);
            }
            faces.push_back(f);
        }
    }
}

mModel::~mModel() {}

int mModel::vertSize() {
    return (int)vertices.size();
}

int mModel::facesSize() {
    return (int)faces.size();
}

Vec4f mModel::getVertex(int index) {
    return Vec4f((vertices[index].x + 1.0f) * 0.5f,
                -(vertices[index].y + 1.0f) * 0.5f,
                 (vertices[index].z + 1.0f) * 0.5f, 1.0f);
}

Vec4f mModel::getVertex(int faceIndex, int vertIndex) {
    return Vec4f((vertices[faces[faceIndex][vertIndex][0]].x + 1.0f) * 0.5f,
                -(vertices[faces[faceIndex][vertIndex][0]].y + 1.0f) * 0.5f,
                 (vertices[faces[faceIndex][vertIndex][0]].z + 1.0f) * 0.5f, 1.0f);
}

vector<int> mModel::getFace(int index) {
    vector<int> face;
    for (int i = 0; i < (int)faces[index].size(); i++) 
        face.push_back(faces[index][i][0]);
    return face;
}

std::array<int, 3> mModel::getFace3(int index) {
    array<int, 3> face;
    for (int i = 0; i < (int)faces[index].size(); i++) 
        face[i] = faces[index][i][0];
    return face;
}
