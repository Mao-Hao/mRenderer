#pragma once
#include "mBase.h"
#include "mMath.hpp"
#include "mColor.h"

class mVertex {
public:
	Vec3f	p;	// position
	mColor	mc;
	Vec3f	normal;
	// Vec2f	tex;
	mVertex () : p(0, 0, 0), mc(White), normal(0, 0, 0) {}

	mVertex (Vec3f & _pos)	: p(_pos), mc(White) {}
	mVertex (Vec3f &  _pos, mColor & _mc)	: p(_pos), mc(_mc) {}

	mVertex (Vec4f & _pos)	: p(_pos.x, _pos.y, _pos.z), mc(White) {}
	mVertex (Vec4f &  _pos, mColor _mc)	: p(_pos.x, _pos.y, _pos.z), mc(_mc) {}
	mVertex (Vec4f &  _pos, mColor & _mc)	: p(_pos.x, _pos.y, _pos.z), mc(_mc) {}

	mVertex (Vec4f &  _pos, mColor & _mc, Vec3f _normal)	
		: p(_pos.x, _pos.y, _pos.z), mc(_mc), normal(_normal) {}
	mVertex (Vec4f &  _pos, mColor & _mc, Vec3f & _normal)	
		: p(_pos.x, _pos.y, _pos.z), mc(_mc), normal(_normal) {}

	~mVertex () {}

private:

};