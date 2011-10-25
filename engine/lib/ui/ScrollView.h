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
 * ScrollView.h
 *
 *  Created on: 2011/08/08
 *      Author: GClue, Inc.
 */
#ifndef SCROLLVIEW_H_
#define SCROLLVIEW_H_

#include "ViewGroup.h"

class Animation;

/**
 * @enum スクロールモードを定義します.
 */
enum ScrollMode {
	ScrollMode_Horizon = 0,	//!< 水平方向へのスクロール
	ScrollMode_Vertical,	//!< 垂直方向へのスクロール
	ScrollMode_None,		//!< 両方にスクロール
};
typedef enum ScrollMode ScrollMode;

/**
 * @define タッチイベントの履歴を保持する個数を定義.
 */
#define POS_HISTROY 3

class ScrollView;

/**
 * スクロールイベントを通知するリスナー
 */
class OnScrollEventListener {
public:
	virtual ~OnScrollEventListener() {}
	/**
	 * スクロールが開始されたことを通知します.
	 * @param[in] view スクロールを開始したview
	 */
	virtual void onScrollStart(ScrollView *view) = 0;
	/**
	 * スクロール中を通知します.
	 * @param[in] view スクロール中のview
	 */
	virtual void onScroll(ScrollView *view) = 0;
	/**
	 * スクロールが終了されたことを通知します.
	 * @param[in] view スクロールが終了したview
	 */
	virtual void onScrollEnd(ScrollView *view) = 0;
};



/**
 * スクロール
 */
class ScrollView : public ViewGroup {
private:
	OnScrollEventListener *listener;

	Pointf startPos;		//!< タッチ開始位置
	Pointf currentPos;		//!< タッチ位置
	Pointf scrollPos;		//!< スクロール位置
	bool isTouch;			//!< タッチフラグ
	Animation *scrollAnim;	//!< スクロールアニメーション

	Pointf scrPos[POS_HISTROY];		//!< タッチイベントのヒストリ
	long startTime[POS_HISTROY];	//!< タッチダウン時間
	int posCount;					//!< 履歴の個数
	float sp;						//!< 慣性の速度


	/**
	 * スクロールアニメーションを作成します.
	 * @param[in] 次にスクロールするインデックス
	 */
	void createScrollAnimation(int nextIndex);

	void onTouchScrollHorizon(TouchEvent &event);
	void onTouchPageScrollHorizon(TouchEvent &event);
	void onTouchScrollVertical(TouchEvent &event);
	void onTouchPageScrollVertical(TouchEvent &event);

	void drawScrollHorizon(double dt);
	void drawPageScrollHorizon(double dt);
	void drawScrollVertical(double dt);
	void drawPageScrollVertical(double dt);

	bool isOverrun();

public:
	int currentIndex;	//!< 現在のページ
	int nextIndex;		//!< 次のページ
	int mode;			//!< スクロールモード
	bool pageScroll;	//!< ページスクロールフラグ.

	/**
	 * コンストラクタ.
	 * @param context View用のコンテキスト
	 */
	ScrollView(GCContext *context);

	/**
	 * デストラクタ.
	 */
	virtual ~ScrollView();

	//////////////////////////////////////////////////////////////
	// publicな関数
	//////////////////////////////////////////////////////////////
	/**
	 * スクロールイベントを受け取る為のリスナーを設定します.
	 * @param listener リスナー
	 */
	void setOnScrollEventListener(OnScrollEventListener *listener);

	/**
	 * 左側にスクロールします.
	 * 左にスクロールできない場合には、この関数は無視されます。
	 */
	void scrollLeft();

	/**
	 * 右側にスクロールします.
	 * 右にスクロールできない場合には、この関数は無視されます。
	 */
	void scrollRight();

	/**
	 * スクロールモードを設定します.
	 *
	 * mode には、以下の値を設定することができます。
	 * <ul>
	 * <li>ScrollMode_Horizon
	 * <li>ScrollMode_Vertical
	 * </ul>
	 * @param[in] mode スクロールモード
	 */
	void setScrollMode(int mode);

	/**
	 * 設定されているスクロールモードを取得します.
	 * @return スクロールモード
	 */
	int getScrollMode();

	/**
	 * ページスクロールのON/OFFを設定します.
	 * @param[in] flag trueの場合はページスクロール
	 */
	void setPageScroll(bool flag);

	/**
	 * ページスクロールフラグを取得します.
	 * @return スクロールフラグ
	 */
	bool isPageScroll();

	/**
	 * 現在表示されているページを取得します.
	 *
	 * isPageScrollがfalseの場合にはページは不定になります。
	 * trueの場合のみ使用してください。
	 *
	 * @return ページ
	 */
	int getCurrentPage();

	/**
	 * 指定されたページにスクロールします.
	 * @param[in] page ページ
	 * @param[in] scroll スクロールフラグ(true: スクロールする)
	 */
	void setCurrentPage(int page, bool scroll = true);

	/**
	 * 先頭にスクロールします.
	 */
	void setScrollTop();

	//////////////////////////////////////////////////////////////
	// Viewからの継承
	//////////////////////////////////////////////////////////////

	/**
	 * タッチイベントを処理します.
	 * @param[in] event タッチイベント
	 * @retval true 次へイベントを渡さない
	 * @retval false 次へイベントを渡す
	 */
	bool onTouch(TouchEvent &event);

	/**
	 * 指定されたアニメーションを反映して描画を行います.
	 * @param[in] dt 前回描画からの差分時間
	 * @param[in] animation 反映するアニメーション
	 */
	virtual void draw(double dt, IAnimation *animation = NULL);
};

#endif /* SCROLLVIEW_H_ */
