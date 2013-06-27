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

bool SceneSample1::onTouchEvent(TouchEvent &event) {
	LOGD("****SceneMain::onTouch");
   	return false;
}

void SceneSample1::onButtonClick(Button *btn){
    LOGD("button click");
    controller->sceneChange(1);
    
}

void SceneSample1::onButtonTouchStart(Button *btn){
    
}
