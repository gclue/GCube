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

#ifndef __IANIMATION_H__
#define __IANIMATION_H__

class IAnimation;
class Matrix3D;

/**
 * アニメーション完了時に呼び出されるコールバック.
 */
class IAnimationListener {
public:
	virtual ~IAnimationListener(){};
	
	/**
	 * アニメーションが開始されたときのイベント.
	 * @param animation 開始したアニメーション
	 */
	virtual void onAnimationStart(IAnimation *animation) = 0;

	/**
	 * アニメーションが完了したときのイベント.
	 * @param animation 完了したアニメーション
	 * @param index AnimationSetの場合には、終了したアニメーションのインデックス
	 */
	virtual void onAnimationEnd(IAnimation *animation, int index) = 0;

	/**
	 * アニメーションが完了したときのイベント.
	 * @param animation 完了したアニメーション
	 */
	virtual void onAnimationAllEnd(IAnimation *animation) = 0;
};

/**
 * アニメーションを行うインターフェイスクラス.
 */
class IAnimation {
protected:
	IAnimationListener *listener;			//!< アニメーションイベントを通知するリスナー
	bool repeat;		//!< リピート再生フラグ

public:
	int tag;			//!< アニメーション識別用タグ
	
	virtual ~IAnimation(){};

	/**
	 * アニメーションイベント通知用リスナーを設定します.
	 * 設定されたリスナーは、このクラスでは破棄しないので、注意してください。
	 * @param[in] listener リスナー
	 */
	void setAnimationListener(IAnimationListener *listener) {
		this->listener = listener;
	}

	/**
	 * アニメーション識別用タグを設定します.
	 * @param[in] tag 識別用のタグ
	 */
	void setTag(int tag) { this->tag = tag; }
	
	/**
	 * アニメーション識別用タグを取得します.
	 * @return タグ
	 */
	int getTag() { return tag; }
	
	/**
	 * リピート設定を行います.
	 * デフォルトでは、falseに設定されているの注意してください。
	 * @param repeat リピートフラグ
	 */
	void setRepeat(bool repeat) {
		this->repeat = repeat;
	}
	
	/**
	 * リピート設定を取得します.
	 * @return リピートする場合はtrue, それ以外はfalse
	 */
	bool isRepeat() {
		return repeat;
	}

	/**
	 * 指定されたmtxに対してアニメーションのマトリクスを計算します.
	 * @param[in] mtx アニメーションのマトリクスを反映させるマトリクス
	 */
	virtual void multiply(Matrix3D *mtx) = 0;

	/**
	 * アニメーションをリセットします.
	 */
	virtual void reset() = 0;

	/**
	 * アニメーションを t 時間分だけ実行します.
	 * <br><br>
	 * @param[in] t 経過時間
	 */
	virtual void step(double dt) = 0;

	/**
	 * アニメーションが開始されているかチェックします.
	 * @retval true: アニメーション中
	 * @retval false: アニメーションが開始されていない
	 */
	virtual bool isStarted() = 0;

	/**
	 * アニメーションの終了確認を行います.
	 * @return 終了確認結果<ul><li>true: 終了<li>false: アニメーション中</ul>
	 */
	virtual bool isFinish() = 0;

	/**
	 * アニメーションのマトリクスを取得します.
	 * <br><br>
	 * @param[out] m マトリクスを格納する配列
	 */
	virtual void getMatrix(float *m) = 0;

	/**
	 * アニメーションのアルファ値を取得します.
	 * <br><br>
	 * @return アルファ値
	 */
	virtual float getAlpha() = 0;

	/**
	 * ブライトネスの値を取得します.
	 * @return
	 */
	virtual float getBright() = 0;
};

#endif	// end of __IANIMATION_H__
