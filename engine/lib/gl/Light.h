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

#ifndef CoinPusher_Light_h
#define CoinPusher_Light_h

#include "Vector3D.h"

/**
 * ライト.
 */
class Light {
public:
	Vector3D position; //!< 位置
	
	/**
	 * コンストラクタ.
	 */
	Light();
	
	/**
	 * デストラクタ.
	 */
	~Light();
	
	/**
	 * ライトのx座標を取得します.
	 * @return x座標
	 */
	float getX();
	
	/**
	 * ライトのy座標を取得します.
	 * @return y座標
	 */
	float getY();
	
	/**
	 * ライトのz座標を取得します.
	 * @return z座標
	 */
	float getZ();
	
	/**
	 * ライトの位置を設定します.
	 * @param[in] p 座標
	 */
	void setPosition(Vector3D &p);
	
	/**
	 * ライトの位置を設定します.
	 * @param[in] x x座標
	 * @param[in] y y座標
	 * @param[in] z z座標
	 */
	void setPosition(float x, float y, float z);
};


#endif
