#include <iostream>
#include <fstream>
#include <sstream>
#include "mModel.h"

using namespace std;

mModel::mModel(const char * path) {
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
            vertices.push_back(v);
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
    //return Vec4f((vertices[index].x + 1.0f) * 0.5f,
    //            -(vertices[index].y + 1.0f) * 0.5f,
    //             (vertices[index].z + 1.0f) * 0.5f, 1.0f);
    return Vec4f((vertices[index].x) * 0.01f,
                -(vertices[index].y) * 0.01f,
                 (vertices[index].z) * 0.01f, 1.0f);
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
