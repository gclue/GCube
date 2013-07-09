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

SceneHello::SceneHello() : Scene() {
    LOGD("*SceneHello::SceneHello");
    
    // テキストの登録
	ApplicationController *controller = ApplicationController::getInstance();
    TextureManager *mgr = controller->texMgr;
    GCDrawText("Hello World!",80,1.0,1.0,1.0);
    PackerTexture *strTexture = GCGetTextTexture();
    mgr->addExtraTexture("Text", strTexture);
	
    // テキストビューの生成
    ImageView *textLabel = new ImageView("Hello World!");
    textLabel->setPosition(0,0);
    
    // テキストビューをビューグループに追加
    ViewGroup *root = new ViewGroup();
    root->addView(textLabel);
    textLabel->release();
	
    // レイヤーにビューグループを追加
    Layer2D *layer = new Layer2D();
    layer->setContentView(root);
    root->release();
    
    // レイヤーをシーンに追加
    addLayer(1,layer);
}

SceneHello::~SceneHello() {
    LOGD("*SceneHello::~SceneHello");
}
