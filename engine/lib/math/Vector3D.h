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

#ifndef MATH_VECTOR3D_H
#define MATH_VECTOR3D_H

/**
 * 3次元のベクトルクラス.
 * <br><br>
 *
 * 足し算や引き算などのオペレータを Overwrite しないで関数として用意しています。
 * Object-C のクラスを移植しているので、元々のソースから変更点が少なくするためです。
 */
class Vector3D {
public:
	float x;	//!< x座標
	float y;	//!< y座標
	float z;	//!< z座標
	float w;	//!< w

	/**
	 * コンストラクタ.
	 */
	Vector3D();
	Vector3D(Vector3D *vec);
	Vector3D(float x, float y, float z);

	/**
	 * デストラクタ.
	 */
	~Vector3D();

	/**
	 * ベクトルの足し算を行います.
	 * @param[in] vec 足し算するベクトル
	 */
	void add(Vector3D *vec);

	/**
	 * ベクトルの引き算を行います.
	 * @param[in] vec 引き算するベクトル
	 */
	void subtract(Vector3D *vec);

	/**
	 * ベクトルのかけ算を行います.
	 * @param[in] val かけ算する値
	 */
	void multiply(float val);

	/**
	 * ベクトルの正規化を行います.
	 */
	void normalize();

	void crossProduct(Vector3D *vec);
};


#endif	// end of MATH_VECTOR3D_H
