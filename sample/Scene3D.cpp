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

#include "Scene3D.h"
#include "ApplicationController.h"
#include "TextureManager.h"
#include "Camera.h"
#include "PrimitiveObjectBuilder.h"
#include "SimpleShader.h"
#include "ColladaDataObject.h"
#include "Joint.h"
#include "Layer3D.h"
#include "Light.h"
#include "WFObjLoader.h"
#include <string>
#include "Animation.h"
#include "AnimationSet.h"


FigureSet *set = NULL;

Scene3D::Scene3D() : Scene() {
	LOGD("****Scene3D");
	rot = 0;
	
	layer3d = new Layer3D();
	addLayer(1, layer3d);
	ApplicationController::getInstance()->setBackgroundColor(0.4, 0.8, 1);
	Texture *tex1 = ApplicationController::getInstance()->texMgr->getTexture("texture/gclue_logo.png");
	Texture *tex2 = ApplicationController::getInstance()->texMgr->getTexture("texture/neko_siro-01metoji.png");
	
	fig = WFObjLoader::loadGCBFile("model/01walk.gcb");
	
	set = new FigureSet();
	set->setFigure(fig);
	set->setTexture(tex2);
	set->makeIdentity();
	set->scale(3, 3, 3);
	set->translate(0, 0, 0);
	set->setUseEdge(false);
	set->setEdgeSize(0.08);

	JointAnimation *a = WFObjLoader::loadGAVFile("model/01walk.gav");
	fig->setAnimation(a);
	
	layer3d->addFigureSet(set);
	
	
	Figure *f = createMesh(50, 50, 10, 10);
	f->build();
	
	FigureSet *set2 = new FigureSet();
	set2->setFigure(f);
	set2->setTexture(tex1);
	set2->translate(0, 0, 0);
	
	layer3d->addFigureSet(set2);
}

Scene3D::~Scene3D() {
	LOGD("****~Scene3D");
}

//////////////////////////////////////////////////////////
// IScene の実装
//////////////////////////////////////////////////////////


static float theta = 0;

// ステップ実行します
void Scene3D::onStep(float dt) {
	
//	theta += 0.01;
	
	float dx = 70.0 * sinf(theta);
	float dy = 70.0 * cosf(theta);
	
	layer3d->getCamera().lookAt(dx,50, dy, 0, 5, 0, 0, 1, 0);

}

// タッチイベント
bool Scene3D::onTouchEvent(TouchEvent &event) {
	LOGD("****Scene3D::onTouch:%.1f-%.1f", event.x, event.y);
	return false;
}

// JNIイベント
void Scene3D::onGameEvent(int type, int param1, int param2, int param3, int param4, const char *param5) {
	LOGI("** Scene3D::onGameEvent: %d,%d,%d,%d,%d,%s",type, param1, param2, param3, param4, param5);
}
