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
 * SceneHelp.cpp
 *
 *  Created on: 2011/08/01
 *      Author: GClue, Inc.
 */
#include "SceneHelp.h"
#include "Camera.h"
#include "Figure.h"
#include "ApplicationController.h"

SceneHelp::SceneHelp(ApplicationController *controllert) : IScene(controller) {
	// カメラ
	camera = new Camera();
	camera->transForm.translate(0, 0, 50);

	// シーンのセットアップ
	this->setup();
}

SceneHelp::~SceneHelp() {
	delete camera;
}

/////////////////////////////////////////////////////////////
// IScene の実装
/////////////////////////////////////////////////////////////

// セットアップ処理を行います.
void SceneHelp::setup() {
}

// リサイズ
void SceneHelp::resize(int width, int height) {
	LOGD("****SceneHelp::resize:%d-%d", width, height);
	float aspect = width / (float)height;
	camera->aspect = aspect;
	camera->loadOrthographic(true);
}

// ステップ実行します
void SceneHelp::step(float dt) {
}

// 活性化します.
void SceneHelp::onActivate() {
	LOGD("****SceneHelp::onActivate");
	activeflg = true;
}

// 休止します.
void SceneHelp::onSuspend() {
	LOGD("****SceneHelp::onSuspend");
	activeflg = false;
}

// 活性化してシーンが切り替え終わったこと通知します.
void SceneHelp::onStart() {

}

// コンテキストが切り替わったことを通知します.
void SceneHelp::onContextChanged() {

}

// バックキーイベント
bool SceneHelp::onPressBackKey() {
	LOGD("****SceneHelp::onPressBackKey");
//	sceneContext.controller->sceneChange(SceneID_Title);
	return true;
}

// タッチイベント
void SceneHelp::onTouch(TouchEvent &event) {
	LOGD("****SceneHelp::onTouch:%.1f-%.1f", event.x, event.y);
//	sceneContext.controller->sceneChange(SceneID_Title);
}

// 加速度センサー
void SceneHelp::onMoveSensor(double sensor) {

}

// JNIイベント
void SceneHelp::onGameEvent(int type, int param1, int param2, int param3, int param4, const char *param5) {
	LOGI("** SceneHelp::onGameEvent: %d,%d,%d,%d,%d,%s",type, param1, param2, param3, param4, param5);
}
