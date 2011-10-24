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

ApplicationController* ApplicationController::singleton = NULL;

ApplicationController::ApplicationController() {
	LOGD("***********ApplicationController");

	// 初期化
	activeScene = NULL;
	nextSceneID = SceneID_None;
	preSceneID = SceneID_None;
	currentSceneID = SceneID_None;

	// ポーズを解除
	pause = false;
	// シーンきりかえフラグ
	changeFlg = false;

	// fadeアニメーション
	fadeAnimation = new Animation(EASING_LINEAR);
	fadeAnimation->setAlpha(0.0, 1.0);
	fadeAnimation->setDuration(0.3333);
	fadeAnimation->finish();

	// アスペクト比
	aspect = 1.0f;

	// 背景色
	red = 0.5f;
	green = 0.5f;
	blue = 0.5f;
}

ApplicationController::~ApplicationController() {
	LOGD("***********~ApplicationController");

	// 各シーン
	std::map<int, IScene*>::iterator it = scenes.begin();
	while (it != scenes.end()) {
		IScene *scene = (*it).second;
		delete scene;
		it++;
	}
	delete fadeAnimation;
}

// シーンを追加
void ApplicationController::addScene(int id, IScene *scene) {
	scenes[id] = scene;
}

// シーン切り替え
void ApplicationController::sceneChange(int sceneid) {
	LOGD("***********sceneChange:%d", sceneid);
	if (nextSceneID != SceneID_None || !fadeAnimation->isFinish()) return;
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

void ApplicationController::setBackgroundColor(float r, float g, float b) {
	red = r;
	green = g;
	blue = b;
}

// 再セットアップ
void ApplicationController::resetup() {
	LOGD("***********resetup");

	// シェーダー再準備
	delete shader;
	shader = new SimpleShader();

	// 各シーン
	std::map<int, IScene*>::iterator it = scenes.begin();
	while (it != scenes.end()) {
		IScene *scene = (*it).second;
		if (scene) {
			scene->onContextChanged();
		}
		it++;
	}

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
}

// 一時停止します.
void ApplicationController::onPause() {
	LOGD("***********onPause");
	pause = true;
}

// 一時停止から復帰します.
void ApplicationController::onResume() {
	LOGD("***********onResume");
	pause = false;
}

// ゲームをステップ実行します
void ApplicationController::step(float dt) {
	// 画面クリア
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (pause) return;
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
		fadeAnimation->step(dt);
		shader->setBaseAlpha(1.0-fadeAnimation->alpha);
		if (activeScene) {
			activeScene->step(dt);
		}
		shader->setBaseAlpha(fadeAnimation->alpha);
		scenes[nextSceneID]->step(dt);
		shader->setBaseAlpha(1.0);

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
	if (activeScene) {
		x = x * 480.0 / width;
		y = y * 800.0 / height;

		Pointf p;
		p.x = (x / 240.0) - 1.0;
		p.y = (((-y) / 400.0) + 1.0) / aspect;
		TouchEvent event;
		event.type = action;
		event.x = p.x;
		event.y = p.y;
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

// Javaからのイベント
void ApplicationController::onGameEvent(int type, int param1, int param2, int param3, int param4, const char *param5) {
	LOGD("***********onGameEvent:%d,%d,%d,%d,%d,%s",type, param1, param2, param3, param4, param5);
	if (activeScene) {
		activeScene->onGameEvent(type, param1, param2, param3, param4, param5);
	}
}

