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
#include "ApplicationController.h"

Scene::Scene() : IScene() {
	LOGD("****Scene");
	controller = ApplicationController::getInstance();
	touchable = true;
}

Scene::Scene(ApplicationController *controller) : IScene(controller) {
	LOGD("****Scene");
	touchable = true;
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
	layers.clear();
	LOGD("****~Scene end");
}

// レイヤーを追加
void Scene::addLayer(int id, Layer *layer) {
	if(layers[id]) {
		delete layers[id];
	}
	layers[id] = layer;
}

// レイヤーを検索
Layer *Scene::getLayer(int id) {
	std::map<int, Layer*>::iterator it = layers.find(id);
	if (it!=layers.end()) {
		return (*it).second;
	} else {
		return NULL;
	}
}

// レイヤの削除
bool Scene::removeLayer(int id) {
	std::map<int, Layer*>::iterator it = layers.find(id);
	if (it != layers.end() ) {
		Layer *layer = (*it).second;
		delete layer;
		layers.erase(it);
		return true;
	}
	
	return false;
}

void Scene::removeAllLayers() {
	std::map<int, Layer*>::iterator it = layers.begin();
	while (it != layers.end()) {
		Layer *layer = (*it).second;
		delete layer;
		it++;
	}
	layers.clear();
}


void Scene::setup() {
	// 各レイヤーの描画
	std::map<int, Layer*>::iterator it = layers.begin();
	while (it != layers.end()) {
		Layer *layer = (*it).second;
		layer->setup();
		it++;
	}
	this->onSetup();
}

void Scene::resize(int width, int height) {
	// アスペクト比の計算
	float aspect = width / (float) height;
	
	// 各レイヤーのリサイズ
	std::map<int, Layer*>::iterator it = layers.begin();
	while (it != layers.end()) {
		Layer *layer = (*it).second;
		layer->resize(aspect);
		it++;
	}
	this->onResize(width, height);
}

void Scene::step(float dt) {
	// 各レイヤーの描画
	std::map<int, Layer*>::iterator it = layers.begin();
	while (it != layers.end()) {
		Layer *layer = (*it).second;
		float t = dt;
		if (layer->pause) t = 0;
		layer->render(t);
		it++;
	}
	this->onStep(dt);
}

void Scene::contextChanged() {
	std::map<int, Layer*>::iterator it = layers.begin();
	while (it != layers.end()) {
		Layer *layer = (*it).second;
		layer->onContextChanged();
		it++;
	}
	this->onContextChanged();
}

bool Scene::dispatchSensorEvent(SensorEvent &event) {
	if (layers.size() > 0) {
		std::map<int, Layer*>::iterator it = layers.end();
		it--;
		while (true) {
			Layer *layer = (*it).second;
			if (layer->onSensorEvent(event)) {
				return true;
			}
			if (it == layers.begin()) {
				break;
			}
			it--;
		}
	}
	return this->onSensorEvent(event);
}

bool Scene::dispatchKeyEvent(KeyEvent &event) {
	return false;
}

bool Scene::dispatchTouchEvent(TouchEvent &event) {
	if (!touchable) {
		return false;
	}
	if (layers.size() > 0) {
		std::map<int, Layer*>::iterator it = layers.end();
		it--;
		while (true) {
			Layer *layer = (*it).second;
			if (layer->onTouchEvent(event)) {
				return true;
			}
			if (it == layers.begin()) {
				break;
			}
			it--;
		}
	}
	return this->onTouchEvent(event);
}

void Scene::setTouchable(bool flag)
{
	touchable = flag;
}

bool Scene::isTouchable()
{
	return touchable;
}

//////////////////////////////////////////////////////////////////////////////////////////
// IScene の実装
//////////////////////////////////////////////////////////////////////////////////////////

// セットアップ処理を行います.
void Scene::onSetup() {
	LOGD("****Scene::onSetup()");
}

// リサイズ
void Scene::onResize(int width, int height) {
	LOGD("****Scene::onResize:%d-%d", width, height);
}

// ステップ実行します
void Scene::onStep(float dt) {
}

// 活性化します.
void Scene::onActivate() {
	LOGD("****Scene::onActivate");
}

// 休止します.
void Scene::onSuspend() {
	LOGD("****Scene::onSuspend");
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
	LOGD("****Scene::onPressBackKey");
	return false;
}

bool Scene::onSensorEvent(SensorEvent &event) {
	return false;
}

// タッチイベント
bool Scene::onTouchEvent(TouchEvent &event) {
	return false;
}
