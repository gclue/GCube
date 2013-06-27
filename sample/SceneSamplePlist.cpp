/*
 * The MIT License (MIT)
 * Copyright (c) 2013 GClue, inc.
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

#include "SceneSamplePlist.h"
#include "ApplicationController.h"
#include "Layer2D.h"
#include "ImageView.h"
#include "TextureManager.h"
#include "SharedTexture.h"
#include "GCContext.h"
#include "ViewGroup.h"
#include "Figure.h"
#include "Texture.h"

SceneSamplePlist::SceneSamplePlist(ApplicationController *controller)
: Scene(controller)
{
    
    Layer2D *layer = new Layer2D(controller);
    ViewGroup *root = new ViewGroup(controller);
    
    SharedTexture *tex = controller->texMgr->getSharedTextureFromPlist("texture/sample_plist.plist");
    
    // テクスチャアトラスから画像を生成
    ImageView *img = new ImageView(controller);
    img->setFigure(tex->makePlate("gclue_logo.png"));
    img->setTexture(tex);
    img->setPosition(0, .3f);
    
    // 回転している画像も同様に生成
    ImageView *rotatedImg = new ImageView(controller);
    rotatedImg->setFigure(tex->makePlate("chara_texture.png"));
    rotatedImg->setTexture(tex);
    rotatedImg->setPosition(0, -.3f);
    
    root->addView(img);
    root->addView(rotatedImg);
    
    layer->setContentView(root);
    root->release();
    
    addLayer(1, layer);
}

SceneSamplePlist::~SceneSamplePlist() {
    
}