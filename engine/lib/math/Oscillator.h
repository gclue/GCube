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
 * Oscilator.h
 *
 *  Created on: 2011/04/27
 *      Author: haga
 */

#ifndef OSCILATOR_H_
#define OSCILATOR_H_

#include <math.h>

/**
 * 振動子のクラス.
 */
class Oscillator {

public:
	/**
	 * コンストラクタ.
	 */
	Oscillator();
	/**
	 * デストラクタ.
	 */
	virtual ~Oscillator();

	float thisTime; //!	現在時間.
	float amplitude; //!	振幅係数
	float period; //! 周期
	float phase; //! 位相
	float dynamicAmpAmp; //! 動的な振幅を取得するための振幅.
	const static float timeStep;// = 0.1; //! 実時間でないタイムステップ.

	/**
	 * Sin波の取得.
	 * @param t 時間.
	 * @return 時間に対するSin値
	 */
	float sinOscillate(float t);

	/**
	 * Cos波の取得.
	 * @param t 時間
	 * @return 時間に対するCos値.
	 */
	float cosOscillate(float t);

	/**
	 * Sin波の取得
	 * @return 現在時間(thisTime)にあわせたSin値.
	 */
	float sinOscillate();

	/**
	 * Cos波の取得
	 * @return 現在時間(thisTime)にあわせたCos値.
	 */
	float cosOscillate();

	/**
	 * 0 ~ PI の周期での 0 ~ size の indexをとるX座標にあわせたSin波の取得.
	 * @param size 0~PIを何分割するか.
	 * @param index 分割された座標のインデックス.
	 * @return 値.
	 */
	float getDynamicSinAmplitude(int size, int index);


	/**
	 * 0 ~ PI の周期での 0 ~ size の indexをとるX座標にあわせたCos波の取得.
	 * @param size 0~PIを何分割するか.
	 * @param index 分割された座標のインデックス.
	 * @return 値.
	 */
	float getDynamicCosAmplitude(int size, int index);

	/**
	 * タイムステップをstepだけ進めます.
	 * @param step タイムステップ.
	 */
	void stepTime(float step);

	/**
	 * タイムステップを進めます.
	 */
	void stepTime();

	/**
	 * 振幅を設定します.
	 * @param amp 振幅.
	 */
	void setAmplitude(float amp);

	/**
	 * 周期を設定します.
	 * @param period 周期.
	 */
	void setPeriod(float period);

	/**
	 * 位相を設定します.
	 * @param phase 位相.
	 */
	void setPhase(float phase);

};

#endif /* OSCILATOR_H_ */
