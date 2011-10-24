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
 * SceneGame.cpp
 *
 *  Created on: 2011/08/01
 *      Author: GClue, Inc.
 */
#include "SceneGame.h"
#include "Camera.h"
#include "Figure.h"
#include "ApplicationController.h"

SceneGame::SceneGame(ApplicationController *controller) : IScene(controller) {
	// カメラ
	camera = new Camera();
	camera->transForm.translate(0, 0, 50);

	// シーンのセットアップ
	this->setup();
}

SceneGame::~SceneGame() {
	delete camera;
}

/////////////////////////////////////////////////////////////
// IScene の実装
/////////////////////////////////////////////////////////////

// セットアップ処理を行います.
void SceneGame::setup() {
}

// リサイズ
void SceneGame::resize(int width, int height) {
	LOGD("****SceneGame::resize:%d-%d", width, height);
	float aspect = width / (float)height;
	camera->aspect = aspect;
	camera->loadOrthographic(true);
}

// ステップ実行します
void SceneGame::step(float dt) {
}

// 活性化します.
void SceneGame::onActivate() {
	LOGD("****SceneGame::onActivate");
	activeflg = true;
}

// 休止します.
void SceneGame::onSuspend() {
	LOGD("****SceneGame::onSuspend");
	activeflg = false;
}

// 活性化してシーンが切り替え終わったこと通知します.
void SceneGame::onStart() {

}

// コンテキストが切り替わったことを通知します.
void SceneGame::onContextChanged() {

}

// バックキーイベント
bool SceneGame::onPressBackKey() {
	LOGD("****SceneGame::onPressBackKey");
//	sceneContext.controller->sceneChange(SceneID_Title);
	return true;
}

// タッチイベント
void SceneGame::onTouch(TouchEvent &event) {
	LOGD("****SceneGame::onTouch:%.1f-%.1f", event.x, event.y);
//	sceneContext.controller->sceneChange(SceneID_Title);
}

// 加速度センサー
void SceneGame::onMoveSensor(double sensor) {

}

// JNIイベント
void SceneGame::onGameEvent(int type, int param1, int param2, int param3, int param4, const char *param5) {
	LOGI("** SceneGame::onGameEvent: %d,%d,%d,%d,%d,%s",type, param1, param2, param3, param4, param5);
}
