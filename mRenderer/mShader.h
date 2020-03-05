#pragma once
#include "mMath.hpp"
#include "mModel.h"
#include "mColor.h"
#include <array>

class mShader {
public:
	Mat m;
	Mat v;
	Mat p;
	Mat mvp;
	
	mModel * model = nullptr;

#pragma region varyings

	std::array<Vec4f, 3> vertices;
	std::array<float, 3> invW = {1, 1, 1};		// 用于透视矫正
	std::array<Vec2f, 3> texcoords;

#pragma endregion varyings

	mShader () {}
	~mShader () {}

	void setMatrix(Mat _model, Mat _view, Mat _proj) {
		m = _model; v = _view; p = _proj;
		mvp = m * v * p;
	}
	void setModel(mModel * p) {
		model = p;
	}
	std::array<Vec4f, 3> VertexShader(int faceIndex) {
		auto face = model->getFace(faceIndex);
		Vec4f v0 = model->getVertex(face[0]);
		Vec4f v1 = model->getVertex(face[1]);
		Vec4f v2 = model->getVertex(face[2]);
		Vec4xMat4(v0, mvp);
		Vec4xMat4(v1, mvp);
		Vec4xMat4(v2, mvp);

		invW = { 1.0f / v0.w, 1.0f / v0.w, 1.0f / v0.w };
		texcoords = { model->getTexcoord(faceIndex, 0),
					  model->getTexcoord(faceIndex, 1),
					  model->getTexcoord(faceIndex, 2)};
		vertices = { v0, v1, v2 };

		return vertices;
	}

	// true -> discard
	bool FrameShader(Vec3f bc, _In_ mColor & color) {
		Vec2f uv = texcoords[0] * bc.x + texcoords[1] * bc.y + texcoords[2] * bc.z;
		color = model->diffuse(uv);
		return false;
	}

};
