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
 * AnimationSet.h
 *
 *  Created on: 2011/05/06
 *      Author: GClue, Inc.
 */
#ifndef ANIMATIONSET_H_
#define ANIMATIONSET_H_

#include <vector>
#include "../util/Animation.h"

/**
 * アニメーションを連続で行う為のクラス.
 */
class AnimationSet : public IAnimation {
private:
	std::vector<Animation*> animations;		//!< アニメーションのリスト
	int index;								//!< 再生しているアニメーションのインデックス
	double delay;							//!< アニメーションを開始するまでの遅延時間
	double delayTime;						//!< アニメーションを開始するまでの遅延時間
	bool startflg;							//!< アニメーション開始フラグ(true: アニメーションが開始されている)
	bool finishflg;							//!< 終了フラグ

public:
	Matrix3D *matrix;	//!< アニメーションの結果を格納するマトリクス
	float alpha;		//!< アルファ値
	float bright;		//!< ブライト値
	float x;			//!< x座標
	float y;			//!< y座標
	float z;			//!< 奥行き
	float xscale;		//!< x方向へのスケール
	float yscale;		//!< y方向へのスケール
	float rotate;		//!< 回転角度
	float r;			//!< 赤色
	float g;			//!< 緑色
	float b;			//!< 青色

	AnimationSet();
	virtual ~AnimationSet();

	/**
	 * アニメーションまでの遅延時間を設定します.
	 * @param[in] delay 遅延時間
	 */
	void setDelay(double delay);

	/**
	 * アニメーションを追加します.
	 * 追加されたAnimationは、このクラスの破棄時に一緒に破棄されます。
	 * @param[in] animation 追加するアニメーション
	 */
	void addAnimation(Animation *animation);

	////////////////////////////////////////////////////////////
	// IAnimationからの継承クラス
	////////////////////////////////////////////////////////////

	/**
	 * 指定されたmtxに対してアニメーションのマトリクスを計算します.
	 * @param[in] mtx アニメーションのマトリクスを反映させるマトリクス
	 */
	void multiply(Matrix3D *mtx);

	/**
	 * アニメーションをリセットします.
	 */
	void reset();

	/**
	 * アニメーションを終了します.
	 */
	void finish();

	/**
	 * アニメーションを t 時間分だけ実行します.
	 * <br><br>
	 * @param[in] t 経過時間
	 */
	void step(double dt);

	/**
	 * アニメーションが開始されているかチェックします.
	 * @retval true: アニメーション中
	 * @retval false: アニメーションが開始されていない
	 */
	bool isStarted();

	/**
	 * アニメーションの終了確認を行います.
	 * @return 終了確認結果<ul><li>true: 終了<li>false: アニメーション中</ul>
	 */
	bool isFinish();

	/**
	 * アニメーションのマトリクスを取得します.
	 * <br><br>
	 * @param[out] m マトリクスを格納する配列
	 */
	void getMatrix(float *m);

	/**
	 * アニメーションのアルファ値を取得します.
	 * <br><br>
	 * @return アルファ値
	 */
	float getAlpha();

	/**
	 * ブライト値を取得します.
	 * @return ブライト値
	 */
	float getBright();
};

#endif /* ANIMATIONSET_H_ */
