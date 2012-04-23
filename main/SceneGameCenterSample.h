//
//  SceneGameCenterSample.h
//  GCube
//
//  Created by Hitoshi Haga on 12/04/22.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#ifndef GCube_SceneGameCenterSample_h
#define GCube_SceneGameCenterSample_h

#include "Scene.h"
#include "Button.h"
#include "Animation.h"

class ViewGroup;

class SceneGameCenterSample : public Scene, OnButtonClickListener, IAnimationListener {
	
	
private:
	typedef Scene super;
	
	
	ViewGroup *createView();
	
	
	void changeClick(int id , bool f);
	void changeVisible(int id, bool f);
	
	void startLoading();
	void stopLoading();
	void startResponse(bool f);
	
	
public:
	SceneGameCenterSample(ApplicationController *controller);
	
	virtual ~SceneGameCenterSample();
	
	
	/**
	 * 活性化します.
	 */
	virtual void onActivate();
	
	
	virtual void onGameCenterEvent(int type, int param1, int param2);
	
	
	
	
	
	/**
	 * ボタンがクリックされたイベント.
	 * @param[in] btn クリックされたボタン
	 */
	virtual void onButtonClick(Button *btn);
	/**
	 * ボタンがタッチされたときに呼び出されます.
	 * @param[in] btn タッチされたボタン
	 */
	virtual void onButtonTouchStart(Button *btn);
	
	
	/**
	 * アニメーションが開始されたときのイベント.
	 * @param animation 開始したアニメーション
	 */
	virtual void onAnimationStart(IAnimation *animation);
	
	/**
	 * アニメーションが完了したときのイベント.
	 * @param animation 完了したアニメーション
	 * @param index AnimationSetの場合には、終了したアニメーションのインデックス
	 */
	virtual void onAnimationEnd(IAnimation *animation, int index);
	
	/**
	 * アニメーションが完了したときのイベント.
	 * @param animation 完了したアニメーション
	 */
	virtual void onAnimationAllEnd(IAnimation *animation);

	
	
	
	
};


#endif
