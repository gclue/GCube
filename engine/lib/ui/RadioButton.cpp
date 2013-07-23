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
 * RadioButton.cpp
 *
 *  Created on: 2011/08/10
 *      Author: GClue, Inc.
 */
#include "RadioButton.h"
#include "Animation.h"
#include "RadioGroup.h"
#include "defines.h"

#include <math.h>

RadioButton::RadioButton() : View() {
	initRadioButton();
}

RadioButton::RadioButton(GCContext *context) : View(context) {
	initRadioButton();
}

RadioButton::~RadioButton() {
    RELEASE(this->view[0]);
    RELEASE(this->view[1]);
}

void RadioButton::initRadioButton() {
	view[0] = NULL;
	view[1] = NULL;
	selectFlag = false;
}

View* RadioButton::findViewByID(int id) {
	if (userID == id) {
		return this;
	}

	View* v = view[0]->findViewByID(id);
	if (v != NULL) {
		return v;
	}
	return view[1]->findViewByID(id);
}

View* RadioButton::findViewByUserObj(void *userObj, COMPARE_FUNC_PTR *func) {
	if (func) {
		if (this->userObj == userObj) {
			return this;
		}
	} else {
		if ((*func)(this->userObj, userObj) == 0) {
			return this;
		}
	}

	View* v = view[0] = findViewByUserObj(userObj, func);
	if (v != NULL) {
		return v;
	}
	return view[1]->findViewByUserObj(userObj, func);
}

bool RadioButton::onTouch(TouchEvent &event) {
	if (!clickable) {
		return false;
	}
	if (!visible) {
		return false;
	}

	if (selectFlag) {
		if (view[1]->onTouch(event)) {
			return true;
		}
	} else {
		if (view[0]->onTouch(event)) {
			return true;
		}
	}
	return false;
}

bool RadioButton::isBound(float x, float y) {
	if (!clickable || !visible) {
		return false;
	}
	
	if (size.x == 0 || size.y == 0) {
		for (int i = 0; i < 2; i++) {
			if (view[i]->isBound(x, y)) {
				return true;
			}
		}
	} else {
		return View::isBound(x, y);
	}
	return false;
}

void RadioButton::draw(double dt, ViewContext *context) {
	if (selectFlag) {
		view[1]->render(dt, context);
	} else {
		view[0]->render(dt, context);
	}
}

void RadioButton::setView(View *view) {
    RELEASE(this->view[0]);
	this->view[0] = view;
	this->view[0]->parent = this;
	this->view[0]->retain();
}

void RadioButton::setSelectedView(View *view) {
    RELEASE(this->view[1]);
	this->view[1] = view;
	this->view[1]->parent = this;
	this->view[1]->retain();
}

bool RadioButton::isSelected() {
	return selectFlag;
}

void RadioButton::selected() {
	if (parent) {
		RadioGroup *rg = (RadioGroup *) parent;
		rg->setSelected(this);
	}
}

void RadioButton::setSelected(bool flag) {
	this->selectFlag = flag;
}
