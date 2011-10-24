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
 * Button.h
 *
 *  Created on: 2011/07/14
 *      Author: GClue, Inc.
 */
#ifndef BUTTON_H_
#define BUTTON_H_

#include "defines.h"
#include "View.h"

class Animation;
class AnimationSet;
class Button;
class Camera;
class Figure;
class SimpleShader;

/**
 * ボタンがクリックされたときのイベントを通知するリスナー.
 */
class OnButtonClickListener {
public:
	virtual ~OnButtonClickListener() {}
	/**
	 * ボタンがクリックされたイベント.
	 * @param[in] btn クリックされたボタン
	 */
	virtual void onButtonClick(Button *btn) = 0;
	/**
	 * ボタンがタッチされたときに呼び出されます.
	 * @param[in] btn タッチされたボタン
	 */
	virtual void onButtonTouchStart(Button *btn) = 0;
};


/**
 * ボタンを表示するためのクラス.
 */
class Button : public View {
protected:
	OnButtonClickListener *listener;	//!< リスナー
	AnimationSet *clickAnim;			//!< クリックアニメーション
	Figure *figure[2];					//!< ボタンの画像(0: 通常用ボタン, 1: 押下用ボタン)

public:
	/**
	 * コンストラクタ.
	 * @param context コンテキスト
	 */
	Button(AEContext *context);

	/**
	 * デストラクタ.
	 */
	virtual ~Button();

	/////////////////////////////////////////////////////////////////
	// public関数
	/////////////////////////////////////////////////////////////////

	/**
	 * ボタンのクリックアニメーションを開始します.
	 */
	virtual void startClickAnimation();

	/**
	 * クリックアニメーション中かチェックします.
	 * @retval true アニメーション中
	 * @retval false 上記以外
	 */
	virtual bool isClickAnimation();

	/**
	 * リスナーを設定します.
	 * @param[in] l 設定するリスナー
	 */
	void setOnButtonClickListener(OnButtonClickListener *l);

	/**
	 * Figureを設定します.
	 * @param[in] figure 表示するフィギュア
	 */
	void setFigure(Figure *figure);

	/**
	 * 選択時Figureを設定します.
	 * @param[in] figure 表示するフィギュア
	 */
	void setSelectFigure(Figure *figure);

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
	 * ボタンを指定されたアニメーションを反映して描画を行います.
	 * @param[in] dt 前回描画からの差分時間
	 * @param[in] animation 反映するアニメーション
	 */
	virtual void draw(double dt, IAnimation *animation = NULL);
};

#endif /* BUTTON_H_ */
