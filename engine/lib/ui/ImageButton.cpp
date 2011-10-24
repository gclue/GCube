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
 * ImageButton.cpp
 *
 *  Created on: 2011/08/09
 *      Author: GClue, Inc.
 */
#include "ImageButton.h"
#include "Animation.h"
#include "AnimationSet.h"
#include "Figure.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "Texture.h"
#include "ImageView.h"

ImageButton::ImageButton(AEContext *context) : Button(context) {
	view[0] = NULL;
	view[1] = NULL;
	listener = NULL;
}

ImageButton::~ImageButton() {
	DELETE(view[0]);
	DELETE(view[1]);

	// これらは他でdeleteするので、ここでは行わない。
	listener = NULL;
}

void ImageButton::setView(View *view) {
	DELETE(this->view[0]);
	this->view[0] = view;
	this->view[0]->parent = this;
}

void ImageButton::setFocusView(View *view) {
	DELETE(this->view[1]);
	this->view[1] = view;
	this->view[1]->parent = this;
}

void ImageButton::startClickAnimation(IAnimation *a) {
	if (a == NULL) {
		Animation *a1 = new Animation();
		a1->setAlpha(0.0, 1.0);
		a1->setDuration(0.2);

		Animation *a2 = new Animation();
		a2->setAlpha(1.0, 0.0);
		a2->setDuration(0.2);

		AnimationSet *animation = new AnimationSet();
		animation->addAnimation(a1);
		animation->addAnimation(a2);
		animation->setRepeat(false);
		animation->reset();
		view[1]->startAnimation((IAnimation *) animation);
	} else {
		view[1]->startAnimation(a);
	}
}

bool ImageButton::onTouch(TouchEvent &event) {
	switch (event.type) {
	case touchUp:
		if (!view[1]->isAnimation() && isBound(event.x, event.y)) {
			startClickAnimation();
			if (listener) {
				listener->onButtonTouchStart(this);
			}
			return true;
		}
		break;
	}
	return view[1]->isAnimation();
}

void ImageButton::draw(double dt, IAnimation *a) {
	if (view[1]->isAnimation()) {
		view[0]->render(dt);
		view[1]->render(dt);
		if (!view[1]->isAnimation()) {
			if (listener) {
				listener->onButtonClick(this);
			}
		}
	} else {
		view[0]->render(dt);
	}
}
