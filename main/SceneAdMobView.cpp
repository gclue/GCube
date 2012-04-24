//
//  SceneAdMobView.cpp
//  GCube
//
//  Created by Hitoshi Haga on 12/04/23.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#include "SceneAdMobView.h"
#include "ImageView.h"
#include "SharedTexture.h"
#include "TextureManager.h"
#include "PackerTexture.h"
#include "ViewGroup.h"
#include "APIGlue.h"
#include "ApplicationController.h"
#include "Layer2D.h"



SceneAdMobView::SceneAdMobView(ApplicationController *controller) :Scene(controller) {
	//TextTexture使用サンプル
	//結構メモリを使用するので動的に生成するのはおすすめしない
    GCDrawText("AdMob表示", 30, 1.0, 1.0, 1.0);
    GCDrawText("AdMob非表示", 30, 1.0, 1.0, 1.0);
    GCDrawText("AdMobきりかえ", 30, 1.0, 1.0, 1.0);
	
    //GCGetTextTextureでテクスチャを取得
    PackerTexture *strTexture = GCGetTextTexture();
	
    //TextureManagerにテクスチャを登録する。
    //登録しておかないと復帰時の再ロードが行われない。
    //任意の名前で登録する "TextTexture"
    //登録後は　mgr->getSharedTexture("TextTexture",NULL);　で取得できる
    controller->texMgr->addExtraTexture("AdMobText", strTexture);
}


SceneAdMobView::~SceneAdMobView() {
	
}


void SceneAdMobView::onActivate() {
	Scene::onActivate();
	
	//AdMobのビューを作成します.
	//３番目の引数に文字列でユニットIDを設定してください。
	//またデフォルトでテスト環境になっているので、必要に応じてAdMobViewController.mの環境を変えてください。
	controller->sendAdMobEvent(AdMobEvent_Create, ADMOB_POS_DOWN_CENTER,"REPLACE_ME");

	
	//いつものようにonActivateでビュー作成
	createViews();
	
}


void SceneAdMobView::createViews() {
	Layer2D *layer = new Layer2D(controller);
	
	ViewGroup *root = new ViewGroup(controller);
	
	
	SharedTexture *tex01 = controller->texMgr->getSharedTexture("AdMobText", NULL);
	
	//表示ボタン
	Button *btVisible = new Button(controller);
	btVisible->setFigure(tex01->makePlate(0,0));
	btVisible->setSelectFigure(tex01->makePlate(0, 0));
	btVisible->setTexture(&tex01->getTexture());
	btVisible->setPosition(-0.4, 0.3);
	btVisible->setOnButtonClickListener(this);
	btVisible->setUserID(100);
	
	
	//非表示ボタン
	Button *btInvisible = new Button(controller);
	btInvisible->setFigure(tex01->makePlate(1, 0));
	btInvisible->setSelectFigure(tex01->makePlate(1, 0));
	btInvisible->setTexture(&tex01->getTexture());
	btInvisible->setPosition(0.4, 0.3);
	btInvisible->setOnButtonClickListener(this);
	btInvisible->setUserID(101);
	
	//ビュー位置変更ボタン
	Button *btChange = new Button(controller);
	btChange->setFigure(tex01->makePlate(2, 0));
	btChange->setSelectFigure(tex01->makePlate(2, 0));
	btChange->setTexture(&tex01->getTexture());
	btChange->setPosition(0, -0.4);
	btChange->setOnButtonClickListener(this);
	btChange->setUserID(102);
	


	root->addView(btVisible);
	root->addView(btInvisible);
	root->addView(btChange);
	
	
	btVisible->release();
	btInvisible->release();
	btChange->release();
	
	
	
	
	layer->setContentView(root);
	root->release();
	addLayer(1, layer);
}

void SceneAdMobView::onButtonTouchStart(Button *btn) {
	
}
static int pos = ADMOB_POS_DOWN_CENTER;
void SceneAdMobView::onButtonClick(Button *btn) {
	int id = btn->getUserID();
	
	
	switch(id) {
		case 100:
			//Admobビューを表示
			controller->sendAdMobEvent(AdMobEvent_Visible);
			break;
		case 101:
			//admobビューを非表示
			controller->sendAdMobEvent(AdMobEvent_Invisible);
			break;
		case 102: //adMobの位置を切り替え
			pos++;
			if(pos > ADMOB_POS_RIGHT_DOWN_CORNER) {
				pos = ADMOB_POS_UP_CENTER;
			}
			//位置を切り替え
			controller->sendAdMobEvent(AdMobEvent_Create, pos,"REPLACE_ME");
			break;
	}
}
