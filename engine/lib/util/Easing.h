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

#ifndef __EASING_H__
#define __EASING_H__

/**
 * easing関数の宣言.
 * @param[in] time 現在時刻 (ミリ秒)
 * @param[in] begin 最初の値
 * @param[in] delta 移動量
 * @param[in] duration 何秒かけて動くか (ミリ秒)
 * @return 補間値
 */
typedef float (*EASING)(float time, float begin, float delta, float duration);

/**
 * @enum Easingのタイプを定義します.
 */
enum {
	EASING_LINEAR = 0,
	EASING_IN_BOUNCE,
	EASING_OUT_BOUNCE,
	EASING_IN_OUT_BOUNCE,
	EASING_IN_BACK,
	EASING_OUT_BACK,
	EASING_IN_OUT_BACK,
	EASING_IN_CUBIC,
	EASING_OUT_CUBIC,
	EASING_IN_OUT_CUBIC,
	EASING_IN_EXPO,
	EASING_OUT_EXPO,
	EASING_IN_OUT_EXPO,
	EASING_IN_QUAD,
	EASING_OUT_QUAD,
	EASING_IN_OUT_QUAD,
	EASING_IN_QUART,
	EASING_OUT_QUART,
	EASING_IN_OUT_QUART,
	EASING_IN_QUINT,
	EASING_OUT_QUINT,
	EASING_IN_OUT_QUINT,
	EASING_IN_SINE,
	EASING_OUT_SINE,
	EASING_IN_OUT_SINE,
};

/**
 * Easingを管理するためのクラス.
 */
class Easing {
private:
	EASING ease;		//!< easing関数

public:
	float begin;		//!< 初期値.
	float delta;		//!< 変化量.
	float duration;		//!< 変化時間.

	/**
	 * コンストラクタ.
	 */
	Easing(int type);

	/**
	 * デストラクタ.
	 */
	virtual ~Easing();

	/**
	 * 指定された t 分だけ移動した値を取得します.
	 * @param[in] t 経過時間
	 * @return tの分だけ経過した後の値
	 */
	float easing(double t);
};

#endif	// end of __EASING_H__
