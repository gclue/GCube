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
 * Scene.cpp
 *
 *  Created on: 2011/09/15
 *      Author: GClue, Inc.
 */
#include "Scene.h"
#include "Layer.h"
#include "Log.h"

Scene::Scene(ApplicationController *controller) : IScene(controller) {
	LOGD("****Scene");
}

Scene::~Scene() {
	LOGD("****~Scene");

	// 各レイヤーの削除
	std::map<int, Layer*>::iterator it = layers.begin();
	while (it != layers.end()) {
		Layer *layer = (*it).second;
		delete layer;
		it++;
	}
}

// レイヤーを追加
void Scene::addLayer(int id, Layer *layer) {
	layers[id] = layer;
}

Layer *Scene::getLayer(int id) {
	return layers[id];
}

/////////////////////////////////////////////////////////////
// IScene の実装
/////////////////////////////////////////////////////////////

// セットアップ処理を行います.
void Scene::setup() {
	LOGD("****Scene::setup()");
	// 各レイヤーの描画
	std::map<int, Layer*>::iterator it = layers.begin();
	while (it != layers.end()) {
		Layer *layer = (*it).second;
		layer->setup();
		it++;
	}
}

// リサイズ
void Scene::resize(int width, int height) {
	LOGD("****Scene::resize:%d-%d", width, height);

	// アスペクト比の計算
	float aspect = width / (float) height;
	// 各レイヤーのリサイズ
	std::map<int, Layer*>::iterator it = layers.begin();
	while (it != layers.end()) {
		Layer *layer = (*it).second;
		layer->resize(aspect);
		it++;
	}
}

// ステップ実行します
void Scene::step(float dt) {
	// 各レイヤーの描画
	std::map<int, Layer*>::iterator it = layers.begin();
	while (it != layers.end()) {
		Layer *layer = (*it).second;
		layer->render(dt);
		it++;
	}
}

// 活性化します.
void Scene::onActivate() {
	LOGD("****Scene::onActivate");
	activeflg = true;
}

// 休止します.
void Scene::onSuspend() {
	LOGD("****Scene::onSuspend");
	activeflg = false;
}

// 活性化してシーンが切り替え終わったこと通知します.
void Scene::onStart() {
	LOGD("****Scene::onStart");
}

// 非活性化してシーンが切り替え始まったこと通知します.
void Scene::onEnd() {
	LOGD("****Scene::onEnd");
}

// コンテキストが切り替わったことを通知します.
void Scene::onContextChanged() {
	LOGD("****Scene::onContextChanged");
}

// バックキーイベント
bool Scene::onPressBackKey() {
	return false;
}

// タッチイベント
void Scene::onTouch(TouchEvent &event) {
	// 各レイヤーの描画
	std::map<int, Layer*>::iterator it = layers.begin();
	while (it != layers.end()) {
		Layer *layer = (*it).second;
		if (layer->onTouch(event)) {
			return;
		}
		it++;
	}
}

// 加速度センサー
void Scene::onMoveSensor(double sensor) {
}

// JNIイベント
void Scene::onGameEvent(int type, int param1, int param2, int param3, int param4, const char *param5) {
}
