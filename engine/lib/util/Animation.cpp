/*
 * The MIT License (MIT)
 * Copyright (c) 2011 GClue, inc.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Animation.h"
#include "AnimationSet.h"
#include "defines.h"
#include <stdio.h>

Animation::Animation(int type) {
	listener = NULL;
	animation = NULL;

	mX = NULL;
	mY = NULL;
	mZ = NULL;
	mxRotation = NULL;
	myRotation = NULL;
	mzRotation = NULL;
	mxScale = NULL;
	myScale = NULL;
	mzScale = NULL;
	mR = NULL;
	mG = NULL;
	mB = NULL;
	mAlpha = NULL;
	mBright = NULL;

	this->type = type;
	time = 0.0;
	delay = 0.0;
	delayTime = 0.0;
	alpha = 1.0;
	bright = 1.0;
	aspect = 1.0;
	width = 1.0;
	height = 1.0;
	depth = 1.0;
	x = 0.0;
	y = 0.0;
	z = 0.0;
	r = 1.0;
	g = 1.0;
	b = 1.0;
	xrotate = 0.0;
	yrotate = 0.0;
	zrotate = 0.0;
	xscale = 1.0;
	yscale = 1.0;
	zscale = 1.0;
	tag = 0;
	startflg = false;
	finishflg = false;
	repeat = false;

	matrix = new Matrix3D();
	if (matrix) {
		matrix->loadIdentity();
	}
}

Animation::~Animation() {
	DELETE(mX);
	DELETE(mY);
	DELETE(mZ);

	DELETE(mxScale);
	DELETE(myScale);
	DELETE(mzScale);
	
	DELETE(mxRotation);
	DELETE(myRotation);
	DELETE(mzRotation);
	
	DELETE(mAlpha);
	DELETE(mBright);

	DELETE (matrix);
	DELETE(mR);
	DELETE(mG);
	DELETE(mB);
	DELETE(animation);
}

bool Animation::isStarted() {
	return startflg;
}

void Animation::setDelay(double delay) {
	this->delay = delay;
	this->delayTime = delay;
}

double Animation::getDelay() {
	return this->delayTime;
}

void Animation::setDuration(float duration) {
	this->duration = duration;

	if (mX) mX->duration = duration;
	if (mY) mY->duration = duration;
	if (mZ) mZ->duration = duration;

	if (mxScale) mxScale->duration = duration;
	if (myScale) myScale->duration = duration;
	if (mzScale) mzScale->duration = duration;
	
	if (mxRotation) mxRotation->duration = duration;
	if (myRotation) myRotation->duration = duration;
	if (mzRotation) mzRotation->duration = duration;
	
	if (mAlpha) {
		mAlpha->duration = duration;
	}
	
	if (mBright) {
		mBright->duration = duration;
	}

	if (mR) {
		mR->duration = duration;
	}
	if (mG) {
		mG->duration = duration;
	}
	if (mB) {
		mB->duration = duration;
	}
}

void Animation::setAlpha(float start, float end) {
	if (!mAlpha) {
		mAlpha = new Easing(type);
	}
	mAlpha->begin = start;
	mAlpha->delta = end - start;
	alpha = start;
}

void Animation::setMoveX(float start, float end) {
	if (!mX) {
		mX = new Easing(type);
	}
	mX->begin = start;
	mX->delta = end - start;
	x = start;
}

void Animation::setMoveY(float start, float end) {
	if (!mY) {
		mY = new Easing(type);
	}
	mY->begin = start;
	mY->delta = end - start;
	y = start;
}

void Animation::setMoveZ(float start, float end) {
	if (!mZ) {
		mZ = new Easing(type);
	}
	mZ->begin = start;
	mZ->delta = end - start;
	z = start;
}

void Animation::setMove(float sx, float sy, float ex, float ey) {
	setMoveX(sx, ex);
	setMoveY(sy, ey);
}

void Animation::setMove(float sx, float sy, float sz, float ex, float ey, float ez) {
	setMoveX(sx, ex);
	setMoveY(sy, ey);
	setMoveY(sz, ez);
}

void Animation::setScaleX(float start, float end) {
	if (!mxScale) {
		mxScale = new Easing(type);
	}
	
	mxScale->begin = start;
	mxScale->delta = end - start;
	xscale = start;
}

void Animation::setScaleY(float start, float end) {
	if (!myScale) {
		myScale = new Easing(type);
	}
	
	myScale->begin = start;
	myScale->delta = end - start;
	yscale = start;
}

void Animation::setScaleZ(float start, float end) {
	if (!mzScale) {
		mzScale = new Easing(type);
	}
	
	mzScale->begin = start;
	mzScale->delta = end - start;
	zscale = start;
}

void Animation::setScale(float sx, float sy, float ex, float ey) {
	setScaleX(sx, ex);
	setScaleY(sy, ey);
}

void Animation::setScale(float sx, float sy, float sz, float ex, float ey, float ez) {
	setScaleX(sx, ex);
	setScaleY(sy, ey);
	setScaleZ(sz, ez);
}

void Animation::setBright(float sb, float eb) {
	if (!mBright) {
		mBright = new Easing(type);
	}
	mBright->begin = sb;
	mBright->delta = eb - sb;
	bright = sb;
}

void Animation::setRGB(float sr, float sg, float sb, float er, float eg, float eb) {
	if (!mR) {
		mR = new Easing(type);
	}
	if (!mG) {
		mG = new Easing(type);
	}
	if (!mB) {
		mB = new Easing(type);
	}

	mR->begin = sr;
	mG->begin = sg;
	mB->begin = sb;
	mR->delta = er - sr;
	mG->delta = eg - sg;
	mB->delta = eb - sb;
	r = sr;
	g = sg;
	b = sb;
}

void Animation::setRotation(float start, float end) {
	if (!mzRotation) {
		mzRotation = new Easing(type);
	}
	mzRotation->begin = start;
	mzRotation->delta = end - start;
	zrotate = start;
}

void Animation::setRotationX(float start, float end) {
	if (!mxRotation) {
		mxRotation = new Easing(type);
	}
	mxRotation->begin = start;
	mxRotation->delta = end - start;
	xrotate = start;
}

void Animation::setRotationY(float start, float end) {
	if (!myRotation) {
		myRotation = new Easing(type);
	}
	myRotation->begin = start;
	myRotation->delta = end - start;
	yrotate = start;
}

void Animation::setRotationZ(float start, float end) {
	if (!mzRotation) {
		mzRotation = new Easing(type);
	}
	mzRotation->begin = start;
	mzRotation->delta = end - start;
	zrotate = start;
}


////////////////////////////////////////////////////////////
// IAnimationからの継承クラス
////////////////////////////////////////////////////////////

void Animation::multiply(Matrix3D *mtx) {
	if (animation) {
		animation->multiply(mtx);
	}
	if (xscale != 1.0 || yscale != 1.0 || zscale != 1.0) {
		mtx->scale(xscale, yscale, zscale);
	}
	if (xrotate != 0) {
		mtx->rotate(xrotate, RotateDirX);
	}
	if (yrotate != 0) {
		mtx->rotate(yrotate, RotateDirY);
	}
	if (zrotate != 0) {
		mtx->rotate(zrotate, RotateDirZ);
	}
	mtx->translate(x, y, z);
}

void Animation::finish() {
	finishflg = true;
	if (listener) {
		listener->onAnimationAllEnd(this);
	}
}

void Animation::reset() {
	time = 0;
	delayTime = delay;
	finishflg = false;
	startflg = false;
	if (mX) x = mX->begin;
	if (mY) y = mY->begin;
	if (mZ) z = mZ->begin;
	if (mxScale) xscale = mxScale->begin;
	if (mxScale) yscale = myScale->begin;
	if (mzScale) zscale = mzScale->begin;
	if (mxRotation) xrotate = mxRotation->begin;
	if (myRotation) yrotate = myRotation->begin;
	if (mzRotation) zrotate = mzRotation->begin;
	if (mAlpha) alpha = mAlpha->begin;
	if (mBright) bright = mBright->begin;
}

void Animation::step(double dt) {
	if (finishflg) return;
	if (delayTime > 0) {
		delayTime -= dt;
		return;
	}

	if (listener && !startflg) {
		listener->onAnimationStart(this);
	}

	startflg = true;

	time += dt;
	easing(dt);
	if (time > duration) {
		if (repeat) {
			// リピート
			reset();
			startflg = true;
		} else {
			finishflg = true;
		}
		if (listener) {
			listener->onAnimationAllEnd(this);
		}
	}
}

bool Animation::isFinish() {
	return finishflg;
}

void Animation::getMatrix(float *m) {
	matrix->getElements(m);
}

float Animation::getAlpha() {
	return alpha;
}

float Animation::getBright() {
	return bright;
}

////////////////////////////////////////////////////////////
// Private関数
////////////////////////////////////////////////////////////

void Animation::easing(double dt) {
	matrix->loadIdentity();

	if (animation) {
		animation->step(dt);
		if (animation->isStarted()) {
			float m[16];
			animation->matrix->getElements(m);
			matrix->setElements(m);
		}
	}

	if (mxScale) {
		xscale = mxScale->easing(time);
	}
	if (myScale) {
		yscale = myScale->easing(time);
	}
	if (mzScale) {
		zscale = mzScale->easing(time);
	}
	matrix->scale(xscale, yscale, zscale);

	if (mxRotation) {
		xrotate = mxRotation->easing(time);
		matrix->rotate(xrotate, RotateDirX);
	}
	if (myRotation) {
		yrotate = myRotation->easing(time);
		matrix->rotate(yrotate, RotateDirY);
	}
	if (mzRotation) {
		zrotate = mzRotation->easing(time);
		matrix->rotate(zrotate, RotateDirZ);
	}

	if (mX) {
		x = mX->easing(time);
	}
	if (mY) {
		y = mY->easing(time);
	}
	if (mZ) {
		z = mZ->easing(time);
	}
	matrix->translate(x, y, z);

	if (mR) {
		r = mR->easing(time);
	}
	if (mG) {
		g = mG->easing(time);
	}
	if (mB) {
		b = mB->easing(time);
	}

	if (mBright) {
		bright = mBright->easing(time);
	}
	if (mAlpha) {
		alpha = mAlpha->easing(time);
	}
}
