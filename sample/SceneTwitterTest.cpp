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

#include "SceneTwitterTest.h"
#include "ApplicationController.h"
#include "TextureManager.h"
#include "SharedTexture.h"
#include "PackerTexture.h"
#include "ImageView.h"
#include "ImageAnimationView.h"
#include "ViewGroup.h"
#include "Layer2D.h"
#include "APIGlue.h"
#include <time.h>

SceneTwitterTest::SceneTwitterTest(ApplicationController *controller) : Scene(controller) {
	LOGD("****SceneTwitterTest");
	index = 0;
	
	
	Layer2D *layer = new Layer2D(controller);
	
	ViewGroup *root = new ViewGroup(controller);
	
	
	SharedTexture *tex01 = controller->texMgr->getSharedTexture("texture/twitter_texture_01.png", "texture/twitter_texture_01.txt");
	
	
	//ダイアログ背景
	ImageView *twBack = new ImageView(controller);
	twBack->setFigure(tex01->makePlate("ivTwitterBack.png"));
	twBack->setTexture(&tex01->getTexture());
	twBack->setPosition(0,0);
	
	//文字背景
	ImageView *textBack = new ImageView(controller);
	textBack->setFigure(tex01->makePlate("ivTextBack.png"));
	textBack->setTexture(&tex01->getTexture());
	textBack->setPosition(0, 0.1);
	
	//投稿
	Button *btPost = new Button(controller);
	btPost->setFigure(tex01->makePlate("btPost.png"));
	btPost->setSelectFigure(tex01->makePlate("btPost.png"));
	btPost->setTexture(&tex01->getTexture());
	btPost->setPosition(-0.3, -0.4);
	btPost->setOnButtonClickListener(this);
	btPost->setUserID(100);
	
	//キャンセル（意味無し）
	Button *btCancel = new Button(controller);
	btCancel->setFigure(tex01->makePlate("btCancel.png"));
	btCancel->setSelectFigure(tex01->makePlate("btCancel.png"));
	btCancel->setTexture(&tex01->getTexture());
	btCancel->setPosition(0.3, -0.4);
	btCancel->setOnButtonClickListener(this);
	btCancel->setUserID(101);
	
	
	//文字列テクスチャの作成
	GCDrawText("ツイッターのテストを行うサンプルです", 40, 1.0, 0.0, 1.0);
	GCDrawText("GCube TwitterAPIテスト.", 40, 0, 0, 0);
	GCDrawText("投稿成功!", 40, 0, 0, 1);
	GCDrawText("投稿失敗...", 40, 1, 0, 0);
	
    //GCGetTextTextureでテクスチャを取得
    PackerTexture *strTexture = GCGetTextTexture();
	
    //TextureManagerにテクスチャを登録する。
    //登録しておかないと復帰時の再ロードが行われない。
    //任意の名前で登録する "TextTexture"
    //登録後は　mgr->getSharedTexture("TextTexture",NULL);　で取得できる
    controller->texMgr->addExtraTexture("TextTexture", strTexture);
	
	
	//説明文
    ImageView *textLabel = new ImageView(controller);
    textLabel->setFigure(strTexture->makeFixPlate(0, 0, textBack->size.x));//登録順のインデックスもしくは、 strTexture->makePlate("Test")　GCDrawText時の文字列を指定する
    textLabel->setTexture(&strTexture->getTexture());
    textLabel->setPosition(0, twBack->point.y + twBack->size.y - textLabel->size.y*2.0);
	
	
	//投稿テキスト
	ImageView *postText = new ImageView(controller);
	postText->setFigure(strTexture->makeFixPlate(1, 0, textBack->size.x));
	postText->setTexture(&strTexture->getTexture());
	postText->setPosition(0, textBack->point.y);
	
	
	
	
	//投稿完了.
	ImageView *successText = new ImageView(controller);
	successText->setFigure(strTexture->makeFixPlate(2, 0, 0.8));
	successText->setTexture(&strTexture->getTexture());
	successText->setPosition(0,0);
	successText->setVisible(false);
	successText->setUserID(102);
	
	
	
	//投稿失敗.
	ImageView *failedText = new ImageView(controller);
	failedText->setFigure(strTexture->makeFixPlate(3, 0, 0.8));
	failedText->setTexture(&strTexture->getTexture());
	failedText->setPosition(0,0);
	failedText->setVisible(false);
	failedText->setUserID(103);
    

	
	
	
	root->addView(twBack);
	root->addView(textBack);
	root->addView(btPost);
	root->addView(btCancel);
	root->addView(textLabel);
	root->addView(postText);
	root->addView(successText);
	root->addView(failedText);
	
	twBack->release();
	textBack->release();
	btPost->release();
	btCancel->release();
	textLabel->release();
	postText->release();
	successText->release();
	failedText->release();
    
	
	layer->setContentView(root);
	
	
	root->release();
	
	
	
	addLayer(1, layer);
	
	
}

SceneTwitterTest::~SceneTwitterTest() {
	LOGD("****~SceneTwitterTest");
}

//////////////////////////////////////////////////////////
// IScene の実装
//////////////////////////////////////////////////////////

// ステップ実行します
void SceneTwitterTest::step(float dt) {
	super::step(dt);
}

// セットアップ処理を行います.
void SceneTwitterTest::setup() {
	super::setup();
	LOGD("****SceneTwitterTest::setup");
}

// リサイズ
void SceneTwitterTest::resize(int width, int height) {
	super::resize(width, height);
	LOGD("****SceneTwitterTest::resize:%d-%d", width, height);
}

// 活性化します.
void SceneTwitterTest::onActivate() {
	super::onActivate();
	LOGD("****SceneTwitterTest::onActivate");
}

// 休止します.
void SceneTwitterTest::onSuspend() {
	super::onSuspend();
	LOGD("****SceneTwitterTest::onSuspend");
}

// 活性化してシーンが切り替え終わったこと通知します.
void SceneTwitterTest::onStart() {
	super::onStart();
	LOGD("****SceneTwitterTest::onStart");
	
}

// 非活性化してシーンが切り替え終わったこと通知します.
void SceneTwitterTest::onEnd() {
	super::onEnd();
	LOGD("****SceneTwitterTest::onEnd");
}

// コンテキストが切り替わったことを通知します.
void SceneTwitterTest::onContextChanged() {
	super::onContextChanged();
	LOGD("****SceneTwitterTest::onContextChanged");
}

bool SceneTwitterTest::onTouch(TouchEvent &event) {
	super::onTouch(event);
	LOGD("****SceneTwitterTest::onTouch");
	return false;
}

void SceneTwitterTest::onTwitterEvent(int type, int param) {
	
	//投稿イベント
	if(type == TwitterEvent_Post) {
		if(param == 0) {
			startPostSuccess();
		}else {
			startPostFailed();
		}
	}
	
	//認証イベント(使ってない)
	if(type == TwitterEvent_Authenticate) {
		if(param == 0) {
			startPostSuccess();
		}else {
			startPostFailed();
		}
	}
}



void SceneTwitterTest::startPostFailed() {
	//アニメーション。投稿失敗.
	Layer2D *layer = (Layer2D*)this->getLayer(1);
	ImageView *view = (ImageView*)layer->getContentView()->findViewByID(103);
	view->setVisible(true);
	
	Animation *anim = new Animation();
	anim->setAlpha(1, 0);
	anim->setDelay(1);
	anim->setDuration(1);
	anim->setAnimationListener(this);
	anim->tag = 101;
	
	
	view->startAnimation(anim);
	
}

void SceneTwitterTest::startPostSuccess() {
	
	//アニメーション. 投稿成功
	Layer2D *layer = (Layer2D*)this->getLayer(1);
	ImageView *view = (ImageView*)layer->getContentView()->findViewByID(102);
	
	
	view->setVisible(true);
	
	Animation *anim = new Animation();
	anim->setAlpha(1, 0);
	anim->setDelay(1);
	anim->setDuration(1);
	anim->setAnimationListener(this);
	anim->tag = 100;
	
	
	view->startAnimation(anim);

}

void SceneTwitterTest::onButtonTouchStart(Button *btn) {
	
}

void SceneTwitterTest::onButtonClick(Button *btn) {
	int id = btn->getUserID();
	if(id == 100) {
		//ツイッター投稿イベントを送信.
		//同じ文字列を連続では送れないので注意
		controller->sendTwitterEvent(TwitterEvent_Post, "GCube TwitterAPIテスト.2");
		
		Layer2D *layer = (Layer2D*)this->getLayer(1);
		layer->getContentView()->findViewByID(100)->setClickable(false);
		layer->getContentView()->findViewByID(101)->setClickable(false);
		
	}
	
	if(id == 101) {
		
	}
}


void SceneTwitterTest::onAnimationEnd(IAnimation *animation, int index) {
	
}

void SceneTwitterTest::onAnimationStart(IAnimation *animation) {
	
}

void SceneTwitterTest::onAnimationAllEnd(IAnimation *animation) {
	Layer2D *layer = (Layer2D*)this->getLayer(1);
	ImageView *view1 = (ImageView*)layer->getContentView()->findViewByID(102);
	ImageView *view2 = (ImageView*)layer->getContentView()->findViewByID(103);
	
	view1->setVisible(false);
	view2->setVisible(false);
	layer->getContentView()->findViewByID(100)->setClickable(true);
	layer->getContentView()->findViewByID(101)->setClickable(true);
	
	
	if(animation->tag == 100) {
		//成功
	}
	if(animation->tag == 101) {
		//失敗
	}
}


