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
#include <stdio.h>

Animation::Animation(int type) {
	listener = NULL;
	animation = NULL;

	mX = NULL;
	mY = NULL;
	mAlpha = NULL;
	mRotation = NULL;
	mxScale = NULL;
	myScale = NULL;
	mR = NULL;
	mG = NULL;
	mB = NULL;
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
	x = 0.0;
	y = 0.0;
	z = 0.0;
	r = 1.0;
	g = 1.0;
	b = 1.0;
	rotate = 0.0;
	xscale = 1.0;
	yscale = 1.0;
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
	if (mX) {
		delete mX;
	}
	mX = NULL;

	if (mY) {
		delete mY;
	}
	mY = NULL;

	if (mAlpha) {
		delete mAlpha;
	}
	mAlpha = NULL;

	if (mBright) {
		delete mBright;
	}
	mBright = NULL;

	if (mRotation) {
		delete mRotation;
	}
	mRotation = NULL;

	if (mxScale) {
		delete mxScale;
	}
	mxScale = NULL;

	if (myScale) {
		delete myScale;
	}
	myScale = NULL;

	if (matrix) {
		delete matrix;
	}
	matrix = NULL;

	if (mR) {
		delete mR;
	}
	mR = NULL;

	if (mG) {
		delete mG;
	}
	mG = NULL;

	if (mB) {
		delete mB;
	}
	mB = NULL;

	if (animation) {
		delete animation;
	}
	animation = NULL;
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

	if (mX) {
		mX->duration = duration;
	}

	if (mY) {
		mY->duration = duration;
	}

	if (mAlpha) {
		mAlpha->duration = duration;
	}

	if (mRotation) {
		mRotation->duration = duration;
	}

	if (mxScale) {
		mxScale->duration = duration;
	}

	if (myScale) {
		myScale->duration = duration;
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

	if (mBright) {
		mBright->duration = duration;
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

void Animation::setMove(float sx, float sy, float ex, float ey) {
	if (!mX) {
		mX = new Easing(type);
	}
	if (!mY) {
		mY = new Easing(type);
	}

	mX->begin = sx;
	mY->begin = sy;
	mX->delta = ex - sx;
	mY->delta = ey - sy;
	x = sx;
	y = sy;
}

void Animation::setScale(float sx, float sy, float ex, float ey) {
	if (!mxScale) {
		mxScale = new Easing(type);
	}
	if (!myScale) {
		myScale = new Easing(type);
	}

	mxScale->begin = sx;
	myScale->begin = sy;
	mxScale->delta = ex - sx;
	myScale->delta = ey - sy;
	xscale = sx;
	yscale = sy;
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
	if (!mRotation) {
		mRotation = new Easing(type);
	}
	mRotation->begin = start;
	mRotation->delta = end - start;
	rotate = start;
}


void Animation::setAnimationListener(IAnimationListener *listener) {
	this->listener = listener;
}

void Animation::setRepeat(bool repeat) {
	this->repeat = repeat;
}

////////////////////////////////////////////////////////////
// IAnimationからの継承クラス
////////////////////////////////////////////////////////////

void Animation::multiply(Matrix3D *mtx) {
	if (xscale != 1.0 || yscale != 1.0) {
		mtx->scale(xscale, yscale, 1.0);
	}
	if (rotate != 0) {
		mtx->rotate(rotate, RotateDirZ);
	}
	mtx->translate(x, y, 0.0);
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
	if(mX) x = mX->begin;
	if(mY) y = mY->begin;
	if(mxScale) xscale = mxScale->begin;
	if(mxScale) yscale = myScale->begin;
	if(mRotation) rotate = mRotation->begin;
	if(mAlpha) alpha = mAlpha->begin;
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

	if (mxScale && myScale) {
		xscale = mxScale->easing(time);
		yscale = myScale->easing(time);
		matrix->scale(xscale, yscale, 1);
	}
	matrix->scale(width, height, 1);

	if (mRotation) {
		rotate = mRotation->easing(time);
		matrix->rotate(rotate, RotateDirZ);
	}

	if (mX && mY) {
		x = mX->easing(time);
		y = mY->easing(time);
		matrix->translate(x, y, 0);
	}
	matrix->translate(0, 0, z);

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
