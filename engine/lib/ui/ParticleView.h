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
 * ParticleView.h
 *
 *  Created on: 2011/08/16
 *      Author: GClue, Inc.
 */
#ifndef PARTICLEVIEW_H_
#define PARTICLEVIEW_H_

#include "View.h"

#include <vector>

class IAnimation;
class Figure;

class ParticleView : public View {
private:
	Figure *figure;		//!< 画像を表示するためのフィギュア
	int emitter;		//!< 発生タイプ
	float life;			//!< パーティクルの寿命

public:
	/**
	 * コンストラクタ.
	 * @param[in] context View用のコンテキスト
	 */
	ParticleView(AEContext *context);

	/**
	 * デストラクタ.
	 */
	virtual ~ParticleView();

	/**
	 * フィギュアを設定します.
	 * @param figure 表示するフィギュア
	 */
	void setFigure(Figure *figure);

	/////////////////////////////////////////////////////////////////
	// Viewからの継承
	/////////////////////////////////////////////////////////////////

	/**
	 * 指定されたアニメーションを反映して描画を行います.
	 * @param[in] dt 前回描画からの差分時間
	 * @param[in] animation 反映するアニメーション
	 */
	virtual void draw(double dt, IAnimation *animation = NULL);
};

#endif /* PARTICLEVIEW_H_ */
