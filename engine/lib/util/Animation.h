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

#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "Easing.h"
#include "IAnimation.h"
#include "../math/Matrix3D.h"

class Animation;
class AnimationSet;

/**
 * アニメーションクラス.
 */
class Animation : public IAnimation {
private:
	Easing *mX;			//!< x軸移動のイージング.
	Easing *mY;			//!< y軸移動のイージング.
	Easing *mZ;			//!< z軸移動のイージング.
	Easing *mAlpha;		//!< アルファ値のイージング.
	Easing *mxRotation;	//!< 回転のイージング.
	Easing *myRotation;	//!< 回転のイージング.
	Easing *mzRotation;	//!< 回転のイージング.
	Easing *mxScale;	//!< x軸スケーリングのイージング.
	Easing *myScale;	//!< y軸スケーリングのイージング.
	Easing *mzScale;	//!< z軸スケーリングのイージング.
	Easing *mR;			//!< 赤色のイージング
	Easing *mG;			//!< 緑色のイージング
	Easing *mB;			//!< 青色のイージング
	Easing *mBright;	//!< ブライト値のイージング

	int type;			//!< アニメーションタイプ.
	double duration;	//!< アニメーション時間.
	double delay;		//!< アニメーション開始時までの遅延時間
	double delayTime;	//!< アニメーション開始時までの遅延時間
	double time;		//!< 経過時間.
	bool startflg;		//!< アニメーション開始フラグ(true: アニメーションが開始されている)
	bool finishflg;		//!< 終了フラグ

	/**
	 * イージング処理を行います.
	 */
	void easing(double dt);

public:
	float width;		//!< 標準の大きさ
	float height;		//!< 標準の大きさ
	float depth;		//!< z軸のdepth
	float aspect;		//!< アスペクト

	Matrix3D *matrix;	//!< マトリクス

	AnimationSet *animation;	//!< アニメーション

	/**
	 * コンストラクタ.
	 * @param[in] type アニメーションタイプ
	 */
	Animation(int type = EASING_LINEAR);

	/**
	 * デストラクタ.
	 */
	virtual ~Animation();

	/**
	 * アニメーションまでの遅延時間を設定します.
	 * @param[in] delay 遅延時間
	 */
	void setDelay(double delay);

	/**
	 * アニメーションまでの遅延時間を取得します.
	 * @return delay 遅延時間
	 */
	double getDelay();

	/**
	 * アルファ値を設定します.
	 * @param[in] start 開始アルファ値
	 */
	void setAlpha(float start, float end);

	/**
	 * x軸方向への移動アニメーションを設定します.
	 * @param[in] start 開始するx座標
	 * @param[in] end 終了するx座標
	 */
	void setMoveX(float start, float end);
	
	/**
	 * y軸方向への移動アニメーションを設定します.
	 * @param[in] start 開始するy座標
	 * @param[in] end 終了するy座標
	 */
	void setMoveY(float start, float end);
	
	/**
	 * z軸方向への移動アニメーションを設定します.
	 * @param[in] start 開始するz座標
	 * @param[in] end 終了するz座標
	 */
	void setMoveZ(float start, float end);
	
	/**
	 * 開始位置と終了位置を設定します.
	 * @param[in] sx 開始するx座標
	 * @param[in] sy 開始するy座標
	 * @param[in] ex 終了するx座標
	 * @param[in] ey 終了するy座標
	 */
	void setMove(float sx, float sy, float ex, float ey);

	/**
	 * xyz軸の開始位置と終了位置を設定します.
	 * @param[in] sx 開始するx座標
	 * @param[in] sy 開始するy座標
	 * @param[in] sz 開始するz座標
	 * @param[in] ex 終了するx座標
	 * @param[in] ey 終了するy座標
	 * @param[in] ez 終了するz座標
	 */
	void setMove(float sx, float sy, float sz, float ex, float ey, float ez);
	
	void setScaleX(float start, float end);
	void setScaleY(float start, float end);
	void setScaleZ(float start, float end);
	
	/**
	 * スケールを設定します.
	 * @param[in] sx 開始時のx軸スケール
	 * @param[in] sy 開始時のy軸スケール
	 * @param[in] ex 終了時のx軸スケール
	 * @param[in] ey 終了時のy軸スケール
	 */
	void setScale(float sx, float sy, float ex, float ey);
	void setScale(float sx, float sy, float sz, float ex, float ey, float ez);

	/**
	 * ブライトネスの値を設定します.
	 * @param sb 開始時のブライトネス
	 * @param eb 終了時のブライトネス
	 */
	void setBright(float sb, float eb);

	/**
	 * 指定した色から指定した色へ変化させます.
	 * @param sr 開始時の赤色
	 * @param sg 開始時の緑色
	 * @param sb 開始時の青色
	 * @param er 終了時の赤色
	 * @param eg 終了時の緑色
	 * @param eb 終了時の青色
	 */
	void setRGB(float sr, float sg, float sb, float er, float eg, float eb);

	void setRotation(float start, float end);

	/**
	 * 回転を設定します.
	 * @param[in] start 開始時の角度
	 * @param[in] end 終了時の角度
	 */
	void setRotationX(float start, float end);
	void setRotationY(float start, float end);
	void setRotationZ(float start, float end);

	/**
	 * アニメーション時間を設定します.
	 * durationは 1.0f で 1 秒になります。
	 * @param[in] duration 時間
	 */
	void setDuration(float duration);

	//////////////////////////////////////////////////
	// IAnimation からの継承
	//////////////////////////////////////////////////

	/**
	 * 指定されたmtxに対してアニメーションのマトリクスを計算します.
	 * @param[in] mtx アニメーションのマトリクスを反映させるマトリクス
	 */
	void multiply(Matrix3D *mtx);

	/**
	 * アニメーションを終了します.
	 */
	void finish();

	/**
	 * アニメーションを開始します.
	 */
	void reset();

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

#endif	// end of __ANIMATION_H__
