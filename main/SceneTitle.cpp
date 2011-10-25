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

#include "SceneTitle.h"
#include "ApplicationController.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "TextureManager.h"
#include "SharedTexture.h"
#include "ImageView.h"
#include "ImageAnimationView.h"
#include "ViewGroup.h"
#include "Layer2D.h"

SceneTitle::SceneTitle(ApplicationController *controller) : Scene(controller) {
	LOGD("****SceneTitle");
	// アクティブフラグをOFF
	activeflg = false;
	index = 0;
	setup();
}

SceneTitle::~SceneTitle() {
	LOGD("****~SceneTitle");
}

//////////////////////////////////////////////////////////
// IScene の実装
//////////////////////////////////////////////////////////

// セットアップ処理を行います.
void SceneTitle::setup() {
	LOGD("****SceneTitle::setup");

	TextureManager *mgr = controller->getTextureManager();

	SharedTexture *tex = mgr->getSharedTexture(
			"texture/chara_texture.png",
			"texture/chara_texture.txt");

	ViewGroup *root = new ViewGroup(controller);

	static const char *filename[] = {
		"chara00.png",
		"chara01.png",
		"chara02.png",
		"chara03.png",
		"chara04.png",
		"chara05.png",
		"chara06.png",
		"chara07.png",
		"chara08.png",
		"chara09.png",
		"chara10.png",
		"chara11.png",
	};

	ImageAnimationView *animView = new ImageAnimationView(controller);
	for (int i = 0; i < 12; i++) {
		ImageView *image = new ImageView(controller);
		image->setFigure(tex->makePlate(filename[i]));
		image->setTexture(&tex->getTexture());
		animView->addView(image);
	}

	// 下
	animView->addAnimationFrame(1, 0, 0.12);
	animView->addAnimationFrame(1, 2, 0.12);
	// 上
	animView->addAnimationFrame(2, 9, 0.12);
	animView->addAnimationFrame(2, 11, 0.12);
	// 右
	animView->addAnimationFrame(3, 6, 0.12);
	animView->addAnimationFrame(3, 8, 0.12);
	// 左
	animView->addAnimationFrame(4, 3, 0.12);
	animView->addAnimationFrame(4, 5, 0.12);

	animView->setAnimationFrameIndex(1);
	animView->setUserID(10);
	
	animView->setPosition(0.5, 0.5);
	animView->setRotate(30);
	animView->setScale(1.5, 1.5);

	root->addView(animView);

	Layer2D *layer = new Layer2D(controller);
	layer->setContentView(root);

	addLayer(1, layer);

	LOGD("****SceneTitle::setup end");
}

// リサイズ
void SceneTitle::resize(int width, int height) {
	LOGD("****SceneTitle::resize:%d-%d", width, height);

	LOGD("****SceneTitle::resize end");
}

// 活性化します.
void SceneTitle::onActivate() {
	LOGD("****SceneTitle::onActivate");
	activeflg = true;
}

// 休止します.
void SceneTitle::onSuspend() {
	LOGD("****SceneTitle::onSuspend");
	activeflg = false;
}

// 活性化してシーンが切り替え終わったこと通知します.
void SceneTitle::onStart() {
	LOGD("****SceneTitle::onStart");

}

// 非活性化してシーンが切り替え終わったこと通知します.
void SceneTitle::onEnd() {
	LOGD("****SceneTitle::onEnd");
}

// コンテキストが切り替わったことを通知します.
void SceneTitle::onContextChanged() {
	LOGD("****SceneTitle::onContextChanged");
}

void SceneTitle::onTouch(TouchEvent &event) {
	LOGD("****SceneTitle::onTouch");
	if (event.type == touchDown) {
		Layer2D *layer = (Layer2D *) getLayer(1);

		ImageAnimationView *v = (ImageAnimationView *) layer->findViewByID(10);
		if (v) {
			index++;
			index %= 4;
			v->setAnimationFrameIndex(index + 1);
		}
	}
}
