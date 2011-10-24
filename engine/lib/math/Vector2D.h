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

#ifndef MATH_VECTOR2D_H
#define MATH_VECTOR2D_H

/**
 * 2次元のベクトルクラス.
 * <br><br>
 *
 * 足し算や引き算などのオペレータを Overwrite しないで関数として用意しています。
 * Object-C のクラスを移植しているので、元々のソースから変更点が少なくするためです。
 */
class Vector2D {
public:
	float x;	//!< x座標
	float y;	//!< y座標

	/**
	 * コンストラクタ.
	 */
	Vector2D();

	/**
	 * デストラクタ.
	 */
	~Vector2D();

	/**
	 * 足し算を行います.
	 * @param[in] vec 足し算する値
	 */
	void add(Vector2D *vec);

	/**
	 * 引き算を行います.
	 * @param[in] vec 引き算する値
	 */
	void subtract(Vector2D* vec);

	/**
	 * かけ算を行います.
	 * @param[in] val かけ算する値
	 */
	void multiply(float val);

	/**
	 * 正規化処理を行います.
	 */
	void normalize();

	/**
	 * 内積を計算します.
	 * @param[in] vec 内積を計算する相手のベクトル
	 * @return 内積
	 */
	float dotProduct(Vector2D* vec);

	/**
	 * 外積を計算します.
	 * @param[in] vec 外積を計算する相手のベクトル
	 * @return 外積
	 */
	float crossProduct(Vector2D* vec);
};

#endif // end of MATH_VECTOR2D_H
