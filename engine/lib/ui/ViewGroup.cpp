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
 * ViewGroup.cpp
 *
 *  Created on: 2011/07/22
 *      Author: GClue, Inc.
 */
#include "ViewGroup.h"
#include "Animation.h"
#include "Matrix3D.h"

ViewGroup::ViewGroup(AEContext *context) : View(context) {
}

ViewGroup::~ViewGroup() {
	removeAllView();
}

void ViewGroup::addView(View *view) {
	// 親Viewにこのクラスを追加
	view->parent = this;
	// リストにViewを追加
	views.push_back(view);
}

void ViewGroup::removeView(View *view) {
	std::vector<View *>::iterator it = views.begin();
	while (!views.empty() && it != views.end()) {
		View *a = *it;
		if (a == view) {
			it = views.erase(it);
			delete a;
		} else {
			it++;
		}
	}
}

void ViewGroup::removeViewByID(int id) {
	std::vector<View *>::iterator it = views.begin();
	while (!views.empty() && it != views.end()) {
		View *a = *it;
		if (a->userID == id) {
			removeView(a);
			return;
		}
		it++;
	}
}

void ViewGroup::removeAllView() {
	int size = views.size();
	for (int i = 0; i < size; i++) {
		delete views.at(i);
	}
	views.clear();
}

View *ViewGroup::getView(int index) {
	return views.at(index);
}

View *ViewGroup::getView(void *userObj) {
	std::vector<View *>::iterator it = views.begin();
	while (!views.empty() && it != views.end()) {
		View *view = *it;
		if (view->userObj == userObj) {
			return view;
		}
		it++;
	}
	return NULL;
}

int ViewGroup::count() {
	return views.size();
}

View* ViewGroup::findViewByID(int id) {
	if (userID == id) {
		return this;
	}

	std::vector<View *>::iterator it = views.begin();
	while (!views.empty() && it != views.end()) {
		View *a = *it;
		View *v = a->findViewByID(id);
		if (v != NULL) {
			return v;
		}
		it++;
	}

	return NULL;
}

View* ViewGroup::findViewByUserObj(void *userObj, COMPARE_FUNC_PTR *func) {
	if (func) {
		if (this->userObj == userObj) {
			return this;
		}
	} else {
		if ((*func)(this->userObj, userObj) == 0) {
			return this;
		}
	}

	std::vector<View *>::iterator it = views.begin();
	while (!views.empty() && it != views.end()) {
		View *a = *it;
		View *v = a->findViewByUserObj(userObj, func);
		if (v != NULL) {
			return v;
		}
		it++;
	}

	return NULL;
}


bool ViewGroup::onTouch(TouchEvent &event) {
	if (!clickable || !visible) {
		return false;
	}

	int size = views.size();
	for (int i = size - 1; i >= 0; i--) {
		View *view = views.at(i);
		if (!view->clickable || !view->visible) {
		} else {
			if (view->onTouch(event)) {
				return true;
			}
		}
	}
	return false;
}

void ViewGroup::draw(double dt, IAnimation *animation) {
	int size = views.size();
	for (int i = 0; i < size; i++) {
		views.at(i)->render(dt);
	}
}

