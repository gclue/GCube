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

/*
 * AnimationSet.cpp
 *
 *  Created on: 2011/05/06
 *      Author: GClue, Inc.
 */
#include "AnimationSet.h"

AnimationSet::AnimationSet() {
	listener = NULL;
	index = 0;
	delay = 0;
	delayTime = 0;
	startflg = false;
	finishflg = false;
	x = 0.0;
	y = 0.0;
	z = 0.0;
	r = 1.0;
	g = 1.0;
	b = 1.0;
	xscale = 1.0;
	yscale = 1.0;
	alpha = 1.0;
	bright = 1.0;
	rotate = 0.0;
	repeat = true;
}

AnimationSet::~AnimationSet() {
	for (int i = 0; i < animations.size(); i++) {
		Animation *a = animations.at(i);
		if (a) {
			delete a;
		}
	}
	animations.clear();
}

bool AnimationSet::isStarted() {
	return startflg;
}

void AnimationSet::setDelay(double delay) {
	this->delay = delay;
	this->delayTime = delay;
}

void AnimationSet::addAnimation(Animation *animation) {
	animations.push_back(animation);

	if (animations.size() == 1) {
		matrix = animation->matrix;
		alpha = animation->alpha;
		x = animation->x;
		y = animation->y;
		z = animation->z;
		xscale = animation->xscale;
		yscale = animation->yscale;
		rotate = animation->rotate;
		r = animation->r;
		g = animation->g;
		b = animation->b;
	}
}

void AnimationSet::setAnimationListener(IAnimationListener *listener) {
	this->listener = listener;
}

void AnimationSet::setRepeat(bool repeat) {
	this->repeat = repeat;
}

////////////////////////////////////////////////////////////
// IAnimationからの継承クラス
////////////////////////////////////////////////////////////

void AnimationSet::multiply(Matrix3D *mtx) {
	if (xscale != 1.0 || yscale != 1.0) {
		mtx->scale(xscale, yscale, 1.0);
	}
	if (rotate != 0) {
		mtx->rotate(rotate, RotateDirZ);
	}
	mtx->translate(x, y, 0.0);
}

void AnimationSet::finish() {
	finishflg = true;
	if (listener) {
		listener->onAnimationAllEnd(this);
	}
}

void AnimationSet::reset() {
	index = 0;
	delayTime = delay;
	finishflg = false;
	startflg = false;

	if (animations.size() > 0) {
		Animation *a = animations.at(index);
		a->reset();

		matrix = a->matrix;
		alpha = a->alpha;
		x = a->x;
		y = a->y;
		z = a->z;
		xscale = a->xscale;
		yscale = a->yscale;
		rotate = a->rotate;
		r = a->r;
		g = a->g;
		b = a->b;
		bright = a->bright;
	}
}

void AnimationSet::step(double dt) {
	if (finishflg) return;

	if (index < 0) {
		return;
	}

	if (delayTime > 0) {
		delayTime -= dt;
		return;
	}

	// アニメーション開始イベントを通知
	if (listener && !startflg) {
		listener->onAnimationStart(this);
	}

	startflg = true;

	if (animations.size() == 0) {
		this->finish();
		return;
	}

	Animation *a = animations.at(index);
	a->step(dt);

	matrix = a->matrix;
	alpha = a->alpha;
	x = a->x;
	y = a->y;
	z = a->z;
	xscale = a->xscale;
	yscale = a->yscale;
	rotate = a->rotate;
	r = a->r;
	g = a->g;
	b = a->b;
	bright = a->bright;

	if (a->isFinish()) {
		if (animations.size() - 1 > index) {
			if (listener) {
				listener->onAnimationEnd(this, index);
			}
			index++;
			// 一応アニメーションをリセットしておく
			Animation *a = animations.at(index);
			a->reset();
		} else {
			if (repeat) {
				// リピート
				reset();
				startflg = true;
			} else {
				// 全部のアニメーションが終了
				finishflg = true;
				if (listener) {
					listener->onAnimationAllEnd(this);
				}
			}
		}
	}
}

bool AnimationSet::isFinish() {
	return finishflg;
}


void AnimationSet::getMatrix(float *m) {
	matrix->getElements(m);
}

float AnimationSet::getAlpha() {
	return alpha;
}

float AnimationSet::getBright() {
	return bright;
}

