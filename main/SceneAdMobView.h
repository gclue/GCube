//
//  SceneAdMobView.h
//  GCube
//
//  Created by Hitoshi Haga on 12/04/23.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#ifndef GCube_SceneAdMobView_h
#define GCube_SceneAdMobView_h


#include "Scene.h"
#include "Button.h"

class SceneAdMobView : public Scene, OnButtonClickListener {
	
	
private:
	void createViews();
	
public:
	SceneAdMobView(ApplicationController *controller);
	virtual ~SceneAdMobView();
	
	
	
	virtual void onActivate();
	
	
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
};



#endif
