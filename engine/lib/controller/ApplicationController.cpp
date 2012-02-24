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

#include "ApplicationController.h"
#include "Animation.h"
#include "Camera.h"
#include "Log.h"
#include "SimpleShader.h"
#include "BoneShader.h"
#include "APIGlue.h"
#include "main.h"
#include "SoundPlayer.h"

// シーンなし
#define SceneID_None 0

// インスタンス初期化
ApplicationController* ApplicationController::singleton = NULL;

// コンストラクタ
ApplicationController::ApplicationController() {
	LOGD("***********ApplicationController");

	// 初期化
	activeScene = NULL;
	nextSceneID = SceneID_None;
	preSceneID = SceneID_None;
	prePreSceneID = SceneID_None;
	currentSceneID = SceneID_None;
	main = new Main(this);

	// ポーズを解除
	pause = false;
	// シーンきりかえフラグ
	changeFlg = false;

	// fadeアニメーション
	defaultFadeAnimation = new Animation(EASING_LINEAR);
	defaultFadeAnimation->setAlpha(0.0, 1.0);
	defaultFadeAnimation->setDuration(0.3333);
	defaultFadeAnimation->finish();

	// アスペクト比
	aspect = 1.0f;

	// 背景色
	glClearColor(0.5, 0.5, 0.5, 1.0f);
}

// デストラクタ
ApplicationController::~ApplicationController() {
	LOGD("***********~ApplicationController");

	// 各シーン
	std::map<int, IScene*>::iterator it = scenes.begin();
	while (it != scenes.end()) {
		IScene *scene = (*it).second;
		delete scene;
		it++;
	}
	delete defaultFadeAnimation;
	defaultFadeAnimation = NULL;
	
	if (fadeAnimation) {
		delete fadeAnimation;
	}
	fadeAnimation = NULL;
	delete main;

	// サウンドプレイヤーを削除
	SoundPlayer::dispose();
}

// シーンを追加
void ApplicationController::addScene(int id, IScene *scene) {
	scenes[id] = scene;
}

// シーン切り替え
void ApplicationController::sceneChange(int sceneid, IAnimation *animation) {
	LOGD("***********sceneChange:%d", sceneid);
	if(animation) {
		fadeAnimation = (Animation*)animation;
		fadeAnimation->finish();
	}else {
		fadeAnimation = defaultFadeAnimation;
	}
	if (nextSceneID != SceneID_None || !fadeAnimation->isFinish()) return;
	prePreSceneID = preSceneID;
	preSceneID = currentSceneID;
	nextSceneID = sceneid;
	changeFlg = true;
}

// 一つ前のシーンに戻る
void ApplicationController::backScene() {
	LOGD("***********backScene:%d", preSceneID);
	if (nextSceneID != SceneID_None || preSceneID == SceneID_None ||
		!fadeAnimation->isFinish()) {
		return;
	}
	nextSceneID = preSceneID;
	scenes[nextSceneID]->onActivate();
	scenes[nextSceneID]->resize(width, height);
	fadeAnimation->reset();
	// 前の履歴は削除
	preSceneID = SceneID_None;
}

// 背景色を設定
void ApplicationController::setBackgroundColor(float r, float g, float b) {
	glClearColor(r, g, b, 1.0f);
}

//前のシーンIDを取得.
int ApplicationController::getPreSceneID() {
	return preSceneID;
}

//前の前のシーンIDを取得
int ApplicationController::getPrePreSceneID() {
	return prePreSceneID;
}

//現在のシーンIDを取得.
int ApplicationController::getCurrentSceneID() {
	return currentSceneID;
}

// 再セットアップ
void ApplicationController::resetup() {
	LOGD("***********resetup");

	// コンテキスト再構成
	this->setupContext();

	LOGD("***********resetup-1");

	// 各シーン
	std::map<int, IScene*>::iterator it = scenes.begin();
	while (it != scenes.end()) {
		IScene *scene = (*it).second;
		if (scene) {
			scene->onContextChanged();
		}
		it++;
	}

	LOGD("***********resetup-2");

	// アクティブになっているシーンのリサイズ処理
	if (activeScene) {
		activeScene->setup();
		activeScene->resize(width, height);
	}
	LOGD("***********resetup end");
}

// リサイズ
void ApplicationController::resize(int width, int height) {
	LOGD("***********resize:%d-%d", width, height);

	this->width = width;
	this->height = height;
	this->aspect = width / (float) height;

	glViewport(0, 0, width, height);

	if (camera) {
		camera->aspect = aspect;
		camera->loadOrthographic(true);
	}

	if (activeScene) {
		activeScene->resize(width, height);
	}
	LOGD("***********resize end");
}

// 一時停止します.
void ApplicationController::onPause() {
	LOGD("***********onPause");
	pause = true;
	// メインに通知
	main->onPause();
	// 各シーンに通知
	std::map<int, IScene*>::iterator it = scenes.begin();
	while (it != scenes.end()) {
		IScene *scene = (*it).second;
		if (scene) {
			scene->onPause();
		}
		it++;
	}
}

// 一時停止から復帰します.
void ApplicationController::onResume() {
	LOGD("***********onResume");
	pause = false;
	// メインに通知
	main->onResume();
	// 各シーンに通知
	std::map<int, IScene*>::iterator it = scenes.begin();
	while (it != scenes.end()) {
		IScene *scene = (*it).second;
		if (scene) {
			scene->onResume();
		}
		it++;
	}
}

// ゲームをステップ実行します
void ApplicationController::step(float dt) {
	if (pause) return;
	// 画面クリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (dt > 0.07) dt = 0.07;

	// アルファブレンド
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// シーン切り替えトランジション（フェード）
	if (nextSceneID != SceneID_None) {
		if (changeFlg) {
			// 遷移先のシーンをアクティブにする
			preSceneID = currentSceneID;
			// 現在のシーンを非活性化
			if (activeScene) {
				activeScene->onEnd();
			}
			// 遷移先のシーンの活性化
			scenes[nextSceneID]->onActivate();
			scenes[nextSceneID]->resize(width, height);
			fadeAnimation->reset();
			changeFlg = false;
		}
		// このやり方だと裏が見えるのでFBOを使うやり方に変更したい
		if(!fadeAnimation) {
			fadeAnimation = defaultFadeAnimation;
		}
		fadeAnimation->step(dt);
		shader->setBaseAlpha(1.0-fadeAnimation->alpha);
		shader3d->setBaseAlpha(1.0-fadeAnimation->alpha);
		if (activeScene) {
			activeScene->step(dt);
		}
		shader->setBaseAlpha(fadeAnimation->alpha);
		shader3d->setBaseAlpha(fadeAnimation->alpha);
		scenes[nextSceneID]->step(dt);
		shader->setBaseAlpha(1.0);
		shader3d->setBaseAlpha(1.0);

		// フェードが終わったらアクティブを切り替え
		if (fadeAnimation->isFinish()) {
			if (activeScene) {
				// 非活性化して、シーンが切り替わったことを通知
				activeScene->onSuspend();
			}
			activeScene = scenes[nextSceneID];
			currentSceneID = nextSceneID;
			nextSceneID = SceneID_None;
			// フェードが終わったことを通知
			activeScene->onStart();
		}
	} else {
		if (activeScene) {
			activeScene->step(dt);
		}
	}

	// GLエラー
	int e = glGetError();
	if (e > 0) {
		LOGE("glGetError:%d", e);
//		exit(1);
	}
}

// バックキーイベント
bool ApplicationController::onPressBackKey() {
	LOGD("***********onPressBackKey");
	if (activeScene) {
		return activeScene->onPressBackKey();
	}
	return false;
}

// タッチイベント
void ApplicationController::onTouch(int action, float x, float y, long time) {
//	LOGD("***********onTouch[%d](%f,%f) %d", action, x, y, time);
	if (activeScene) {
		TouchEvent event;
		event.type = action;
		event.x = x / width * 2.0 - 1.0;
		event.y = -(y / height * 2.0 - 1.0) / aspect;
		event.time = time;
		activeScene->onTouch(event);
	}
}

/**
 * 加速度のセンサーイベント.
 * @param sensor 加速度のイベント
 */
void ApplicationController::onMoveSenser(double sensor) {
	if (activeScene) {
		activeScene->onMoveSensor(sensor);
	}
}

// ゲームイベントを送信します.
void ApplicationController::sendGameEvent(int type, int param1, long param2, double param3, int param4, const char *param5) {
	GCSendGameEvent(type, param1, param2, param3, param4, param5);
}

// ゲームイベントを処理します.
void ApplicationController::onGameEvent(int type, int param1, long param2, double param3, int param4, const char *param5) {
	LOGD("***********onGameEvent:%d,%d,%ld,%f,%d,%s",type, param1, param2, param3, param4, param5);
	// メインに通知
	bool flg = main->onGameEvent(type, param1, param2, param3, param4, param5);
	// アクティブシーンに通知
	if (activeScene && !flg) {
		activeScene->onGameEvent(type, param1, param2, param3, param4, param5);
	}
}

// デバッグコマンド
void ApplicationController::onDebugCommand(const char *command, int param) {
	LOGD("***********onDebugCommand:%s,%d", command, param);
	// メインに通知
	bool flg = main->onDebugCommand(command, param);
	// アクティブシーンに通知
	if (activeScene && !flg) {
		activeScene->onDebugCommand(command, param);
	}
}

