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
 * RadioGroup.cpp
 *
 *  Created on: 2011/08/10
 *      Author: GClue, Inc.
 */
#include "RadioGroup.h"
#include "Animation.h"
#include "RadioButton.h"
#include "defines.h"
#include <math.h>

RadioGroup::RadioGroup(AEContext *context) : ViewGroup(context) {
	listener = NULL;
}

RadioGroup::~RadioGroup() {
	listener = NULL;
}

void RadioGroup::addRadioButton(RadioButton *view) {
	this->addView(view);
	if (views.size() == 1) {
		view->setSelected(true);
	} else {
		view->setSelected(false);
	}
}

bool RadioGroup::onTouch(TouchEvent &event) {
	if (!clickable) {
		return false;
	}
	if (!visible) {
		return false;
	}

	float x = event.x;
	float y = event.y;

	// ラジオボタンの中でタッチイベント処理がされた場合には
	// ラジオの選択は移動しない。
	int size = views.size();
	for (int i = size - 1; i >= 0; i--) {
		View *view = views.at(i);
		if (view->onTouch(event)) {
			return true;
		}
	}

	float dx;
	float dy;

	// クリックされた場合には、RadioButtonの選択を切り替える
	switch (event.type) {
	case touchDown:
		startPos.x = event.x;
		startPos.y = event.y;
		currentPos.x = event.x;
		currentPos.y = event.y;
		isTouch = true;
		break;
	case touchMove:
	case touchCancel:
		currentPos.x = event.x;
		currentPos.y = event.y;
		dx = currentPos.x - startPos.x;
		dy = currentPos.y - startPos.y;
		if (dx*dx + dy * dy > 0.05) {
			isTouch = false;
		}
		break;
	case touchUp:
		if (isTouch) {
			isTouch = false;
			int size = views.size();
			for (int i = size - 1; i >= 0; i--) {
				View *view = views.at(i);
				if (view->isBound(x, y)) {
					RadioButton *radio = (RadioButton *) view;
					bool select = radio->isSelected();
					// タッチされたRadioButtonを選択中に設定
					for (int j = 0; j < size; j++) {
						RadioButton *btn = (RadioButton *) views.at(j);
						btn->setSelected(j == i);
					}
					// 前からtrueの場合にはイベントを発生させない
					if (listener && !select) {
						listener->onCheckedChanged(this, (RadioButton *) view);
					}
					return true;
				}
			}
		}
		break;
	}

	return false;
}

void RadioGroup::setOnCheckedChangeListener(OnCheckedChangeListener *listener) {
	this->listener = listener;
}

void RadioGroup::setSelected(RadioButton *view) {
	int size = views.size();
	for (int i = 0; i < size; i++) {
		RadioButton *btn = (RadioButton *) views.at(i);
		btn->setSelected(view == btn);
		if (view == btn) {
			if (listener) {
				listener->onCheckedChanged(this, view);
			}
		}
	}
}
