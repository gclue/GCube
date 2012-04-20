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

#ifndef ISCENE_H_
#define ISCENE_H_

#include "View.h"

class ApplicationController;
class Camera;
class SimpleShader;
class TextureManager;

/**
 * シーン.
 *
 * 以下の順に関数が呼ばれます.
 * onActivate() → resize() → onStart() → onEnd() → onSuspend()
 *
 */
class IScene {
protected:
	ApplicationController *controller;	//!< コントローラー
	bool _activeflg;	//!< アクティブフラグ(true: アクティブ)
	bool _startflg;		//!<

public:
	/**
	 * コンストラクタ.
	 * @param controller コントローラー
	 */
	IScene(ApplicationController *controller) {
		this->controller = controller;
		_activeflg = false;
		_startflg = false;
	}

	/**
	 * デストラクタ.
	 */
	virtual ~IScene() {};


	/**
	 * リサイズ処理を行います.
	 * @param[in] width 横幅
	 * @param[in] height 縦幅
	 */
	virtual void resize(int width, int height) = 0;

	/**
	 * セットアップ処理を行います.
	 */
	virtual void setup() = 0;

	/**
	 * ステップ実行します.
	 */
	virtual void step(float dt) = 0;

	/**
	 * 活性化します.
	 */
	virtual void onActivate() {_activeflg = true;};

	/**
	 * 非活性化して休止します.
	 */
	virtual void onSuspend() {_activeflg = false;};

	/**
	 * 活性化してシーンが切り替え終わったこと通知します.
	 */
	virtual void onStart() {_startflg = true;};

	/**
	 * 非活性化してシーンが切り替え始まったこと通知します.
	 */
	virtual void onEnd() {_startflg = false;};

	/**
	 * アプリ停止を通知します.
	 */
	virtual void onPause(){};
	
	/**
	 * アプリ再開を通知します.
	 */
	virtual void onResume(){};
	
	/**
	 * コンテキストが切り替わったことを通知します.
	 */
	virtual void onContextChanged() = 0;

	/**
	 * 活性化状態かを返します.
	 * @return 活性化状態の場合はtrue
	 */
	virtual bool isActive() {return _activeflg;};

	/**
	 * 開始状態かを返します.
	 * @return 開始状態の場合はtrue
	 */
	virtual bool isStarted() {return _startflg;};

	/**
	 * バックキーのイベント.
	 * @return 処理をした場合はtrue,しないでシステムに返す場合はfalse
	 */
	virtual bool onPressBackKey() = 0;

	/**
	 * タッチイベント.
	 * @param[in] event タッチイベント
	 */
	virtual bool onTouch(TouchEvent &event) = 0;

	/**
	 * 加速度センサーイベント.
	 * @param sensor 加速度の値
	 */
	virtual void onMoveSensor(double sensor) = 0;

	/**
	 * ゲームイベントを受け取るメソッド.
	 * @param type イベントタイプ
	 * @param param1 イベントパラメータ
	 * @param param2 イベントパラメータ
	 * @param param3 イベントパラメータ
	 * @param param4 イベントパラメータ
	 * @param param5 イベントパラメータ
	 */
	virtual void onGameEvent(int type, int param1, long param2, double param3, int param4, const char *param5) = 0;
	
	
	/**
	 * ツイッターイベントを受け取ります.
	 * @param[in] type イベントタイプ.
	 * @param[in] param1 パラメータ.
	 */
	virtual void onTwitterEvent(int type, int param1) = 0;
	
	/**
	 * デバッグコマンドを受け取るメソッド.
	 * @param command コマンド
	 * @param param パラメータ
	 */
	virtual void onDebugCommand(const char *command, int param) {};
};

#endif /* ISCENE_H_ */
