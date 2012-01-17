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
 * Button.cpp
 *
 *  Created on: 2011/07/14
 *      Author: GClue, Inc.
 */
#include "Button.h"
#include "Animation.h"
#include "AnimationSet.h"
#include "Figure.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "Texture.h"

Button::Button(GCContext *context) : View(context) {
	clickAnim = NULL;
	figure[0] = NULL;
	figure[1] = NULL;
	listener = NULL;
}

Button::~Button() {
	DELETE(clickAnim);
	// これらは他でdeleteするので、ここでは行わない。
	listener = NULL;
}
void Button::setOnButtonClickListener(OnButtonClickListener *l) {
	this->listener = l;
}

bool Button::isClickAnimation() {
	return clickAnim && !clickAnim->isFinish();
}

void Button::startClickAnimation() {
	DELETE(clickAnim);
	clickAnim = new AnimationSet();

	Animation *a1 = new Animation();
	a1->setAlpha(1.0, 1.0);
	a1->setScale(1.0, 1.0, 1.05, 1.05);
	a1->setDuration(0.10);
	clickAnim->addAnimation(a1);

	Animation *a2 = new Animation();
	a2->setAlpha(1.0, 1.0);
	a2->setScale(1.05, 1.05, 1.0, 1.0);
	a2->setDuration(0.10);
	clickAnim->addAnimation(a2);
	clickAnim->setRepeat(false);
	clickAnim->reset();
	
}

void Button::setFigure(Figure *figure) {
	this->figure[0] = figure;

	if (figure) {
		size.x = figure->size.x / 2.0;
		size.y = figure->size.y / 2.0;
	}
}

void Button::setSelectFigure(Figure *figure) {
	this->figure[1] = figure;
}

bool Button::onTouch(TouchEvent &event) {
	if (!clickable || !visible) {
		return false;
	}
	
	switch (event.type) {
	case touchDown:
		if (!isClickAnimation() && isBound(event.x, event.y)) {
			startClickAnimation();
			if (listener) {
				listener->onButtonTouchStart(this);
			}
			return true;
		}
		break;
	}
	return isClickAnimation();
}

void Button::draw(double dt, IAnimation *a) {
	if (clickAnim && !clickAnim->isFinish()) {
		float alpha = 1.0;
		float bright = 1.0;
		Matrix3D mtx;

		clickAnim->step(dt);
		clickAnim->multiply(&mtx);
		alpha = clickAnim->getAlpha();
		bright = clickAnim->getBright();


		testMatrix3D(&mtx, &alpha, &bright);

		context->shader->setMVPMatrix(context->camera, &mtx);
		context->shader->setAlpha(alpha);
		context->shader->setBright(bright);

		figure[1]->bind();
		figure[1]->draw();

		// animationの削除は上のクラスで行うので、ここでは行わない。
		if (clickAnim->isFinish()) {
			if (listener) {
				// クリックイベントを送ります
				listener->onButtonClick(this);
			}
		}
	} else {
		float alpha = 1.0;
		float bright = 1.0;
		Matrix3D mtx;
		testMatrix3D(&mtx, &alpha, &bright);

		context->shader->setMVPMatrix(context->camera, &mtx);
		context->shader->setAlpha(alpha);
		context->shader->setBright(bright);

		figure[0]->bind();
		figure[0]->draw();
	}
}
