#pragma once
#include "mBase.h"
#include "mMath.hpp"
#include "mColor.h"

class mVertex {
public:
	Vec3f	p;	// position
	mColor	mc;
	// Vec2f	tex;
	mVertex () : p(0, 0, 0), mc(White) {}

	mVertex (Vec3f & _pos)	: p(_pos), mc(White) {}
	mVertex (Vec3f &  _pos, mColor & _mc)	: p(_pos), mc(_mc) {}

	mVertex (Vec4f & _pos)	: p(_pos.x, _pos.y, _pos.z), mc(White) {}
	mVertex (Vec4f &  _pos, mColor _mc)	: p(_pos.x, _pos.y, _pos.z), mc(_mc) {}
	mVertex (Vec4f &  _pos, mColor & _mc)	: p(_pos.x, _pos.y, _pos.z), mc(_mc) {}

	~mVertex () {}

private:

};