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
 * ImageButton.h
 *
 *  Created on: 2011/08/09
 *      Author: GClue, Inc.
 */
#ifndef IMAGEBUTTON_H_
#define IMAGEBUTTON_H_

#include "View.h"
#include "Button.h"

class Figure;

/**
 * ImageViewをボタンとするクラス.
 */
class ImageButton : public Button {
protected:
	View *view[2];						//!< ボタンの画像

public:
	/**
	 * コンストラクタ.
	 */
	ImageButton();
	
	/**
	 * コンストラクタ.
	 * @param[in] context View用のコンテキスト
	 * @deprecated コンテキストを使用しないように変更したので、コンストラクタで設定する必要がない
	 */
	ImageButton(GCContext *context);

	/**
	 * デストラクタ.
	 */
	virtual ~ImageButton();

	/////////////////////////////////////////////////////////////////
	// Viewからの継承
	/////////////////////////////////////////////////////////////////

	/**
	 * タッチイベントを通知します.
	 * @param[in] event タッチイベント
	 * @retval true 次へイベントを渡さない
	 * @retval false 次へイベントを渡す
	 */
	virtual bool onTouch(TouchEvent &event);

	/**
	 * 指定されたアニメーションを反映して描画を行います.
	 * @param[in] dt 前回描画からの差分時間
	 * @param[in] animation 反映するアニメーション
	 */
	virtual void draw(double dt, ViewContext *context);

	/////////////////////////////////////////////////////////////////
	// public
	/////////////////////////////////////////////////////////////////

	/**
	 * 通常時のボタンに表示するViewを設定します.
	 * @param view 表示するView
	 */
	virtual void setView(View *view);

	/**
	 * 選択時のボタンに表示するViewを設定します.
	 * @param view 表示するView
	 */
	virtual void setFocusView(View *view);

	/**
	 * ボタンのクリックアニメーションを開始します.
	 * @param[in] a アニメーション
	 */
	virtual void startClickAnimation(IAnimation *a = NULL);
};

#endif /* IMAGEBUTTON_H_ */
