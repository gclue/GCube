//
//  SceneGameCenterSample.cpp
//  GCube
//
//  Created by Hitoshi Haga on 12/04/22.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#include "SceneGameCenterSample.h"
#include "Layer2D.h"
#include "ViewGroup.h"
#include "ImageView.h"
#include "Button.h"
#include "ApplicationController.h"
#include "APIGlue.h"
#include "SharedTexture.h"
#include "TextureManager.h"
#include "PackerTexture.h"
#include "AnimationSet.h"


#define GAME_SCORE 12345



SceneGameCenterSample::SceneGameCenterSample(ApplicationController *controller) :Scene(controller) {
	
	//TextTexture使用サンプル
	//結構メモリを使用するので動的に生成するのはおすすめしない
    GCDrawText("GameCenterサンプル", 30, 1.0, 1.0, 1.0);
    GCDrawText("スコア送信成功", 30, 1.0, 0.0, 0.0);
    GCDrawText("スコア送信失敗", 40, 1.0, 0.0, 1.0);
	
    //GCGetTextTextureでテクスチャを取得
    PackerTexture *strTexture = GCGetTextTexture();
	
    //TextureManagerにテクスチャを登録する。
    //登録しておかないと復帰時の再ロードが行われない。
    //任意の名前で登録する "TextTexture"
    //登録後は　mgr->getSharedTexture("TextTexture",NULL);　で取得できる
    controller->texMgr->addExtraTexture("GameCenterText", strTexture);
}


SceneGameCenterSample::~SceneGameCenterSample() {
	
}


ViewGroup* SceneGameCenterSample::createView() {
	ViewGroup *root =new ViewGroup(controller);
	
	//テキストテクスチャ
	SharedTexture *tex01 = controller->texMgr->getSharedTexture("GameCenterText", NULL);
	
	//UIテクスチャー
	SharedTexture *tex02 = controller->texMgr->getSharedTexture("texture/game_center_ui.png", "texture/game_center_ui.txt");
	
	
	//テキスト
	ImageView *textView = new ImageView(controller);
	textView->setFigure(tex01->makeFixPlate(0,0,0.8));
	textView->setTexture(tex01);
	textView->setPosition(0, 0.5);
	
	
	//ゲームセンターAchivement表示
	Button *btOpen = new Button(controller);
	btOpen->setFigure(tex02->makePlate("game_center_icon_gclue.png"));
	btOpen->setSelectFigure(tex02->makePlate("game_center_icon_gclue.png"));
	btOpen->setTexture(tex02);
	btOpen->setOnButtonClickListener(this);
	btOpen->setUserID(100);
	btOpen->setPosition(-0.4, -0.3);
	btOpen->setClickable(true);
	
	//ゲームセンタースコア送信
	Button *btSend = new Button(controller);
	btSend->setFigure(tex02->makePlate("bt_sendScore.png"));
	btSend->setSelectFigure(tex02->makePlate("bt_sendScore.png"));
	btSend->setTexture(tex02);
	btSend->setOnButtonClickListener(this);
	btSend->setUserID(101);
	btSend->setClickable(true);
	
	btSend->setPosition(0.4, -0.3);
	
	
	//ローディングビュー
	ImageView *loading = new ImageView(controller);
	loading->setFigure(tex02->makePlate("lodinginfo.png"));
	loading->setTexture(tex02);
	loading->setVisible(false);
	loading->setUserID(104);
	
	
	
	//スコア送信成功
	ImageView *textSuccess = new ImageView(controller);
	textSuccess->setFigure(tex01->makeFixPlate(1, 0, 0.7));
	textSuccess->setTexture(tex01);
	textSuccess->setUserID(102);
	textSuccess->setVisible(false);
	
	//スコア送信失敗
	ImageView *textFailed = new ImageView(controller);
	textFailed->setFigure(tex01->makeFixPlate(2, 0, 0.7));
	textFailed->setTexture(tex01);
	textFailed->setUserID(103);
	textFailed->setVisible(false);
	
	
	//ビューの追加
	root->addView(textView);
	root->addView(btOpen);
	root->addView(btSend);
	root->addView(loading);
	root->addView(textSuccess);
	root->addView(textFailed);
	
	
	//ビューのリリース
	textView->release();
	btOpen->release();
	btSend->release();
	loading->release();
	textSuccess->release();
	textFailed->release();
	
	
	return root;
}



void SceneGameCenterSample::onActivate() {
	super::onActivate();
	
	
	//ビューの準備
	Layer2D *layer = new Layer2D(controller);
	
	ViewGroup *root = createView();
	layer->setContentView(root);
	root->release();
	
	addLayer(1, layer);
	
	
	//ゲームセンターのログイン
//	controller->sendGameCenterEvent(GameCenterEvent_Login, 0, 0, 0);
	
}

//ゲームセンターイベントのレスポンスを取得.
void SceneGameCenterSample::onGameCenterEvent(int type, int param1, int param2) {
	super::onGameCenterEvent(type, param1, param2);
	
	LOGD("SceneGameCenterSample::onGameCenterEvent %d %d %d", type, param1, param2);
	
	
	switch( type) {
		case GameCenterEvent_Login://ログイン処理.
			if(param1 == 0) {
				//ログイン処理が終ったらボタンを有効にする
				//ログイン中にスコア送信をするとレスポンスが帰ってこなくなるときがあるので。
				//ゲームの最初にログインを行うような場合は気にしなくていいです。
				changeClick(100, true);
				changeClick(101, true);
			}
			break;
		case GameCenterEvent_Send_Score_Integer://スコア送信
			//ローディングの停止.
			stopLoading();
			
			//レスポンスの表示
			if(param1 == 0) {//success
				startResponse(true);
			}else {
				startResponse(false);
			}
			break;
		case GameCenterEvent_Close_LeaderBoard://リーダーボードを閉じる
			//ボタンクリックを可能に.
			changeClick(100, true);
			changeClick(101, true);
			break;
			
	}
}

void SceneGameCenterSample::startResponse(bool f) {
	//レスポンスビューのアニメーション(おまけ）.
	ImageView *view;
	Layer2D* layer = (Layer2D*)getLayer(1);
	if(f) {
		view = (ImageView*)layer->getContentView()->findViewByID(102);
	}else {
		view = (ImageView*)layer->getContentView()->findViewByID(103);
	}
	view->setVisible(true);
	
	
	Animation *anim = new Animation();
	anim->setAlpha(0.5, 1);
	anim->setMove(0, -0.5, 0, 0);
	anim->setDuration(1);
	
	
	Animation *anim2 = new Animation();
	anim2->setDuration(1);
	
	AnimationSet *set = new AnimationSet();
	set->setRepeat(false);
	
	set->addAnimation(anim);
	set->addAnimation(anim2);
	set->setAnimationListener(this);
	set->tag = 100;
	
	view->startAnimation(set);
	
	
	
}

void SceneGameCenterSample::startLoading() {
	//ローディングビューのアニメーション（おまけ）
	changeVisible(104, true);
	
	Layer2D* layer = (Layer2D*)getLayer(1);
	ImageView *view = (ImageView*)layer->getContentView()->findViewByID(104);
	
	Animation *anim = new Animation();
	anim->setRotation(0, -359);
	anim->setDuration(0.5);
	anim->setRepeat(false);
	
	
	view->startAnimation(anim);
}


void SceneGameCenterSample::stopLoading() {
	changeVisible(104, false);
}

//clickableを変える関数。
//こんな感じに持っておくとあとあと便利.
void SceneGameCenterSample::changeClick(int id, bool f) {
	Layer2D* layer = (Layer2D*)getLayer(1);
	View *view = layer->getContentView()->findViewByID(id);
	view->setClickable(f);
}

//Visibleを変える関数.
//毎回キャストしたりするのは面倒なので関数化しときましょう。
void SceneGameCenterSample::changeVisible(int id, bool f) {
	Layer2D* layer = (Layer2D*)getLayer(1);
	View *view = layer->getContentView()->findViewByID(id);
	view->setVisible(f);
}


void SceneGameCenterSample::onButtonTouchStart(Button *btn) {
	
}

void SceneGameCenterSample::onButtonClick(Button *btn) {
	int id = btn->getUserID();
	
	switch (id) {
		case 100: //リーダーボード表示ボタン
			
			//表示するイベントを投げて、ボタンをfalseに
			controller->sendGameCenterEvent(GameCenterEvent_Open_LeaderBoard, 0, 0, 0);
			changeClick(100, false);
			changeClick(101, false);
			break;
		case 101: //スコア送信ボタン
			
			//スコア送信イベントを送る。スコアはdefineで定義されてます。適当に変えてね！
			controller->sendGameCenterEvent(GameCenterEvent_Send_Score_Integer, 0, GAME_SCORE, 0);
			//連続で押されないようにfalseに
			changeClick(100, false);
			changeClick(101,false);
			
			//レスポンスを待つ間にローディング表示（一瞬なので見えないかも)
			startLoading();
			
			break;
			
		default:
			break;
	}
}


void SceneGameCenterSample::onAnimationEnd(IAnimation *animation, int index) {
	
}
void SceneGameCenterSample::onAnimationAllEnd(IAnimation *animation) {
	
	//レスポンスアニメーションのイベント(オマケ）
	int tag = animation->tag;
	if(tag == 100) {
		changeVisible(102, false);
		changeVisible(103, false);
		
		changeClick(100, true);
		changeClick(101, true);
	}
}
void SceneGameCenterSample::onAnimationStart(IAnimation *animation) {
	
}

