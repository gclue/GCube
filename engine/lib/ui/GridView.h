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
 * GridView.h
 *
 *  Created on: 2011/08/08
 *      Author: GClue, Inc.
 */

#ifndef GRIDVIEW_H_
#define GRIDVIEW_H_

#include "ViewGroup.h"

/**
 * グリッド上にViewを表示するクラス.
 */
class GridView : public ViewGroup {
public:
	int row;			//!< 行数
	int column;			//!< 列数
	float aspect;		//!< アスペクト比

	/**
	 * コンストラクタ.
	 * @param[in] context View用のコンテキスト
	 * @param[in] row 行数
	 * @param[in] column 列数
	 */
	GridView(GCContext *context, int row = 4, int column = 4);

	/**
	 * デストラクタ.
	 */
	virtual ~GridView();

	//////////////////////////////////////////////////////////////
	// Viewからの継承
	//////////////////////////////////////////////////////////////

	/**
	 * 指定されたアニメーションを反映して描画を行います.
	 * @param[in] dt 前回描画からの差分時間
	 * @param[in] animation 反映するアニメーション
	 */
	virtual void draw(double dt, ViewContext *context);
};

#endif /* GRIDVIEW_H_ */
