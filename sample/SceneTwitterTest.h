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

#ifndef SceneTwitterTest_H_
#define SceneTwitterTest_H_

#include "Scene.h"
#include "Button.h"
#include "Animation.h"


/**
 * ツイッターサンプルコードクラス.
 */
class SceneTwitterTest : public Scene, OnButtonClickListener, IAnimationListener {
private:
	typedef Scene super;
	int index;
	
	//成功文字の表示
	void startPostSuccess();
	//失敗文字の表示
	void startPostFailed();
	
public:
	/**
	 * コンストラクタ.
	 * @param[in] controller コントローラー
	 */
	SceneTwitterTest(ApplicationController *controller);
	
	/**
	 * デストラクタ.
	 */
	virtual ~SceneTwitterTest();
	
	//////////////////////////////////////////////////////////
	// IScene の実装
	//////////////////////////////////////////////////////////
	
	/**
	 * ステップ実行します.
	 */
	virtual void step(float dt);
	
	/**
	 * セットアップ処理を行います.
	 */
	virtual void setup();
	
	/**
	 * リサイズ処理を行います.
	 * @param[in] width 横幅
	 * @param[in] height 縦幅
	 */
	virtual void resize(int width, int height);
	
	/**
	 * 活性化します.
	 */
	virtual void onActivate();
	
	/**
	 * 休止します.
	 */
	virtual void onSuspend();
	
	/**
	 * 活性化してシーンが切り替え終わったこと通知します.
	 */
	virtual void onStart();
	
	/**
	 * 非活性化してシーンが切り替え終わったこと通知します.
	 */
	virtual void onEnd();
	
	/**
	 * コンテキストが切り替わったことを通知します.
	 */
	virtual void onContextChanged();
	
	/**
	 * タッチイベント.
	 * @param[in] event タッチイベント
	 */
	virtual bool onTouch(TouchEvent &event);
	
	
	/**
	 * ツイッターイベントを受け取ります.
	 */
	virtual void onTwitterEvent(int type, int param);
	
	
	/**
	 * ボタンがクリックされたイベント.
	 * @param[in] btn クリックされたボタン
	 */
	virtual void onButtonClick(Button *btn);
	/**
	 * ボタンがタッチされたときに呼び出されます.
	 * @param[in] btn タッチされたボタン
	 */
	virtual void onButtonTouchStart(Button *btn);
	
	
	/**
	 * アニメーションが開始されたときのイベント.
	 * @param animation 開始したアニメーション
	 */
	virtual void onAnimationStart(IAnimation *animation);
	
	/**
	 * アニメーションが完了したときのイベント.
	 * @param animation 完了したアニメーション
	 * @param index AnimationSetの場合には、終了したアニメーションのインデックス
	 */
	virtual void onAnimationEnd(IAnimation *animation, int index);
	
	/**
	 * アニメーションが完了したときのイベント.
	 * @param animation 完了したアニメーション
	 */
	virtual void onAnimationAllEnd(IAnimation *animation);
	
	
	
	
};

#endif /* SceneTwitterTest_H_ */
