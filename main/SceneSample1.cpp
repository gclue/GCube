//
//  SceneSample1.cpp
//  GCube
//
//  Created by  on 12/04/20.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SceneSample1.h"
#include "ApplicationController.h"
#include "GCContext.h"
#include "LayoutInflater.h"
#include "View.h"
#include "Button.h"
#include "Layer2D.h"

SceneSample1::SceneSample1(ApplicationController *controller) : Scene(controller) {
    LayoutInflater *inflater = controller->getLayoutInflater();
    
    View *root = inflater->inflate("layout/main.xml");
    inflater->release();
    
    Button *button = (Button*)root->findViewByID(3);
    button->setOnButtonClickListener(this);
    
    Layer2D *layer = new Layer2D(controller);
    layer->setContentView(root);
    root->release();
    addLayer(1, layer);
}

SceneSample1::~SceneSample1() {
	LOGD("****~SceneMain");
}

//////////////////////////////////////////////////////////
// IScene の実装
//////////////////////////////////////////////////////////

// ステップ実行します
void SceneSample1::step(float dt) {
    //LOGD("****SceneMain::step:%f",dt);
	super::step(dt);
}

// セットアップ処理を行います.
void SceneSample1::setup() {
	super::setup();
	LOGD("****SceneMain::setup");
}

// リサイズ
void SceneSample1::resize(int width, int height) {
	super::resize(width, height);
	LOGD("****SceneMain::resize:%d-%d", width, height);
}

// 活性化します.
void SceneSample1::onActivate() {
	super::onActivate();
	LOGD("****SceneMain::onActivate");
}

// 休止します.
void SceneSample1::onSuspend() {
	super::onSuspend();
	LOGD("****SceneMain::onSuspend");
}

// 活性化してシーンが切り替え終わったこと通知します.
void SceneSample1::onStart() {
	super::onStart();
	LOGD("****SceneMain::onStart");
}

// 非活性化してシーンが切り替え終わったこと通知します.
void SceneSample1::onEnd() {
	super::onEnd();
	LOGD("****SceneMain::onEnd");
}

// コンテキストが切り替わったことを通知します.
void SceneSample1::onContextChanged() {
	super::onContextChanged();
	LOGD("****SceneMain::onContextChanged");
}

bool SceneSample1::onTouch(TouchEvent &event) {
	super::onTouch(event);
	LOGD("****SceneMain::onTouch");
   	return false;
}

void SceneSample1::onButtonClick(Button *btn){
    LOGD("button click");
    controller->sceneChange(1);
    
}

void SceneSample1::onButtonTouchStart(Button *btn){
    
}
