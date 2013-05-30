/*
 * The MIT License (MIT)
 * Copyright (c) 2012 GClue, inc.
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

#include "SceneHello.h"
#include "APIGlue.h"
#include "Layer2D.h"
#include "ApplicationController.h"
#include "TextureManager.h"
#include "PackerTexture.h"
#include "ImageView.h"

SceneHello::SceneHello(ApplicationController *controller) : Scene(controller){
    LOGD("*SceneHello::SceneHello");
    
    // テキストの登録
    TextureManager *mgr = controller->texMgr;
    GCDrawText("Hello World!",80,1.0,1.0,1.0);
    PackerTexture *strTexture = GCGetTextTexture();
    mgr->addExtraTexture("Text",strTexture);
    
    // テキストビューの生成
    ImageView *textLabel = new ImageView(controller);
    textLabel->setFigure(strTexture->makePlate("Hello World!"));
    textLabel->setTexture(&strTexture->getTexture());
    textLabel->setPosition(0,0);
    
    // テキストビューをビューグループに追加
    ViewGroup *root = new ViewGroup(controller);
    root->addView(textLabel);
    textLabel->release();
    
    // レイヤーにビューグループを追加
    Layer2D *layer = new Layer2D(controller);
    layer->setContentView(root);
    root->release();
    
    // レイヤーをシーンに追加
    addLayer(1,layer);
    
}

SceneHello::~SceneHello(){
    LOGD("*SceneHello::~SceneHello");
}

/* IScene */

void SceneHello::step(float dt){
    super::step(dt);
    //    LOGD("*SceneHello::step");
}

void SceneHello::setup(){
    super::setup();
    LOGD("*SceneHello::setup");
}

void SceneHello::resize(int width, int height){
    super::resize(width, height);
    LOGD("*SceneHello::resize");
}

void SceneHello::onActivate(){
    super::onActivate();
    LOGD("*SceneHello::onActive");
}

void SceneHello::onSuspend(){
    super::onSuspend();
    LOGD("*SceneHello::onSuspend");
}

void SceneHello::onStart(){
    super::onStart();
    LOGD("*SceneHello::onStart");
}

void SceneHello::onEnd(){
    super::onEnd();
    LOGD("*SceneHello::onEnd");
}

void SceneHello::onContextChanged(){
    super::onContextChanged();
    LOGD("*SceneHello::onContextChanged");
}

bool SceneHello::onTouch(TouchEvent &event){
    super::onTouch(event);
    LOGD("*SceneHello::onTouch");
    return false;
}