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
 * NumberView.h
 *
 *  Created on: 2011/08/15
 *      Author: GClue, Inc.
 */
#ifndef NUMBERVIEW_H_
#define NUMBERVIEW_H_

#include "View.h"

/**
 * アラインの定義.
 */
enum {
	ALIGN_LEFT = 0,		//!< 左詰め
	ALIGN_RIGHT,		//!< 右詰め
};

/**
 * 数値を表示するためのView.
 */
class NumberView : public View {
private:
	Figure *figure[15];		//!< 0〜9の文字列
	long long value;		//!< 表示する数値
	int digit;				//!< 表示する桁
	int aligned;			//!< アライン
	bool zeroFill;			//!< ゼロ埋めフラグ(true: ゼロで埋める
	float w;
	float h;
	
	float adjustX[10];
	float adjustY[10];
	float space;

	void initNumberView();
	void drawFigure(ViewContext *context, int index, float x, float y);

public:
	/**
	 * コンストラクタ.
	 */
	NumberView();
	
	/**
	 * コンストラクタ.
	 * @param context View用のコンテキスト
	 * @deprecated コンテキストを使用しないように変更したので、コンストラクタで設定する必要がない
	 */
	NumberView(GCContext *context);

	/**
	 * デストラクタ.
	 */
	virtual ~NumberView();

	/**
	 * 表示する数値を設定します.
	 * @param[in] value 数値
	 */
	void setValue(long long value);

	/**
	 * 表示している数値を取得します.
	 * @return 数値
	 */
	int getValue();

	/**
	 * 表示する桁数を設定します.
	 * @param[in] digit 桁数
	 */
	void setDigit(int digit);

	/**
	 * 表示する桁数を取得します.
	 * @return 桁数
	 */
	int getDigit();

	/**
	 * ゼロ埋めフラグを設定します.
	 * true のときは、足りない桁数をすべて0で埋めます.
	 * @param[in] flag フラグ
	 */
	void setZeroFill(bool flag);

	/**
	 * ゼロ埋めフラグの設定を取得します.
	 * @return ゼロ埋めフラグ
	 */
	bool isZeroFill();

	/**
	 * アラインを設定します.
	 * @param[in] aligned アライン
	 */
	void setAlign(int aligned);

	/**
	 * 設定されているアラインを取得します.
	 * @return アライン
	 */
	int getAlign();

	/**
	 * 指定された数値のフィギュアを設定します.
	 * @param[in] index インデックス
	 * @param[in] figure フィギュア
	 */
	void setFigure(int index, Figure *figure);

	/**
	 * 指定された数値のFigureを設定します.
	 * @param[in] index インデックス
	 * @param[in] fname 数値のテクスチャ名
	 */
	void setFigure(int index, const char *fname);
	
	/**
	 * 指定されたテクスチャ名の配列でFigureを設定します.
	 *
	 * テクスチャは、0〜9の10個のテクスチャ名の配列になっていない場合は
	 * 動作不定になりますので、ご注意ください。
	 *
	 * @param[in] fname テクスチャ名の配列
	 */
	void setFigure(const char *fname[]);
	
	/**
	 * 数値のFigureの大きさを設定します.
	 * @param w 横幅
	 * @param h 縦幅
	 */
	void setFontSize(float w, float h);
	
	/**
	 * 数値と数値の間のスペースサイズを設定します。
	 * デフォルトでは0に設定されています。
	 * @param[in] space スペースサイズ
	 */
	void setSpace(float space);
	
	/**
	 * 指定された数値の位置を調整します。
	 * @param[in] index 数値
	 * @param[in] x 移動するサイズ
	 * @param[in] y 移動するサイズ
	 */
	void setAdjustPositionEachNumbers(int index, float x, float y);

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

#endif /* NUMBERVIEW_H_ */
