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

ViewGroup::ViewGroup() : View() {
}

ViewGroup::ViewGroup(GCContext *context) : View(context) {
}

ViewGroup::~ViewGroup() {
	removeAllView();
}

void ViewGroup::addView(View *view) {
	// 親Viewにこのクラスを追加
	view->parent = this;
	
	//参照カウンタをプラス
	view->retain();
	
	// リストにViewを追加
	views.push_back(view);
	
	// サイズが設定されていない場合はサイズを入れておく
	if (size.x == 0 && size.y == 0) {
		setSize(view->size);
	}
}

void ViewGroup::removeView(View *view) {
	std::vector<View *>::iterator it = views.begin();
	while (!views.empty() && it != views.end()) {
		View *a = *it;
		if (a == view) {
			it = views.erase(it);
            a->parent = NULL;
			a->release();
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
		views.at(i)->release();
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

static bool compareView(View *a, View *b) {
	return a->getUserID() < b->getUserID();
}

void ViewGroup::sort(ViewSortFunc func) {
	if (func) {
		std::sort(views.begin(), views.end(), func);
	} else {
		std::sort(views.begin(), views.end(), compareView);
	}
}

void ViewGroup::front(View *v) {
	bool flag = false;
	std::vector<View *>::iterator it = views.begin();
	while (!views.empty() && it != views.end()) {
		View *view = *it;
		if (view == v) {
			it = views.erase(it);
			flag = true;
		} else {
			it++;
		}
	}
	
	if (flag) {
		views.push_back(v);
	}
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

bool ViewGroup::isBound(float x, float y) {
	if (!clickable || !visible) {
		return false;
	}
	
	if (size.x == 0 || size.y == 0) {
		int size = views.size();
		for (int i = 0; i < size; i++) {
			if (views.at(i)->isBound(x, y)) {
				return true;
			}
		}
	} else {
		return View::isBound(x, y);
	}
	return false;
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

void ViewGroup::draw(double dt, ViewContext *context) {
	if (visible) {
		int size = views.size();
		for (int i = 0; i < size; i++) {
			views.at(i)->render(dt, context);
		}
	}
}
