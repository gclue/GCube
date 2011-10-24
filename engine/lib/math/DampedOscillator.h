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
 * DampedOscillator.h
 *
 *  Created on: 2011/04/27
 *      Author: haga
 */

#ifndef DAMPEDOSCILLATOR_H_
#define DAMPEDOSCILLATOR_H_

#include <math.h>
#include "Oscillator.h"


/**
 * 減衰振動を管理するクラス.
 * <br><br>
 * Oscillatorクラスを継承します.
 */
class DampedOscillator: public Oscillator{

public:
	/**
	 * コンストラクタ.
	 */
	DampedOscillator();
	/**
	 * デストラクタ.
	 */
	~DampedOscillator();

	float ak;	//!	初期振動係数
	float h;	//!	減衰係数 h < 1 で減衰
	float alpha;	//!	初期位相
	float n;	//!	係数
	float doTime;	//!	時間

	/**
	 * 各種係数を設定します.
	 * @param ak	初期振動係数
	 * @param h	減衰係数
	 * @param n	係数
	 * @param alpha	初期位相
	 */
	void setParameters(float ak, float h, float n, float alpha);

	/**
	 * 振動値の取得.
	 * @return 振動値.
	 */
	float dampedOscillate();
	/**
	 * 振動値の取得.
	 * @param t 時間
	 * @return	振動値.
	 */
	float dampedOscillate(float t);


};

#endif /* DAMPEDOSCILLATOR_H_ */
