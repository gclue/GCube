//
//  SceneSample1.h
//  GCube
//
//  Created by  on 12/04/20.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef GCube_SceneSample1_h
#define GCube_SceneSample1_h

#include "Scene.h"
#include "Button.h"

class SceneSample1 : public Scene , public OnButtonClickListener {
private:
	typedef Scene super;
    
    
public:
	/**
	 * コンストラクタ.
	 * @param[in] controller コントローラー
	 */
	SceneSample1(ApplicationController *controller);
    
	/**
	 * デストラクタ.
	 */
	virtual ~SceneSample1();
    
	//////////////////////////////////////////////////////////
	// IScene の実装
	//////////////////////////////////////////////////////////
	
	/**
	 * ステップ実行します.
	 */
	virtual void step(float dt);
	
	/**
	 * セットアップ処理を行います.
	 */
	virtual void setup();
    
	/**
	 * リサイズ処理を行います.
	 * @param[in] width 横幅
	 * @param[in] height 縦幅
	 */
	virtual void resize(int width, int height);
    
	/**
	 * 活性化します.
	 */
	virtual void onActivate();
    
	/**
	 * 休止します.
	 */
	virtual void onSuspend();
    
	/**
	 * 活性化してシーンが切り替え終わったこと通知します.
	 */
	virtual void onStart();
    
	/**
	 * 非活性化してシーンが切り替え終わったこと通知します.
	 */
	virtual void onEnd();
    
	/**
	 * コンテキストが切り替わったことを通知します.
	 */
	virtual void onContextChanged();
    
	/**
	 * タッチイベント.
	 * @param[in] event タッチイベント
	 */
	virtual bool onTouch(TouchEvent &event);
    
    virtual void onButtonClick(Button *btn);
    virtual void onButtonTouchStart(Button *btn);
};



#endif
