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
 * Layer.h
 *
 *  Created on: 2011/09/15
 *      Author: GClue, Inc.
 */
#ifndef LAYER_H_
#define LAYER_H_

#include "View.h"

/**
 * レイヤーの基底クラス.
 */
class Layer {
protected:
	GCContext *context;
	bool visible;
	bool touchable;

public:
	bool pause;
	
	Layer() {
		pause=false;
		visible=true;
		touchable=true;
	}
	
	Layer(GCContext *context) {
		this->context=context;
		pause=false;
		visible=true;
		touchable=true;
	}
	virtual ~Layer() {};

	virtual bool isVisible() {
		return visible;
	}
	
	virtual void setVisible(bool visible) {
		this->visible = visible;
	}
	
	virtual bool isTouchable() {
		return touchable;
	}
	
	virtual void setTouchable(bool touchable) {
		this->touchable = touchable;
	}
	
	/**
	 * セットアップを行います.
	 */
	virtual void setup() = 0;

	/**
	 * 画面のリサイズを行います.
	 * @param aspect 画面のアスペクト比
	 */
	virtual void resize(float aspect) = 0;

	/**
	 * 画面の描画を行います.
	 * @param dt 前回描画からの差分時間
	 */
	virtual void render(double dt = 0.033) = 0;

	/**
	 * タッチイベント.
	 * @param event タッチイベント
	 * @return true: 次のレイヤーにイベントを渡さない、false: 次のレイヤーにイベントを渡す
	 */
	virtual bool onTouchEvent(TouchEvent &event) {
		return false;
	};
	
	virtual bool onSensorEvent(SensorEvent &event) {
		return false;
	};
	
	/**
	 * コンテキストが切り替わったことを通知します.
	 */
	virtual void onContextChanged() {};
	
};

#endif /* LAYER_H_ */
