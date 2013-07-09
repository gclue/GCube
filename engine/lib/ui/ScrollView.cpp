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
 * ScrollView.cpp
 *
 *  Created on: 2011/08/08
 *      Author: GClue, Inc.
 */
#include "ScrollView.h"
#include "Animation.h"
#include "defines.h"

#include <math.h>

/**
 * @define スクロール画面を超えてスクロールしている場合の減衰率.
 */
#define OVERRUN_DAMPING_RATE 0.60

/**
 * @define スクロールスピードの減衰率.
 */
#define DAMPING_RATE 0.98

/**
 * @define 描画範囲の拡張.
 */
#define RENDER_RANGE 1.0

enum {
	TOUCH_NONE = 0,
	TOUCH_START,
};

ScrollView::ScrollView() : ViewGroup() {
	initScrollView();
}

ScrollView::ScrollView(GCContext *context) : ViewGroup(context) {
	initScrollView();
}

ScrollView::~ScrollView() {
	DELETE(scrollAnim);
	listener = NULL;
}

//////////////////////////////////////////////////////////////
// Viewからの継承
//////////////////////////////////////////////////////////////

bool ScrollView::onTouch(TouchEvent &event) {
	if (!clickable || !visible) {
		return false;
	}
	// viewsが空のときは何もしない
	if (views.empty()) {
		return false;
	}
	// カレントのviewがない場合は何もしない
	int size = views.size();
	if (currentIndex < 0 || currentIndex >= size) {
		return false;
	}

	int type = event.type;

	// ScrollViewの範囲外の処理
	if (!isBound(event.x, event.y)) {
		if (!isTouch) {
			// タッチされていない場合には何もしない
			return false;
		}
		// タッチされていた場合には、タッチイベントキャンセル
		// してスクロールを一旦終了する
		type = touchCancel;
	}

	float dx = 0.0;
	float dy = 0.0;
	switch (type) {
	case touchDown:
		// タッチの開始位置を保持
		startPos.x = event.x;
		startPos.y = event.y;
		currentPos.x = event.x;
		currentPos.y = event.y;

		// タッチイベントの履歴保持
		posCount = 0;
		scrPos[posCount].x = event.x;
		scrPos[posCount].y = event.y;
		startTime[posCount] = event.time;
		sp = 0.0;
		
		touchState = TOUCH_START;
		break;
	case touchMove:
			if (touchState == TOUCH_NONE) {
				startPos.x = event.x;
				startPos.y = event.y;
				currentPos.x = event.x;
				currentPos.y = event.y;
				
				// タッチイベントの履歴保持
				posCount = 0;
				scrPos[posCount].x = event.x;
				scrPos[posCount].y = event.y;
				startTime[posCount] = event.time;
				sp = 0.0;
				
				touchState = TOUCH_START;
			}

		currentPos.x = event.x;
		currentPos.y = event.y;

		// タッチイベントの履歴保持
		if (posCount < POS_HISTROY - 1) {
			posCount++;
		} else {
			for (int i = 1; i < POS_HISTROY; i++) {
				scrPos[i - 1].x = scrPos[i].x;
				scrPos[i - 1].y = scrPos[i].y;
				startTime[i - 1] = startTime[i];
			}
		}
		scrPos[posCount].x = event.x;
		scrPos[posCount].y = event.y;
		startTime[posCount] = event.time;

		// ある程度スライドされた場合は、スライドモードに切り替える
		switch (mode) {
		case ScrollMode_Horizon:
			dx = currentPos.x - startPos.x;
			if (dx * dx > 0.03) {
				if (listener) {
					if (!isTouch) {
						listener->onScrollStart(this);
					} else {
						listener->onScroll(this);
					}
				}
				isTouch = true;
			}
			break;
		case ScrollMode_Vertical:
			dy = currentPos.y - startPos.y;
			if (dy * dy > 0.03) {
				if (listener) {
					if (!isTouch) {
						listener->onScrollStart(this);
					} else {
						listener->onScroll(this);
					}
				}
				isTouch = true;
			}
			break;
		case ScrollMode_None:
		default:
			dx = currentPos.x - startPos.x;
			dy = currentPos.y - startPos.y;
			if (dy * dy > 0.04) {
				if (listener) {
					if (!isTouch) {
						listener->onScrollStart(this);
					} else {
						listener->onScroll(this);
					}
				}
				isTouch = true;
			}
			break;
		}
		return true;
	case touchCancel:
	case touchUp:
		touchState = TOUCH_NONE;

		currentPos.x = event.x;
		currentPos.y = event.y;
		if (isTouch) {
			isTouch = false;

			if (pageScroll) {
				switch (mode) {
				case ScrollMode_Horizon:
					onTouchPageScrollHorizon(event);
					break;
				case ScrollMode_Vertical:
					onTouchPageScrollVertical(event);
					break;
				}
			} else {
				switch (mode) {
				case ScrollMode_Horizon:
					onTouchScrollHorizon(event);
					break;
				case ScrollMode_Vertical:
					onTouchScrollVertical(event);
					break;
				}
			}
			return true;
		}
		break;
	}

	if (pageScroll) {
		// カレントのviewでのタッチイベント
		if (currentIndex == nextIndex && views[currentIndex]->onTouch(event)) {
			// カレントのviewでタッチイベントで、何か処理があった
			return true;
		}
	} else {
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
	}

	return false;
}

void ScrollView::draw(double dt, ViewContext *context) {
	if (views.empty()) {
		// 表示するviewが無い場合は何もしない
		return;
	}

	if (pageScroll) {
		switch (mode) {
		case ScrollMode_Horizon:
			drawPageScrollHorizon(dt, context);
			break;
		case ScrollMode_Vertical:
			drawPageScrollVertical(dt, context);
			break;
		}
	} else {
		switch (mode) {
		case ScrollMode_Horizon:
			drawScrollHorizon(dt, context);
			break;
		case ScrollMode_Vertical:
			drawScrollVertical(dt, context);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////
// privateな関数
//////////////////////////////////////////////////////////////

void ScrollView::initScrollView() {
	listener = NULL;
	currentIndex = 0;
	nextIndex = 0;
	isTouch = false;
	scrollAnim = NULL;
	mode = ScrollMode_Horizon;
	pageScroll = true;
	startPos.x = 0.0;
	startPos.y = 0.0;
	currentPos.x = 0.0;
	currentPos.y = 0.0;
	scrollPos.x = 0.0;
	scrollPos.y = 0.0;
	sp = 0.0;
	touchState = TOUCH_NONE;
}

void ScrollView::onTouchScrollVertical(TouchEvent &event) {
	float dy = currentPos.y - scrPos[0].y;

	// スクロール量を計算
	scrollPos.y += currentPos.y - startPos.y;

	// 初期化
	startPos.x = 0.0;
	startPos.y = 0.0;
	currentPos.x = 0.0;
	currentPos.y = 0.0;

	// 慣性の速度計算
	float delta = (event.time - startTime[0]) / 50.0;
	if (delta != 0.0) {
		sp = dy / delta;
	}
}

void ScrollView::onTouchPageScrollVertical(TouchEvent &event) {
	nextIndex = currentIndex;
	float dy = currentPos.y - startPos.y;
	if (dy > 0.8) {
		if (currentIndex + 1 < views.size()) {
			nextIndex = currentIndex + 1;
		}
	} else if (dy < -0.8) {
		if (currentIndex - 1 >= 0) {
			nextIndex = currentIndex - 1;
		}
	}
	createScrollAnimation(nextIndex);
}

void ScrollView::onTouchScrollHorizon(TouchEvent &event) {
	float dx = currentPos.x - scrPos[0].x;

	// スクロール量を計算
	scrollPos.x += currentPos.x - startPos.x;;

	// 初期化
	startPos.x = 0.0;
	startPos.y = 0.0;
	currentPos.x = 0.0;
	currentPos.y = 0.0;

	// 慣性の速度計算
	float delta = (event.time - startTime[0]) / 50.0;
	if (delta != 0.0) {
		sp = dx / delta;
	}
}

void ScrollView::onTouchPageScrollHorizon(TouchEvent &event) {
	nextIndex = currentIndex;
	float dx = currentPos.x - startPos.x;
	if (dx > 0.8) {
		if (currentIndex - 1 >= 0) {
			nextIndex = currentIndex - 1;
		}
	} else if (dx < -0.8) {
		if (currentIndex + 1 < views.size()) {
			nextIndex = currentIndex + 1;
		}
	}
	createScrollAnimation(nextIndex);
}

void ScrollView::createScrollAnimation(int nextIndex) {
	float sx = point.x;
	float sy = point.y;
	float ex = point.x;
	float ey = point.y;
	float delta = 1.0;

	if (pageScroll) {
		switch (mode) {
		case ScrollMode_Horizon:
			sx = currentPos.x - startPos.x;
			sy = 0;
			ex = 0;
			ey = 0;

			if (nextIndex > currentIndex){
				ex = - 2.0 * this->size.x;
			} else if (nextIndex < currentIndex) {
				ex = + 2.0 * this->size.x;
			}
			break;
		case ScrollMode_Vertical:
			sx = 0;
			sy = currentPos.y - startPos.y;
			ex = 0;
			ey = 0;

			if (nextIndex > currentIndex){
				ey = + 2.0 * this->size.y;
			} else if (nextIndex < currentIndex) {
				ey = - 2.0 * this->size.y;
			}
			break;
		}
	} else {
		switch (mode) {
		case ScrollMode_Horizon:
		{
			sx = scrollPos.x;
			sy = 0;
			ex = scrollPos.x;
			ey = 0;

			// 子Viewのサイズを計算
			float xsize = 0.0;
			for (int i = 0; i < views.size(); i++) {
				xsize += views[i]->size.x * 2.0;
			}

			// 子Viewのサイズを超えていた場合
			if (xsize < size.x * 2.0) {
				float a = size.x - views[0]->size.x;
				if (scrollPos.x > a) {
					ex = a;
				}
			} else {
				float b = views[views.size() - 1]->size.x;
				float c = -xsize + size.x + b;
				if (scrollPos.x < c) {
					ex = c;
				}
			}

			// 子Viewよりも小さいとき
			float a = views[0]->size.x - size.x;
			if (scrollPos.x > a) {
				ex = a;
			}

			delta = fabs(ex - sx);

			// 開始位置と終了位置が同じ場合は、アニメーションをさせない
			if (sx == ex) {
				return;
			}
		}
			break;
		case ScrollMode_Vertical:
			{
				sx = 0;
				sy = scrollPos.y;
				ex = 0;
				ey = scrollPos.y;

				// 子Viewのサイズを計算
				float ysize = 0.0;
				for (int i = 0; i < views.size(); i++) {
					ysize += views[i]->size.y * 2.0;
				}

				// 子Viewのサイズを超えていた場合
				if (ysize < size.y * 2.0) {
					float a = size.y - views[0]->size.y;
					if (scrollPos.y > a) {
						ey = a;
					}
				} else {
					float b = views[0]->size.y;
					float c = ysize - size.y - b;
					if (scrollPos.y > c) {
						ey = c;
					}
				}

				// 子Viewよりも小さいとき
				float a = size.y - views[0]->size.y;
				if (scrollPos.y < a) {
					ey = a;
				}

				delta = fabs(ey - sy);

				// 開始位置と終了位置が同じ場合は、アニメーションをさせない
				if (sy == ey) {
					return;
				}
			}
			break;
		}
	}

	DELETE(scrollAnim);
	scrollAnim = new Animation();
	scrollAnim->setMove(sx, sy, ex, ey);
	scrollAnim->setDuration(0.20 * delta);
	scrollAnim->reset();
}

void ScrollView::drawScrollHorizon(double dt, ViewContext *context) {
	float dx = 0;
	if (isTouch) {
		// 画面がタッチされている場合
		dx = currentPos.x - startPos.x;
	}

	float x = 0;
	float y = 0;

	if (fabs(sp) > 0.001) {
		scrollPos.x += sp;
		if (isOverrun()) {
			sp *= OVERRUN_DAMPING_RATE;
		} else {
			sp *= DAMPING_RATE;
		}
	} else {
		sp = 0.0;
		if (!scrollAnim) {
			createScrollAnimation(0);
		}
	}

	if (scrollAnim) {
		scrollAnim->step(dt);
		scrollPos.x = scrollAnim->x;
		if (scrollAnim->isFinish()) {
			DELETE(scrollAnim);
			// スクロールが終わったことを通知
			if (listener) {
				listener->onScrollEnd(this);
			}
		} else {
			if (listener) {
				listener->onScroll(this);
			}
		}
	}

	x += scrollPos.x + dx;
	y += scrollPos.y;

	float left = -this->size.x * RENDER_RANGE;
	float top = this->size.y * RENDER_RANGE;
	float right = this->size.x * RENDER_RANGE;
	float bottom = -this->size.y * RENDER_RANGE;
	int size = views.size();
	for (int i = 0; i < size; i++) {
		views[i]->setPosition(x, y);
		// 範囲内にあるViewのみを描画するようにする
		if (left < x + views[i]->size.x && right > x - views[i]->size.x) {
			if (top > y - views[i]->size.y && bottom < y + views[i]->size.y) {
				views[i]->render(dt, context);
			}
		}
		x += views[i]->size.x * 2.0;
	}
}

void ScrollView::drawPageScrollHorizon(double dt, ViewContext *context) {
	int size = views.size();
	if (currentIndex < 0 || currentIndex >= size) {
		// カレントになっているviewが存在しない場合は何もしない
		return;
	}

	float dx = 0;
	if (isTouch) {
		// 画面がタッチされている場合
		dx = currentPos.x - startPos.x;
	}

	float x = 0;
	float y = 0;
	if(!isTouch && scrollAnim) {
		scrollAnim->step(dt);
		x = scrollAnim->x;
		if (!scrollAnim->isFinish()) {
			if (listener) {
				listener->onScroll(this);
			}
		}
	}
	views[currentIndex]->setPosition(x + dx, y);
	views[currentIndex]->render(dt, context);

	if (currentIndex - 1 >= 0) {
		float cx = x - 2.0 * this->size.x + dx;
		views[currentIndex - 1]->setPosition(cx, y);
		views[currentIndex - 1]->render(dt, context);
	}

	if (currentIndex + 1 < size) {
		float cx = x + 2.0 * this->size.x + dx;
		views[currentIndex + 1]->setPosition(cx, y);
		views[currentIndex + 1]->render(dt, context);
	}

	if (scrollAnim) {
		if (scrollAnim->isFinish()) {
			currentIndex = nextIndex;
			DELETE(scrollAnim);
			// スクロールが終わったことを通知
			if (listener) {
				listener->onScrollEnd(this);
			}
		}
	}
}

void ScrollView::drawScrollVertical(double dt, ViewContext *context) {
	float dy = 0;
	if (isTouch) {
		// 画面がタッチされている場合
		dy = currentPos.y - startPos.y;
	}

	float x = 0;
	float y = 0;

	if (fabs(sp) > 0.001) {
		scrollPos.y += sp;
		if (isOverrun()) {
			sp *= OVERRUN_DAMPING_RATE;
		} else {
			sp *= DAMPING_RATE;
		}
	} else {
		sp = 0.0;
		if (!scrollAnim) {
			createScrollAnimation(0);
		}
	}

	if (scrollAnim) {
		scrollAnim->step(dt);
		scrollPos.y = scrollAnim->y;
		if (scrollAnim->isFinish()) {
			DELETE(scrollAnim);
			// スクロールが終わったことを通知
			if (listener) {
				listener->onScrollEnd(this);
			}
		} else {
			if (listener) {
				listener->onScroll(this);
			}
		}
	}

	x += scrollPos.x;
	y += scrollPos.y + dy;

	float left = -this->size.x * RENDER_RANGE;
	float top = this->size.y * RENDER_RANGE;
	float right = this->size.x * RENDER_RANGE;
	float bottom = -this->size.y * RENDER_RANGE;
	int size = views.size();
	for (int i = 0; i < size; i++) {
		if (i > 0) {
			y -= views[i]->size.y;
		}
		views[i]->setPosition(x, y);
		// 範囲内にあるViewのみを描画するようにする
		if (left < x + views[i]->size.x && right > x - views[i]->size.x) {
			if (top > y - views[i]->size.y && bottom < y + views[i]->size.y) {
				views[i]->render(dt, context);
			}
		}
		y -= views[i]->size.y;
	}
}

void ScrollView::drawPageScrollVertical(double dt, ViewContext *context) {
	int size = views.size();
	if (currentIndex < 0 || currentIndex >= size) {
		// カレントになっているviewが存在しない場合は何もしない
		return;
	}

	float dy = 0;
	if (isTouch) {
		// 画面がタッチされている場合
		dy = currentPos.y - startPos.y;
	}

	float x = 0;
	float y = 0;
	if(!isTouch && scrollAnim) {
		scrollAnim->step(dt);
		y = scrollAnim->y;
		if (!scrollAnim->isFinish()) {
			if (listener) {
				listener->onScroll(this);
			}
		}
	}
	views[currentIndex]->setPosition(x, y + dy);
	views[currentIndex]->render(dt,context);

	if (currentIndex - 1 >= 0) {
		float cy = y + 2.0 * this->size.y + dy;
		views[currentIndex - 1]->setPosition(x, cy);
		views[currentIndex - 1]->render(dt, context);
	}

	if (currentIndex + 1 < size) {
		float cy = y - 2.0 * this->size.y + dy;
		views[currentIndex + 1]->setPosition(x, cy);
		views[currentIndex + 1]->render(dt, context);
	}

	if (scrollAnim) {
		if (scrollAnim->isFinish()) {
			currentIndex = nextIndex;
			DELETE(scrollAnim);
			// スクロールが終わったことを通知
			if (listener) {
				listener->onScrollEnd(this);
			}
		}
	}
}

bool ScrollView::isOverrun() {
	switch (mode) {
	case ScrollMode_Horizon:
	{
		// 子Viewのサイズを計算
		float b = views[views.size() - 1]->size.x;
		float xsize = 0.0;
		for (int i = 0; i < views.size(); i++) {
			xsize += views[i]->size.x * 2.0;
		}

		// 子Viewのサイズを超えていた場合
		float c = -xsize + size.x + b;
		if (scrollPos.x < c) {
			return true;
		}

		// 子Viewよりも小さいとき
		float a = views[0]->size.x - size.x;
		if (scrollPos.x > a) {
			return true;
		}
	}
		break;
	case ScrollMode_Vertical:
	{
		// 子Viewのサイズを計算
		float b = views[views.size() - 1]->size.y;
		float ysize = 0.0;
		for (int i = 0; i < views.size(); i++) {
			ysize += views[i]->size.y * 2.0;
		}

		// 子Viewのサイズを超えていた場合
		float c = ysize - size.y - b;
		if (scrollPos.y > c) {
			return true;
		}

		// 子Viewよりも小さいとき
		float a = - views[0]->size.y + size.y;
		if (scrollPos.y < a) {
			return true;
		}
	}
		break;
	}
	return false;
}

//////////////////////////////////////////////////////////////
// publicな関数
//////////////////////////////////////////////////////////////

void ScrollView::setOnScrollEventListener(OnScrollEventListener *listener) {
	this->listener = listener;
}

int ScrollView::getCurrentPage() {
	return currentIndex;
}

void ScrollView::setCurrentPage(int page, bool scroll) {
	if (!pageScroll) {
		return;
	}

	if (page < 0 || page > views.size() - 1) {
		return;
	}

	currentPos.x = 0;
	currentPos.y = 0;
	startPos.x = 0;
	startPos.y = 0;

	if (scroll) {
		// スクロールフラグがtrueの場合には、アニメーションを行う
		createScrollAnimation(page);
		nextIndex = page;
	} else {
		scrollPos.x = point.x;
		scrollPos.y = point.y;
		currentIndex = nextIndex = page;
	}
}

void ScrollView::setScrollMode(int mode) {
	this->mode = mode;
}

int ScrollView::getScrollMode() {
	return mode;
}

void ScrollView::setPageScroll(bool flag) {
	pageScroll = flag;
}

bool ScrollView::isPageScroll() {
	return pageScroll;
}

void ScrollView::scrollLeft() {
	if (nextIndex != currentIndex) {

	} else if (currentIndex - 1 >= 0) {
		startPos.x = 0;
		currentPos.x = 0;
		nextIndex = currentIndex - 1;
		createScrollAnimation(nextIndex);
		scrollAnim->setDuration(0.3);
		if (listener) {
			listener->onScrollStart(this);
		}
	}
}

void ScrollView::scrollRight() {
	if (nextIndex != currentIndex) {

	} else if (currentIndex + 1 < views.size()) {
		startPos.x = 0;
		currentPos.x = 0;
		nextIndex = currentIndex + 1;
		createScrollAnimation(nextIndex);
		scrollAnim->setDuration(0.3);
		if (listener) {
			listener->onScrollStart(this);
		}
	}
}

void ScrollView::setScrollTop() {
	if (pageScroll) {
	} else {
		switch (mode) {
		case ScrollMode_Horizon: {
			float a = size.x - views[0]->size.x;
			scrollPos.x = a;
		}	break;
		case ScrollMode_Vertical: {
			float a = size.y - views[0]->size.y;
			scrollPos.y = a;
		}	break;
		}
	}
}

