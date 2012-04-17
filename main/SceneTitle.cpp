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
#include "PackerTexture.h"
#include "ImageView.h"
#include "ImageAnimationView.h"
#include "ViewGroup.h"
#include "Layer2D.h"
#include "PhysicsLayer2D.h"
//#include "Layer3D.h"
#include "PrimitiveObjectBuilder.h"
#include "WFObjLoader.h"
#include "Light.h"
#include "HttpClient.h"
#include "XMLParser.h"
#include "PngData.h"
#include "WebView.h"

SceneTitle::SceneTitle(ApplicationController *controller) : Scene(controller) {
	LOGD("****SceneTitle");
	index = 0;
	
//	HttpClient client;
//	PngData png;
//	Scene sn(controller);
//
//	class ps : public XMLParser {
//		virtual void startElement(const XML_Char *name, const XML_Char *atts[]) {};
//		virtual void endElement(const XML_Char *name) {};
//		virtual void bodyElement(const XML_Char *s, int len) {};
//	};
//
//	ps a;



	TextureManager *mgr = controller->texMgr;

	SharedTexture *tex = mgr->getSharedTexture(
			"texture/chara_texture.png",
			"texture/chara_texture.txt");

	ViewGroup *root = new ViewGroup(controller);

	//TextTexture使用サンプル
	//結構メモリを使用するので動的に生成するのはおすすめしない
    GCDrawText("Test", 60, 1.0, 1.0, 1.0);
    GCDrawText("Red", 60, 1.0, 0.0, 0.0);
    GCDrawText("日本語", 40, 1.0, 0.0, 1.0);

    //GCGetTextTextureでテクスチャを取得
    PackerTexture *strTexture = GCGetTextTexture();

    //TextureManagerにテクスチャを登録する。
    //登録しておかないと復帰時の再ロードが行われない。
    //任意の名前で登録する "TextTexture"
    //登録後は　mgr->getSharedTexture("TextTexture",NULL);　で取得できる
    mgr->addExtraTexture("TextTexture", strTexture);

    ImageView *textLabel = new ImageView(controller);
    textLabel->setFigure(strTexture->makePlate(0, 0));//登録順のインデックスもしくは、 strTexture->makePlate("Test")　GCDrawText時の文字列を指定する
    textLabel->setTexture(&strTexture->getTexture());
    textLabel->setPosition(0, 1.0/controller->getAspect()-textLabel->size.y);
    root->addView(textLabel);
    textLabel->release();

    ImageView *textLabel2 = new ImageView(controller);
    textLabel2->setFigure(strTexture->makePlate(1, 0));
    textLabel2->setTexture(&strTexture->getTexture());
    textLabel2->setPosition(1.0 - textLabel2->size.x, 1.0/controller->getAspect()-textLabel2->size.y);
    root->addView(textLabel2);
    textLabel2->release();

    ImageView *textLabel3 = new ImageView(controller);
    textLabel3->setFigure(strTexture->makePlate(2, 0));
    textLabel3->setTexture(&strTexture->getTexture());
    textLabel3->setPosition(-1.0 + textLabel3->size.x, -1.0/controller->getAspect()+textLabel3->size.y);
    root->addView(textLabel3);
    textLabel3->release();

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
	
//	animView->setRotate(30);
	animView->setScale(2.0, 2.0);

	root->addView(animView);
	
	


	Layer2D *layer = new Layer2D(controller);
	layer->setContentView(root);
	
	
	
	PhysicsLayer2D *pLayer = new PhysicsLayer2D(controller);
	ImageView *pv = new ImageView(controller);
	pv->setFigure(tex->makePlate(filename[0]));
	pv->setTexture(&tex->getTexture());
	pv->setPosition(0, 1);
	pv->setScale(2, 2);
	
	
	ImageView *wall = new ImageView(controller);
//	wall->setFigure(tex->makePlate(filename[0]));
	wall->setPosition(0, -1/controller->getAspect());
	wall->setSize(1, 0.05);
//	wall->setTexture(&tex->getTexture());
	
	
	
	PhysicsParams param;
	param.type = b2_dynamicBody;
	PhysicsParams param2;
	param2.type = b2_staticBody;
	
	pLayer->addBody(pv, param);
	pLayer->addBody(wall, param2);
	pv->release();
	wall->release();
	
	addLayer(1, layer);
	addLayer(2, pLayer);
	
/*
	Matrix3D *mtx1 = new Matrix3D();
	mtx1->translate(1.5, 1, -1);
	
	// キューブ
	Figure *fig = createBox(0.5, 0.5, 0.5);
	fig->build();
	Matrix3D *mtx2 = new Matrix3D();
	mtx2->rotate(30, RotateDirX);
	mtx2->translate(2, 3, 0);
	RigidBodyOption opt;
	opt.sizeX = 0.5;
	opt.sizeY = 0.5;
	opt.sizeZ = 0.5;
	opt.mass = 3.0;
	opt.restitution = 0.3;
	opt.friction = 1.0;

	// 床
	Figure *fig2 = createPlate(3, 3);
	fig2->build();
	fig2->transForm->rotate(90, RotateDirX);
	RigidBodyOption opt2;
	opt.restitution = 0.3;
	opt.friction = 1.0;
	
	// 
	WFObjLoader loader;
	Figure *fig3 = loader.loadFile("model/kotoji.model");
	fig3->build();
	fig3->transForm->translate(-1.5, 5, 0);
	
	Light *light = new Light();
	light->position.x = 0;
	light->position.y = 4;
	light->position.z = 5;
	
	Texture *tex2 = new Texture("texture/gclue_logo.png");
	//
	Layer3D *l3 = new Layer3D(controller);
	l3->addLight(1, light);
	
	FigureInfo info = {NULL};
	info.fig = fig;
	info.mtx = mtx1;
	l3->addFigure(info);
	
	info.tex = tex2;
	info.mtx = mtx2;
	opt.type = RigidBodyType_Box;
	l3->addFigure(info, opt);
	
	info.fig = fig2;
	info.mtx = NULL;
	info.tex = NULL;
	opt.type = RigidBodyType_Ground;
	l3->addFigure(info, opt);
	
	info.fig = fig3;
	info.mtx = NULL;
	info.tex = NULL;
	opt.type = RigidBodyType_Mesh;
	l3->addFigure(info, opt);
	
	fig->release();
	fig2->release();
	fig3->release();
	mtx1->release();
	mtx2->release();
	tex2->release();
	
	addLayer(0, l3);
	*/
}

SceneTitle::~SceneTitle() {
	LOGD("****~SceneTitle");
}

//////////////////////////////////////////////////////////
// IScene の実装
//////////////////////////////////////////////////////////

float ddt = 0;

// ステップ実行します
void SceneTitle::step(float dt) {

	ddt+=dt;
	if (ddt>5) {
		/*
		Layer3D *l3 = (Layer3D*)getLayer(0);
		Matrix3D *mtx = l3->findMatrixByID(2);
		mtx->loadIdentity();
		mtx->rotate(rand()%180, RotateDirZ);
		mtx->translate(2,3,0);

		mtx = l3->findMatrixByID(4);
		mtx->loadIdentity();
		mtx->rotate(rand()%50, RotateDirX);
		mtx->translate(-1.5,5,0);
		*/


		Layer2D *layer = (Layer2D *) getLayer(1);

		ImageAnimationView *v = (ImageAnimationView *) layer->findViewByID(10);
		if (v) {
			index++;
			index %= 4;
			v->setAnimationFrameIndex(index + 1);
		}

		ddt = 0;
	}

	super::step(dt);
}

// セットアップ処理を行います.
void SceneTitle::setup() {
	super::setup();
	LOGD("****SceneTitle::setup");
}

// リサイズ
void SceneTitle::resize(int width, int height) {
	super::resize(width, height);
	LOGD("****SceneTitle::resize:%d-%d", width, height);
}

// 活性化します.
void SceneTitle::onActivate() {
	super::onActivate();
	LOGD("****SceneTitle::onActivate");
}

// 休止します.
void SceneTitle::onSuspend() {
	super::onSuspend();
	LOGD("****SceneTitle::onSuspend");
}

// 活性化してシーンが切り替え終わったこと通知します.
void SceneTitle::onStart() {
	super::onStart();
	LOGD("****SceneTitle::onStart");

}

// 非活性化してシーンが切り替え終わったこと通知します.
void SceneTitle::onEnd() {
	super::onEnd();
	LOGD("****SceneTitle::onEnd");
}

// コンテキストが切り替わったことを通知します.
void SceneTitle::onContextChanged() {
	super::onContextChanged();
	LOGD("****SceneTitle::onContextChanged");
}

bool SceneTitle::onTouch(TouchEvent &event) {
	super::onTouch(event);
	LOGD("****SceneTitle::onTouch");
	if (event.type == touchDown) {
//
//		ImageAnimationView *v = (ImageAnimationView *) layer->findViewByID(10);
//		if (v) {
//			index++;
//			index %= 4;
//			v->setAnimationFrameIndex(index + 1);
//		}
//		controller->sceneChange(2);
		return true;
	}
	return false;
}
