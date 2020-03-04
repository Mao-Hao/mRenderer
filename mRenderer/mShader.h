#pragma once
#include "mMath.hpp"
#include "mModel.h"
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
	std::array<Vec4f, 3> invW;		// 用于透视矫正

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
		auto face = model->getFace3(faceIndex);
		Vec4f v0 = model->getVertex(face[0]);
		Vec4f v1 = model->getVertex(face[1]);
		Vec4f v2 = model->getVertex(face[2]);
		Vec4xMat4(v0, mvp);
		Vec4xMat4(v1, mvp);
		Vec4xMat4(v2, mvp);
		if (!mClip(v0) && !mClip(v1) && !mClip(v2)) {
			v0 = v0 / v0.w;
			v1 = v1 / v1.w;
			v2 = v2 / v2.w;
			mVertex p0 = v0;
			mVertex p1 = v1;
			mVertex p2 = v2;
			mTriangleZ(p0, p1, p2);
		}
		return std::array<Vec4f, 3>();
	}

};
