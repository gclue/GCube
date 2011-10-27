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

#ifndef SCENETITLE_H_
#define SCENETITLE_H_

#include "Scene.h"

class Camera;
class TitleRenderer;
class SimpleShader;
class Figure;

/**
 * タイトルシーンクラス.
 */
class SceneTitle : public Scene {
private:
	typedef Scene super;
	int index;
	
public:
	/**
	 * コンストラクタ.
	 * @param[in] controller コントローラー
	 */
	SceneTitle(ApplicationController *controller);

	/**
	 * デストラクタ.
	 */
	virtual ~SceneTitle();

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
	virtual void onTouch(TouchEvent &event);
};

#endif /* SCENETITLE_H_ */
