#pragma once

#include "mBase.h"
#include "mMath.hpp"
#include "mColor.h"

class mPoint2D {
public:
	Vec2i	p;
	mColor	mc;

	mPoint2D () : p(0, 0), mc(White) {}
	mPoint2D (Vec2i _p)		: p(_p), mc(White) {}
	mPoint2D (Vec2i & _p)	: p(_p), mc(White) {}
	mPoint2D (Vec2i _p, mColor _mc)			: p(_p), mc(_mc) {}
	mPoint2D (Vec2i &  _p, mColor & _mc)	: p(_p), mc(_mc) {}

	~mPoint2D () {}

private:

};
